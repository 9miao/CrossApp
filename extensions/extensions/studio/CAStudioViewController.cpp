//
//  CAStudioViewController.cpp
//  Demo
//
//  Created by lh on 14-6-30.
//
//

#include "CAStudioViewController.h"
CAStudioViewController::CAStudioViewController()
:m_pDelegateTarget(NULL)
,m_pDataSourceTarget(this)
, m_sJsonName("")
{
	m_pViewDicByName = CCDictionary::create();
	m_pViewDicByName->retain();
}

CAStudioViewController::~CAStudioViewController()
{
    
}
void CAStudioViewController::viewDidLoad()
{
	
	
}
void CAStudioViewController::viewDidUnload()
{

}
bool CAStudioViewController::init()
{

	return true;
}
bool CAStudioViewController::initWithJson(CSJson::Value var)
{
	CAViewController::init();
	sendStudioJson(var);
	return true;
}
void CAStudioViewController::sendStudioJson(CSJson::Value var)
{

	BaseAnalyze *analyze = new BaseAnalyze();
	analyze->name = var["name"].asString();
	analyze->width = var["width"].asFloat();
	analyze->height = var["height"].asFloat();

	controls = var["controls"];
	names = controls.getMemberNames();
	for (std::vector<std::string>::iterator i = names.begin(); i != names.end(); i++)
	{
		CSJson::Value value = controls[(*i).c_str()];
		getItemType(value, (*i));
	}
}

