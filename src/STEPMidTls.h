#ifndef _STEP_MID_STEPMIDTLS_H_
#define _STEP_MID_STEPMIDTLS_H_

#include <windows.h>

class STEPMidTls {
public:
    // Must be called in the DllMain
    bool initialize();
    // Must be called in the DllMain
    bool deinitialize();
    // Must be called in the DllMain
    LPVOID allocAndSet();
    LPVOID get();
    // Must be called in the DllMain
    void free();

private:
    DWORD dwTlsIndex = TLS_OUT_OF_INDEXES;
};

#endif