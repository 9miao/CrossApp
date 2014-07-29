

#ifndef __CC_COMMON_H__
#define __CC_COMMON_H__

#include "CCPlatformMacros.h"

NS_CC_BEGIN

/**
 * @addtogroup platform
 * @{
 */

/// The max length of CCLog message.
static const int kMaxLogLen = 16*1024;

/**
@brief Output Debug message.
*/
void CC_DLL CCLog(const char * pszFormat, ...) CC_FORMAT_PRINTF(1, 2);

/**
 * lua can not deal with ...
 */
void CC_DLL CCLuaLog(const char * pszFormat);

/**
@brief Pop out a message box
*/
void CC_DLL CCMessageBox(const char * pszMsg, const char * pszTitle);

/**
@brief Enum the language type supported now
*/
typedef enum LanguageType
{
    kLanguageEnglish = 0,
    kLanguageChinese,
    kLanguageFrench,
    kLanguageItalian,
    kLanguageGerman,
    kLanguageSpanish,
    kLanguageDutch,
    kLanguageRussian,
    kLanguageKorean,
    kLanguageJapanese,
    kLanguageHungarian,
    kLanguagePortuguese,
    kLanguageArabic
} ccLanguageType;

// end of platform group
/// @}

NS_CC_END

#endif    // __CC_COMMON_H__
