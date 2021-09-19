#pragma once

#include "strencrypt.hpp"
const char* ProxyHost = ENC("");

#define PROD
//#define SENDLOGSTOCONSOLE
#define WRITELOGSTOFILE
//#define READFILE

const char* FNhost = "fortnitestorm.herokuapp.com";
constexpr bool INGAME = true;

#ifdef READFILE // man that 1kb is important ok :/
std::string myText;
std::string hybridyesorno;
std::ifstream MyReadFile("info.txt");
#endif

#ifndef READFILE
constexpr bool bIsHybrid = true;
#endif
#ifdef READFILE
bool bIsHybrid;
#endif
