//
//  CATempTypeFont.h
//  CrossApp
//
//  Created by Zhujian on 15-2-10.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __PLATFORM_CATEMPFONT_H
#define __PLATFORM_CATEMPFONT_H

#include "CAFreeTypeFont.h"


NS_CC_BEGIN

class CC_DLL CATempTypeFont : public CAFreeTypeFont
{
public:

	CATempTypeFont();
	virtual ~CATempTypeFont();

	static CATempTypeFont& getInstance();

	void initTempTypeFont(unsigned long nSize);
	void finiTempTypeFont();

	FT_Face m_CurFontFace;
private:
	std::map<unsigned long, FT_Face> m_mTempFontMap;
};

NS_CC_END

#endif