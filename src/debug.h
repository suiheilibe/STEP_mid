#ifndef _STEP_MID_DEBUG_H_
#define _STEP_MID_DEBUG_H_

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#if defined(DEBUG) || defined(_DEBUG)
#define DEBUGOUT(fmt, ...) _debugOut(fmt, __VA_ARGS__)
#else
#define DEBUGOUT(fmt, ...)
#endif

inline void _debugOut(LPCTSTR fmt, ...)
{
    static const int BUF_SIZE = 256;
    va_list	argp;
    TCHAR buf[BUF_SIZE];
    va_start(argp, fmt);
    _vstprintf_s(buf, BUF_SIZE - 1, fmt, argp);
    va_end(argp);
    OutputDebugString(buf);
}

#endif