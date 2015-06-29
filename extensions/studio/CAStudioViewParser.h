//
//  CAStudioView.h
//  CrossApp
//
//  Created by Zhujian on 15-1-22.
//  Copyright (c) 2015 http://www.9miao.com All rights reserved.
//

#ifndef __Extensions__CAStudioManager__
#define __Extensions__CAStudioManager__

#include <iostream>
#include "CrossApp.h"
#include "Json/CSContentJsonDictionary.h"
#include "Json/DictionaryHelper.h"


NS_CC_EXT_BEGIN;

enum eItemsType
{
	CAType_View = 0,
	CAType_ActivityIndicatorView,
	CAType_AlertView,
	CAType_ImageView,
	CAType_Label,
	CAType_Progress,
	CAType_Button,
	CAType_SegmentedControl,
	CAType_Slider,
	CAType_Stepper,
	CAType_Switch,
	CAType_TextField,
	CAType_WindowView,
	CAType_Control,
};


class CAStudioViewParser
{
public:
	CAStudioViewParser();
    virtual ~CAStudioViewParser();
    
	static CAStudioViewParser* getInstance();

	static void destroyInstance();

	CAView* initWithFile(const char* file);
	CAView* initWithData(const std::string& strDoc);
	CAView* initWithJson(CSJson::Value& var);
	
private:
	void registAllParseFunc();
	CAView* createViewFactory(eItemsType eType);
	CAView* ParseJsonForView(CSJsonDictionary& csJson, CAView* pView);
	CAView* ParseJsonForActivityIndicatorView(CSJsonDictionary& csJson, CAActivityIndicatorView* pView);
	CAView* ParseJsonForAlertView(CSJsonDictionary& csJson, CAAlertView* pView);
	CAView* ParseJsonForImageView(CSJsonDictionary& csJson, CAImageView* pView);
	CAView* ParseJsonForLabel(CSJsonDictionary& csJson, CALabel* pView);
	CAView* ParseJsonForProgress(CSJsonDictionary& csJson, CAProgress* pView);
	CAView* ParseJsonForButton(CSJsonDictionary& csJson, CAButton* pView);
	CAView* ParseJsonForSegmentedControl(CSJsonDictionary& csJson, CASegmentedControl* pView);
	CAView* ParseJsonForSlider(CSJsonDictionary& csJson, CASlider* pView);
	CAView* ParseJsonForStepper(CSJsonDictionary& csJson, CAStepper* pView);
	CAView* ParseJsonForSwitch(CSJsonDictionary& csJson, CASwitch* pView);
	CAView* ParseJsonForTextField(CSJsonDictionary& csJson, CATextField* pView);
	CAView* ParseJsonForControl(CSJsonDictionary& csJson, CAControl* pView);

	typedef CAView* (CAStudioViewParser::*ParseJsonForViewFunc)(CSJsonDictionary& csJson, CAView* pView);
	std::map<eItemsType, ParseJsonForViewFunc> m_RegParseFunMap;
};

NS_CC_EXT_END

#endif /* defined(__Extensions__CAStudioManager__) */
