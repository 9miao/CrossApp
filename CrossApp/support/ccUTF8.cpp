

#include "ccUTF8.h"
#include "platform/CCCommon.h"
#include "ConvertUTF.h"

NS_CC_BEGIN

namespace StringUtils {

/*
 * @str:    the string to search through.
 * @c:        the character to not look for.
 *
 * Return value: the index of the last character that is not c.
 * */
unsigned int getIndexOfLastNotChar16(const std::vector<char16_t>& str, char16_t c)
{
    int len = static_cast<int>(str.size());

    int i = len - 1;
    for (; i >= 0; --i)
        if (str[i] != c) return i;

    return i;
}

/*
 * @str:    the string to trim
 * @index:    the index to start trimming from.
 *
 * Trims str st str=[0, index) after the operation.
 *
 * Return value: the trimmed string.
 * */
static void trimUTF16VectorFromIndex(std::vector<char16_t>& str, int index)
{
    int size = static_cast<int>(str.size());
    if (index >= size || index < 0)
        return;

    str.erase(str.begin() + index, str.begin() + size);
}

/*
 * @ch is the unicode character whitespace?
 *
 * Reference: http://en.wikipedia.org/wiki/Whitespace_character#Unicode
 *
 * Return value: weather the character is a whitespace character.
 * */
bool isUnicodeSpace(char16_t ch)
{
    return  (ch >= 0x0009 && ch <= 0x000D) || ch == 0x0020 || ch == 0x0085 || ch == 0x00A0 || ch == 0x1680
    || (ch >= 0x2000 && ch <= 0x200A) || ch == 0x2028 || ch == 0x2029 || ch == 0x202F
    ||  ch == 0x205F || ch == 0x3000;
}

bool isCJKUnicode(char16_t ch)
{
    return (ch >= 0x4E00 && ch <= 0x9FBF)   // CJK Unified Ideographs
        || (ch >= 0x2E80 && ch <= 0x2FDF)   // CJK Radicals Supplement & Kangxi Radicals
        || (ch >= 0x2FF0 && ch <= 0x30FF)   // Ideographic Description Characters, CJK Symbols and Punctuation & Japanese
        || (ch >= 0x3100 && ch <= 0x31BF)   // Korean
        || (ch >= 0xAC00 && ch <= 0xD7AF)   // Hangul Syllables
        || (ch >= 0xF900 && ch <= 0xFAFF)   // CJK Compatibility Ideographs
        || (ch >= 0xFE30 && ch <= 0xFE4F)   // CJK Compatibility Forms
        || (ch >= 0x31C0 && ch <= 0x4DFF);  // Other exiensions
}

void trimUTF16Vector(std::vector<char16_t>& str)
{
    int len = static_cast<int>(str.size());

    if ( len <= 0 )
        return;

    int last_index = len - 1;

    // Only start trimming if the last character is whitespace..
    if (isUnicodeSpace(str[last_index]))
    {
        for (int i = last_index - 1; i >= 0; --i)
        {
            if (isUnicodeSpace(str[i]))
                last_index = i;
            else
                break;
        }

        trimUTF16VectorFromIndex(str, last_index);
    }
}


bool UTF8ToUTF32(const std::string& utf8, std::u32string& outUtf32)
{
	if (utf8.empty())
	{
		outUtf32.clear();
		return true;
	}

	bool ret = false;

	const size_t utf32Bytes = (utf8.length() + 1) * sizeof(char32_t);
	char32_t* utf32 = (char32_t*)malloc(utf32Bytes);
	memset(utf32, 0, utf32Bytes);

	char* utf32ptr = reinterpret_cast<char*>(utf32);
	const UTF8* error = NULL;

	if (llvm::ConvertUTF8toWide(4, utf8, utf32ptr, error))
	{
		outUtf32 = utf32;
		ret = true;
	}

	free(utf32);

	return ret;
}

bool UTF8ToUTF16(const std::string& utf8, std::u16string& outUtf16)
{
    if (utf8.empty())
    {
        outUtf16.clear();
        return true;
    }

    bool ret = false;
    
    const size_t utf16Bytes = (utf8.length()+1) * sizeof(char16_t);
    char16_t* utf16 = (char16_t*)malloc(utf16Bytes);
    memset(utf16, 0, utf16Bytes);

    char* utf16ptr = reinterpret_cast<char*>(utf16);
    const UTF8* error = NULL;

    if (llvm::ConvertUTF8toWide(2, utf8, utf16ptr, error))
    {
        outUtf16 = utf16;
        ret = true;
    }

    free(utf16);

    return ret;
}

bool UTF16ToUTF8(const std::u16string& utf16, std::string& outUtf8)
{
    if (utf16.empty())
    {
        outUtf8.clear();
        return true;
    }

    return llvm::convertUTF16ToUTF8String(utf16, outUtf8);
}

std::vector<char16_t> getChar16VectorFromUTF16String(const std::u16string& utf16)
{
    std::vector<char16_t> ret;
    size_t len = utf16.length();
    ret.reserve(len);
    for (size_t i = 0; i < len; ++i)
    {
        ret.push_back(utf16[i]);
    }
    return ret;
}

long getCharacterCountInUTF8String(const std::string& utf8)
{
    return getUTF8StringLength((const UTF8*)utf8.c_str());
}

} //namespace StringUtils {