void CAStudioViewController::getItemType(CSJson::Value sender, std::string itemName)
{
    ITEMSTYPE typeId =(ITEMSTYPE)sender["TypeMode"].asInt();
    
    CSJson::Value location = sender["Location"];
    CSJson::Value size = sender["Size"];
    CCRect rect = CCRectMake(location["X"].asFloat(), location["Y"].asFloat(), size["Width"].asFloat(), size["Height"].asFloat());
    
    if (typeId == CA_TYPE_VIEW)
    {
        CAView *view = CAView::createWithFrame(rect);
		setPropertiesForView(view, sender);
		m_pViewDicByName->setObject(view,itemName);
    }
    else if (typeId == CA_TYPE_TEXTFIELD)
    {
        CATextField *textfiled = CATextField::createWithFrame(rect);
		setPropertiesForTextField(textfiled, sender);
		m_pViewDicByName->setObject(textfiled,itemName);
    }
    else if(typeId == CA_TYPE_TABLEVIEW)
    {
        CATableView *tableview = CATableView::createWithFrame(rect);
		setPropertiesForTableView(tableview, sender);
		m_pViewDicByName->setObject(tableview,itemName);
    }
    else if (typeId == CA_TYPE_SWITCH)
    {
        CASwitch *switchs = CASwitch::createWithFrame(rect);
		setPropertiesForSwitch(switchs, sender);
		m_pViewDicByName->setObject(switchs, itemName);

    }
    else if (typeId == CA_TYPE_SCROLLVIEW)
    {
        CAScrollView *scrollview = CAScrollView::createWithFrame(rect);
		setPropertiesForScrollView(scrollview, sender);
		m_pViewDicByName->setObject(scrollview,itemName);
    }
    else if(typeId == CA_TYPE_PROGRESS)
    {
        CAProgress *progress = CAProgress::create();
        progress->setFrame(rect);
		setPropertiesForProgress(progress, sender);
		m_pViewDicByName->setObject(progress,itemName);
    }
    else if(typeId == CA_TYPE_LABEL)
    {
        CALabel *label = CALabel::createWithFrame(rect);
		setPropertiesForLabel(label, sender);
		m_pViewDicByName->setObject(label, itemName);
    }
    else if(typeId == CA_TYPE_IMAGE)
    {
        CAImageView *imageview = CAImageView::createWithFrame(rect);
		setPropertiesForImageView(imageview, sender);
		m_pViewDicByName->setObject(imageview,itemName);
    }
    else if(typeId == CA_TYPE_BUTTON)
    {
        CAButton *button = CAButton::createWithFrame(rect,(CAButtonType)(sender["BtnType"].asInt()-1));
		setPropertiesForButton(button, sender);
		m_pViewDicByName->setObject(button,itemName);
    }

}
CAView* CAStudioViewController::getViewByName(std::string viewName)
{
	return (CAView*)m_pViewDicByName->objectForKey(viewName.c_str());
}
CAView* CAStudioViewController::getViewByTag(int tag)
{
	return (CAView*)m_pViewDicByName->objectForKey(tag);
}
void CAStudioViewController::setPropertiesForBaseObject(CAObject *sender, CSJson::Value dic)
{
 
}
void CAStudioViewController::setPropertiesForButton(CAObject *sender, CSJson::Value dic)
{
    CAButton *btn = (CAButton *)sender;
    CSJson::Value touchsize = dic["TouchSize"];
    CSJson::Value backGround = dic["BackGroundcolor"];
    CSJson::Value foreText = dic["ForeText"];
    CSJson::Value textColorStruct = foreText["Color"];
    std::string backGroundImage = dic["BackgroundImage"].asString();
    std::string bgHighLighted = dic["BgHighLighted"].asString();
    std::string bgDisabled = dic["BgDisabled"].asString();
    std::string bgSelected = dic["BgSelected"].asString();
    std::string foreHighLighted = dic["ForeHighLighted"].asString();
    std::string foreDisabled = dic["ForeDisabled"].asString();
    std::string foreSelected = dic["ForeSelected"].asString();
    std::string btnText = foreText["Text"].asString();
    bool tc_isEmpty = textColorStruct["IsEmpty"].asBool();
    bool bgc_isEmpty = backGround["IsEmpty"].asBool();
    bool tcs_isEmpty = touchsize["IsEmpty"].asBool();
    bool isEffect = dic["IsEffect"].asBool();
    bool isClick = dic["IsClick"].asBool();
    bool isTouchSuspend = dic["IsTouchSuspend"].asBool();
    int alpha = dic["Alpha"].asInt();
    int tag = dic["Tag"].asInt();
    CAColor4B backgroundcolor = ccc4(backGround["R"].asInt(), backGround["G"].asInt(), backGround["B"].asInt(), backGround["A"].asInt());
    
    CAColor4B textColor = ccc4(textColorStruct["R"].asInt(), textColorStruct["G"].asInt(), textColorStruct["B"].asInt(), textColorStruct["A"].asInt());
    btn->setAlpha(alpha);
    btn->setTag(tag);
    
    if (!bgc_isEmpty)
    {
        btn->setBackGroundViewForState(CAControlStateNormal, CAView::createWithColor(backgroundcolor));
    }
    
    std::string s = backGroundImage.substr(0,backGroundImage.length()-1);

    if (strcmp(s.c_str(), ""))
    {
        btn->setBackGroundViewForState(CAControlStateNormal,CAScale9ImageView::createWithImage(CAImage::create(s.c_str())));
    }
    if (strcmp(bgHighLighted.c_str(), ""))
    {
        btn->setBackGroundViewForState(CAControlStateHighlighted, CAImageView::createWithImage(CAImage::create(bgHighLighted.c_str())));
    }
    if (strcmp(bgDisabled.c_str(), ""))
    {
        btn->setBackGroundViewForState(CAControlStateDisabled, CAImageView::createWithImage(CAImage::create(bgDisabled.c_str())));
    } 
    if (strcmp(bgSelected.c_str(), ""))
    {       
		btn->setBackGroundViewForState(CAControlStateSelected, CAImageView::createWithImage(CAImage::create(bgSelected.c_str())));
    }
    if (strcmp(foreHighLighted.c_str(), ""))
    {
        btn->setBackGroundViewForState(CAControlStateSelected, CAImageView::createWithImage(CAImage::create(bgSelected.c_str())));
    }
    if (strcmp(foreDisabled.c_str(), ""))
    {
        
    }
    if (strcmp(foreSelected.c_str(), ""))
    {
        
    }
    if (strcmp(btnText.c_str(), ""))
    {
        btn->setTitleForState(CAControlStateNormal, btnText);
        btn->setTitleFontName("Arial");
        if (!tc_isEmpty)
        {
            btn->setTitleColorForState(CAControlStateNormal, textColor);
        }
        
        
    }
    if (!tcs_isEmpty)
    {
        
    }
    if (isEffect)
    {
        
    }
    if (isClick)
    {
        
    }
    if (isTouchSuspend)
    {
        
    }
    this->getView()->addSubview(btn);
    
}
void CAStudioViewController::setPropertiesForLabel(CAObject *sender, CSJson::Value dic)
{
    CALabel *label = (CALabel *)sender;
    std::string text = dic["Text"].asString();
    std::string fontName = dic["Font"].asString();
    CSJson::Value FontColorStruct = dic["FontColor"];
    CAColor4B fontColor = ccc4(FontColorStruct["R"].asInt(), FontColorStruct["G"].asInt(), FontColorStruct["B"].asInt(), FontColorStruct["A"].asInt());
    float fontSie = dic["FontSize"].asFloat();
    CATextAlignment textAlignment = (CATextAlignment)dic["Alignment"].asInt();
    CAVerticalTextAlignment verticalTextAlignment = (CAVerticalTextAlignment)dic["LineAlignment"].asInt();
    CSJson::Value backColorStruct = dic["BackColor"];
    label->setColor(fontColor);
    label->setText(text);
    label->setFontName(fontName);
    label->setFontSize(fontSie);
    label->setTextAlignment(textAlignment);
    label->setVerticalTextAlignmet(verticalTextAlignment);
    this->getView()->addSubview(label);
    
}
void CAStudioViewController::setPropertiesForView(CAObject *sender, CSJson::Value dic)
{
    CAView *view = (CAView *)sender;
    CSJson::Value BackColorStruct = dic["BackColor"];
    bool bc_isEmpty = BackColorStruct["IsEmpty"].asBool();
    CAColor4B backColor = ccc4(BackColorStruct["R"].asInt(), BackColorStruct["G"].asInt(), BackColorStruct["B"].asInt(), BackColorStruct["A"].asInt());
    if (!bc_isEmpty)
    {
        view->setColor(backColor);
    }
    this->getView()->addSubview(view);
}
void CAStudioViewController::setPropertiesForScrollView(CAObject *sender, CSJson::Value dic)
{
    CAScrollView *scrollview = (CAScrollView *)sender;
    std::string backGroundImage = dic["BackgroundImage"].asString();
    CSJson::Value viewSizeStruct = dic["ViewSize"];
    bool vs_isEmpty = viewSizeStruct["IsEmpty"].asBool();
    CCSize viewsize = CCSizeMake(viewSizeStruct["Width"].asFloat(), viewSizeStruct["Height"].asFloat());
    if (!vs_isEmpty)
    {
        scrollview->setViewSize(viewsize);
    }
    
    bool bounces = dic["Bounces"].asBool();
    bool bouncesHorizontal = dic["BouncesHorizontal"].asBool();
    bool bouncesVertical = dic["BouncesVertical"].asBool();
    float minimumZoomScale = dic["MinimumZoomScale"].asFloat();
    float maximumZoomScale = dic["MaximumZoomScale"].asFloat();
//    float zoomScale = dic["ZoomScale"].asFloat();
    CSJson::Value backColorStruct = dic["BackColor"];
    CAColor4B backColor = ccc4(backColorStruct["R"].asInt(), backColorStruct["G"].asInt(), backColorStruct["B"].asInt(), backColorStruct["A"].asInt());
    bool bc_isEmpty = backColorStruct["IsEmpty"].asBool();
    if (!bc_isEmpty)
    {
        scrollview->setBackGroundColor(backColor);
    }
    if (strcmp(backGroundImage.c_str(), ""))
    {
        scrollview->setBackGroundImage(CAImage::create(backGroundImage.c_str()));
    }
    scrollview->setBounces(bounces);
    scrollview->setBounceHorizontal(bouncesHorizontal);
    scrollview->setBounceVertical(bouncesVertical);
    scrollview->setMinimumZoomScale(minimumZoomScale);
    scrollview->setMaximumZoomScale(maximumZoomScale);
    this->getView()->addSubview(scrollview);

}
void CAStudioViewController::setPropertiesForTableView(CAObject *sender, CSJson::Value dic)
{

    CATableView *tableview = (CATableView *)sender;
    std::string backGoundImage = dic["BackgroundImage"].asString();
    bool Bounces = dic["Bounces"].asBool();
    bool ScrollEnabled = dic["ScrollEnabled"].asBool();
    CSJson::Value backColorStruct = dic["BackColor"];
    bool bc_isEmpty = backColorStruct["IsEmpty"].asBool();
    CAColor4B backColor = ccc4(backColorStruct["R"].asInt(), backColorStruct["G"].asInt(), backColorStruct["B"].asInt(), backColorStruct["A"].asInt());
    tableview->setBounces(Bounces);
    tableview->setScrollEnabled(true);
    if (m_pDataSourceTarget)
    {
        tableview->setTableViewDataSource(m_pDataSourceTarget);
    }
    if (m_pDelegateTarget)
    {
        tableview->setTableViewDelegate(m_pDelegateTarget);
    }
    if (!bc_isEmpty)
    {
        tableview->setBackGroundColor(backColor);
    }
    tableview->setScrollEnabled(ScrollEnabled);
    this->getView()->addSubview(tableview);
    
}

