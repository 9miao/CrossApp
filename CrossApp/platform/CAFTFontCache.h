//
//  CAFTFontCache.h
//  CrossApp
//
//  Created by Zhujian on 14-8-12.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CC_PLATFORM_CAFTFONTCACHE_H
#define __CC_PLATFORM_CAFTFONTCACHE_H


#include "platform/CAFreeTypeFont.h"

NS_CC_BEGIN


typedef struct FontDataTable
{
	std::string szFontName;
	int iFontSize;
	int iFontHeight;
	CAFreeTypeFont ftFont;
} FontDataTable;


class CC_DLL CAFTFontCache
{
public:
	CAFTFontCache();
	virtual ~CAFTFontCache();

	static CAFTFontCache& getAFTFontCache();

	int getFontHeight(const char* pFontName, unsigned long nSize);

	int getStringWidth(const char* pFontName, unsigned long nSize, const std::string& text, bool bBold = false, bool bItalics = false);

	int cutStringByWidth(const char* pFontName, unsigned long nSize, const std::string& text, int iLimitWidth, int& cutWidth);

	int getStringHeight(const char* pFontName, unsigned long nSize, const std::string& text, int iLimitWidth, int iLineSpace, bool bWordWrap);

	CAImage* initWithString(const char* pText, const char* pFontName, int nSize, int width, int height,
		CATextAlignment hAlignment, CAVerticalTextAlignment vAlignment, bool bWordWrap = true, int iLineSpacing = 0, bool bBold = false, bool bItalics = false, bool bUnderLine = false);

	CAImage* initWithStringEx(const char* pText, const char* pFontName, int nSize, int inWidth, int inHeight, 
		std::vector<TextViewLineInfo>& linesText, int iLineSpace=0, bool bWordWrap = true);

protected:
	void initDefaultFont();
	void destroyAllFontData();


	void setCurrentFontData(const char* pFontName, int nSize);

private:
	FontDataTable* m_pCurFontData;

	std::vector<FontDataTable*> m_FontDataVect;
};


#define g_AFTFontCache CAFTFontCache::getAFTFontCache()


NS_CC_END

#endif