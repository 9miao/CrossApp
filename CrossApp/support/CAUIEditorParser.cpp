//
//  CAUIEditorParser.cpp
//  CrossApp
//
//  Created by 栗元峰 on 15/11/26.
//  Copyright © 2015年 http://www.9miao.com. All rights reserved.
//

#include "CAUIEditorParser.h"
#include "platform/CCFileUtils.h"


NS_CC_BEGIN


CAUIEditorParser::CAUIEditorParser()
:m_pSupverview(NULL)
,m_pViewController(NULL)
,m_pMyDocument(NULL)
{
    
}

CAUIEditorParser::~CAUIEditorParser()
{
	CC_SAFE_DELETE(m_pMyDocument);
}

CAView* layoutView(tinyxml2::XMLElement* viewXml, CAView* superview, CAMap<std::string, CAView*>& map)
{
    CAView* view = NULL;
    
    DRect bounds = superview->getBounds();
    
    std::string contrlType = viewXml->Attribute("contrlType");
    if (contrlType.compare("CAView") == 0)
    {
        view = CAView::create();
        superview->addSubview(view);
        map.insert(viewXml->Attribute("textTag"), view);
        
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        view->setFrame(rect);
        
        if (const char* value = viewXml->Attribute("color"))
        {
            view->setColor(ccc4Int(atoi(value)));
        }
    }
    else if (contrlType.compare("CAImageView") == 0)
    {
        CAImageView* imageView = CAImageView::create();
        superview->addSubview(imageView);
        map.insert(viewXml->Attribute("textTag"), imageView);
        
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        imageView->setFrame(rect);
        
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
            imageView->setImageViewScaleType((CAImageViewScaleType)atoi(value));
        }
    }
    else if (contrlType.compare("CAScale9ImageView") == 0)
    {
        CAScale9ImageView* scale9ImageVew = CAScale9ImageView::create();
        superview->addSubview(scale9ImageVew);
        map.insert(viewXml->Attribute("textTag"), scale9ImageVew);
        
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        scale9ImageVew->setFrame(rect);
        
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
        map.insert(viewXml->Attribute("textTag"), label);
        
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        label->setFrame(rect);
        
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
			bool var = (atoi(value) == 1) ? true : false;
			label->setBold(var);
		}
        
		if (const char* value = viewXml->Attribute("underLine"))
		{
			bool var = (atoi(value) == 1) ? true : false;
			label->setUnderLine(var);
		}

		if (const char* value = viewXml->Attribute("italics"))
		{
			bool var = (atoi(value) == 1) ? true : false;
			label->setItalics(var);
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
			bool var = (atoi(value) == 1) ? true : false;
			label->setEnableCopy(var);
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
        CAButton* btn = CAButton::create((CAButtonType)atoi(viewXml->Attribute("type")));
        superview->addSubview(btn);
        
        map.insert(viewXml->Attribute("textTag"), btn);
        
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        btn->setFrame(rect);
        
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
        
        DSize titleOffSize = DSizeZero;
        if (const char* value1 = viewXml->Attribute("titleOffSizeX"))
        {
            titleOffSize.width = atof(value1);
        }
        if (const char* value1 = viewXml->Attribute("titleOffSizeY"))
        {
            titleOffSize.height = atof(value1);
        }
        btn->setTitleOffset(titleOffSize);
        
        if (const char* value = viewXml->Attribute("titleNormal"))
        {
            btn->setTitleForState(CAControlStateNormal, value);
        }
        
        if (const char* value = viewXml->Attribute("titleHighted"))
        {
            btn->setTitleForState(CAControlStateHighlighted, value);
        }
        
        if (const char* value = viewXml->Attribute("titleSelected"))
        {
            btn->setTitleForState(CAControlStateSelected, value);
        }
        
        if (const char* value = viewXml->Attribute("titleDisabled"))
        {
            btn->setTitleForState(CAControlStateDisabled, value);
        }
        
        if (const char* value = viewXml->Attribute("titleColorNormal"))
        {
            btn->setTitleColorForState(CAControlStateNormal, ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("titleColorHighted"))
        {
            btn->setTitleColorForState(CAControlStateHighlighted, ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("titleColorSelected"))
        {
            btn->setTitleColorForState(CAControlStateSelected, ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("titleColorDisabled"))
        {
            btn->setTitleColorForState(CAControlStateDisabled, ccc4Int(atoi(value)));
        }
        
        DSize imageOffSize = DSizeZero;
        if (const char* value1 = viewXml->Attribute("imageOffSizeX"))
        {
            imageOffSize.width = atof(value1);
        }
        if (const char* value1 = viewXml->Attribute("imageOffSizeY"))
        {
            imageOffSize.height = atof(value1);
        }
        btn->setImageOffset(imageOffSize);
        
        DSize imageSize = DSizeZero;
        if (const char* value1 = viewXml->Attribute("imageSizeW"))
        {
            imageSize.width = atoi(value1);
        }
        if (const char* value1 = viewXml->Attribute("imageSizeH"))
        {
            imageSize.height = atoi(value1);
        }
        btn->setImageSize(imageSize);
        
        if (const char* value = viewXml->Attribute("imageNormal"))
        {
            btn->setImageForState(CAControlStateNormal, CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("imageHighted"))
        {
            btn->setImageForState(CAControlStateHighlighted, CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("imageSelected"))
        {
            btn->setImageForState(CAControlStateSelected, CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("imageDisabled"))
        {
            btn->setImageForState(CAControlStateDisabled, CAImage::create(value));
        }

        if (strcmp(viewXml->Attribute("backgroundType"), "Scale9") == 0)
        {
            if (const char* value = viewXml->Attribute("backgroundNormal"))
            {
                CAScale9ImageView* scale9ImageView = CAScale9ImageView::create();
                scale9ImageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateNormal, scale9ImageView);
            }
            
            if (const char* value = viewXml->Attribute("backgroundHighted"))
            {
                CAScale9ImageView* scale9ImageView = CAScale9ImageView::create();
                scale9ImageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateHighlighted, scale9ImageView);
            }
            
            if (const char* value = viewXml->Attribute("backgroundSelected"))
            {
                CAScale9ImageView* scale9ImageView = CAScale9ImageView::create();
                scale9ImageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateSelected, scale9ImageView);
            }
            
            if (const char* value = viewXml->Attribute("backgroundDisabled"))
            {
                CAScale9ImageView* scale9ImageView = CAScale9ImageView::create();
                scale9ImageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateDisabled, scale9ImageView);
            }
        }
        else if (strcmp(viewXml->Attribute("backgroundType"), "ColorView") == 0)
        {
            if (viewXml->Attribute("backgroundColorNormal"))
            {
                btn->setBackgroundViewForState(CAControlStateNormal, CAView::createWithColor(CAColor_clear));
            }
            
            if (viewXml->Attribute("backgroundColorHighted"))
            {
                btn->setBackgroundViewForState(CAControlStateHighlighted, CAView::createWithColor(CAColor_clear));
            }
            
            if (viewXml->Attribute("backgroundColorSelected"))
            {
                btn->setBackgroundViewForState(CAControlStateSelected, CAView::createWithColor(CAColor_clear));
            }
            
            if (viewXml->Attribute("backgroundColorDisabled"))
            {
                btn->setBackgroundViewForState(CAControlStateDisabled, CAView::createWithColor(CAColor_clear));
            }
        }
        else
        {
            if (const char* value = viewXml->Attribute("backgroundNormal"))
            {
                CAImageView* imageView = CAImageView::create();
                imageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateNormal, imageView);
            }
            
            if (const char* value = viewXml->Attribute("backgroundHighted"))
            {
                CAImageView* imageView = CAImageView::create();
                imageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateHighlighted, imageView);
            }
            
            if (const char* value = viewXml->Attribute("backgroundSelected"))
            {
                CAImageView* imageView = CAImageView::create();
                imageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateSelected, imageView);
            }
            
            if (const char* value = viewXml->Attribute("backgroundDisabled"))
            {
                CAImageView* imageView = CAImageView::create();
                imageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateDisabled, imageView);
            }
        }
        
        if (const char* value = viewXml->Attribute("backgroundColorNormal"))
        {
            if (CAView* backgroundView = btn->getBackgroundViewForState(CAControlStateNormal))
            {
                backgroundView->setColor(ccc4Int(atoi(value)));
            }
        }
        
        if (const char* value = viewXml->Attribute("backgroundColorHighted"))
        {
            if (CAView* backgroundView = btn->getBackgroundViewForState(CAControlStateHighlighted))
            {
                backgroundView->setColor(ccc4Int(atoi(value)));
            }
        }
        
        if (const char* value = viewXml->Attribute("backgroundColorSelected"))
        {
            if (CAView* backgroundView = btn->getBackgroundViewForState(CAControlStateSelected))
            {
                backgroundView->setColor(ccc4Int(atoi(value)));
            }
        }
        
        if (const char* value = viewXml->Attribute("backgroundColorDisabled"))
        {
            if (CAView* backgroundView = btn->getBackgroundViewForState(CAControlStateDisabled))
            {
                backgroundView->setColor(ccc4Int(atoi(value)));
            }
        }


		if (const char* value = viewXml->Attribute("controlState"))
		{
			CAControlState state = CAControlState(atoi(value));

			if (state == CAControlStateNormal)
			{
				if (const char* value = viewXml->Attribute("backgroundNormal"))
				{
					CAImage* img = CAImage::create(value);
					CAScale9ImageView* imageView = CAScale9ImageView::createWithImage(img);
					btn->setBackgroundViewForState(CAControlStateNormal, imageView);
				}
			}
			else if (state == CAControlStateHighlighted)
			{
				if (const char* value = viewXml->Attribute("backgroundHighted"))
				{
					CAImage* img = CAImage::create(value);
					CAScale9ImageView* imageView = CAScale9ImageView::createWithImage(img);
					btn->setBackgroundViewForState(CAControlStateNormal, imageView);
				}
			}
			if (state == CAControlStateDisabled)
			{
				if (const char* value = viewXml->Attribute("backgroundDisabled"))
				{
					CAImage* img = CAImage::create(value);
					CAScale9ImageView* imageView = CAScale9ImageView::createWithImage(img);
					btn->setBackgroundViewForState(CAControlStateNormal, imageView);
				}
			}
			else if (state == CAControlStateSelected)
			{
				if (const char* value = viewXml->Attribute("backgroundSelected"))
				{
					CAImage* img = CAImage::create(value);
					CAScale9ImageView* imageView = CAScale9ImageView::createWithImage(img);
					btn->setBackgroundViewForState(CAControlStateNormal, imageView);
				}
			}
		}
    }
	else if (contrlType.compare("CATextField") == 0)
	{
		DRect rect;
		rect.origin.x = atoi(viewXml->Attribute("x"));
		rect.origin.y = atoi(viewXml->Attribute("y"));
		rect.size.width = atoi(viewXml->Attribute("w"));
		rect.size.height = atoi(viewXml->Attribute("h"));

		CATextField* textField = CATextField::createWithFrame(rect);
		superview->addSubview(textField);
		map.insert(viewXml->Attribute("textTag"), textField);

		textField->setFrame(rect);

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
			textField->setTextFieldAlign(CATextField::TextFieldAlign(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("fontSize"))
		{
			textField->setFontSize(atoi(value));
		}

		if (const char* value = viewXml->Attribute("maxLenght"))
		{
			textField->setMaxLenght(atoi(value));
		}

		if (const char* value = viewXml->Attribute("maginLeft"))
		{
			textField->setMarginLeft(atoi(value));
		}

		if (const char* value = viewXml->Attribute("maginLeftImage"))
		{
			DSize leftSize = DSizeZero;
			if (const char* value = viewXml->Attribute("maginLeftImageSize_w"))
			{
				leftSize.width = atoi(value);
			}
			if (const char* value = viewXml->Attribute("maginLeftImageSize_h"))
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
			if (const char* value = viewXml->Attribute("maginRightImageSize_w"))
			{
				rightSize.width = atoi(value);
			}
			if (const char* value = viewXml->Attribute("maginRightImageSize_h"))
			{
				rightSize.height = atoi(value);
			}
			textField->setMarginImageLeft(rightSize, value);
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
        CASwitch* sw = CASwitch::create();
        superview->addSubview(sw);
        map.insert(viewXml->Attribute("textTag"), sw);
        
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        sw->setFrame(rect);
        
        if (const char* value = viewXml->Attribute("color"))
        {
            sw->setColor(ccc4Int(atoi(value)));
        }
        
		if (const char* value = viewXml->Attribute("isOn"))
		{
			bool var = (atoi(value) == 1) ? true : false;
			sw->setIsOn(var,false);
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
    else if (contrlType.compare("CASegmentedControl") == 0)
    {
		int itemCount = atoi(viewXml->Attribute("itemCount"));

		CASegmentedControl* segmentControl = CASegmentedControl::create(itemCount);
        superview->addSubview(segmentControl);
        map.insert(viewXml->Attribute("textTag"), segmentControl);
        
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        segmentControl->setFrame(rect);
        
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
			segmentControl->setImageForSegmentAtIndex(CAImage::create(value), 1, CAControlStateNormal);
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
				segmentControl->setImageForSegmentAtIndex(CAImage::create(value), index, CAControlStateNormal);
			}

			if (const char* value = itemXml->Attribute("selectedImage"))
			{
				segmentControl->setImageForSegmentAtIndex(CAImage::create(value), index, CAControlStateSelected);
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
        map.insert(viewXml->Attribute("textTag"), progress);
        
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        progress->setFrame(rect);
        
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
		DRect rect;
		rect.origin.x = atoi(viewXml->Attribute("x"));
		rect.origin.y = atoi(viewXml->Attribute("y"));
		rect.size.width = atoi(viewXml->Attribute("w"));
		rect.size.height = atoi(viewXml->Attribute("h"));

		CATextView* textView = CATextView::createWithFrame(rect);
		superview->addSubview(textView);
		map.insert(viewXml->Attribute("textTag"), textView);


		if (const char* value = viewXml->Attribute("titleColor"))
		{
			textView->setTextColor(ccc4Int(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("textViewAlign"))
		{
			textView->setTextViewAlign(CATextView::TextViewAlign(atoi(value)));
		}

		if (const char* value = viewXml->Attribute("fontSize"))
		{
			textView->setTextFontSize(atoi(value));
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
        map.insert(viewXml->Attribute("textTag"), slider);
        
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        slider->setFrame(rect);
        
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
        CAStepper* stepper = CAStepper::create((CAStepperOrientation)(atoi(viewXml->Attribute("orientation"))));
        superview->addSubview(stepper);
        map.insert(viewXml->Attribute("textTag"), stepper);
        
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        stepper->setFrame(rect);
        
        if (const char* value = viewXml->Attribute("color"))
        {
            stepper->setColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("increntmentImage"))
        {
			stepper->setIncrementImage(CAImage::create(value), CAControlStateNormal);
        }
        
        if (const char* value = viewXml->Attribute("increntmentSelectedImage"))
        {
			stepper->setDecrementImage(CAImage::create(value), CAControlStateNormal);
        }

		if (const char* value = viewXml->Attribute("decrentmentImage"))
		{
			stepper->setIncrementImage(CAImage::create(value), CAControlStateSelected);
		}

		if (const char* value = viewXml->Attribute("decrentmentSelectedImage"))
		{
			stepper->setDecrementImage(CAImage::create(value), CAControlStateSelected);
		}
        
        if (const char* value = viewXml->Attribute("background"))
        {
			stepper->setBackgroundImage(CAImage::create(value), CAControlStateNormal);
        }
    }
    else if (contrlType.compare("CAActivityIndicatorView") == 0)
    {
        CAActivityIndicatorView* activity = CAActivityIndicatorView::create();
        superview->addSubview(activity);
        map.insert(viewXml->Attribute("textTag"), activity);
        
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        activity->setFrame(rect);
        
        if (const char* value = viewXml->Attribute("color"))
        {
            activity->setColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("style"))
        {
            activity->setStyle((CAActivityIndicatorViewStyle)(atoi(value)));
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
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        
        CAPickerView* pickerView = CAPickerView::createWithFrame(rect);
        superview->addSubview(pickerView);
        map.insert(viewXml->Attribute("textTag"), pickerView);
        
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
    }
	else if (contrlType.compare("CADatePickerView") == 0)
	{
		DRect rect;
		rect.origin.x = atoi(viewXml->Attribute("x"));
		rect.origin.y = atoi(viewXml->Attribute("y"));
		rect.size.width = atoi(viewXml->Attribute("w"));
		rect.size.height = atoi(viewXml->Attribute("h"));

		CADatePickerView* datePicker = CADatePickerView::createWithFrame(rect, (CADatePickerMode)atoi(viewXml->Attribute("mode")));
		superview->addSubview(datePicker);
		map.insert(viewXml->Attribute("textTag"), datePicker);

		if (const char* value = viewXml->Attribute("color"))
		{
			datePicker->setColor(ccc4Int(atoi(value)));
		}
	}
    else if (contrlType.compare("CAWebView") == 0)
    {
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        
        CAWebView* webView = CAWebView::createWithFrame(rect);
        superview->addSubview(webView);
        map.insert(viewXml->Attribute("textTag"), webView);
        
        if (const char* value = viewXml->Attribute("url"))
        {
            webView->loadURL(value);
        }
    }
	else if (contrlType.compare("CAGifView") == 0)
	{
		DRect rect;
		rect.origin.x = atoi(viewXml->Attribute("x"));
		rect.origin.y = atoi(viewXml->Attribute("y"));
		rect.size.width = atoi(viewXml->Attribute("w"));
		rect.size.height = atoi(viewXml->Attribute("h"));

		CAGif* gif = NULL;
		if (const char* value = viewXml->Attribute("path"))
		{
			gif = CAGif::createWithFilePath(value);
		}
		
		CAGifView* gifView = CAGifView::createWithFrame(rect);
		superview->addSubview(gifView);
		map.insert(viewXml->Attribute("textTag"), gifView);
		if (gif)
		{
			gifView->setGif(gif);
		}

	}
    else if (contrlType.compare("CAPageView") == 0)
    {
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        
        CAPageView* pageView = CAPageView::createWithFrame(rect, (CAPageViewDirection)(atoi(viewXml->Attribute("driection"))));
        
        superview->addSubview(pageView);
        map.insert(viewXml->Attribute("textTag"), pageView);
        
        if (const char* value = viewXml->Attribute("color"))
        {
            pageView->setBackgroundColor(ccc4Int(atoi(value)));
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
    else if (contrlType.compare("CATableView") == 0)
    {
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        
        CATableView* tableView = CATableView::createWithFrame(rect);
        superview->addSubview(tableView);
        map.insert(viewXml->Attribute("textTag"), tableView);
   
        if (const char* value = viewXml->Attribute("color"))
        {
            tableView->setBackgroundColor(ccc4Int(atoi(value)));
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
            bool var = (atoi(value)==1) ? true : false;
            tableView->CAScrollView::setBounces(var);
        }

        if (const char* value = viewXml->Attribute("showsScrollIndicator"))
        {
            bool var = (atoi(value)==1) ? true : false;
            tableView->setShowsScrollIndicators(var);
        }

        if (const char* value = viewXml->Attribute("headerRefreshView"))
        {
        
            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeHeader);
                tableView->setHeaderRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {

            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeFooter);
                tableView->setFooterRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("alwaysTopSectionHeader"))
        {
            
            bool var = (atoi(value)==1) ? true : false;
            tableView->setAlwaysTopSectionHeader(var);
        }
        
        if (const char* value = viewXml->Attribute("alwaysBottomSectionFooter"))
        {
            
            bool var = (atoi(value)==1) ? true : false;
            tableView->setAlwaysBottomSectionFooter(var);
        }
    }

    else if (contrlType.compare("CAListView") == 0)
    {
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        
        CAListView* listView = CAListView::createWithFrame(rect);
        superview->addSubview(listView);
        map.insert(viewXml->Attribute("textTag"), listView);
 
        if (const char* value = viewXml->Attribute("color"))
        {
            listView->setBackgroundColor(ccc4Int(atoi(value)));
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
        
        if (const char* value = viewXml->Attribute("orientation"))
        {
            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                listView->setListViewOrientation(CAListViewOrientation::CAListViewOrientationHorizontal);
            }
            else
            {
                listView->setListViewOrientation(CAListViewOrientation::CAListViewOrientationVertical);
            }
        }
        
        if (const char* value = viewXml->Attribute("bounce"))
        {
            bool var = (atoi(value)==1) ? true : false;
            listView->CAScrollView::setBounces(var);
        }
        
        if (const char* value = viewXml->Attribute("showsScrollIndicator"))
        {
            bool var = (atoi(value)==1) ? true : false;
            listView->setShowsScrollIndicators(var);
        }

        if (const char* value = viewXml->Attribute("headerRefreshView"))
        {

            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeHeader);
                listView->setHeaderRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {
            
            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeFooter);
                listView->setFooterRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("allowsHeadAndFootHover"))
        {
            bool var = (atoi(value)==1) ? true : false;
            listView->setAllowsHeadAndFootHover(var);
        }
    }
    else if (contrlType.compare("CAAutoCollectionView") == 0)
    {
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        
        CAAutoCollectionView* collectionView = CAAutoCollectionView::createWithFrame(rect);
        superview->addSubview(collectionView);
        map.insert(viewXml->Attribute("textTag"), collectionView);
  
        if (const char* value = viewXml->Attribute("color"))
        {
            collectionView->setBackgroundColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("headerHeight"))
        {
            collectionView->setCollectionHeaderHeight(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("footerHeight"))
        {
            collectionView->setCollectionFooterHeight(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("horiMargins"))
        {
            collectionView->setHoriMargins(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("vertMargins"))
        {
            collectionView->setVertMargins(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("horiCellInterval"))
        {
            collectionView->setHoriCellInterval(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("vertCellInterval"))
        {
            collectionView->setVertCellInterval(atoi(value));
        }

        if (const char* value = viewXml->Attribute("bounce"))
        {
            bool var = (atoi(value)==1) ? true : false;
            collectionView->CAScrollView::setBounces(var);
        }

        if (const char* value = viewXml->Attribute("showsScrollIndicator"))
        {
            bool var = (atoi(value)==1) ? true : false;
            collectionView->setShowsScrollIndicators(var);
        }
        
        if (const char* value = viewXml->Attribute("headerRefreshView"))
        {
            
            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeHeader);
                collectionView->setHeaderRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {
            
            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeFooter);
                collectionView->setFooterRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("alwaysTopSectionHeader"))
        {
            bool var = (atoi(value)==1) ? true : false;
            collectionView->setAlwaysTopSectionHeader(var);
        }
        
        if (const char* value = viewXml->Attribute("alwaysBottomSectionFooter"))
        {
            bool var = (atoi(value)==1) ? true : false;
            collectionView->setAlwaysBottomSectionFooter(var);
        }
        
        if (const char* value = viewXml->Attribute("allowsSelection"))
        {
            bool var = (atoi(value)==1) ? true : false;
            collectionView->setAllowsSelection(var);
        }
        
        if (const char* value = viewXml->Attribute("allowsMultipleSelection"))
        {
            bool var = (atoi(value)==1) ? true : false;
            collectionView->setAllowsMultipleSelection(var);
        }
    }
    else if (contrlType.compare("CAWaterfallView") == 0)
    {
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        
        CAWaterfallView* waterfallView = CAWaterfallView::createWithFrame(rect);
        superview->addSubview(waterfallView);
        map.insert(viewXml->Attribute("textTag"), waterfallView);
        
        if (const char* value = viewXml->Attribute("color"))
        {
            waterfallView->setBackgroundColor(ccc4Int(atoi(value)));
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
            bool var = (atoi(value)==1) ? true : false;
            waterfallView->CAScrollView::setBounces(var);
        }
        
        if (const char* value = viewXml->Attribute("showsScrollIndicator"))
        {
            bool var = (atoi(value)==1) ? true : false;
            waterfallView->setShowsScrollIndicators(var);
        }
        
        if (const char* value = viewXml->Attribute("headerRefreshView"))
        {
            
            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeHeader);
                waterfallView->setHeaderRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {
            
            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = CAPullToRefreshView::create(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeFooter);
                waterfallView->setFooterRefreshView(pullview);
            }
        }
        
        if (const char* value = viewXml->Attribute("alwaysTopSectionHeader"))
        {
            bool var = (atoi(value)==1) ? true : false;
            waterfallView->setAlwaysTopSectionHeader(var);
        }
        
        if (const char* value = viewXml->Attribute("alwaysBottomSectionFooter"))
        {
            bool var = (atoi(value)==1) ? true : false;
            waterfallView->setAlwaysBottomSectionFooter(var);
        }
        
        if (const char* value = viewXml->Attribute("allowsSelection"))
        {
            bool var = (atoi(value)==1) ? true : false;
            waterfallView->setAllowsSelection(var);
        }
        
        if (const char* value = viewXml->Attribute("allowsMultipleSelection"))
        {
            bool var = (atoi(value)==1) ? true : false;
            waterfallView->setAllowsMultipleSelection(var);
        }
    }
    else if (contrlType.compare("CAScrollView") == 0)
    {
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        
        CAScrollView* scrollView = CAScrollView::createWithFrame(rect);
        superview->addSubview(scrollView);
        map.insert(viewXml->Attribute("textTag"), scrollView);
        
        if (const char* value = viewXml->Attribute("color"))
        {
            scrollView->setBackgroundColor(ccc4Int(atoi(value)));
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
            bool var = (atoi(value)==1) ? true : false;
            scrollView->setTouchEnabledAtSubviews(var);
        }
        
        if (const char* value = viewXml->Attribute("showsHorizontalScrollIndicator"))
        {
            bool var = (atoi(value)==1) ? true : false;
            scrollView->setShowsHorizontalScrollIndicator(var);
        }
        
        if (const char* value = viewXml->Attribute("showsVerticalScrollIndicator"))
        {
            bool var = (atoi(value)==1) ? true : false;
            scrollView->setShowsVerticalScrollIndicator(var);
        }
        
        if (const char* value = viewXml->Attribute("bounceHorizontal"))
        {
            bool var = (atoi(value)==1) ? true : false;
            scrollView->setBounceHorizontal(var);
        }
        
        if (const char* value = viewXml->Attribute("bounceVertical"))
        {
            bool var = (atoi(value)==1) ? true : false;
            scrollView->setBounceVertical(var);
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
    const char* data = (const char*)CCFileUtils::sharedFileUtils()->getFileData(filePath.c_str(), "rb", &size);
    if (size > 0)
    {
        std::string str;
        str.resize(size);
        for (size_t i = 0; i < size; i++)
        {
            str[i] = data[i];
        }
        
        m_pMyDocument = new tinyxml2::XMLDocument();
		m_pMyDocument->Parse(data, size);
		tinyxml2::XMLElement* rootElement = m_pMyDocument->RootElement();
        
        tinyxml2::XMLElement* entity = NULL;
        if (rootElement)
        {
            entity = rootElement->FirstChildElement();
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
	if (rootElement)
	{
		entity = rootElement->FirstChildElement();
	}


	entity = entity->NextSiblingElement();

	std::string contrlID = entity->Attribute("id");

	if (entity)
	{
		//CATabBarItem
		if (contrlID.compare("CATabBarItem") == 0)
		{
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
	}

	entity = entity->NextSiblingElement();
	std::string navigationID = entity->Attribute("id");
	if (entity)
	{
		//CANavigationItem
		if (navigationID.compare("CANavigationBarItem") == 0)
		{
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
}



NS_CC_END