void CAStudioViewController::setPropertiesForSwitch(CAObject *sender, CSJson::Value dic)
{
    
}

void CAStudioViewController::setPropertiesForProgress(CAObject *sender, CSJson::Value dic)
{
    CAProgress *progress =(CAProgress*)sender;
    float progressPer = dic["Progress"].asFloat();
    std::string backGroundImage = dic["BackgroundImage"].asString();
    std::string foreImage = dic["ForeImage"].asString();
    CSJson::Value BackColorStruct = dic["BackColor"];
    bool bc_isEmpty = BackColorStruct["IsEmpty"].asBool();
    CAColor4B backColor = ccc4(BackColorStruct["R"].asInt(), BackColorStruct["G"].asInt(), BackColorStruct["B"].asInt(), BackColorStruct["A"].asInt());
    if (!bc_isEmpty) {
        progress->setColor(backColor);
    }
    progress->setProgressTintImage(CAImage::create(backGroundImage.substr(0,backGroundImage.length()-1).c_str()));
    progress->setProgressTrackImage(CAImage::create(foreImage.substr(0,foreImage.length()-1).c_str()));
    progress->setProgress(progressPer);
    progress->setTag(100);
    this->getView()->addSubview(progress);

   
    
}

void CAStudioViewController::setPropertiesForTextField(CAObject *sender, CSJson::Value dic)
{
    CATextField *textfield = (CATextField *)sender;
    std::string placeHolder = dic["TextCue"].asString();
    std::string fontName = dic["Font"].asString();
    CSJson::Value FontColorStruct = dic["FontColor"];
    CAColor4B fontColor = ccc4(FontColorStruct["R"].asInt(), FontColorStruct["G"].asInt(), FontColorStruct["B"].asInt(), FontColorStruct["A"].asInt());
    bool fc_isEmpty = FontColorStruct["IsEmpty"].asBool();
    int fontSize = dic["FontSize"].asInt();
    eKeyBoardInputType inputMode = (eKeyBoardInputType)dic["InputMode"].asInt();
    CSJson::Value BackColorStruct = dic["BackColor"];
    bool bc_isEmpty = BackColorStruct["IsEmpty"].asBool();
    CAColor4B backColor = ccc4(BackColorStruct["R"].asInt(), BackColorStruct["G"].asInt(), BackColorStruct["B"].asInt(), BackColorStruct["A"].asInt());
    CSJson::Value textCueColorStruct = dic["ColorCue"];
    bool tcc_isEmpty = textCueColorStruct["IsEmpty"].asBool();
    CAColor4B textCueColor = ccc4(textCueColorStruct["R"].asInt(), textCueColorStruct["G"].asInt(), textCueColorStruct["B"].asInt(), textCueColorStruct["A"].asInt());
	textfield->setPlaceHolder(placeHolder);
    if (!tcc_isEmpty)
    {
        textfield->setSpaceHolderColor(textCueColor);
    }
    if (!fc_isEmpty)
    {
        textfield->setTextColor(fontColor);
    }
    if (!bc_isEmpty)
    {
        textfield->setColor(backColor);
    }
    textfield->setFontSize(fontSize);
    textfield->setInputType(inputMode);
    this->getView()->addSubview(textfield);
    
}
void CAStudioViewController::setPropertiesForImageView(CAObject *sender, CSJson::Value dic)
{
	CAImageView *imageview = (CAImageView *)sender;
    std::string backGroundImage = dic["BackgroundImage"].asString();
    CSJson::Value backGroundStruct = dic["BackColor"];
    bool bc_isEmpty = backGroundStruct["IsEmpty"].asBool();
    CAColor4B backGroundColor = ccc4(backGroundStruct["R"].asInt(), backGroundStruct["G"].asInt(), backGroundStruct["B"].asInt(), backGroundStruct["A"].asInt());
	imageview->setImage(CAImage::create(backGroundImage.c_str()));
    if (!bc_isEmpty)
    {
        imageview->setColor(backGroundColor);
    }
    this->getView()->addSubview(imageview);
    
}
void CAStudioViewController::setDataSourceTarget(CrossApp::CATableViewDataSource *var)
{
    m_pDataSourceTarget = var;
}
CrossApp::CATableViewDataSource *CAStudioViewController::getDataSourceTarget()
{
    return m_pDataSourceTarget;
}
void CAStudioViewController::setDelegateTarget(CrossApp::CATableViewDelegate *var)
{
    m_pDelegateTarget = var;
}

CrossApp::CATableViewDelegate *CAStudioViewController::getDelegateTarget()
{
    return m_pDelegateTarget;
}
void CAStudioViewController::reshapeViewRectDidFinish()
{

}