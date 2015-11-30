//
//  CAUIEditorParser.cpp
//  CrossApp
//
//  Created by 栗元峰 on 15/11/26.
//  Copyright © 2015年 http://www.9miao.com. All rights reserved.
//

#include "CAUIEditorParser.h"
#include "platform/CCFileUtils.h"
#include "tinyxml2/tinyxml2.h"
NS_CC_BEGIN

CAUIEditorParser::CAUIEditorParser()
:m_pSupverview(NULL)
{
    
}

CAUIEditorParser::~CAUIEditorParser()
{
    
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
        map.insert(viewXml->Attribute("textTag"), superview);
        
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
        
        if (const char* value1 = viewXml->Attribute("titleOffSizeX"))
        {
            const char* value2 = viewXml->Attribute("titleOffSizeY");
            DSize offSize = DSize(atoi(value1), atoi(value2));
            btn->setTitleOffset(offSize);
        }
        
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
        
        if (const char* value1 = viewXml->Attribute("imageOffSizeX"))
        {
            const char* value2 = viewXml->Attribute("imageOffSizeY");
            DSize offSize = DSize(atoi(value1), atoi(value2));
            btn->setImageOffset(offSize);
        }
        
        if (const char* value1 = viewXml->Attribute("imageSizeW"))
        {
            const char* value2 = viewXml->Attribute("imageSizeH");
            DSize size = DSize(atoi(value1), atoi(value2));
            btn->setImageSize(size);
        }
        
        if (atoi(viewXml->Attribute("isScale9")))
        {
            if (const char* value = viewXml->Attribute("backgroundNormal"))
            {
                CAScale9ImageView* scale9ImageView = CAScale9ImageView::create();
                scale9ImageView->setImage(CAImage::create(value));
                btn->setBackGroundViewForState(CAControlStateNormal, scale9ImageView);
            }
            
            if (const char* value = viewXml->Attribute("backgroundHighted"))
            {
                CAScale9ImageView* scale9ImageView = CAScale9ImageView::create();
                scale9ImageView->setImage(CAImage::create(value));
                btn->setBackGroundViewForState(CAControlStateHighlighted, scale9ImageView);
            }
            
            if (const char* value = viewXml->Attribute("backgroundSelected"))
            {
                CAScale9ImageView* scale9ImageView = CAScale9ImageView::create();
                scale9ImageView->setImage(CAImage::create(value));
                btn->setBackGroundViewForState(CAControlStateSelected, scale9ImageView);
            }
            
            if (const char* value = viewXml->Attribute("backgroundDisabled"))
            {
                CAScale9ImageView* scale9ImageView = CAScale9ImageView::create();
                scale9ImageView->setImage(CAImage::create(value));
                btn->setBackGroundViewForState(CAControlStateDisabled, scale9ImageView);
            }
        }
        else if (atoi(viewXml->Attribute("isColorView")))
        {
            if (viewXml->Attribute("backgroundColorNormal"))
            {
                btn->setBackGroundViewForState(CAControlStateNormal, CAView::createWithColor(CAColor_clear));
            }
            
            if (viewXml->Attribute("backgroundColorHighted"))
            {
                btn->setBackGroundViewForState(CAControlStateHighlighted, CAView::createWithColor(CAColor_clear));
            }
            
            if (viewXml->Attribute("backgroundColorSelected"))
            {
                btn->setBackGroundViewForState(CAControlStateSelected, CAView::createWithColor(CAColor_clear));
            }
            
            if (viewXml->Attribute("backgroundColorDisabled"))
            {
                btn->setBackGroundViewForState(CAControlStateDisabled, CAView::createWithColor(CAColor_clear));
            }
        }
        else
        {
            if (const char* value = viewXml->Attribute("backgroundNormal"))
            {
                CAImageView* imageView = CAImageView::create();
                imageView->setImage(CAImage::create(value));
                btn->setBackGroundViewForState(CAControlStateNormal, imageView);
            }
            
            if (const char* value = viewXml->Attribute("backgroundHighted"))
            {
                CAImageView* imageView = CAImageView::create();
                imageView->setImage(CAImage::create(value));
                btn->setBackGroundViewForState(CAControlStateHighlighted, imageView);
            }
            
            if (const char* value = viewXml->Attribute("backgroundSelected"))
            {
                CAImageView* imageView = CAImageView::create();
                imageView->setImage(CAImage::create(value));
                btn->setBackGroundViewForState(CAControlStateSelected, imageView);
            }
            
            if (const char* value = viewXml->Attribute("backgroundDisabled"))
            {
                CAImageView* imageView = CAImageView::create();
                imageView->setImage(CAImage::create(value));
                btn->setBackGroundViewForState(CAControlStateDisabled, imageView);
            }
        }
        
        if (const char* value = viewXml->Attribute("backgroundColorNormal"))
        {
            if (CAView* backgroundView = btn->getBackGroundViewForState(CAControlStateNormal))
            {
                backgroundView->setColor(ccc4Int(atoi(value)));
            }
        }
        
        if (const char* value = viewXml->Attribute("backgroundColorHighted"))
        {
            if (CAView* backgroundView = btn->getBackGroundViewForState(CAControlStateHighlighted))
            {
                backgroundView->setColor(ccc4Int(atoi(value)));
            }
        }
        
        if (const char* value = viewXml->Attribute("backgroundColorSelected"))
        {
            if (CAView* backgroundView = btn->getBackGroundViewForState(CAControlStateSelected))
            {
                backgroundView->setColor(ccc4Int(atoi(value)));
            }
        }
        
        if (const char* value = viewXml->Attribute("backgroundColorDisabled"))
        {
            if (CAView* backgroundView = btn->getBackGroundViewForState(CAControlStateDisabled))
            {
                backgroundView->setColor(ccc4Int(atoi(value)));
            }
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
    else if (contrlType.compare("CASegmentControl") == 0)
    {
        CASegmentedControl* segmentControl = CASegmentedControl::create(2);
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
            segmentControl->setTitleColor(ccc4Int(atoi(value)));
        }
        
        if (const char* value = viewXml->Attribute("tintColor"))
        {
            segmentControl->setTintColor(ccc4Int(atoi(value)));
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
            progress->setProgress(atof(value));
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
            stepper->setIncrementImage(CAImage::create(value), CAControlStateDisabled);
        }
        
        if (const char* value = viewXml->Attribute("decrentmentImage"))
        {
            stepper->setDecrementImage(CAImage::create(value), CAControlStateDisabled);
        }
        
        if (const char* value = viewXml->Attribute("background"))
        {
            stepper->setBackgroundImage(CAImage::create(value), CAControlStateDisabled);
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
            activity->setActivityIndicatorView(CAImageView::createWithImage(CAImage::create(value)));
        }
        
        if (const char* value = viewXml->Attribute("background"))
        {
            activity->setActivityBackView(CAImageView::createWithImage(CAImage::create(value)));
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
            parserXml(((tinyxml2::XMLElement*)viewXml->FirstChildElement()), view, map);
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
        
        tinyxml2::XMLDocument* myDocument = new tinyxml2::XMLDocument();
        myDocument->Parse(data, size);
        tinyxml2::XMLElement* rootElement = myDocument->RootElement();
        
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




NS_CC_END