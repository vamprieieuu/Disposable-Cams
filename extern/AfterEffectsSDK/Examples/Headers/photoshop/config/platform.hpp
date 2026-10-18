#ifndef PHOTOSHOP_CONFIG_PLATFORM_HPP
#define PHOTOSHOP_CONFIG_PLATFORM_HPP

#if defined(_WIN32) || defined(WIN32) || defined(MSWindows)
    #define PS_OS_WINDOWS 1
    #define PS_PLATFORM_APPLE 0
    #define PS_OS_MAC 0
    #define PS_OS_IOS 0
#elif defined(__APPLE__)
    #define PS_OS_WINDOWS 0
    #define PS_PLATFORM_APPLE 1
    #define PS_OS_MAC 1
    #define PS_OS_IOS 0
#else
    #define PS_OS_WINDOWS 0
    #define PS_PLATFORM_APPLE 0
    #define PS_OS_MAC 0
    #define PS_OS_IOS 0
#endif

#endif /* PHOTOSHOP_CONFIG_PLATFORM_HPP */
