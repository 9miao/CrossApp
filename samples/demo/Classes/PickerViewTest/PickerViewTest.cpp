#include "PickerViewTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

PickerViewTest::PickerViewTest()
: pickerView(NULL)
, year("")
, month("")
, day("")
, yearFlag(false)
, daysOfMonth(30)
{
    
}

PickerViewTest::~PickerViewTest()
{
    //CC_SAFE_RELEASE(pickerView);
}

void PickerViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	CALabel* dateTime = CALabel::createWithCenter(CCRect(size.width*0.5, size.height*0.1, size.width, size.height*0.1));
	dateTime->setTag(100);
	dateTime->setText(year + "/" + month + "/" + day);
	dateTime->setFontSize(30 * CROSSAPP_ADPTATION_RATIO);
	dateTime->setColor(CAColor_blueStyle);
	dateTime->setTextAlignment(CATextAlignmentCenter);
	dateTime->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	this->getView()->addSubview(dateTime);

	pickerView = CAPickerView::createWithCenter(CCRect(size.width*0.5, size.height*0.5, size.width*0.6, size.height*0.5));
	pickerView->setDelegate(this);
	pickerView->setDataSource(this);
	pickerView->setFontSizeNormal(30 * CROSSAPP_ADPTATION_RATIO);
	pickerView->setFontSizeSelected(30 * CROSSAPP_ADPTATION_RATIO);
	pickerView->setFontColorNormal(CAColor_blueStyle);
	pickerView->setFontColorSelected(CAColor_blueStyle);
	pickerView->setSeparateColor(CAColor_blueStyle);
	pickerView->reloadAllComponents();
	this->getView()->addSubview(pickerView);
}

void PickerViewTest::viewDidUnload()
{
	this->getView()->removeAllSubviews();
}

void PickerViewTest::didSelectRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
{
	CALabel* label = (CALabel*)this->getView()->getSubviewByTag(100);
	if (component == 0)
	{
		CCLog("%d",row);
		if ((((1975 + row) % 4 == 0) && ((1975 + row) % 100 != 0)) || ((1975 + row) % 400 == 0))
		{
			yearFlag = true;
		}
		year = CCString::createWithFormat("%d", 1975 + row)->getCString();
	}
	else if (component == 1)
	{
		if (row == 1)
		{
			daysOfMonth = yearFlag ? 29 : 28;
			CCLog("%d",daysOfMonth);
		}
		else
		{
			int b[7] = { 0, 2, 4, 6, 7, 9, 11 };
			for (int j = 0; j < 7; j++)
			{
				daysOfMonth = (row == b[j]) ? 31 : 30;
			}
		}
		pickerView->reloadComponent(2);
		month = CCString::createWithFormat("%02d", row + 1)->getCString();
	}
	else
	{
		day = CCString::createWithFormat("%02d", row + 1)->getCString();
	}
	if (month.empty() && day.empty())
	{
		month = "01";
		day = "01";
	}
	label->setText(year+"/"+month+"/"+day);
}

unsigned int PickerViewTest::numberOfComponentsInPickerView(CAPickerView* pickerView)
{
    return 3;
}

unsigned int PickerViewTest::numberOfRowsInComponent(CAPickerView* pickerView, unsigned int component)
{
	int rowNum = 0;
	CCLog("%d", daysOfMonth);
	return rowNum = component == 0 ? 50 : (component == 1 ? 12 : daysOfMonth);
}

float PickerViewTest::widthForComponent(CAPickerView* pickerView, unsigned int component)
{
    return pickerView->getFrame().size.width/3;
}

float PickerViewTest::rowHeightForComponent(CAPickerView* pickerView, unsigned int component)
{
	return pickerView->getFrame().size.height / 7;
}

CCString* PickerViewTest::titleForRow(CAPickerView* pickerView, unsigned int row, unsigned int component)
{
	char dateValue[20] = "";
	if (component == 0)
	{
		sprintf(dateValue, "%d", 1975 + row);
	}
	else
	{
		sprintf(dateValue, "%d", row+1);
	}
	return CCString::create(dateValue);
}



