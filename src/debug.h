#ifndef _STEP_MID_DEBUG_H_
#define _STEP_MID_DEBUG_H_

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>

#if defined(DEBUG) || defined(_DEBUG)
#define DEBUGOUT(fmt, ...) _debugOut(__func__, _T(fmt), ##__VA_ARGS__)
#else
#define DEBUGOUT(fmt, ...)
#endif

inline void _debugOut(LPCSTR fnname, LPCTSTR fmt, ...)
{
    static const int BUF_SIZE = 256;
    va_list	argp;
    TCHAR buf[BUF_SIZE];
    TCHAR fmtBuf[BUF_SIZE];
#if defined(UNICODE) || defined(_UNICODE)
    MultiByteToWideChar(CP_ACP, 0, fnname, -1, fmtBuf, BUF_SIZE);
#else
    strncpy(fmtBuf, fnname, BUF_SIZE);
#endif
    _tcsncat(fmtBuf, _T(": "), BUF_SIZE);
    _tcsncat(fmtBuf, fmt, BUF_SIZE);
    va_start(argp, fmt);
    _vstprintf_s(buf, BUF_SIZE - 1, fmtBuf, argp);
    va_end(argp);
    OutputDebugString(buf);
}

#endif