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
    strncpy_s(fmtBuf, BUF_SIZE, fnname, _TRUNCATE);
#endif
    _tcsncat_s(fmtBuf, BUF_SIZE, _T(": "), _TRUNCATE);
    _tcsncat_s(fmtBuf, BUF_SIZE, fmt, _TRUNCATE);
    va_start(argp, fmt);
    _vstprintf_s(buf, BUF_SIZE, fmtBuf, argp);
    va_end(argp);
    OutputDebugString(buf);
}

#endif