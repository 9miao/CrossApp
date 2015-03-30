#include "FourthViewController.h"
#include "FifthViewController.h"

#pragma execution_character_set("utf-8")

#define CAColor_blueStyle ccc4(51,204,255,255)

FourthViewController::FourthViewController()
{

}

FourthViewController::~FourthViewController()
{

}

void FourthViewController::viewDidLoad()
{
    this->getView()->setColor(ccc4(240, 240, 240, 255));
	size = this->getView()->getBounds().size;
    action_index = 0;
    isDoAction = false;
    flag = false;

    playAnimationBtn = CAButton::createWithCenter(CADipRect(size.width*0.5, 100, 200, 60), CAButtonTypeRoundedRect);
    playAnimationBtn->setTitleForState(CAControlStateNormal, "点击播放动画");
    playAnimationBtn->addTarget(this, CAControl_selector(FourthViewController::playAnimation), CAControlEventTouchUpInSide);
    this->getView()->addSubview(playAnimationBtn);
    
    defaultView = CAView::createWithColor(CAColor_gray);
    defaultView->setFrame(CADipRect(20, 200, size.width - 40, size.height - 240));
    this->getView()->addSubview(defaultView);
    
    viewSize = defaultView->getFrame().size;
    
    defaultLable = CALabel::createWithFrame(CADipRect(0, 0, viewSize.width, 60));
    defaultLable->setTextAlignment(CATextAlignmentCenter);
    defaultLable->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    defaultView->insertSubview(defaultLable, 10);
    defaultLable->setText("play animation 1");
    
    animationView1 = CAView::createWithFrame(defaultView->getBounds());
    defaultView->insertSubview(animationView1, 0);
    animationView1->setColor(ccc4(80, 200, 255, 255));
    

    animationView2 = CAScale9ImageView::createWithCenter(CADipRect(100, 100, 50, 50));
    animationView2->setImage(CAImage::create("source_material/btn_rounded_normal.png"));
    defaultView->insertSubview(animationView2, 1);

    defaultBtnL = CAButton::createWithCenter(CADipRect(60, 50, 100, 60), CAButtonTypeRoundedRect);
    defaultBtnL->setTitleForState(CAControlStateNormal, "last");
    defaultBtnL->addTarget(this, CAControl_selector(FourthViewController::buttonCallback), CAControlEventTouchUpInSide);
    defaultBtnL->setTag(1);
    defaultView->insertSubview(defaultBtnL, 11);
    defaultBtnL->setVisible(action_index > 0);
    
    defaultBtnR = CAButton::createWithCenter(CADipRect(viewSize.width-60, 40, 100, 60), CAButtonTypeRoundedRect);
    defaultBtnR->setTitleForState(CAControlStateNormal, "next");
    defaultBtnR->addTarget(this, CAControl_selector(FourthViewController::buttonCallback), CAControlEventTouchUpInSide);
    defaultBtnR->setTag(2);
    defaultView->insertSubview(defaultBtnR, 11);
    
}

void FourthViewController::viewDidAppear()
{
	CANavigationBarItem* item = CANavigationBarItem::create("Animation Test");
	this->getTabBarController()->setNavigationBarItem(item);
}

void FourthViewController::viewDidUnload()
{
    
}

void FourthViewController::playAnimation(CAControl* btn, CCPoint point)
{
    CC_RETURN_IF(isDoAction);
    this->doAction();
}

void FourthViewController::buttonCallback(CrossApp::CAControl *btn, CrossApp::CCPoint point)
{
    point = btn->convertToWorldSpace(point);
    CAButton* button = (CAButton*)btn;
    
    if (button->getTag() == 1)
    {
        --action_index;
        action_index = MAX(action_index, 0);
    }
    else
    {
        ++action_index;
        action_index = MIN(action_index, 9);
    }
    
    defaultBtnL->setVisible(action_index > 0);
    defaultBtnR->setVisible(action_index < 9);
    
    if (action_index < 6)
    {
        char str[32];
        sprintf(str, "play animation %d", action_index+1);
        defaultLable->setText(str);
    }
    else if (action_index < 9)
    {
        char str[32];
        sprintf(str, "play animation %d", action_index+1 - 6);
        defaultLable->setText(str);
    }
    else
    {
        defaultLable->setText("play animation delay");
    }
    
    
    flag = true;
    this->refreshView(false);
}

