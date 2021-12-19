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

bool STEPMidTls::deinitialize() {
    assert(dwTlsIndex != TLS_OUT_OF_INDEXES);

    return (::TlsFree(dwTlsIndex) == TRUE);
}