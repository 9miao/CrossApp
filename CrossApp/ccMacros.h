

#ifndef __CCMACROS_H__
#define __CCMACROS_H__

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "platform/CACommon.h"
#include "CCStdC.h"

#ifndef CCAssert
#if CROSSAPP_DEBUG > 0
#define CCAssert(cond, msg) do {								\
if (!(cond)) {													\
if (strlen(msg))												\
	CrossApp::CCLog("Assert failed: %s", msg);					\
	CC_ASSERT(cond);											\
} \
} while (0)
#else
#define CCAssert(cond, msg) ((void)(cond))
#endif
#endif  // CCAssert

#define GP_ASSERT(cond) CCAssert(cond, "")
#define CCASSERT(cond, msg) CCAssert(cond, msg)

#include "ccConfig.h"

/** @def CC_SWAP
simple macro that swaps 2 variables
*/
#define CC_SWAP(x, y, type)    \
{    type temp = (x);        \
    x = y; y = temp;        \
}


/** @def CCRANDOM_MINUS1_1
 returns a random float between -1 and 1
 */
#define CCRANDOM_MINUS1_1() ((2.0f*((float)rand()/RAND_MAX))-1.0f)

/** @def CCRANDOM_0_1
 returns a random float between 0 and 1
 */
#define CCRANDOM_0_1() ((float)rand()/RAND_MAX)

/** @def CC_DEGREES_TO_RADIANS
 converts degrees to radians
 */
#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180

/** @def CC_RADIANS_TO_DEGREES
 converts radians to degrees
 */
#define CC_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

#define kCCRepeatForever (UINT_MAX -1)

/** @def CC_BLEND_SRC
default gl blend src function. Compatible with premultiplied alpha images.
*/
#define CC_BLEND_SRC GL_ONE
#define CC_BLEND_DST GL_ONE_MINUS_SRC_ALPHA


#define SET_DIRTY_RECURSIVELY(__BATCH__) {                                       \
if (__BATCH__ && ! m_bRecursiveDirty)      \
{                                               \
m_bRecursiveDirty = true;                   \
setDirty(true);                             \
if (m_bHasChildren)                         \
setDirtyRecursively(true);              \
}                                               \
}

#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F
#endif // FLT_EPSILON

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
            TypeName(const TypeName&);\
            void operator=(const TypeName&)

/**
Helper macros which converts 4-byte little/big endian 
integral number to the machine native number representation
 
It should work same as apples CFSwapInt32LittleToHost(..)
*/

/// when define returns true it means that our architecture uses big endian
#define CC_HOST_IS_BIG_ENDIAN (bool)(*(unsigned short *)"\0\xff" < 0x100) 
#define CC_SWAP32(i)  ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#define CC_SWAP16(i)  ((i & 0x00ff) << 8 | (i &0xff00) >> 8)   
#define CC_SWAP_INT32_LITTLE_TO_HOST(i) ((CC_HOST_IS_BIG_ENDIAN == true)? CC_SWAP32(i) : (i) )
#define CC_SWAP_INT16_LITTLE_TO_HOST(i) ((CC_HOST_IS_BIG_ENDIAN == true)? CC_SWAP16(i) : (i) )
#define CC_SWAP_INT32_BIG_TO_HOST(i)    ((CC_HOST_IS_BIG_ENDIAN == true)? (i) : CC_SWAP32(i) )
#define CC_SWAP_INT16_BIG_TO_HOST(i)    ((CC_HOST_IS_BIG_ENDIAN == true)? (i):  CC_SWAP16(i) )

/**********************/
/** Profiling Macros **/
/**********************/
#if CC_ENABLE_PROFILERS

#define CC_PROFILER_DISPLAY_TIMERS() CCProfiler::sharedProfiler()->displayTimers()
#define CC_PROFILER_PURGE_ALL() CCProfiler::sharedProfiler()->releaseAllTimers()

#define CC_PROFILER_START(__name__) CCProfilingBeginTimingBlock(__name__)
#define CC_PROFILER_STOP(__name__) CCProfilingEndTimingBlock(__name__)
#define CC_PROFILER_RESET(__name__) CCProfilingResetTimingBlock(__name__)

