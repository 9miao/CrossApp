

#ifndef __cocos2dx__ccUTF8__
#define __cocos2dx__ccUTF8__

#include "platform/CCPlatformMacros.h"
#include <vector>
#include <string>
#include "ConvertUTF.h"

NS_CC_BEGIN

#define UTF8(x) unicode_to_utf8( L##x ).c_str()
inline int _unicode_to_utf8( const wchar_t *in, std::string& out , int *poutsize )
{
    int outsize = 0;
    int charscount = 0;
    char *result = NULL;
    char *tmp = NULL;
    
    charscount = wcslen( in );
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
    *poutsize = outsize;
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
	int outsize;
	_unicode_to_utf8( in , s , &outsize );
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
 *  @brief Converts utf8 string to utf16 string
 *  @param utf8 The utf8 string to be converted
 *  @param outUtf16 The output utf16 string
 *  @return true if succeed, otherwise false
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
CC_DLL bool UTF8ToUTF16(const std::string& utf8, std::u16string& outUtf16);

/**
 *  @brief Converts utf16 string to utf8 string
 *  @param utf16 The utf16 string to be converted
 *  @param outUtf8 The output utf8 string
 *  @return true if succeed, otherwise false
 *  @note Please check the return value before using \p outUtf8
 *  e.g.
 *  @code
 *    std::string utf8;
 *    bool ret = StringUtils::UTF16ToUTF8(u"\u4f60\u597d", utf16);
 *    if (ret) {
 *        do_some_thing_with_utf8(utf8);
 *    }
 *  @endcode
 */
CC_DLL bool UTF16ToUTF8(const std::u16string& utf16, std::string& outUtf8);

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

} // namespace StringUtils {

/**
 * Returns the character count in UTF16 string
 * @param str pointer to the start of a UTF-16 encoded string. It must be an NULL terminal UTF8 string.
 * @deprecated Please use c++11 `std::u16string::length` instead, don't use `unsigned short*` directly
 */
CC_DEPRECATED_ATTRIBUTE CC_DLL int cc_wcslen(const unsigned short* str);

/** Trims the space characters at the end of UTF8 string 
 *  @deprecated Please use `StringUtils::trimUTF16Vector` instead
 */

CC_DEPRECATED_ATTRIBUTE void cc_utf8_trim_ws(std::vector<unsigned short>* str);

/**
 * Whether the character is a whitespace character.
 *
 * @param ch    the unicode character
 * @returns     whether the character is a white space character.
 * @deprecated Please use `StringUtils::isUnicodeSpace` instead
 *
 * @see http://en.wikipedia.org/wiki/Whitespace_character#Unicode
 * */
CC_DEPRECATED_ATTRIBUTE bool isspace_unicode(unsigned short ch);

/**
 * Whether the character is a Chinese/Japanese/Korean character.
 *
 * @param ch    the unicode character
 * @returns     whether the character is a Chinese character.
 * @deprecated Please use `StringUtils::isCJKUnicode` instead
 *
 * @see http://www.searchtb.com/2012/04/chinese_encode.html
 * @see http://tieba.baidu.com/p/748765987
 * */
CC_DEPRECATED_ATTRIBUTE bool iscjk_unicode(unsigned short ch);

/**
 * Returns the length of the string in characters.
 *
 * @param p pointer to the start of a UTF-8 encoded string. It must be an NULL terminal UTF8 string.
 * @param max Not used from 3.1, just keep it for backward compatibility
 * @deprecated Please use `StringUtils::getCharacterCountInUTF8String` instead
 * @returns the length of the string in characters
 **/
CC_DEPRECATED_ATTRIBUTE long cc_utf8_strlen (const char * p, int max = -1);

/**
 * Find the last character that is not equal to the character given.
 *
 * @param str   the string to be searched.
 * @param c     the character to be searched for.
 * @deprecated Please use `StringUtils::getIndexOfLastNotChar16` instead
 * @returns the index of the last character that is not \p c.
 * */
CC_DEPRECATED_ATTRIBUTE unsigned int cc_utf8_find_last_not_char(const std::vector<unsigned short>& str, unsigned short c);

/**
 *  @brief Gets `unsigned short` vector from a given utf16 string
 *  @deprecated Please use `StringUtils::getChar16VectorFromUTF16String` instead
 */
CC_DEPRECATED_ATTRIBUTE std::vector<unsigned short> cc_utf16_vec_from_utf16_str(const unsigned short* str);

/**
 * Creates an utf8 string from a c string. The result will be null terminated.
 *
 * @param str_old pointer to the start of a C string. It must be an NULL terminal UTF8 string.
 * @param length  not used from 3.1, keep it just for backward compatibility
 * @param rUtf16Size The character count in the return UTF16 string.
 * @deprecated Please use `StringUtils::UTF8ToUTF16` instead
 * @returns the newly created utf16 string, it must be released with `delete[]`,
 *          If an error occurs, %NULL will be returned.
 * */
CC_DEPRECATED_ATTRIBUTE CC_DLL unsigned short* cc_utf8_to_utf16(const char* str_old, int length = -1, int* rUtf16Size = NULL);

/**
 * Converts a string from UTF-16 to UTF-8. The result will be null terminated.
 *
 * @param utf16 an UTF-16 encoded string, It must be an NULL terminal UTF16 string.
 * @param len not used from 3.1, keep it just for backward compatibility
 * @param items_read     not used from 3.1, keep it just for backward compatibility
 * @param items_written  not used from 3.1, keep it just for backward compatibility
 * @deprecated Please use `StringUtils::UTF16ToUTF8` instead
 * @returns a pointer to a newly allocated UTF-8 string. This value must be
 *          released with `delete[]`. If an error occurs, %NULL will be returned.
 **/
CC_DEPRECATED_ATTRIBUTE CC_DLL char * cc_utf16_to_utf8 (const unsigned short  *str,
                  int             len = -1,
				  long            *items_read = NULL,
                  long            *items_written = NULL);


NS_CC_END

#endif /* defined(__cocos2dx__ccUTF8__) */
