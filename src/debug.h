#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

inline void debugOut(LPCTSTR pszFormat, ...)
{
    va_list	argp;
    TCHAR pszBuf[256];
    va_start(argp, pszFormat);
    _vstprintf_s(pszBuf, 255, pszFormat, argp);
    va_end(argp);
    OutputDebugString(pszBuf);
}

#endif