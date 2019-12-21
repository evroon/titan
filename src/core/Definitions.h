#define UNKNOWN 0
#define LINUX 1
#define WINDOWS 2
#define ANDROID 3
#define MACOSX 4

#ifdef __linux__
#define PLATFORM LINUX
#elif defined(_WIN64)
#define PLATFORM WINDOWS
#elif __ANDROID__
#define PLATFORM ANDROID
#elif __APPLE__
#define PLATFORM MACOSX
#else
#define PLATFORM UNKNOWN
#endif
