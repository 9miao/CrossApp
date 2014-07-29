

#ifndef __CC_PLATFORM_CONFIG_H__
#define __CC_PLATFORM_CONFIG_H__

/**
Config of CrossApp project, per target platform.
*/

//////////////////////////////////////////////////////////////////////////
// pre configure
//////////////////////////////////////////////////////////////////////////

// define supported target platform macro which CC uses.
#define CC_PLATFORM_UNKNOWN            0
#define CC_PLATFORM_IOS                1
#define CC_PLATFORM_ANDROID            2
#define CC_PLATFORM_WIN32              3
#define CC_PLATFORM_LINUX              5
#define CC_PLATFORM_BADA               6
#define CC_PLATFORM_MAC                8
#define CC_PLATFORM_EMSCRIPTEN        10
#define CC_PLATFORM_WINRT             12
#define CC_PLATFORM_WP8               13

// Determine target platform by compile environment macro.
#define CC_TARGET_PLATFORM             CC_PLATFORM_UNKNOWN

// mac
#if defined(CC_TARGET_OS_MAC)
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_MAC
#endif

// iphone
#if defined(CC_TARGET_OS_IPHONE)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM         CC_PLATFORM_IOS
#endif

// android
#if defined(ANDROID)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM         CC_PLATFORM_ANDROID
#endif

// WinRT (Windows Store App)
#if defined(WINRT) && defined(_WINRT)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM			CC_PLATFORM_WINRT
#endif

// WP8 (Windows Phone 8 App)
#if defined(WP8) && defined(_WP8)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM			CC_PLATFORM_WP8
#endif

// win32
#if defined(WIN32) && defined(_WINDOWS)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM         CC_PLATFORM_WIN32
#endif

// linux
#if defined(LINUX)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM         CC_PLATFORM_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_MARMALADE
#endif

// bada
#if defined(SHP)
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_BADA
#endif

// qnx
#if defined(__QNX__)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM     CC_PLATFORM_BLACKBERRY
#endif

// native client
#if defined(__native_client__)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM     CC_PLATFORM_NACL
#endif

// Emscripten
#if defined(EMSCRIPTEN)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM     CC_PLATFORM_EMSCRIPTEN
#endif

// tizen
#if defined(TIZEN)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM     CC_PLATFORM_TIZEN
#endif

//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if ! CC_TARGET_PLATFORM
    #error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma warning (disable:4127)  
#endif  // CC_PLATFORM_WIN32

#endif  // __CC_PLATFORM_CONFIG_H__

