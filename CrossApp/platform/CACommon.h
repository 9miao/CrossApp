

#ifndef __CC_COMMON_H__
#define __CC_COMMON_H__

#include "CCPlatformMacros.h"

NS_CC_BEGIN

/// The max length of CCLog message.
static const int kMaxLogLen = 16*1024;

/**
@brief Output Debug message.
*/
void CC_DLL CCLog(const char * pszFormat, ...) CC_FORMAT_PRINTF(1, 2);

/**
@brief Enum the language type supported now
*/
enum class LanguageType
{
    ENGLISH = 0,
    CHINESE,
    FRENCH,
    ITALIAN,
    GERMAN,
    SPANISH,
    DUTCH,
    RUSSIAN,
    KOREAN,
    JAPANESE,
    HUNGARIAN,
    PORTUGUESE,
    ARABIC,
    NORWEGIAN,
    POLISH,
    TURKISH,
    UKRAINIAN,
    ROMANIAN,
    BULGARIAN
};

// end of platform group
/// @}

NS_CC_END

#endif    // __CC_COMMON_H__
