#include "TextViewTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

TextViewTest::TextViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(false);
}

TextViewTest::~TextViewTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(true);
}

bool TextViewTest::onTextViewAttachWithIME(CATextView* sender)
{
	CCLog("----1----");
	return true;
}

bool TextViewTest::onTextViewDetachWithIME(CATextView* sender)
{
	CCLog("----2----");
	return true;
}

bool TextViewTest::onTextViewInsertText(CATextView* sender, const char * text, int nLen)
{
	CCLog("----3----");
	return true;
}

//bool TextViewTest::onTextViewDeleteBackward(CATextView* sender, const char * delText, int nLen)
//{	CCLog("----4----");
//	return true;
//}

void TextViewTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

    CATextField* textField = CATextField::createWithCenter(CADipRect(size.width*0.5, size.height*0.2, size.width*0.6, size.height*0.05));
    textField->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded_highlighted.png")));
    textField->setPlaceHolder("Input");
    textField->setFontSize(_px(24));
    textField->setKeyboardType(KEY_BOARD_TYPE_NUMBER);
    this->getView()->addSubview(textField);
    
	CATextView* textView = CATextView::createWithCenter(CADipRect(size.width*0.5, size. height*0.5, 500, 300));
	textView->setTextViewDelegate(this);
	textView->setPlaceHolder("Multiline textfiled");
	//textView->setSpaceHolderColor(CAColor_blueStyle);
	//textView->setTextColor(CAColor_green);
	//textView->setCursorColor(CAColor_red);
	textView->setFontSize(_px(40));
	//textView->setWordWrap(false);
	//textView->setLineSpacing(50);
	//textView->setFontColor(CAColor_gray);
	//textView->setBackGroundColor(CAColor_blueStyle);
	textView->setBackGroundImage(CAImage::create("source_material/btn_square_selected.png"));
	//textView->setColor(CAColor_blue);
	this->getView()->addSubview(textView);
}

void TextViewTest::viewDidUnload()
{
	
}
