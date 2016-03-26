#ifndef OS_H
#define OS_H

#ifdef _WIN32
#include <windows.h>
#include <ShellApi.h>
#endif

#ifdef linux
#include <unistd.h>
#endif

#endif
