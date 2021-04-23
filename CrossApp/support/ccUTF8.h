

#ifndef __CrossApp__ccUTF8__
#define __CrossApp__ccUTF8__

#include "platform/CCPlatformMacros.h"
#include <vector>
#include <string>
#include "ConvertUTF.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif
NS_CC_BEGIN

#define UTF8(x) unicode_to_utf8( L##x ).c_str()
inline int _unicode_to_utf8( const wchar_t *in, std::string& out)
{
    int outsize = 0;
    int charscount = 0;
    char *result = NULL;
    char *tmp = NULL;
    
    charscount = (int)wcslen( in );
    result = (char *)new char[charscount * 3 + 1];
    memset(result, 0, charscount * 3 + 1);
    tmp = result;
    
    for (int i = 0; i < charscount; i++)
    {
        wchar_t unicode = in[i];
        unsigned int i_unicode = (unsigned int)unicode;
        if (/*i_unicode >= 0x0000 && */i_unicode <= 0x007f)
        {
            *tmp = (char)unicode;
            tmp += 1;
            outsize += 1;
        }
        else if (i_unicode >= 0x0080 && i_unicode <= 0x07ff)
        {
            *tmp = 0xc0 | (unicode >> 6);
            tmp += 1;
            *tmp = 0x80 | (unicode & (0xff >> 2));
            tmp += 1;
            outsize += 2;
        }
        else if (i_unicode >= 0x0800 && i_unicode <= 0xffff)
        {
            *tmp = 0xe0 | (unicode >> 12);
            tmp += 1;
            *tmp = 0x80 | (unicode >> 6 & 0x003f);
            tmp += 1;
            *tmp = 0x80 | (unicode & (0xff >> 2));
            tmp += 1;
            outsize += 3;
        }
    }

    *tmp = '\0';
    out = result;
    out.resize(outsize);
    for (int i=0; i<outsize; i++)
    {
        out[i] = result[i];
    }
    delete [] result;
    return 1;
}
inline int _utf8_to_unicode( const char *in, wchar_t **out, int *outsize )
{
	unsigned char *p = (unsigned char*)in;
    wchar_t *result = NULL;
    int resultsize = 0;
    unsigned char *tmp = NULL;
    
    result = (wchar_t *)new wchar_t[strlen(in) +1];
	*out = result;
    memset(result, 0, strlen(in) * sizeof(wchar_t) + sizeof(wchar_t));
    tmp = (unsigned char *)result;
    
    while(*p)
    {
    	unsigned int i_p = (unsigned int)*p;
        if (/*i_p >= 0x00 && */i_p <= 0x7f)
        {
            *tmp = *p;
            tmp++;
            *tmp = '\0';
			tmp++;
            resultsize += 2;
            if ( sizeof( wchar_t ) == 4 )
            {
                *tmp = '\0';
                tmp++;
                *tmp = '\0';
                tmp++;
                resultsize += 2;
            }
        }
        else if ((i_p & (0xff << 5))== 0xc0)
        {
            unsigned char t1 = 0;
            unsigned char t2 = 0;
            
            t1 = *p & (0xff >> 3);
            p++;
			if( ((*p) & 0x80) != 0x80 )
				return 0;
            t2 = *p & (0xff >> 2);
            
            *tmp = t2 | ((t1 & (0xff >> 6)) << 6);//t1 >> 2;
            tmp++;
            
            *tmp = t1 >> 2;//t2 | ((t1 & (0xff >> 6)) << 6);
            tmp++;
            
            resultsize += 2;
            if ( sizeof( wchar_t ) == 4 )
            {
                *tmp = '\0';
                tmp++;
                *tmp = '\0';
                tmp++;
                resultsize += 2;
            }
        }
        else if ((*p & (0xff << 4))== 0xe0)
        {
            unsigned char t1 = 0;
            unsigned char t2 = 0;
            unsigned char t3 = 0;
            
            t1 = *p & (0xff >> 3);
            p++;
			if( ((*p) & 0x80) != 0x80 )
				return 0;
            t2 = *p & (0xff >> 2);
            p++;
			if( ((*p) & 0x80) != 0x80 )
			{
				if( (*p) == 13 )
				{
					p++;
					if( (*p) == 10 )
					{
						p++;
						if( (*p) == 32 )
							p++;
					}
				}
				else
					return 0;
			}
            t3 = *p & (0xff >> 2);
            
            //Little Endian
            *tmp = ((t2 & (0xff >> 6)) << 6) | t3;//(t1 << 4) | (t2 >> 2);
            tmp++;
            
            *tmp = (t1 << 4) | (t2 >> 2);//((t2 & (0xff >> 6)) << 6) | t3;
            tmp++;
            resultsize += 2;
            if ( sizeof( wchar_t ) == 4 )
            {
                *tmp = '\0';
                tmp++;
                *tmp = '\0';
                tmp++;
                resultsize += 2;
            }
        }
        
        p++;
    }
    
    *tmp = '\0';
    tmp++;
    *tmp = '\0';
    tmp++;
    resultsize += 2;
    if ( sizeof( wchar_t ) == 4 )
    {
        *tmp = '\0';
        tmp++;
        *tmp = '\0';
        tmp++;
        resultsize += 2;
    }
    
    *outsize = resultsize;
    return 1;
}
std::wstring inline utf8_to_unicode( const char *in )
{
	wchar_t *str;
	int size;
	if( !_utf8_to_unicode( in , &str , &size ) )
	{
		delete[] str;
		return L"";
	}
	std::wstring s = str;
	delete[] str;
	return s;
}