int cc_wcslen(const unsigned short* str)
{
    if (str == NULL)
        return -1;
    int i=0;
    while(*str++) i++;
    return i;
}

void cc_utf8_trim_ws(std::vector<unsigned short>* str)
{
	if (str == NULL)
        return;
    // unsigned short and char16_t are both 2 bytes
    std::vector<char16_t>* ret = reinterpret_cast<std::vector<char16_t>*>(str);
    StringUtils::trimUTF16Vector(*ret);
}

bool isspace_unicode(unsigned short ch)
{
    return StringUtils::isUnicodeSpace(ch);
}


bool iscjk_unicode(unsigned short ch)
{
    return StringUtils::isCJKUnicode(ch);
}


long cc_utf8_strlen (const char * p, int max)
{
    CC_UNUSED_PARAM(max);
	if (p == NULL)
        return -1;
    return StringUtils::getCharacterCountInUTF8String(p);
}

unsigned int cc_utf8_find_last_not_char(const std::vector<unsigned short>& str, unsigned short c)
{
    std::vector<char16_t> char16Vector;
	for (int i = 0; i < str.size(); i++)
	{
		char16Vector.push_back(str[i]);
	}
   
    return StringUtils::getIndexOfLastNotChar16(char16Vector, c);
}

std::vector<unsigned short> cc_utf16_vec_from_utf16_str(const unsigned short* str)
{
    std::vector<unsigned short> str_new;
    
	if (str == NULL)
        return str_new;
    
    int len = cc_wcslen(str);
    
    for (int i = 0; i < len; ++i)
    {
        str_new.push_back(str[i]);
    }
    return str_new;
}

unsigned short* cc_utf8_to_utf16(const char* str_old, int length/* = -1*/, int* rUtf16Size/* = NULL*/)
{
	if (str_old == NULL)
		return NULL;
    
	unsigned short* ret = NULL;
    
    std::u16string outUtf16;
    bool succeed = StringUtils::UTF8ToUTF16(str_old, outUtf16);
    
    if (succeed)
    {
        ret = new unsigned short[outUtf16.length() + 1];
        ret[outUtf16.length()] = 0;
        memcpy(ret, outUtf16.data(), outUtf16.length() * sizeof(unsigned short));
        if (rUtf16Size)
        {
            *rUtf16Size = static_cast<int>(outUtf16.length());
        }
    }
    
    return ret;
}

char * cc_utf16_to_utf8 (const unsigned short  *str,
                  int             len,
                  long            *items_read,
                  long            *items_written)
{
	if (str == NULL)
		return NULL;
    
    
    std::u16string utf16;
    int utf16Len = len < 0 ? cc_wcslen(str) : len;
    
    for (int i = 0; i < utf16Len; ++i)
    {
        utf16.push_back(str[i]);
    }
    
	char* ret = NULL;
    std::string outUtf8;
    bool succeed = StringUtils::UTF16ToUTF8(utf16, outUtf8);
    
    if (succeed)
    {
        ret = new char[outUtf8.length() + 1];
        ret[outUtf8.length()] = '\0';
        memcpy(ret, outUtf8.data(), outUtf8.length());
    }
    
    return ret;
}


NS_CC_END
