
#include "TextFieldTest.h"

#define CAColor_blueStyle ccc4(51,204,255,255)

TextFieldTest::TextFieldTest() : m_pTextField(NULL)
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(false);
}

TextFieldTest::~TextFieldTest()
{
	CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
	drawer->setTouchMoved(true);
}

void TextFieldTest::viewDidLoad()
{
	size = this->getView()->getBounds().size;

	CATextField* textField = CATextField::createWithCenter(CADipRect(size.width*0.5, size.height*0.2, size.width*0.6, size.height*0.05));
	textField->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded_highlighted.png")));
	textField->setPlaceHolder("Input");
	textField->setFontSize(_px(24));
    textField->setKeyboardType(KEY_BOARD_TYPE_NORMAL);
	this->getView()->addSubview(textField);

	CATextField* textField1 = CATextField::createWithCenter(CADipRect(size.width*0.5, size.height*0.4, size.width*0.6, size.height*0.05));
	textField1->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/ex1.png")));
	textField1->setPlaceHolder("Please enter some infomation");
	textField1->setFontSize(_px(24));
    textField1->setKeyboardType(KEY_BOARD_TYPE_NUMBER);
	textField1->setSpaceHolderColor(CAColor_yellow);
	this->getView()->addSubview(textField1);

	CATextField* textField2 = CATextField::createWithCenter(CADipRect(size.width*0.5, size.height*0.6, size.width*0.6, size.height*0.05));
	textField2->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_rounded3D_selected.png")));
	textField2->setPlaceHolder("Please enter some infomation");
	textField2->setFontSize(_px(24));
	textField2->setTextColor(CAColor_red);
	textField2->setSpaceHolderColor(CAColor_blue);
    textField2->setDelegate(this);
	this->getView()->addSubview(textField2);

	CATextField* textField3 = CATextField::createWithCenter(CADipRect(size.width*0.5, size.height*0.8, size.width*0.6, size.height*0.05));
	textField3->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create("source_material/ex3.png")));
	textField3->setPlaceHolder("Please enter some infomation");
	textField3->setFontSize(_px(24));
	textField3->setSpaceHolderColor(CAColor_green);
	textField3->setInputType(KEY_BOARD_INPUT_PASSWORD);
    textField3->setDelegate(this);
	this->getView()->addSubview(textField3);
}

void TextFieldTest::viewDidUnload() 
{
}

bool TextFieldTest::getKeyBoardHeight(int height)
{
    int fixHeight = height + 2 * m_pTextField->getFrame().size.height + m_pTextField->getFrameOrigin().y - getView()->getFrame().size.height;
    if(fixHeight > 0){
        //开始执行动画
        CAViewAnimation::beginAnimations("", NULL);
        //动画时长
        CAViewAnimation::setAnimationDuration(0.2f);
        //动画延迟时长执行
        //CAViewAnimation::setAnimationDelay(0.3f);
        //动画波频控制
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        
        //动画开始前回调(两参数)
        CAViewAnimation::setAnimationWillStartSelector(this, CAViewAnimation2_selector(TextFieldTest::willStartAction));
        //动画完成回调(两参数)
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation2_selector(TextFieldTest::didStopAction));
        
        CCRect rect = getView()->getFrame();
        rect.origin.y = -fixHeight;
        getView()->setFrame(rect);
        
        //执行动画
        CAViewAnimation::commitAnimations();

    }
    return true;
}

bool TextFieldTest::onTextFieldAttachWithIME(CATextField * sender)
{
    m_pTextField = sender;
    return true;
}

bool TextFieldTest::onTextFieldDetachWithIME(CATextField * sender)
{
    if(getView()->getFrameOrigin().y != 0){
        //开始执行动画
        CAViewAnimation::beginAnimations("", NULL);
        //动画时长
        CAViewAnimation::setAnimationDuration(0.2f);
        //动画延迟时长执行
        //CAViewAnimation::setAnimationDelay(0.3f);
        //动画波频控制
        CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
        
        //动画开始前回调(两参数)
        CAViewAnimation::setAnimationWillStartSelector(this, CAViewAnimation2_selector(TextFieldTest::willStartAction));
        //动画完成回调(两参数)
        CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation2_selector(TextFieldTest::didStopAction));
        
        CCRect rect = getView()->getFrame();
        rect.origin.y = 0;
        getView()->setFrame(rect);
        
        //执行动画
        CAViewAnimation::commitAnimations();

    }
    return true;
}

#pragma mark - CAViewAnimation
void TextFieldTest::willStartAction(const string& animationID, void* context)
{
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
}

void TextFieldTest::didStopAction(const string& animationID, void* context)
{
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
}