std::string inline unicode_to_utf8( const wchar_t *in )
{
    std::string s;
	_unicode_to_utf8( in , s);
	return s;
}

std::string inline utf8_to_url_string( const char *in )
{
	std::string str = in;
	std::string str2;
	char buf[64];
	for ( std::string::size_type i = 0 ; i < str.size() ; i ++ )
	{
		if ( str[i] < 0 )
		{
			sprintf( buf , "%x" , (int)str[i] );
			str2 += std::string("%") + buf[6] + buf[7];
		}
		else str2 += str[i];
	}
	return str2;
}

namespace StringUtils {


/**
 *  @brief Converts from UTF8 string to UTF16 string.
 *
 *  This function resizes \p outUtf16 to required size and
 *  fill its contents with result UTF16 string if conversion success.
 *  If conversion fails it guarantees not to change \p outUtf16.
 *
 *  @param inUtf8 The source UTF8 string to be converted from.
 *  @param outUtf16 The output string to hold the result UTF16s.
 *  @return True if succeed, otherwise false.
 *  @note Please check the return value before using \p outUtf16
 *  e.g.
 *  @code
 *    std::u16string utf16;
 *    bool ret = StringUtils::UTF8ToUTF16("你好hello", utf16);
 *    if (ret) {
 *        do_some_thing_with_utf16(utf16);
 *    }
 *  @endcode
 */
CC_DLL bool UTF8ToUTF16(const std::string& inUtf8, std::u16string& outUtf16);

/**
 *  @brief Same as \a UTF8ToUTF16 but converts form UTF8 to UTF32.
 *
 *  @see UTF8ToUTF16
 */
CC_DLL bool UTF8ToUTF32(const std::string& inUtf8, std::u32string& outUtf32);

/**
 *  @brief Same as \a UTF8ToUTF16 but converts form UTF16 to UTF8.
 *
 *  @see UTF8ToUTF16
 */
CC_DLL bool UTF16ToUTF8(const std::u16string& inUtf16, std::string& outUtf8);

/**
 *  @brief Same as \a UTF8ToUTF16 but converts form UTF16 to UTF32.
 *
 *  @see UTF8ToUTF16
 */
CC_DLL bool UTF16ToUTF32(const std::u16string& inUtf16, std::u32string& outUtf32);

/**
 *  @brief Same as \a UTF8ToUTF16 but converts form UTF32 to UTF8.
 *
 *  @see UTF8ToUTF16
 */
CC_DLL bool UTF32ToUTF8(const std::u32string& inUtf32, std::string& outUtf8);

/**
 *  @brief Same as \a UTF8ToUTF16 but converts form UTF32 to UTF16.
 *
 *  @see UTF8ToUTF16
 */
CC_DLL bool UTF32ToUTF16(const std::u32string& inUtf32, std::u16string& outUtf16);

/**
 *  @brief Trims the unicode spaces at the end of char16_t vector
 */
CC_DLL void trimUTF16Vector(std::vector<char16_t>& str);

/**
 *  @brief Whether the character is a whitespace character.
 *
 *  @param ch    the unicode character
 *  @returns     whether the character is a white space character.
 *
 *  @see http://en.wikipedia.org/wiki/Whitespace_character#Unicode
 *
 */
CC_DLL bool isUnicodeSpace(char16_t ch);

/**
 *  @brief Whether the character is a Chinese/Japanese/Korean character.
 *
 *  @param ch    the unicode character
 *  @returns     whether the character is a Chinese character.
 *
 *  @see http://www.searchtb.com/2012/04/chinese_encode.html
 *  @see http://tieba.baidu.com/p/748765987
 *
 */
CC_DLL bool isCJKUnicode(char16_t ch);

/**
 *  @brief Returns the length of the string in characters.
 *
 *  @param utf8 an UTF-8 encoded string.
 *  @returns the length of the string in characters
 */
CC_DLL long getCharacterCountInUTF8String(const std::string& utf8);

/**
 *  @brief Gets the index of the last character that is not equal to the character given.
 *
 *  @param str   the string to be searched.
 *  @param c     the character to be searched for.
 *
 *  @returns the index of the last character that is not \p c.
 *
 */
CC_DLL unsigned int getIndexOfLastNotChar16(const std::vector<char16_t>& str, char16_t c);

/**
 *  @brief Gets char16_t vector from a given utf16 string
 */
CC_DLL std::vector<char16_t> getChar16VectorFromUTF16String(const std::u16string& utf16);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    
    /**
     *  @brief convert jstring to utf8 std::string,  same function with env->getStringUTFChars.
     *         because getStringUTFChars can not pass special emoticon
     *  @param env   The JNI Env
     *  @param srcjStr The jstring which want to convert
     *  @param ret   True if the conversion succeeds and the ret pointer isn't null
     *  @returns the result of utf8 string
     */
    CC_DLL std::string getStringUTFCharsJNI(JNIEnv* env, jstring srcjStr, bool* ret = nullptr);
    
    /**
     *  @brief create a jstring with utf8 std::string, same function with env->newStringUTF
     *         because newStringUTF can not convert special emoticon
     *  @param env   The JNI Env
     *  @param srcjStr The std::string which want to convert
     *  @param ret     True if the conversion succeeds and the ret pointer isn't null
     *  @returns the result of jstring,the jstring need to DeleteLocalRef(jstring);
     */
    CC_DLL jstring newStringUTFJNI(JNIEnv* env, const std::string& utf8Str, bool* ret = nullptr);
#endif
    
} // namespace StringUtils {

NS_CC_END

#endif /* defined(__CrossApp__ccUTF8__) */
