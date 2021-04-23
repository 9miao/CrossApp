//
//  CAFTFontCache.h
//  CrossApp
//
//  Created by Zhujian on 14-8-12.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CC_PLATFORM_CAFTFONTCACHE_H
#define __CC_PLATFORM_CAFTFONTCACHE_H

#include "platform/CACommon.h"
#include "images/CAImage.h"
#include <vector>

NS_CC_BEGIN


typedef struct _TextAttribute
{
	int charSize;
	float charlength;

}TextAttribute;

typedef struct _TextViewLineInfo
{
	std::vector<TextAttribute> TextAttrVect;
	int iStartCharPos;
	int iEndCharPos;

}TextViewLineInfo;

struct FontDataTable;

class CC_DLL CAFTFontCache
{
	friend class CAFTRichFont;
public:
	CAFTFontCache();
	virtual ~CAFTFontCache();

	static CAFTFontCache& getAFTFontCache();

	int getFontHeight(const char* pFontName, unsigned long nSize);

	int getStringWidth(const char* pFontName, unsigned long nSize, const std::string& text, bool bBold = false, bool bItalics = false);

	int cutStringByWidth(const char* pFontName, unsigned long nSize, const std::string& text, int iLimitWidth, int& cutWidth, bool bBold = false, bool bItalics = false);

	int cutStringByDSize(std::string& text, const DSize& lableSize, const char* pFontName, unsigned long nSize, bool bWordWrap = true, int iLineSpacing = 0, bool bBold = false, bool bItalics = false);

	int getStringHeight(const char* pFontName, unsigned long nSize, const std::string& text, int iLimitWidth, int iLineSpace, bool bWordWrap);

	CAImage* initWithString(const char* pText, const CAColor4B& fontColor, const char* pFontName, int nSize, int width, int height,
		CATextAlignment hAlignment, CAVerticalTextAlignment vAlignment, bool bWordWrap = true, int iLineSpacing = 0, bool bBold = false, bool bItalics = false, bool bUnderLine = false, bool bDeleteLine = false, std::vector<TextViewLineInfo>* pLinesText = 0);

    void destroyAllFontData();

protected:
	void initDefaultFont();


	void setCurrentFontData(const char* pFontName, int nSize);

	FontDataTable* m_pCurFontData;

	std::vector<FontDataTable*> m_FontDataVect;
};


#define g_AFTFontCache CAFTFontCache::getAFTFontCache()


NS_CC_END

#endif