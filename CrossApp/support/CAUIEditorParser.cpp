//
//  CAUIEditorParser.cpp
//  CrossApp
//
//  Created by 栗元峰 on 15/11/26.
//  Copyright © 2015年 http://www.9miao.com. All rights reserved.
//

#include "CAUIEditorParser.h"
#include "platform/CAFileUtils.h"
#include "controller/CAViewController.h"
#include "control/CAButton.h"
#include "control/CAProgress.h"
#include "control/CASwitch.h"
#include "control/CACheckbox.h"
#include "control/CASlider.h"
#include "control/CASegmentedControl.h"
#include "control/CAPageControl.h"
#include "control/CAStepper.h"
#include "view/CAView.h"
#include "view/CAImageView.h"
#include "view/CAScale9ImageView.h"
#include "view/CAGifView.h"
#include "view/CALabel.h"
#include "view/CARichLabel.h"
#include "view/CAPullToRefreshView.h"
#include "view/CACell.h"
#include "view/CAScrollView.h"
#include "view/CAPageView.h"
#include "view/CAListView.h"
#include "view/CATableView.h"
#include "view/CACollectionView.h"
#include "view/CAAutoCollectionView.h"
#include "view/CAWaterfallView.h"
#include "view/CAWebView.h"
#include "view/CARenderImage.h"
#include "view/CAClippingView.h"
#include "view/CAAlertView.h"
#include "view/CAPickerView.h"
#include "view/CADatePickerView.h"
#include "view/CAActivityIndicatorView.h"
#include "platform/CATextField.h"
#include "platform/CATextView.h"

NS_CC_BEGIN


CAUIEditorParser::CAUIEditorParser()
:m_pSupverview(nullptr)
,m_pViewController(nullptr)
,m_pMyDocument(nullptr)
{
    
}

CAUIEditorParser::~CAUIEditorParser()
{
	CC_SAFE_DELETE(m_pMyDocument);
}

