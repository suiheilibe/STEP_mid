#ifndef STEP_MID_STEPMIDTLS_H_
#define STEP_MID_STEPMIDTLS_H_

#include <windows.h>

class STEPMidTls {
public:
    // Must be called in the DllMain
    static bool initialize();
    // Must be called in the DllMain
    static bool deinitialize();
    // Must be called in the DllMain
    // Allocated memory will be zero filled
    static void* allocAndSet(size_t size);
    static void* get();
    // Must be called in the DllMain
    static void free(void *ptr);

private:
    static DWORD tlsIndex;
};

#endif