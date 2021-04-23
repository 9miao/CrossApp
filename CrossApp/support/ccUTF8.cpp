

#include "ccUTF8.h"
#include "platform/CACommon.h"
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
    || (ch >= 0x31C0 && ch <= 0x4DFF);  // Other extensions
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


template <typename T>
struct ConvertTrait {
    typedef T ArgType;
};
template <>
struct ConvertTrait<char> {
    typedef UTF8 ArgType;
};
template <>
struct ConvertTrait<char16_t> {
    typedef UTF16 ArgType;
};
template <>
struct ConvertTrait<char32_t> {
    typedef UTF32 ArgType;
};

template <typename From, typename To, typename FromTrait = ConvertTrait<From>, typename ToTrait = ConvertTrait<To>>
bool utfConvert(const std::basic_string<From>& from, std::basic_string<To>& to, ConversionResult(*cvtfunc)(const typename FromTrait::ArgType**, const typename FromTrait::ArgType*, typename ToTrait::ArgType**, typename ToTrait::ArgType*, ConversionFlags)
                )
{
    static_assert(sizeof(From) == sizeof(typename FromTrait::ArgType), "Error size mismatched");
    static_assert(sizeof(To) == sizeof(typename ToTrait::ArgType), "Error size mismatched");
    
    if (from.empty())
    {
        to.clear();
        return true;
    }
    
    // See: http://unicode.org/faq/utf_bom.html#gen6
    static const int most_bytes_per_character = 4;
    
    const size_t maxNumberOfChars = from.length(); // all UTFs at most one element represents one character.
    const size_t numberOfOut = maxNumberOfChars * most_bytes_per_character / sizeof(To);
    
    std::basic_string<To> working(numberOfOut, 0);
    
    auto inbeg = reinterpret_cast<const typename FromTrait::ArgType*>(&from[0]);
    auto inend = inbeg + from.length();
    
    
    auto outbeg = reinterpret_cast<typename ToTrait::ArgType*>(&working[0]);
    auto outend = outbeg + working.length();
    auto r = cvtfunc(&inbeg, inend, &outbeg, outend, strictConversion);
    if (r != conversionOK)
        return false;
    
    working.resize(reinterpret_cast<To*>(outbeg) - &working[0]);
    to = std::move(working);
    
    return true;
};


bool UTF8ToUTF16(const std::string& inUtf8, std::u16string& outUtf16)
{
    return utfConvert(inUtf8, outUtf16, ConvertUTF8toUTF16);
}

bool UTF8ToUTF32(const std::string& inUtf8, std::u32string& outUtf32)
{
    return utfConvert(inUtf8, outUtf32, ConvertUTF8toUTF32);
}

bool UTF16ToUTF8(const std::u16string& inUtf16, std::string& outUtf8)
{
    return utfConvert(inUtf16, outUtf8, ConvertUTF16toUTF8);
}

bool UTF16ToUTF32(const std::u16string& inUtf16, std::u32string& outUtf32)
{
    return utfConvert(inUtf16, outUtf32, ConvertUTF16toUTF32);
}

bool UTF32ToUTF8(const std::u32string& inUtf32, std::string& outUtf8)
{
    return utfConvert(inUtf32, outUtf8, ConvertUTF32toUTF8);
}

bool UTF32ToUTF16(const std::u32string& inUtf32, std::u16string& outUtf16)
{
    return utfConvert(inUtf32, outUtf16, ConvertUTF32toUTF16);
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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string getStringUTFCharsJNI(JNIEnv* env, jstring srcjStr, bool* ret)
    {
        std::string utf8Str;
        const unsigned short * unicodeChar = ( const unsigned short *)env->GetStringChars(srcjStr, nullptr);
        size_t unicodeCharLength = env->GetStringLength(srcjStr);
        const std::u16string unicodeStr((const char16_t *)unicodeChar, unicodeCharLength);
        bool flag = UTF16ToUTF8(unicodeStr, utf8Str);
        
        if (ret)
        {
            *ret = flag;
        }
        
        if (!flag)
        {
            utf8Str = "";
        }
        env->ReleaseStringChars(srcjStr, unicodeChar);
        return utf8Str;
    }
    
    jstring newStringUTFJNI(JNIEnv* env, const std::string& utf8Str, bool* ret)
    {
        std::u16string utf16Str;
        bool flag = CrossApp::StringUtils::UTF8ToUTF16(utf8Str, utf16Str);
        
        if (ret)
        {
            *ret = flag;
        }
        
        if(!flag)
        {
            utf16Str.clear();
        }
        jstring stringText = env->NewString((const jchar*)utf16Str.data(), utf16Str.length());
        return stringText;
    }
#endif
    
} //namespace StringUtils {

NS_CC_END
