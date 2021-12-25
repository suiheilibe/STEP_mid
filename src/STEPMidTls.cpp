#include <windows.h>

#include <cassert>

#include "STEPMidTls.h"
#include "debug.h"

DWORD STEPMidTls::tlsIndex = TLS_OUT_OF_INDEXES;

bool STEPMidTls::initialize()
{
    assert(tlsIndex == TLS_OUT_OF_INDEXES);

    if ((tlsIndex = ::TlsAlloc()) == TLS_OUT_OF_INDEXES) {
        DEBUGOUT("STEPMidTls::initialize() failed\n");
        return false;
    }

    DEBUGOUT("STEPMidTls::initialize() succeeded\n");

    return true;
}

bool STEPMidTls::deinitialize()
{
    assert(tlsIndex != TLS_OUT_OF_INDEXES);

    if (::TlsFree(tlsIndex) == TRUE) {
        DEBUGOUT("STEPMidTls::deinitialize() succeeded\n");
        return true;
    } else {
        DEBUGOUT("STEPMidTls::deinitialize() failed\n");
        return false;
    }
}

void* STEPMidTls::allocAndSet(size_t size)
{
    assert(tlsIndex != TLS_OUT_OF_INDEXES);

    void *ptr = (void *)::LocalAlloc(LPTR, (SIZE_T)size);
    if (ptr != nullptr) {
        DEBUGOUT("STEPMidTls::allocAndSet(): LocalAlloc() failed\n");
        BOOL tlsSetResult = ::TlsSetValue(tlsIndex, (LPVOID)ptr);
        if (tlsSetResult != TRUE) {
            if (::LocalFree((HLOCAL)ptr) != NULL) {
                DEBUGOUT("STEPMidTls::allocAndSet(): LocalFree() failed\n");
            }
            ptr = nullptr;
        }
    }
    return ptr;
}

void* STEPMidTls::get()
{
    assert(tlsIndex != TLS_OUT_OF_INDEXES);

    return (void *)::TlsGetValue(tlsIndex);
}

void STEPMidTls::free(void *ptr) {
    if (::LocalFree((HLOCAL)ptr) != NULL) {
        DEBUGOUT("STEPMidTls::free(): LocalFree() failed\n");
    }
}