void FourthViewController::refreshView(bool animation)
{
    if (flag)
    {
        flag = false;
        if (animation)
        {
            switch (action_index)
            {
                case 6:
                {
                    CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseIn);
                }
                    break;
                case 7:
                {
                    CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
                }
                    break;
                case 8:
                {
                    CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseInOut);
                }
                    break;
                case 9:
                {
                    CAViewAnimation::setAnimationDelay(0.5f);
                    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
                }
                    break;
                default:
                    break;
            }
        }
        
        animationView1->setFrame(defaultView->getBounds());
        animationView1->setAlpha(1.0f);
        animationView1->setColor(ccc4(80, 200, 255, 255));
        animationView1->setRotation(0);
        animationView1->setScale(1.0f);
        animationView1->setSkewX(0);
        animationView1->setSkewY(0);
        animationView1->setVertexZ(0);
        animationView2->setFrame(CADipRect(100, 100, 50, 50));
        animationView2->setAlpha(1.0f);
        animationView2->setColor(CAColor_white);
        animationView2->setRotation(0);
        animationView2->setScale(1.0f);
        animationView2->setSkewX(0);
        animationView2->setSkewY(0);
        animationView2->setVertexZ(0);
    }
    else
    {
        flag = true;
        switch (action_index)
        {
            case 0:
            {
                animationView1->setCenter(CADipRect(viewSize.width/2, viewSize.height/2, 100, 100));
                animationView2->setFrame(CADipRect(400, 400, 50, 50));
            }
                break;
            case 1:
            {
                animationView1->setColor(CAColor_clear);
                animationView2->setFrame(CADipRect(100, 100, 400, 50));
            }
                break;
            case 2:
            {
                animationView1->setColor(CAColor_yellow);
                animationView1->setRotation(-1080);
                animationView1->setFrame(CADipRect(viewSize.width - 100 - 50, 100, 50, 50));
                animationView2->setRotation(1080);
                animationView2->setColor(CAColor_red);
            }
                break;
            case 3:
            {
                animationView1->setColor(CAColor_magenta);
                animationView2->setCenter(CADipRect(viewSize.width/2, viewSize.height/2, 100, 100));
                animationView2->setRotationY(360);
                animationView2->setColor(ccc4(255, 255, 0, 128));
            }
                break;
            case 4:
            {
                animationView2->setCenter(CADipRect(viewSize.width/2, viewSize.height/2, 50, 50));
                animationView2->setColor(ccc4(0, 255, 255, 100));
                animationView2->setVertexZ(300);
                animationView2->setSkewX(90);
                animationView2->setSkewY(-90);
            }
                break;
            case 5:
            {
                animationView2->setCenter(CADipRect(viewSize.width/2, viewSize.height/2, 50, 50));
                animationView2->setColor(CAColor_green);
                animationView2->setVertexZ(300);
                animationView2->setSkewX(180);
                animationView2->setSkewY(180);
            }
                break;
            case 6:
            {
                if (animation)
                {
                    CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseIn);
                }
                animationView1->setCenter(CADipRect(viewSize.width/2, viewSize.height/2, 100, 100));
                animationView2->setFrame(CADipRect(400, 400, 50, 50));
            }
                break;
            case 7:
            {
                if (animation)
                {
                    CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseOut);
                }
                animationView1->setCenter(CADipRect(viewSize.width/2, viewSize.height/2, 100, 100));
                animationView2->setFrame(CADipRect(400, 400, 50, 50));
            }
                break;
            case 8:
            {
                if (animation)
                {
                    CAViewAnimation::setAnimationCurve(CAViewAnimationCurveEaseInOut);
                }
                animationView1->setCenter(CADipRect(viewSize.width/2, viewSize.height/2, 100, 100));
                animationView2->setFrame(CADipRect(400, 400, 50, 50));
            }
                break;
            case 9:
            {
                if (animation)
                {
                    CAViewAnimation::setAnimationDelay(0.5f);
                }
                animationView1->setColor(CAColor_yellow);
                animationView1->setRotation(-1080);
                animationView1->setFrame(CADipRect(viewSize.width - 100 - 50, 100, 50, 50));
                animationView2->setRotation(1080);
                animationView2->setColor(CAColor_red);
                CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
            }
                break;
            default:
                break;
        }
    }
    
}

void FourthViewController::doAction()
{
    //开始执行动画
    CAViewAnimation::beginAnimations("", NULL);
    //动画时长
    CAViewAnimation::setAnimationDuration(1.0f);
    //动画延迟时长执行
    //CAViewAnimation::setAnimationDelay(0.3f);
    //动画波频控制
    CAViewAnimation::setAnimationCurve(CAViewAnimationCurveLinear);

    CAViewAnimation::setAnimationRepeatCount(1.5);

    CAViewAnimation::setAnimationRepeatAutoreverses(true);
    //动画开始前回调(两参数)
    CAViewAnimation::setAnimationWillStartSelector(this, CAViewAnimation2_selector(FourthViewController::willStartAction));
    //动画完成回调(两参数)
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation2_selector(FourthViewController::didStopAction));
    
    this->refreshView(true);
    
    //执行动画
    CAViewAnimation::commitAnimations();
}

void FourthViewController::willStartAction(const string& animationID, void* context)
{
    CCLog("willStartAction");
    isDoAction = true;
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsFalse();
}

void FourthViewController::didStopAction(const string& animationID, void* context)
{
    CCLog("didStopAction");
    isDoAction = false;
    CAApplication::getApplication()->getTouchDispatcher()->setDispatchEventsTrue();
}