CAView* layoutView(tinyxml2::XMLElement* viewXml, CAView* superview, CAMap<std::string, CAView*>& map)
{
    CAView* view = NULL;
    
    //DRect bounds = superview->getBounds();
    
    std::string contrlType = viewXml->Attribute("contrlType");
    if (contrlType.compare("CAView") == 0)
    {
        view = CAView::create();
        superview->addSubview(view);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, view);
        view->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        view->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            view->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("color"))
        {
            view->setColor(ccc4Int(atoi(value)));
        }
    }
    else if (contrlType.compare("CAImageView") == 0)
    {
        CAImageView* imageView = CAImageView::create();
        superview->addSubview(imageView);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, imageView);
        imageView->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        imageView->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            imageView->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("color"))
        {
            imageView->setColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("image"))
        {
            imageView->setImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("scaleType"))
        {
            imageView->setScaleType((CAImageView::ScaleType)atoi(value));
        }
    }
    else if (contrlType.compare("CAScale9ImageView") == 0)
    {
        CAScale9ImageView* scale9ImageVew = CAScale9ImageView::create();
        superview->addSubview(scale9ImageVew);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, scale9ImageVew);
        scale9ImageVew->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        scale9ImageVew->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            scale9ImageVew->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("color"))
        {
            scale9ImageVew->setColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("image"))
        {
            scale9ImageVew->setImage(CAImage::create(value));
        }

		if (const char* value = viewXml->Attribute("alpha"))
		{
			scale9ImageVew->setAlpha(atof(value));
		}

		if (const char* value = viewXml->Attribute("insetLeft"))
		{
			scale9ImageVew->setInsetLeft(atof(value));
		}

		if (const char* value = viewXml->Attribute("insetTop"))
		{
			scale9ImageVew->setInsetTop(atof(value));
		}

		if (const char* value = viewXml->Attribute("insetRight"))
		{
			scale9ImageVew->setInsetRight(atof(value));
		}

		if (const char* value = viewXml->Attribute("insetBottom"))
		{
			scale9ImageVew->setInsetBottom(atof(value));
		}
    }
    else if (contrlType.compare("CALabel") == 0)
    {
        CALabel* label = CALabel::create();
        superview->addSubview(label);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, label);
        label->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        label->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            label->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("fontColor"))
        {
            label->setColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("text"))
        {
            label->setText(value);
        }
        
        if (const char* value = viewXml->Attribute("fontSize"))
        {
            label->setFontSize(atoi(value));
        }

		if (const char* value = viewXml->Attribute("fontName"))
		{
			label->setFontName(value);
		}

		if (const char* value = viewXml->Attribute("bold"))
		{
			label->setBold(bool(atoi(value)));
		}
        
		if (const char* value = viewXml->Attribute("underLine"))
		{
			label->setUnderLine(bool(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("italics"))
		{
			label->setItalics(bool(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("lineSpacing"))
		{
			label->setLineSpacing(atoi(value));
		}
        
		if (const char* value = viewXml->Attribute("textAlignment"))
		{
			label->setTextAlignment(CATextAlignment(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("verticalTextAlignment"))
		{
			label->setVerticalTextAlignmet(CAVerticalTextAlignment(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("enableCopy"))
		{
			label->setEnableCopy(bool(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("numberOfLine"))
		{
			label->setNumberOfLine(atoi(value));
		}

		if (const char* value = viewXml->Attribute("wordWrap"))
		{
			label->setWordWrap(atoi(value));
		}

    }
    else if (contrlType.compare("CAButton") == 0)
    {
        CAButton* btn = CAButton::create((CAButton::Type)atoi(viewXml->Attribute("type")));
        superview->addSubview(btn);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, btn);
        btn->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        btn->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            btn->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("isRecSpe"))
        {
            btn->setRecSpe(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("color"))
        {
            btn->setColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("fontSize"))
        {
            btn->setTitleFontSize(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("fontName"))
        {
            btn->setTitleFontName(value);
        }
        
        if (const char* value = viewXml->Attribute("titleBold"))
        {
            btn->setTitleBold((bool)atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("titleTextAlignment"))
        {
            btn->setTitleTextAlignment((CATextAlignment)atoi(value));
        }
        
        {
            bool enabled = false;
            DSize titleOffSize = DSizeZero;
            if (const char* value1 = viewXml->Attribute("titleOffSizeX"))
            {
                titleOffSize.width = atof(value1);
                enabled = true;
            }
            if (const char* value1 = viewXml->Attribute("titleOffSizeY"))
            {
                titleOffSize.height = atof(value1);
                enabled = true;
            }
            if (enabled)
            {
                btn->setTitleOffset(titleOffSize);
            }
        }
        
        
        if (const char* value = viewXml->Attribute("titleNormal"))
        {
            btn->setTitleForState(CAControl::State::Normal, value);
        }
        
        if (const char* value = viewXml->Attribute("titleHighted"))
        {
            btn->setTitleForState(CAControl::State::Highlighted, value);
        }
        
        if (const char* value = viewXml->Attribute("titleSelected"))
        {
            btn->setTitleForState(CAControl::State::Selected, value);
        }
        
        if (const char* value = viewXml->Attribute("titleDisabled"))
        {
            btn->setTitleForState(CAControl::State::Disabled, value);
        }
        
        if (const char* value = viewXml->Attribute("titleColorNormal"))
        {
            btn->setTitleColorForState(CAControl::State::Normal, ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("titleColorHighted"))
        {
            btn->setTitleColorForState(CAControl::State::Highlighted, ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("titleColorSelected"))
        {
            btn->setTitleColorForState(CAControl::State::Selected, ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("titleColorDisabled"))
        {
            btn->setTitleColorForState(CAControl::State::Disabled, ccc4Int(atoi(value)));
        }
        
        {
            bool enabled = false;
            DSize imageOffSize = DSizeZero;
            if (const char* value1 = viewXml->Attribute("imageOffSizeX"))
            {
                imageOffSize.width = atof(value1);
                enabled = true;
            }
            if (const char* value1 = viewXml->Attribute("imageOffSizeY"))
            {
                imageOffSize.height = atof(value1);
                enabled = true;
            }
            if (enabled)
            {
                btn->setImageOffset(imageOffSize);
            }
        }
        
        {
            bool enabled = false;
            DSize imageSize = DSizeZero;
            if (const char* value1 = viewXml->Attribute("imageSizeW"))
            {
                imageSize.width = atoi(value1);
                enabled = true;
            }
            if (const char* value1 = viewXml->Attribute("imageSizeH"))
            {
                imageSize.height = atoi(value1);
                enabled = true;
            }
            if (enabled)
            {
                btn->setImageSize(imageSize);
            }
        }
        
        if (const char* value = viewXml->Attribute("imageNormal"))
        {
            btn->setImageForState(CAControl::State::Normal, CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("imageHighted"))
        {
            btn->setImageForState(CAControl::State::Highlighted, CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("imageSelected"))
        {
            btn->setImageForState(CAControl::State::Selected, CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("imageDisabled"))
        {
            btn->setImageForState(CAControl::State::Disabled, CAImage::create(value));
        }

        if (strcmp(viewXml->Attribute("backgroundType"), "Scale9") == 0)
        {
            if (const char* value = viewXml->Attribute("backgroundNormal"))
            {
                btn->setBackgroundImageForState(CAControl::State::Normal, CAImage::create(value), true);
            }
            
            if (const char* value = viewXml->Attribute("backgroundHighted"))
            {
                btn->setBackgroundImageForState(CAControl::State::Highlighted, CAImage::create(value), true);
            }
            
            if (const char* value = viewXml->Attribute("backgroundDisabled"))
            {
                btn->setBackgroundImageForState(CAControl::State::Disabled, CAImage::create(value), true);
            }
        }
        else if (strcmp(viewXml->Attribute("backgroundType"), "ColorView") == 0)
        {
            if (const char* value = viewXml->Attribute("backgroundColorNormal"))
            {
                btn->setBackgroundImageForState(CAControl::State::Normal, CAImage::createWithColor4B(ccc4Int(atoi(value))), false);
            }
            
            if (const char* value = viewXml->Attribute("backgroundColorHighted"))
            {
                btn->setBackgroundImageForState(CAControl::State::Highlighted, CAImage::createWithColor4B(ccc4Int(atoi(value))), false);
            }
            
            if (const char* value = viewXml->Attribute("backgroundColorDisabled"))
            {
                btn->setBackgroundImageForState(CAControl::State::Disabled, CAImage::createWithColor4B(ccc4Int(atoi(value))), false);
            }
        }
        else
        {
            if (const char* value = viewXml->Attribute("backgroundNormal"))
            {
                btn->setBackgroundImageForState(CAControl::State::Normal, CAImage::create(value), false);
            }
            
            if (const char* value = viewXml->Attribute("backgroundHighted"))
            {
                btn->setBackgroundImageForState(CAControl::State::Highlighted, CAImage::create(value), false);
            }
            
            if (const char* value = viewXml->Attribute("backgroundDisabled"))
            {
                btn->setBackgroundImageForState(CAControl::State::Disabled, CAImage::create(value), false);
            }
        }
        
		if (const char* value = viewXml->Attribute("controlState"))
		{
            btn->setControlState((CAControl::State)(atoi(value)));
		}
    }
	else if (contrlType.compare("CATextField") == 0)
	{
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));

        CATextField* textField = CATextField::createWithLayout(layout);
		superview->addSubview(textField);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, textField);
        textField->setTextTag(key);


        if (const char* value = viewXml->Attribute("z"))
        {
            textField->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("isRecSpe"))
        {
            textField->setRecSpe(bool(atoi(value)));
        }
        
		if (const char* value = viewXml->Attribute("textColor"))
		{
			textField->setTextColor(ccc4Int(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("keyboartType"))
		{
			textField->setKeyboardType(CATextField::KeyboardType(atoi(value)));
		}
		
		if (const char* value = viewXml->Attribute("returnType"))
		{
			textField->setReturnType(CATextField::ReturnType(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("clearButtonMode"))
		{
			textField->setClearButtonMode(CATextField::ClearButtonMode(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("textFieldAlign"))
		{
			textField->setAlign((CATextField::Align)(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("fontSize"))
		{
			textField->setFontSize(atoi(value));
		}

		if (const char* value = viewXml->Attribute("maxLength"))
		{
			textField->setMaxLength(atoi(value));
		}

		if (const char* value = viewXml->Attribute("maginLeft"))
		{
			textField->setMarginLeft(atoi(value));
		}

		if (const char* value = viewXml->Attribute("maginLeftImage"))
		{
			DSize leftSize = DSizeZero;
			if (const char* value = viewXml->Attribute("maginLeftImageSizeW"))
			{
				leftSize.width = atoi(value);
			}
			if (const char* value = viewXml->Attribute("maginLeftImageSizeH"))
			{
				leftSize.height= atoi(value);
			}
			textField->setMarginImageLeft(leftSize,value);
		}

		if (const char* value = viewXml->Attribute("maginRight"))
		{
			textField->setMarginRight(atoi(value));
		}

		if (const char* value = viewXml->Attribute("maginRightImage"))
		{
			DSize rightSize = DSizeZero;
			if (const char* value = viewXml->Attribute("maginRightImageSizeW"))
			{
				rightSize.width = atoi(value);
			}
			if (const char* value = viewXml->Attribute("maginRightImageSizeH"))
			{
				rightSize.height = atoi(value);
			}
			textField->setMarginImageRight(rightSize, value);
		}

		if (const char* value = viewXml->Attribute("placeGolderColor"))
		{
			textField->setPlaceHolderColor(ccc4Int(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("background"))
		{
			textField->setBackgroundImage(CAImage::create(value));
		}

		if (const char* value = viewXml->Attribute("placeHolderText"))
		{
			textField->setPlaceHolderText(value);
		}

		if (const char* value = viewXml->Attribute("text"))
		{
			textField->setText(value);
		}
	}
    else if (contrlType.compare("CASwitch") == 0)
    {
        CASwitch* sw = CASwitch::create((CASwitch::Type)atoi(viewXml->Attribute("type")));
        superview->addSubview(sw);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, sw);
        sw->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        sw->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            sw->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("color"))
        {
            sw->setColor(ccc4Int(atoi(value)));
        }
        
		if (const char* value = viewXml->Attribute("isOn"))
		{
			sw->setIsOn(bool(atoi(value)),false);
		}

        if (const char* value = viewXml->Attribute("onImage"))
        {
            sw->setOnImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("offImage"))
        {
            sw->setOffImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("tintImage"))
        {
            sw->setThumbTintImage(CAImage::create(value));
        }
    }
    else if (contrlType.compare("CACheckbox") == 0)
    {
        CACheckbox* sw = CACheckbox::create((CACheckbox::Type)atoi(viewXml->Attribute("type")));
        superview->addSubview(sw);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, sw);
        sw->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        sw->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            sw->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("color"))
        {
            sw->setColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("imageNormal"))
        {
            sw->setImageStateNormal(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("imageSelected"))
        {
            sw->setImageStateSelected(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("imageColorNormal"))
        {
            sw->setImageColorStateNormal(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("imageColorSelected"))
        {
            sw->setImageColorStateSelected(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("titleNormal"))
        {
            sw->setTitleStateNormal(value);
        }
        
        if (const char* value = viewXml->Attribute("titleSelected"))
        {
            sw->setTitleStateSelected(value);
        }
        
        if (const char* value = viewXml->Attribute("titleColorNormal"))
        {
            sw->setTitleColorStateNormal(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("titleColorSelected"))
        {
            sw->setTitleColorStateSelected(ccc4Int(atoi(value)));
        }

        {
            bool enabled = false;
            DSize imageOffSize = DSizeZero;
            if (const char* value1 = viewXml->Attribute("imageOffSizeX"))
            {
                imageOffSize.width = atof(value1);
                enabled = true;
            }
            if (const char* value1 = viewXml->Attribute("imageOffSizeY"))
            {
                imageOffSize.height = atof(value1);
                enabled = true;
            }
            if (enabled)
            {
                sw->setImageOffset(imageOffSize);
            }
        }
        
        {
            bool enabled = false;
            DSize imageSize = DSizeZero;
            if (const char* value1 = viewXml->Attribute("imageSizeW"))
            {
                imageSize.width = atoi(value1);
                enabled = true;
            }
            if (const char* value1 = viewXml->Attribute("imageSizeH"))
            {
                imageSize.height = atoi(value1);
                enabled = true;
            }
            if (enabled)
            {
                sw->setImageSize(imageSize);
            }
        }
       
        if (const char* value = viewXml->Attribute("fontSize"))
        {
            sw->setTitleFontSize(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("fontName"))
        {
            sw->setTitleFontName(value);
        }
        
        if (const char* value = viewXml->Attribute("titleBold"))
        {
            sw->setTitleBold((bool)atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("titleTextAlignment"))
        {
            sw->setTitleTextAlignment((CATextAlignment)atoi(value));
        }
        
        {
            bool enabled = false;
            DSize titleOffSet = DSizeZero;
            if (const char* value1 = viewXml->Attribute("titleOffSizeX"))
            {
                titleOffSet.width = atof(value1);
                enabled = true;
            }
            if (const char* value1 = viewXml->Attribute("titleOffSizeY"))
            {
                titleOffSet.height = atof(value1);
                enabled = true;
            }
            if (enabled)
            {
                sw->setTitleOffset(titleOffSet);
            }
        }
        if (const char* value = viewXml->Attribute("isOn"))
        {
            sw->setIsOn(bool(atoi(value)));
        }
    }
    else if (contrlType.compare("CASegmentedControl") == 0)
    {
		int itemCount = atoi(viewXml->Attribute("itemCount"));

		CASegmentedControl* segmentControl = CASegmentedControl::create(itemCount);
        superview->addSubview(segmentControl);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, segmentControl);
        segmentControl->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        segmentControl->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            segmentControl->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("isRecSpe"))
        {
            segmentControl->setRecSpe(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("color"))
        {
            segmentControl->setColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("background"))
        {
            segmentControl->setBackgroundImage(CAImage::create(value));
        }
        
		if (const char* value = viewXml->Attribute("fontName"))
		{
			segmentControl->setTitleFontName(value);
		}

		if (const char* value = viewXml->Attribute("fontSize"))
		{
			segmentControl->setTitleFontSize(atoi(value));
		}

        if (const char* value = viewXml->Attribute("segmentItemImage"))
        {
            segmentControl->setSegmentItemBackgroundImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("titleColorNormal"))
        {
            segmentControl->setTitleColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("titleColorSelected"))
        {
            segmentControl->setTitleSelectedColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("tintColor"))
        {
            segmentControl->setTintColor(ccc4Int(atoi(value)));
        }

		if (const char* value = viewXml->Attribute("image"))
		{
			segmentControl->setImageForSegmentAtIndex(CAImage::create(value), 1, CAControl::State::Normal);
		}
        
        if (const char* value = viewXml->Attribute("selectedAtIndex"))
        {
            segmentControl->setSelectedAtIndex(atoi(value));
        }
		
		tinyxml2::XMLElement* itemXml = NULL;
		
		for (int index = 0; index < itemCount; index++)
		{
			if (itemXml == NULL	)
			{
				itemXml = viewXml->FirstChildElement();
			}
			else
			{
				itemXml = itemXml->NextSiblingElement();
			}

			if (const char* value = itemXml->Attribute("title"))
			{
				segmentControl->setTitleForSegmentAtIndex(value, index);
			}

			if (const char* value = itemXml->Attribute("image"))
			{
				segmentControl->setImageForSegmentAtIndex(CAImage::create(value), index, CAControl::State::Normal);
			}

			if (const char* value = itemXml->Attribute("selectedImage"))
			{
				segmentControl->setImageForSegmentAtIndex(CAImage::create(value), index, CAControl::State::Selected);
			}

			DSize size = DSizeZero;
			if (const char* value = itemXml->Attribute("imageSize_w"))
			{
				size.width = atoi(value);
			}
			if (const char* value = itemXml->Attribute("imageSize_h"))
			{
				size.height = atoi(value);
			}
			segmentControl->setImageSizeAtIndex(size, index);

			DSize offset = DSizeZero;
			if (const char* value = itemXml->Attribute("imageOffSize_x"))
			{
				offset.width = atoi(value);
			}
			if (const char* value = itemXml->Attribute("imageOffSize_y"))
			{
				offset.height = atoi(value);
			}
			segmentControl->setContentOffsetForSegmentAtIndex(offset, index);

			if (const char* value = itemXml->Attribute("isEnabled"))
			{
				segmentControl->setEnabledForSegmentAtIndex((bool)atoi(value), index);
			}
		}
    }
    else if (contrlType.compare("CAProgress") == 0)
    {
        CAProgress* progress = CAProgress::create();
        superview->addSubview(progress);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, progress);
        progress->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        progress->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            progress->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("isRecSpe"))
        {
            progress->setRecSpe(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("color"))
        {
            progress->setColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("trackImage"))
        {
            progress->setProgressTrackImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("tintImage"))
        {
            progress->setProgressTintImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("trackColor"))
        {
            progress->setProgressTrackColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("tintolor"))
        {
            progress->setProgressTintColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("progress"))
        {
            progress->setProgress(atof(value)/100);
        }
    }
	else if (contrlType.compare("CATextView") == 0)
	{
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));

		CATextView* textView = CATextView::createWithLayout(layout);
		superview->addSubview(textView);
		
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, textView);
        textView->setTextTag(key);

        if (const char* value = viewXml->Attribute("z"))
        {
            textView->setZOrder(atoi(value));
        }

		if (const char* value = viewXml->Attribute("titleColor"))
		{
			textView->setTextColor(ccc4Int(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("textViewAlign"))
		{
			textView->setAlign(CATextView::Align(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("fontSize"))
		{
			textView->setFontSize(atoi(value));
		}

		if (const char* value = viewXml->Attribute("text"))
		{
			textView->setText(value);
		}

		if (const char* value = viewXml->Attribute("background"))
		{
			textView->setBackgroundImage(CAImage::create(value));
		}

	}
    else if (contrlType.compare("CASlider") == 0)
    {
        CASlider* slider = CASlider::create();
        superview->addSubview(slider);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, slider);
        slider->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        slider->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            slider->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("isRecSpe"))
        {
            slider->setRecSpe(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("color"))
        {
            slider->setColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("minTrackImage"))
        {
            slider->setMinTrackTintImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("maxTrackImage"))
        {
            slider->setMaxTrackTintImage(CAImage::create(value));
        }
		
		if (const char* value = viewXml->Attribute("tintImage"))
		{
			slider->setThumbTintImage(CAImage::create(value));
		}
        
        if (const char* value = viewXml->Attribute("minValue"))
        {
            slider->setMinValue(atof(value));
        }
        
        if (const char* value = viewXml->Attribute("maxValue"))
        {
            slider->setMaxValue(atof(value));
        }
        
        if (const char* value = viewXml->Attribute("sliderValue"))
        {
            slider->setValue(atof(value));
        }
        
        if (const char* value = viewXml->Attribute("trackHeight"))
        {
            slider->setTrackHeight(atof(value));
        }
    }
    else if (contrlType.compare("CAStepper") == 0)
    {
        CAStepper* stepper = CAStepper::create((CAStepper::Orientation)(atoi(viewXml->Attribute("orientation"))));
        superview->addSubview(stepper);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, stepper);
        stepper->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        stepper->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            stepper->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("isRecSpe"))
        {
            stepper->setRecSpe(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("color"))
        {
            stepper->setColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("increntmentImage"))
        {
			stepper->setIncrementImage(CAControl::State::Normal, CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("increntmentSelectedImage"))
        {
			stepper->setIncrementImage(CAControl::State::Highlighted, CAImage::create(value));
        }

		if (const char* value = viewXml->Attribute("decrentmentImage"))
		{
			stepper->setDecrementImage(CAControl::State::Normal, CAImage::create(value));
		}

		if (const char* value = viewXml->Attribute("decrentmentSelectedImage"))
		{
			stepper->setDecrementImage(CAControl::State::Highlighted, CAImage::create(value));
		}
        
        if (const char* value = viewXml->Attribute("background"))
        {
			stepper->setBackgroundImage(CAControl::State::Normal, CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("minValue"))
        {
            stepper->setMinValue(atof(value));
        }
        
        if (const char* value = viewXml->Attribute("maxValue"))
        {
            stepper->setMaxValue(atof(value));
        }
        
        if (const char* value = viewXml->Attribute("value"))
        {
            stepper->setValue(atof(value));
        }
        
        if (const char* value = viewXml->Attribute("stepValue"))
        {
            stepper->setStepValue(atof(value));
        }
        
        if (const char* value = viewXml->Attribute("tintColor"))
        {
            stepper->setDividerColor(ccc4Int(atoi(value)));
        }

        if (const char* value = viewXml->Attribute("autoRepeat"))
        {
            stepper->setAutoRepeat(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("wraps"))
        {
            stepper->setWraps(bool(atoi(value)));
        }
        
        //error 缺少 backgroundSelected
    }
    else if (contrlType.compare("CAActivityIndicatorView") == 0)
    {
        CAActivityIndicatorView* activity = CAActivityIndicatorView::create();
        superview->addSubview(activity);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, activity);
        activity->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        activity->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            activity->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("color"))
        {
            activity->setColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("style"))
        {
            activity->setStyle((CAActivityIndicatorView::Style)(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("imageNormal"))
        {
			DSize imageSize = CrossApp::DSizeZero;
			if (const char* value = viewXml->Attribute("imageWidth"))
			{
				imageSize.width = atoi(value);
			}

			if (const char* value = viewXml->Attribute("imageHeight"))
			{
				imageSize.height = atoi(value);
			}

            CAImage* image = CAImage::create(value);
            DRect rect = DRectZero;
			if (imageSize.width !=0 || imageSize.height != 0)
			{
				rect.size = imageSize;
			}
			else
			{
				rect.size = image->getContentSize();
			}
            
            CAImageView* imageView = CAImageView::createWithImage(CAImage::create(value));
            imageView->setFrame(rect);
            activity->setActivityIndicatorView(imageView);        
        }
        
        if (const char* value = viewXml->Attribute("background"))
        {
			DSize backgroundSize = CrossApp::DSizeZero;
			if (const char* value = viewXml->Attribute("backgroundWidth"))
			{
				backgroundSize.width = atoi(value);
			}

			if (const char* value = viewXml->Attribute("backgroundHeight"))
			{
				backgroundSize.height = atoi(value);
			}

            CAImage* image = CAImage::create(value);
            DRect rect = DRectZero;
			if (backgroundSize.width != 0 || backgroundSize.height != 0)
			{
				rect.size = backgroundSize;
			}
			else
			{
				rect.size = image->getContentSize();
			}
            CAImageView* imageView = CAImageView::createWithImage(CAImage::create(value));
            imageView->setFrame(rect);
            activity->setActivityBackView(imageView);
        }
    }
    else if (contrlType.compare("CAPickerView") == 0)
    {
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        
        CAPickerView* pickerView = CAPickerView::createWithLayout(layout);
        superview->addSubview(pickerView);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, pickerView);
        pickerView->setTextTag(key);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            pickerView->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("color"))
        {
            pickerView->setColor(ccc4Int(atoi(value)));
        }

        if (const char* value = viewXml->Attribute("fontSizeNormal"))
        {
            pickerView->setFontSizeNormal(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("fontSizeSelected"))
        {
            pickerView->setFontSizeSelected(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("fontColorNormal"))
        {
            pickerView->setFontColorNormal(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("fontColorSelected"))
        {
            pickerView->setFontColorSelected(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("separateColor"))
        {
            pickerView->setSeparateColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("backgroundColor"))
        {
            pickerView->setColor(ccc4Int(atoi(value)));
        }
    }
	else if (contrlType.compare("CADatePickerView") == 0)
	{
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));

        CADatePickerView* datePicker = CADatePickerView::createWithLayout(layout, (CADatePickerView::Mode)atoi(viewXml->Attribute("mode")));
		superview->addSubview(datePicker);
		
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, datePicker);
        datePicker->setTextTag(key);

        if (const char* value = viewXml->Attribute("z"))
        {
            datePicker->setZOrder(atoi(value));
        }
        
		if (const char* value = viewXml->Attribute("color"))
		{
			datePicker->setColor(ccc4Int(atoi(value)));
		}
	}
    else if (contrlType.compare("CAWebView") == 0)
    {
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        
        CAWebView* webView = CAWebView::createWithLayout(layout);
        superview->addSubview(webView);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, webView);
        webView->setTextTag(key);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            webView->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("url"))
        {
            webView->loadURL(value);
        }
    }
	else if (contrlType.compare("CAGifView") == 0)
	{
		CAGif* gif = NULL;
		if (const char* value = viewXml->Attribute("path"))
		{
			gif = CAGif::create(value);
		}
		
        CAGifView* gifView = CAGifView::createWithGif(gif);
		superview->addSubview(gifView);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, gifView);
        gifView->setTextTag(key);
        
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        gifView->setLayout(layout);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            gifView->setZOrder(atoi(value));
        }

	}
    else if (contrlType.compare("CAScrollView") == 0)
    {
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        
        CAScrollView* scrollView = CAScrollView::createWithLayout(layout);
        superview->addSubview(scrollView);
        view = scrollView;
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, scrollView);
        scrollView->setTextTag(key);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            scrollView->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("backgroundColor"))
        {
            scrollView->setBackgroundImage(CAImage::createWithColor4B(ccc4Int(atoi(value))));
        }
        
        if (const char* value = viewXml->Attribute("backgroundImage"))
        {
            scrollView->setBackgroundImage(CAImage::create(value));
        }
        
        DSize viewSize = CrossApp::DSizeZero;
        if (const char* value = viewXml->Attribute("viewSize_w"))
        {
            viewSize.width = atoi(value);
        }
        
        if (const char* value = viewXml->Attribute("viewSize_h"))
        {
            viewSize.height = atoi(value);
        }
        scrollView->setViewSize(viewSize);
        
        DPoint contentOffSet = DPointZero;
        if (const char* value = viewXml->Attribute("contentOffSet_x"))
        {
            contentOffSet.x = atoi(value);
        }
        if (const char* value = viewXml->Attribute("contentOffSet_y"))
        {
            contentOffSet.y = atoi(value);
        }
        scrollView->setContentOffset(contentOffSet, false);
        
        if (const char* value = viewXml->Attribute("minZoomScale"))
        {
            scrollView->setMinimumZoomScale(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("maxZoomScale"))
        {
            scrollView->setMaximumZoomScale(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("touchEnabledAtSubviews"))
        {
            scrollView->setTouchEnabledAtSubviews(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("showsHorizontalScrollIndicator"))
        {
            scrollView->setShowsHorizontalScrollIndicator(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("showsVerticalScrollIndicator"))
        {
            scrollView->setShowsVerticalScrollIndicator(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("bounceHorizontal"))
        {
            scrollView->setBounceHorizontal(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("bounceVertical"))
        {
            scrollView->setBounceVertical(bool(atoi(value)));
        }
    }
    else if (contrlType.compare("CAPageView") == 0)
    {
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        
        CAPageView* pageView = CAPageView::createWithLayout(layout, (CAPageView::Orientation)(atoi(viewXml->Attribute("orientation"))));
        
        superview->addSubview(pageView);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, pageView);
        pageView->setTextTag(key);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            pageView->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("backgroundColor"))
        {
            pageView->setBackgroundImage(CAImage::createWithColor4B(ccc4Int(atoi(value))));
        }
        
        if (const char* value = viewXml->Attribute("backgroundImage"))
        {
            pageView->setBackgroundImage(CAImage::create(value));
        }
        
        if(const char* value = viewXml->Attribute("spacing"))
        {
            pageView->setSpacing(atoi(value));
        }
        
        if(const char* value = viewXml->Attribute("spacing"))
        {
            pageView->setSpacing(atoi(value));
        }
        
        if(const char* value = viewXml->Attribute("showsScrollIndicator"))
        {
            pageView->setShowsScrollIndicators(atoi(value));
        }
        
        if(const char* value = viewXml->Attribute("currPage"))
        {
            pageView->setCurrPage(atoi(value),false);
        }
        
        if(const char* value = viewXml->Attribute("bounce"))
        {
            pageView->CAScrollView::setBounces(atoi(value));
        }
    }
    else if (contrlType.compare("CAListView") == 0)
    {
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        
        CAListView* listView = CAListView::createWithLayout(layout);
        superview->addSubview(listView);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, listView);
        listView->setTextTag(key);
 
        if (const char* value = viewXml->Attribute("z"))
        {
            listView->setZOrder(atoi(value));
        }

        if (const char* value = viewXml->Attribute("backgroundColor"))
        {
            listView->setBackgroundImage(CAImage::createWithColor4B(ccc4Int(atoi(value))));
        }
        
        if (const char* value = viewXml->Attribute("backgroundImage"))
        {
            listView->setBackgroundImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("orientation"))
        {
            listView->setOrientation((CAListView::Orientation)atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("headerHeight"))
        {
            listView->setListHeaderHeight(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("footerHeight"))
        {
            listView->setListFooterHeight(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("separatorHeight"))
        {
            listView->setSeparatorViewHeight(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("separatorColor"))
        {
            listView->setSeparatorColor(ccc4Int(atoi(value)));
        }

        if (const char* value = viewXml->Attribute("bounce"))
        {
            listView->CAScrollView::setBounces(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("showsScrollIndicator"))
        {
            listView->setShowsScrollIndicators(bool(atoi(value)));
        }

        if (const char* value = viewXml->Attribute("headerRefreshView"))
        {
            if(bool(atoi(value)))
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::Type::Header);
                listView->setHeaderRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {
            if(bool(atoi(value)))
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::Type::Footer);
                listView->setFooterRefreshView(pullview);
            }
        }
    }
    else if (contrlType.compare("CATableView") == 0)
    {
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        
        CATableView* tableView = CATableView::createWithLayout(layout);
        superview->addSubview(tableView);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, tableView);
        tableView->setTextTag(key);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            tableView->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("backgroundColor"))
        {
            tableView->setBackgroundImage(CAImage::createWithColor4B(ccc4Int(atoi(value))));
        }
        
        if (const char* value = viewXml->Attribute("backgroundImage"))
        {
            tableView->setBackgroundImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("headerHeight"))
        {
            tableView->setTableHeaderHeight(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("footerHeight"))
        {
            tableView->setTableFooterHeight(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("separatorHeight"))
        {
            tableView->setSeparatorViewHeight(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("separatorColor"))
        {
            tableView->setSeparatorColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("bounce"))
        {
            tableView->CAScrollView::setBounces(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("showsScrollIndicator"))
        {
            tableView->setShowsScrollIndicators(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("headerRefreshView"))
        {
            if(bool(atoi(value)))
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::Type::Header);
                tableView->setHeaderRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {
            if(bool(atoi(value)))
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::Type::Footer);
                tableView->setFooterRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("alwaysTopSectionHeader"))
        {
            tableView->setAlwaysTopSectionHeader(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("alwaysBottomSectionFooter"))
        {
            tableView->setAlwaysBottomSectionFooter(bool(atoi(value)));
        }
    }

    else if (contrlType.compare("CAAutoCollectionView") == 0)
    {
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        
        CAAutoCollectionView* autoCollectionView = CAAutoCollectionView::createWithLayout(layout);
        superview->addSubview(autoCollectionView);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, autoCollectionView);
        autoCollectionView->setTextTag(key);
  
        if (const char* value = viewXml->Attribute("z"))
        {
            autoCollectionView->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("backgroundColor"))
        {
            autoCollectionView->setBackgroundImage(CAImage::createWithColor4B(ccc4Int(atoi(value))));
        }
        
        if (const char* value = viewXml->Attribute("backgroundImage"))
        {
            autoCollectionView->setBackgroundImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("orientation"))
        {
            autoCollectionView->setOrientation((CAAutoCollectionView::Orientation)atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("horiCellInterval"))
        {
            autoCollectionView->setHoriCellInterval(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("vertCellInterval"))
        {
            autoCollectionView->setVertCellInterval(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("horiMargins"))
        {
            autoCollectionView->setHoriMargins(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("vertMargins"))
        {
            autoCollectionView->setVertMargins(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("headerHeight"))
        {
            autoCollectionView->setCollectionHeaderHeight(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("footerHeight"))
        {
            autoCollectionView->setCollectionFooterHeight(atoi(value));
        }

        if (const char* value = viewXml->Attribute("bounce"))
        {
            autoCollectionView->CAScrollView::setBounces(bool(atoi(value)));
        }

        if (const char* value = viewXml->Attribute("showsScrollIndicator"))
        {
            autoCollectionView->setShowsScrollIndicators(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("headerRefreshView"))
        {
            if(bool(atoi(value)))
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::Type::Header);
                autoCollectionView->setHeaderRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {
            if(bool(atoi(value)))
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::Type::Footer);
                autoCollectionView->setFooterRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("alwaysTopSectionHeader"))
        {
            autoCollectionView->setAlwaysTopSectionHeader(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("alwaysBottomSectionFooter"))
        {
            autoCollectionView->setAlwaysBottomSectionFooter(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("allowsSelection"))
        {
            autoCollectionView->setAllowsSelection(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("allowsMultipleSelection"))
        {
            autoCollectionView->setAllowsMultipleSelection(bool(atoi(value)));
        }
    }

    else if (contrlType.compare("CACollectionView") == 0)
    {
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        
        CACollectionView* collectionView = CACollectionView::createWithLayout(layout);
        superview->addSubview(collectionView);

        const char* key = viewXml->Attribute("textTag");
        map.insert(key, collectionView);
        collectionView->setTextTag(key);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            collectionView->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("backgroundColor"))
        {
            collectionView->setBackgroundImage(CAImage::createWithColor4B(ccc4Int(atoi(value))));
        }
        
        if (const char* value = viewXml->Attribute("backgroundImage"))
        {
            collectionView->setBackgroundImage(CAImage::create(value));
        }

        if (const char* value = viewXml->Attribute("horiInterval"))
        {
            collectionView->setHoriInterval(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("vertInterval"))
        {
            collectionView->setVertInterval(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("headerHeight"))
        {
            collectionView->setCollectionHeaderHeight(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("footerHeight"))
        {
            collectionView->setCollectionFooterHeight(atoi(value));
        }

        if (const char* value = viewXml->Attribute("bounce"))
        {
            collectionView->CAScrollView::setBounces(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("showsScrollIndicator"))
        {
            collectionView->setShowsScrollIndicators(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("headerRefreshView"))
        {
            if(bool(atoi(value)))
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::Type::Header);
                collectionView->setHeaderRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {
            if(bool(atoi(value)))
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::Type::Footer);
                collectionView->setFooterRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("alwaysTopSectionHeader"))
        {
            collectionView->setAlwaysTopSectionHeader(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("alwaysBottomSectionFooter"))
        {
            collectionView->setAlwaysBottomSectionFooter(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("allowsSelection"))
        {
            collectionView->setAllowsSelection(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("allowsMultipleSelection"))
        {
            collectionView->setAllowsMultipleSelection(bool(atoi(value)));
        }
    }
    
    else if (contrlType.compare("CAWaterfallView") == 0)
    {
        DLayout layout;
        layout.horizontal = DHorizontalLayout(atof(viewXml->Attribute("h_var1")),
                                              atof(viewXml->Attribute("h_var2")),
                                              (DHorizontalLayout::Type)atoi(viewXml->Attribute("HLayoutType")));
        layout.vertical = DVerticalLayout(atof(viewXml->Attribute("v_var1")),
                                          atof(viewXml->Attribute("v_var2")),
                                          (DVerticalLayout::Type)atoi(viewXml->Attribute("VLayoutType")));
        
        CAWaterfallView* waterfallView = CAWaterfallView::createWithLayout(layout);
        superview->addSubview(waterfallView);
        
        const char* key = viewXml->Attribute("textTag");
        map.insert(key, waterfallView);
        waterfallView->setTextTag(key);
        
        if (const char* value = viewXml->Attribute("z"))
        {
            waterfallView->setZOrder(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("backgroundColor"))
        {
            waterfallView->setBackgroundImage(CAImage::createWithColor4B(ccc4Int(atoi(value))));
        }
        
        if (const char* value = viewXml->Attribute("backgroundImage"))
        {
            waterfallView->setBackgroundImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("headerHeight"))
        {
            waterfallView->setWaterfallHeaderHeight(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("footerHeight"))
        {
            waterfallView->setWaterfallFooterHeight(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("columnCount"))
        {
            waterfallView->setColumnCount(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("itemMargin"))
        {
            waterfallView->setItemMargin(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("columnMargin"))
        {
            waterfallView->setColumnMargin(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("bounce"))
        {
            waterfallView->CAScrollView::setBounces(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("showsScrollIndicator"))
        {
            waterfallView->setShowsScrollIndicators(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("headerRefreshView"))
        {
            if(bool(atoi(value)))
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::Type::Header);
                waterfallView->setHeaderRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {
            if(bool(atoi(value)))
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::Type::Footer);
                waterfallView->setFooterRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("alwaysTopSectionHeader"))
        {
            waterfallView->setAlwaysTopSectionHeader(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("alwaysBottomSectionFooter"))
        {
            waterfallView->setAlwaysBottomSectionFooter(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("allowsSelection"))
        {
            waterfallView->setAllowsSelection(bool(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("allowsMultipleSelection"))
        {
            waterfallView->setAllowsMultipleSelection(bool(atoi(value)));
        }
    }
    

    return view;

}

void parserXml(tinyxml2::XMLElement* viewXml, CAView* superview, CAMap<std::string, CAView*>& map)
{
    while (viewXml)
    {
        CAView* view = layoutView(viewXml, superview, map);
		if (view)
		{
			if (typeid(*view) == typeid(CAView) || typeid(*view) == typeid(CAScrollView))
			{
				parserXml(((tinyxml2::XMLElement*)viewXml->FirstChildElement()), view, map);
			}
		}

        viewXml = viewXml->NextSiblingElement();
    }
}

bool CAUIEditorParser::initWithPath(const std::string& filePath, CAView* superview)
{
    this->init();
    
    unsigned long size = 0;
    unsigned char* data = FileUtils::getInstance()->getFileData(filePath, "rb", &size);
    
    if (size > 0)
    {
        m_pMyDocument = new tinyxml2::XMLDocument();
		m_pMyDocument->Parse((const char*)data, size);
		tinyxml2::XMLElement* rootElement = m_pMyDocument->RootElement();
        
        tinyxml2::XMLElement* entity = NULL;
        if (rootElement)
        {
            entity = rootElement->FirstChildElement("ViewList");
        }
        
        tinyxml2::XMLElement* viewXml = ((tinyxml2::XMLElement*)entity->FirstChildElement());
        parserXml(viewXml, superview, m_mViews);
    }
    
    return true;
}

void CAUIEditorParser::parseViewControllItems(CAViewController* viewController)
{
	m_pViewController = viewController;

    CC_RETURN_IF(!m_pMyDocument);
    
	tinyxml2::XMLElement* rootElement = m_pMyDocument->RootElement();

	tinyxml2::XMLElement* entity = NULL;

    entity = rootElement->FirstChildElement("CATabBarItem");

	if (entity)
	{
		//CATabBarItem
			std::string barTitle = "";
			CAImage* image = NULL;
			CAImage* selectImage = NULL;

			if (const char* value = entity->Attribute("image"))
			{
				image = CAImage::create(value);
			}
			if (const char* value = entity->Attribute("selectImage"))
			{
				selectImage = CAImage::create(value);
			}
			if (const char* value = entity->Attribute("title"))
			{
				barTitle = value;
			}
			CATabBarItem* item = CATabBarItem::create(barTitle, image, selectImage);
			if (const char* value = entity->Attribute("badgeValue"))
			{
				item->setBadgeValue(value);
			}

			viewController->setTabBarItem(item);
	}

	entity = rootElement->FirstChildElement("CANavigationBarItem");
	if (entity)
	{
		//CANavigationItem

			CANavigationBarItem* navigationBarItem = NULL;
			if (const char* value = entity->Attribute("title"))
			{
				navigationBarItem = CANavigationBarItem::create(value);
			}
			if (const char* value = entity->Attribute("titleImage"))
			{
				navigationBarItem->setTitleViewImage(CAImage::create(value));
			}
			if (navigationBarItem)
			{
				tinyxml2::XMLElement* leftXml = entity->FirstChildElement();

				tinyxml2::XMLElement* lElementXml = leftXml->FirstChildElement();

				while (lElementXml)
				{
					std::string leftTitle = "";
					CAImage* leftImage = NULL;
					CAImage* leftHightImage = NULL;

					if (const char* value = lElementXml->Attribute("title"))
					{
						leftTitle = value;
					}
					if (const char* value = lElementXml->Attribute("image"))
					{
						leftImage = CAImage::create(value);
					}
					if (const char* value = lElementXml->Attribute("hightImage"))
					{
						leftHightImage = CAImage::create(value);
					}
					CABarButtonItem* leftButton = CABarButtonItem::create(leftTitle, leftImage, leftHightImage);
					navigationBarItem->addLeftButtonItem(leftButton);
					lElementXml = lElementXml->NextSiblingElement();
				}


				tinyxml2::XMLElement* rightXml = leftXml->NextSiblingElement();

				tinyxml2::XMLElement* rElementXml = rightXml->FirstChildElement();

				while (rElementXml)
				{
					std::string rightTitle = "";
					CAImage* rightImage = NULL;
					CAImage* rightHightImage = NULL;
					if (const char* value = rElementXml->Attribute("title"))
					{
						rightTitle = value;
					}
					if (const char* value = rElementXml->Attribute("image"))
					{
						rightImage = CAImage::create(value);
					}
					if (const char* value = rElementXml->Attribute("hightImage"))
					{
						rightImage = CAImage::create(value);
					}
					CABarButtonItem* rightButton = CABarButtonItem::create(rightTitle, rightImage, rightHightImage);
					navigationBarItem->addRightButtonItem(rightButton);
					rElementXml = rElementXml->NextSiblingElement();
				}

				viewController->setNavigationBarItem(navigationBarItem);
			}
	}
}



NS_CC_END
