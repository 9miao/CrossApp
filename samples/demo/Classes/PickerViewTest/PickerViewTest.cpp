//
//  PickerViewTest.cpp
//  HelloCpp
//
//  Created by dai xinping on 14-7-24.
//
//

#include "PickerViewTest.h"

#define PICKER_VIEW_LINE_HEIGHT 80

#define PICKER_VIEW_LINE_MAX 7

#define PICKER_VIEW_HEIGHT (PICKER_VIEW_LINE_HEIGHT * PICKER_VIEW_LINE_MAX)

PickerViewTest::PickerViewTest()
: pPickerView(NULL)
{
    
}

PickerViewTest::~PickerViewTest()
{
    CC_SAFE_RELEASE(pPickerView);
}

void PickerViewTest::viewDidLoad()
{
//    CAButton* button = CAButton::createWithCenter(CCRectMake(getView()->getFrame().size.width/3,
//                                                             getView()->getFrame().size.height/3, 
//                                                             200 * CROSSAPP_ADPTATION_RATIO, 
//                                                             50 * CROSSAPP_ADPTATION_RATIO), 
//                                                  CAButtonTypeRoundedRect);
    CALabel* label1 = CALabel::createWithCenter(CCRectMake(getView()->getFrame().size.width/3,
                                                          getView()->getFrame().size.height/3, 
                                                          400 * CROSSAPP_ADPTATION_RATIO, 
                                                          50 * CROSSAPP_ADPTATION_RATIO));
    label1->setText("TEXT");
    label1->setColor(ccc4(0, 0, 0, 255));
    label1->setTag(100);
    label1->setFontSize(50 * CROSSAPP_ADPTATION_RATIO);
    getView()->addSubview(label1);
    
    CALabel* label2 = CALabel::createWithCenter(CCRectMake(getView()->getFrame().size.width*2/3,
                                                          getView()->getFrame().size.height/3, 
                                                          400 * CROSSAPP_ADPTATION_RATIO, 
                                                          50 * CROSSAPP_ADPTATION_RATIO));
    label2->setText("TEXT");
    label2->setColor(ccc4(0, 0, 0, 255));
    label2->setTag(101);
    label2->setFontSize(50 * CROSSAPP_ADPTATION_RATIO);
    getView()->addSubview(label2);
    
    float pickerViewHeight = PICKER_VIEW_HEIGHT * CROSSAPP_ADPTATION_RATIO;
    CCRect rect = CCRectMake(getView()->getFrame().size.width/2, 
                             getView()->getFrame().size.height*2/3, 
                             getView()->getFrame().size.width, 
                             pickerViewHeight);
    pPickerView = CAPickerView::createWithCenter(rect);
    pPickerView->retain();
    pPickerView->setDelegate(this);
    pPickerView->setDataSource(this);
    pPickerView->setFontSizeNormal(50 * CROSSAPP_ADPTATION_RATIO);
    pPickerView->setFontSizeSelected(50 * CROSSAPP_ADPTATION_RATIO);
    pPickerView->reloadAllComponents();
    pPickerView->setDisplayRange(false);
    getView()->addSubview(pPickerView);
}

void PickerViewTest::viewDidUnload()
{
    
}

void PickerViewTest::didSelectRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
{
    if (component == 0) {
        CALabel* label = (CALabel*)getView()->getSubviewByTag(100);
        label->setText(CCString::createWithFormat("ROW + %d", row)->getCString());        
    } else {
        CALabel* label = (CALabel*)getView()->getSubviewByTag(101);
        label->setText(CCString::createWithFormat("ROW + %d", row)->getCString());                
    }
}

unsigned int PickerViewTest::numberOfComponentsInPickerView(CAPickerView* pickerView)
{
    return 2;
}

unsigned int PickerViewTest::numberOfRowsInComponent(CAPickerView* pickerView, unsigned int component)
{
    if (component == 0) {
        return 50;
    }
    return 2;
}

float PickerViewTest::widthForComponent(CAPickerView* pickerView, unsigned int component)
{
    return pickerView->getFrame().size.width/3;
}

float PickerViewTest::rowHeightForComponent(CAPickerView* pickerView, unsigned int component)
{
    return PICKER_VIEW_LINE_HEIGHT * CROSSAPP_ADPTATION_RATIO;
}

CCString* PickerViewTest::titleForRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
{
    return CCString::createWithFormat("ROW + %d", row);
}


