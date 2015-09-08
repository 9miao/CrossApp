//
//  CAEmojiFont.h
//  CrossApp
//
//  Created by Zhujian on 15-7-8.
//  Copyright (c) 2015 http://www.9miao.com All rights reserved.
//

#ifndef __CC_PLATFORM_CAEMOJIFONT_H
#define __CC_PLATFORM_CAEMOJIFONT_H

#define FT_CONFIG_OPTION_USE_PNG

#include "platform/CCCommon.h"
#include "images/CAImage.h"
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>
#include <ftoutln.h>
#include <fttrigon.h>

NS_CC_BEGIN


class CC_DLL CAEmojiFont
{
public:
	CAEmojiFont();
	virtual ~CAEmojiFont();

	bool initEmojiFont();
	void finiEmojiFont();

	bool isEmojiCodePoint(unsigned int codepoint);

	CAImage* getEmojiImage(unsigned int codepoint, unsigned long nFontSize);

protected:
	unsigned char* loadEmojiFontBuffer(unsigned long& size);

private:
	FT_Face m_CurFontFace;
	FT_Library m_gFtLibrary;
};

NS_CC_END

#endif