#define CC_PROFILER_START_CATEGORY(__cat__, __name__) do{ if(__cat__) CCProfilingBeginTimingBlock(__name__); } while(0)
#define CC_PROFILER_STOP_CATEGORY(__cat__, __name__) do{ if(__cat__) CCProfilingEndTimingBlock(__name__); } while(0)
#define CC_PROFILER_RESET_CATEGORY(__cat__, __name__) do{ if(__cat__) CCProfilingResetTimingBlock(__name__); } while(0)


#else

#define CC_PROFILER_DISPLAY_TIMERS() do {} while (0)
#define CC_PROFILER_PURGE_ALL() do {} while (0)

#define CC_PROFILER_START(__name__)  do {} while (0)
#define CC_PROFILER_STOP(__name__) do {} while (0)
#define CC_PROFILER_RESET(__name__) do {} while (0)

#define CC_PROFILER_START_CATEGORY(__cat__, __name__) do {} while(0)
#define CC_PROFILER_STOP_CATEGORY(__cat__, __name__) do {} while(0)
#define CC_PROFILER_RESET_CATEGORY(__cat__, __name__) do {} while(0)

#define CC_PROFILER_START_INSTANCE(__id__, __name__) do {} while(0)
#define CC_PROFILER_STOP_INSTANCE(__id__, __name__) do {} while(0)
#define CC_PROFILER_RESET_INSTANCE(__id__, __name__) do {} while(0)

#endif

#if !defined(CROSSAPP_DEBUG) || CROSSAPP_DEBUG == 0
#define CHECK_GL_ERROR_DEBUG()
#else
#define CHECK_GL_ERROR_DEBUG() \
    do { \
        GLenum __error = glGetError(); \
        if(__error) { \
            CCLog("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)
#endif

/*******************/
/** Notifications **/
/*******************/
/** @def CACustomAnimationFrameDisplayedNotification
 Notification name when a CAImageFrame is displayed
 */
#define CACustomAnimationFrameDisplayedNotification "CACustomAnimationFrameDisplayedNotification"

/**
* This header is used for defining event types using in CANotificationCenter
*/

// The application will come to foreground.
// This message is used for reloading resources before come to foreground on Android.
// This message is posted in main.cpp.
#define EVENT_COME_TO_FOREGROUND    "event_come_to_foreground"

// The application will come to background.
// This message is used for doing something before coming to background, such as save CARenderImage.
// This message is posted in CrossApp/platform/android/jni/MessageJni.cpp.
#define EVENT_COME_TO_BACKGROUND    "event_come_to_background"

// new callbacks based on C++11
#define CALLBACK_BIND_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CALLBACK_BIND_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CALLBACK_BIND_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CALLBACK_BIND_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)
#define CALLBACK_BIND_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)
#define CALLBACK_BIND_5(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, ##__VA_ARGS__)

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define LINE_WIDTH MAX(s_px_to_dip(2.0f) + 0.01f, 2.01f)
#else
#define LINE_WIDTH MAX(s_px_to_dip(1.0f) + 0.01f, 1.01f)
#endif


#if CC_NODE_RENDER_SUBPIXEL
#define RENDER_IN_SUBPIXEL
#else
#define RENDER_IN_SUBPIXEL(__ARGS__) (ceil(__ARGS__))
#endif

#if defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 4))) \
	|| (defined(__clang__) && (__clang_major__ >= 3)) || (_MSC_VER >= 1800)
#define M_DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &) = delete; \
	TypeName &operator =(const TypeName &) = delete;
#else
#define M_DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &); \
	TypeName &operator =(const TypeName &);
#endif


#define INCREMENT_GL_DRAWS(__n__) CrossApp::CAApplication::getApplication()->getRenderer()->addDrawnBatches(__n__)
#define INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(__drawcalls__, __vertices__) \
do {                                                                \
auto __renderer__ = CrossApp::CAApplication::getApplication()->getRenderer();     \
__renderer__->addDrawnBatches(__drawcalls__);                   \
__renderer__->addDrawnVertices(__vertices__);                   \
} while(0)



#if !defined(M_REQUIRES_NULL_TERMINATION)
#if defined(__APPLE_CC__) && (__APPLE_CC__ >= 5549)
#define M_REQUIRES_NULL_TERMINATION __attribute__((sentinel(0,1)))
#elif defined(__GNUC__)
#define M_REQUIRES_NULL_TERMINATION __attribute__((sentinel))
#else
#define M_REQUIRES_NULL_TERMINATION
#endif
#endif

#define TagInvalid -1
#endif // __CCMACROS_H__
