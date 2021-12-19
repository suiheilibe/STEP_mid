#include <windows.h>

#include <cassert>

#include "STEPMidTls.h"
#include "debug.h"

bool STEPMidTls::initialize()
{
    assert(dwTlsIndex == TLS_OUT_OF_INDEXES);

    if ((dwTlsIndex = ::TlsAlloc()) == TLS_OUT_OF_INDEXES) {
        return false;
    }

    return true;
}

bool STEPMidTls::deinitialize()
{
    assert(dwTlsIndex != TLS_OUT_OF_INDEXES);

    return (::TlsFree(dwTlsIndex) == TRUE);
}

void* STEPMidTls::allocAndSet(size_t size)
{
    assert(dwTlsIndex != TLS_OUT_OF_INDEXES);

    void *ptr = (void *)::LocalAlloc(LPTR, (SIZE_T)size);
    if (ptr != nullptr) {
        ::TlsSetValue(dwTlsIndex, (LPVOID)ptr);
    }
    return ptr;
}

void* STEPMidTls::get()
{
    assert(dwTlsIndex != TLS_OUT_OF_INDEXES);

    return (void *)::TlsGetValue(dwTlsIndex);
}

void STEPMidTls::free(void *ptr) {
    ::LocalFree((HLOCAL)ptr);
}