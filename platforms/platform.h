#ifdef __GNUC__
    #include "linux_platform.h"
#elif WIN32
    #include "windows_platform.h"
#else
    #error UNSUPPORTED PLATFORM
#endif
