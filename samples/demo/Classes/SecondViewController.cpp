//
//  SecondViewController.cpp
//  HelloCpp
//
//  Created by MAC on 14-6-14.
//
//

#include "SecondViewController.h"
#include "FirstViewController.h"
SecondViewController::SecondViewController()
{

}

SecondViewController::~SecondViewController()
{

}

void SecondViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    CCSize size = this->getView()->getBounds().size;
    
    CAButton* btn = CAButton::createWithCenter(CCRect(size.width/2, 100, 150 * CROSSAPP_ADPTATION_RATIO, 60 * CROSSAPP_ADPTATION_RATIO), CAButtonTypeRoundedRect);
    btn->setTitleForState(CAControlStateAll, "pressenView");
    this->getView()->addSubview(btn);
    btn->addTarget(this, CAControl_selector(SecondViewController::touchUpInSide), CAControlEventTouchUpInSide);
    btn->setTag(100);
    
    CALabel* pLabel = new CALabel();
	pLabel->initWithCenter(CCRectMake(size.width / 2, size.height / 2 + 200, size.width-100, size.height - 200));
	pLabel->setTextAlignment(CATextAlignmentLeft);
	pLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	pLabel->setText("昨天下午，四惠东附近的一公交车队内，一辆满是乘客的公交车内上演惊险一幕。一中年男子站在车尾，将手中水瓶里的“汽油”倾倒在地，随后拿出打火机，激动地喊话。此时，乘务员一边分散男子注意力，一边试图疏导乘客。同时，车上的专职安全员也上前劝阻并准备拿起灭火器。很快，公交车慢慢降速，停靠在路边并熄火，打开双闪灯。男子见司机要打开车门疏散乘客，情绪越来越激动，最终“点燃”了车厢内泼洒的“汽油”。同时车门迅速打开，大家在乘务员和安全员的帮助下捂鼻逃生，司机立即启动车厢内自动灭火系统。不到两分钟，“火焰”被扑灭，“纵火”男子被安全员和司乘人员制服。随后110民警赶到现场将嫌犯带走。此情此景，是昨日北京市公安局联合公交集团开展公交车反恐防暴的一场演练。");
	pLabel->setFontSize(36);
	this->getView()->addSubview(pLabel);
	pLabel->release();
    
    
    CAView* bg = CAView::createWithFrame(pLabel->getBounds(), CAColor_green);
    pLabel->insertSubview(bg, -1);
    
}

void SecondViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
    
    
}

void SecondViewController::touchUpInSide(CAControl* sender, CCPoint point)
{
    FirstViewController* viewController = new FirstViewController();
    viewController->init();
    
    this->presentModalViewController(viewController, true);
}