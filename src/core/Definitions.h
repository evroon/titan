#define UNKNOWN 0
#define LINUX 1
#define WINDOWS 2
#define ANDROID 3
#define MACOSX 4

#define APP_TYPE_EDITOR 0
#define APP_TYPE_GAME 1
#define APP_TYPE_SCRIPT 2

#define APP_TYPE APP_TYPE_EDITOR

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

#define TESTING 1