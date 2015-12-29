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
        
        if (strcmp(viewXml->Attribute("BackgroundType"), "Scale9") == 0)
        {
            if (const char* value = viewXml->Attribute("BackgroundNormal"))
            {
                CAScale9ImageView* scale9ImageView = CAScale9ImageView::create();
                scale9ImageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateNormal, scale9ImageView);
            }
            
            if (const char* value = viewXml->Attribute("BackgroundHighted"))
            {
                CAScale9ImageView* scale9ImageView = CAScale9ImageView::create();
                scale9ImageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateHighlighted, scale9ImageView);
            }
            
            if (const char* value = viewXml->Attribute("BackgroundSelected"))
            {
                CAScale9ImageView* scale9ImageView = CAScale9ImageView::create();
                scale9ImageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateSelected, scale9ImageView);
            }
            
            if (const char* value = viewXml->Attribute("BackgroundDisabled"))
            {
                CAScale9ImageView* scale9ImageView = CAScale9ImageView::create();
                scale9ImageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateDisabled, scale9ImageView);
            }
        }
        else if (strcmp(viewXml->Attribute("BackgroundType"), "ColorView") == 0)
        {
            if (viewXml->Attribute("BackgroundColorNormal"))
            {
                btn->setBackgroundViewForState(CAControlStateNormal, CAView::createWithColor(CAColor_clear));
            }
            
            if (viewXml->Attribute("BackgroundColorHighted"))
            {
                btn->setBackgroundViewForState(CAControlStateHighlighted, CAView::createWithColor(CAColor_clear));
            }
            
            if (viewXml->Attribute("BackgroundColorSelected"))
            {
                btn->setBackgroundViewForState(CAControlStateSelected, CAView::createWithColor(CAColor_clear));
            }
            
            if (viewXml->Attribute("BackgroundColorDisabled"))
            {
                btn->setBackgroundViewForState(CAControlStateDisabled, CAView::createWithColor(CAColor_clear));
            }
        }
        else
        {
            if (const char* value = viewXml->Attribute("BackgroundNormal"))
            {
                CAImageView* imageView = CAImageView::create();
                imageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateNormal, imageView);
            }
            
            if (const char* value = viewXml->Attribute("BackgroundHighted"))
            {
                CAImageView* imageView = CAImageView::create();
                imageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateHighlighted, imageView);
            }
            
            if (const char* value = viewXml->Attribute("BackgroundSelected"))
            {
                CAImageView* imageView = CAImageView::create();
                imageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateSelected, imageView);
            }
            
            if (const char* value = viewXml->Attribute("BackgroundDisabled"))
            {
                CAImageView* imageView = CAImageView::create();
                imageView->setImage(CAImage::create(value));
                btn->setBackgroundViewForState(CAControlStateDisabled, imageView);
            }
        }
        
        if (const char* value = viewXml->Attribute("BackgroundColorNormal"))
        {
            if (CAView* BackgroundView = btn->getBackgroundViewForState(CAControlStateNormal))
            {
                BackgroundView->setColor(ccc4Int(atoi(value)));
            }
        }
        
        if (const char* value = viewXml->Attribute("BackgroundColorHighted"))
        {
            if (CAView* BackgroundView = btn->getBackgroundViewForState(CAControlStateHighlighted))
            {
                BackgroundView->setColor(ccc4Int(atoi(value)));
            }
        }
        
        if (const char* value = viewXml->Attribute("BackgroundColorSelected"))
        {
            if (CAView* BackgroundView = btn->getBackgroundViewForState(CAControlStateSelected))
            {
                BackgroundView->setColor(ccc4Int(atoi(value)));
            }
        }
        
        if (const char* value = viewXml->Attribute("BackgroundColorDisabled"))
        {
            if (CAView* BackgroundView = btn->getBackgroundViewForState(CAControlStateDisabled))
            {
                BackgroundView->setColor(ccc4Int(atoi(value)));
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
    else if (contrlType.compare("CASegmentedControl") == 0)
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
        
        if (const char* value = viewXml->Attribute("Background"))
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
        
        if (const char* value = viewXml->Attribute("minTrackTintImage"))
        {
            slider->setMinTrackTintImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("maxTrackTintImage"))
        {
            slider->setMaxTrackTintImage(CAImage::create(value));
        }
        
        if (const char* value = viewXml->Attribute("thumbTintImage"))
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
        
        if (const char* value = viewXml->Attribute("value"))
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
            stepper->setIncrementImage(CAImage::create(value), CAControlStateDisabled);
        }
        
        if (const char* value = viewXml->Attribute("decrentmentImage"))
        {
            stepper->setDecrementImage(CAImage::create(value), CAControlStateDisabled);
        }
        
        if (const char* value = viewXml->Attribute("Background"))
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
            CAImage* image = CAImage::create(value);
            DRect rect = DRectZero;
            rect.size = image->getContentSize();
            CAImageView* imageView = CAImageView::createWithImage(CAImage::create(value));
            imageView->setFrame(rect);
            activity->setActivityIndicatorView(imageView);
            
        }
        
        if (const char* value = viewXml->Attribute("Background"))
        {
            CAImage* image = CAImage::create(value);
            DRect rect = DRectZero;
            rect.size = image->getContentSize();
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
        
        if (const char* value = viewXml->Attribute("loadURL"))
        {
            webView->loadURL(value);
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
                CrossApp::CAPullToRefreshView *pullview = new CrossApp::CAPullToRefreshView(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeHeader);
                tableView->setHeaderRefreshView(pullview);
            }
            else
            {
                tableView->setHeaderRefreshView(NULL);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {

            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = new CrossApp::CAPullToRefreshView(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeFooter);
                tableView->setHeaderRefreshView(pullview);
            }
            else
            {
                tableView->setHeaderRefreshView(NULL);
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
                CrossApp::CAPullToRefreshView *pullview = new CrossApp::CAPullToRefreshView(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeHeader);
                listView->setHeaderRefreshView(pullview);
            }
            else
            {
                listView->setHeaderRefreshView(NULL);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {
            
            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = new CrossApp::CAPullToRefreshView(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeFooter);
                listView->setHeaderRefreshView(pullview);
            }
            else
            {
                listView->setHeaderRefreshView(NULL);
            }
        }
        
        if (const char* value = viewXml->Attribute("allowsHeadAndFootHover"))
        {
            bool var = (atoi(value)==1) ? true : false;
            listView->setAllowsHeadAndFootHover(var);
        }
    }
    else if (contrlType.compare("CACollectionView") == 0)
    {
        DRect rect;
        rect.origin.x = atoi(viewXml->Attribute("x"));
        rect.origin.y = atoi(viewXml->Attribute("y"));
        rect.size.width = atoi(viewXml->Attribute("w"));
        rect.size.height = atoi(viewXml->Attribute("h"));
        
        CACollectionView* collectionView = CACollectionView::createWithFrame(rect);
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
        
        if (const char* value = viewXml->Attribute("horiInterval"))
        {
            collectionView->setHoriInterval(atoi(value));
        }
        
        if (const char* value = viewXml->Attribute("vertInterval"))
        {
            collectionView->setVertInterval(atoi(value));
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
                CrossApp::CAPullToRefreshView *pullview = new CrossApp::CAPullToRefreshView(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeHeader);
                collectionView->setHeaderRefreshView(pullview);
            }
            else
            {
                collectionView->setHeaderRefreshView(NULL);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {
            
            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = new CrossApp::CAPullToRefreshView(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeFooter);
                collectionView->setHeaderRefreshView(pullview);
            }
            else
            {
                collectionView->setHeaderRefreshView(NULL);
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
                CrossApp::CAPullToRefreshView *pullview = new CrossApp::CAPullToRefreshView(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeHeader);
                waterfallView->setHeaderRefreshView(pullview);
            }
            else
            {
                waterfallView->setHeaderRefreshView(NULL);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {
            
            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = new CrossApp::CAPullToRefreshView(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeFooter);
                waterfallView->setHeaderRefreshView(pullview);
            }
            else
            {
                waterfallView->setHeaderRefreshView(NULL);
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

        if (const char* value = viewXml->Attribute("headerRefreshView"))
        {
            
            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = new CrossApp::CAPullToRefreshView(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeHeader);
                scrollView->setHeaderRefreshView(pullview);
            }
            else
            {
                scrollView->setHeaderRefreshView(NULL);
            }
        }
        
        if (const char* value = viewXml->Attribute("footerRefreshView"))
        {
            
            bool var = (atoi(value)==1) ? true : false;
            if(var)
            {
                CrossApp::CAPullToRefreshView *pullview = new CrossApp::CAPullToRefreshView(CrossApp::CAPullToRefreshView::CAPullToRefreshType::CAPullToRefreshTypeFooter);
                scrollView->setHeaderRefreshView(pullview);
            }
            else
            {
                scrollView->setHeaderRefreshView(NULL);
            }
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