#include "framework.h"
#include "util.h"
#include "MinHook.h"
#include "curl.h"

// I went a little overkill with the encryption.

void Hook()
{
    MH_Initialize();
    FILE* a;
#ifdef SENDLOGSTOCONSOLE
    AllocConsole(); // BUG: Console does not get written to without logs enabled.
    freopen_s(&a, ENC("CONOUT$"), "w", stdout); 
    freopen_s(&a, ENC("CONOUT$"), "w", stdin);
    freopen_s(&a, ENC("CONOUT$"), "w", stderr);
#endif
#ifdef WRITELOGSTOFILE
    freopen_s(&a, ENC("logs.txt"), "w", stdout); // Writes processevent (logs) to file.
#endif


    checkSig(CurlEasyOptP, ENC("curl_easy_set_opt"));
    checkSig(CurlSetOptP, ENC("curl_set_opt"));

    MH_CreateHook((void*)CurlEasyOptP, changeeasyopt, (void**)&_curl_easy_setopt);

    if (INGAME)
    {
        LPVOID lpCurlSetopt;
        INT(*CurlSetoptCast)(LPVOID, INT, va_list) = NULL;
        CurlSetoptCast = reinterpret_cast<decltype(CurlSetoptCast)>(CurlSetOptP);
    }

    MH_STATUS status = MH_EnableHook((void*)CurlEasyOptP);
    if (status != MH_OK)
    {
        const char* StatusStr = MH_StatusToString(status);
        MessageBoxA(0, ENC("Error while hooking!"), ENC("Cobalt Fatal Error!"), MB_ICONERROR);
        if (status == MH_ERROR_NOT_INITIALIZED)
        {
            Log(ENC("Somehow, Minhook was not initialized. The builder of this dll can add \"MH_Initialize();\" at the beginning of the hook function."));
            return; // TODO: Create Exit Function.
        }
        Log(charcmb(ENC("MH Error (report this to devs): "), StatusStr)); // TOOD: Add better error handling, with maybe solutions for the user.
        return;
    }

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Hook();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

/*

Features:
NON-Skidded
Ingame
Error handling
String encryption

*/