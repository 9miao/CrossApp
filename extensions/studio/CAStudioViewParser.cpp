#include "CAStudioViewParser.h"
#include "Json/CSContentJsonDictionary.h"

NS_CC_EXT_BEGIN;

static CAStudioViewParser* s_pCAStudioParser = NULL;


CAStudioViewParser::CAStudioViewParser()
{
	registAllParseFunc();
}

CAStudioViewParser::~CAStudioViewParser()
{
    
}

CAStudioViewParser* CAStudioViewParser::getInstance()
{
	if (s_pCAStudioParser == NULL)
	{
		s_pCAStudioParser = new CAStudioViewParser();
	}
	return s_pCAStudioParser;
}

void CAStudioViewParser::destroyInstance()
{
	if (s_pCAStudioParser)
	{
		delete s_pCAStudioParser;
		s_pCAStudioParser = NULL;
	}
}

CAView* CAStudioViewParser::initWithFile(const char* file)
{
	unsigned long size = 0;
	unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(file, "r", &size);
	if (data == NULL || size <= 0)
		return NULL;

	std::string strDoc;
	strDoc.append((char*)data, size);
	delete []data;
	
	return initWithData(strDoc);
}

CAView* CAStudioViewParser::initWithData(const std::string& strDoc)
{
	CSJson::Value value;
	CSJson::Reader cReader;
	if (!cReader.parse(strDoc, value, false))
		return NULL;

	return initWithJson(value);
}

CAView* CAStudioViewParser::initWithJson(CSJson::Value& var)
{
	CSJsonDictionary csJson;
	csJson.initWithValue(var);

	eItemsType e = (eItemsType)csJson.getItemIntValue("CaType", -1);
	CAView* pView = createViewFactory(e);
	if (pView != NULL)
	{
		return (this->*m_RegParseFunMap[e])(csJson, pView);
	}
	return NULL;
}

void CAStudioViewParser::registAllParseFunc()
{
	m_RegParseFunMap[CAType_View] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForView;
	m_RegParseFunMap[CAType_ActivityIndicatorView] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForActivityIndicatorView;
	m_RegParseFunMap[CAType_AlertView] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForAlertView;
	m_RegParseFunMap[CAType_ImageView] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForImageView;
	m_RegParseFunMap[CAType_Label] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForLabel;
	m_RegParseFunMap[CAType_Progress] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForProgress;
	m_RegParseFunMap[CAType_Button] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForButton;
	m_RegParseFunMap[CAType_SegmentedControl] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForSegmentedControl;
	m_RegParseFunMap[CAType_Slider] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForSlider;
	m_RegParseFunMap[CAType_Stepper] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForStepper;
	m_RegParseFunMap[CAType_Switch] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForSwitch;
	m_RegParseFunMap[CAType_TextField] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForTextField;
	m_RegParseFunMap[CAType_WindowView] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForView;
	m_RegParseFunMap[CAType_Control] = (ParseJsonForViewFunc)&CAStudioViewParser::ParseJsonForControl;
}

CAView* CAStudioViewParser::createViewFactory(eItemsType eType)
{
	CAView* pView = NULL;
	switch (eType)
	{
	case CrossApp::extension::CAType_View:
		pView = new CAView;
		break;
	case CrossApp::extension::CAType_ActivityIndicatorView:
		pView = new CAActivityIndicatorView;
		break;
	case CrossApp::extension::CAType_AlertView:
		pView = new CAAlertView;
		break;
	case CrossApp::extension::CAType_ImageView:
		pView = new CAImageView;
		break;
	case CrossApp::extension::CAType_Label:
		pView = new CALabel;
		break;
	case CrossApp::extension::CAType_Progress:
		pView = new CAProgress;
		break;
	case CrossApp::extension::CAType_Button:
		pView = new CAButton(CAButtonTypeCustom);
		break;
	case CrossApp::extension::CAType_SegmentedControl:
		pView = new CASegmentedControl(1);
		break;
	case CrossApp::extension::CAType_Slider:
		pView = new CASlider;
		break;
	case CrossApp::extension::CAType_Stepper:
		pView = new CAStepper;
		break;
	case CrossApp::extension::CAType_Switch:
		pView = new CASwitch;
		break;
	case CrossApp::extension::CAType_TextField:
		pView = new CATextField;
		break;
	case CrossApp::extension::CAType_WindowView:
		pView = new CAView;
		break;
	case CrossApp::extension::CAType_Control:
		pView = new CAControl;
		break;
	default:
		break;
	}
	if (pView)
	{
		pView->init();
		pView->autorelease();
	}
	return pView;
}


CAView* CAStudioViewParser::ParseJsonForView(CSJsonDictionary& csJson, CAView* pView)
{
	pView->setTag(csJson.getItemIntValue("Tag", -1));
	pView->setTextTag(csJson.getItemStringValue("TextTag"));

	CSJsonDictionary* pSubJson = csJson.getSubDictionary("BackColor");
	pView->setColor(ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));

	int PointLR = csJson.getItemIntValue("PointLR", 0);
	int PointTB = csJson.getItemIntValue("PointTB", 0);
	int X = csJson.getItemIntValue("X", 0);
	int Y = csJson.getItemIntValue("Y", 0);
	int W = csJson.getItemIntValue("Width", 0);
	int H = csJson.getItemIntValue("Height", 0);

	CAView* pParentView = pView->getSuperview();
	if (PointLR && pParentView)
	{
		CCSize size = pParentView->getBounds().size;
		X = size.width - W - X;
	}
	if (PointTB && pParentView)
	{
		CCSize size = pParentView->getBounds().size;
		Y = size.height - H - Y;
	}
	pView->setFrame(CCRectMake(X, Y, W, H));

	pView->setAlpha(csJson.getItemFloatValue("Alpha", 1));
	pView->setZOrder(csJson.getItemIntValue("ZOrder", 0));
	pView->setScaleX(csJson.getItemFloatValue("ScaleX", 1));
	pView->setScaleY(csJson.getItemFloatValue("ScaleY", 1));
	pView->setVisible(csJson.getItemBoolvalue("Visible", true));
	pView->setRotation(csJson.getItemFloatValue("Rotaion", 0));
	pView->setSkewX(csJson.getItemFloatValue("SkewX", 0));
	pView->setSkewY(csJson.getItemFloatValue("SkewY", 0));
	pView->setAnchorPoint(CCPointMake(csJson.getItemFloatValue("CenterX", 0.5f), csJson.getItemFloatValue("CenterY", 0.5f)));

	int childViewSize = csJson.getArrayItemCount("childViews");
	for (size_t i = 0; i < childViewSize; i++)
	{
		CSJsonDictionary* pSubJson = csJson.getSubItemFromArray("childViews", i);
		eItemsType e = (eItemsType)pSubJson->getItemIntValue("CaType", -1);
		CAView* pSubView = createViewFactory(e);
		if (pSubView)
		{
			pView->addSubview(pSubView);
			((this->*m_RegParseFunMap[e])(*pSubJson, pSubView));
		}
	}

	return pView;
}

CAView* CAStudioViewParser::ParseJsonForActivityIndicatorView(CSJsonDictionary& csJson, CAActivityIndicatorView* pView)
{
	pView->setStyle((CAActivityIndicatorViewStyle)csJson.getItemIntValue("ActivityIndicatorStyle", 0));
	pView->setLoadingMinTime(csJson.getItemFloatValue("LoadingMinTime", 0));

	CAImageView* pBack = NULL;
	CAImageView* pIndicator = NULL;
	std::string cszBack = csJson.getItemStringValue("ActivityBack");
	if (!cszBack.empty())
	{
		pView->setActivityBackView(pBack = CAImageView::createWithImage(CAImage::create(cszBack)));
	}
	std::string cszActivityIndicator = csJson.getItemStringValue("ActivityIndicator");
	if (!cszActivityIndicator.empty())
	{
		pView->setStyle(CAActivityIndicatorViewStyleImage);
		pView->setActivityIndicatorView(pIndicator = CAImageView::createWithImage(CAImage::create(cszActivityIndicator)));
	}
	ParseJsonForView(csJson, pView);
	if (pBack)
	{
		pBack->setFrame(pView->getBounds());
	}
	if (pIndicator)
	{
		pIndicator->setFrame(pView->getBounds());
	}
	return pView;
}

CAView* CAStudioViewParser::ParseJsonForAlertView(CSJsonDictionary& csJson, CAAlertView* pView)
{
	return pView;
}

CAView* CAStudioViewParser::ParseJsonForImageView(CSJsonDictionary& csJson, CAImageView* pView)
{
	pView->setImage(CAImage::create(csJson.getItemStringValue("ImagePath")));
	pView->setImageViewScaleType((CAImageViewScaleType)csJson.getItemIntValue("CAImageViewScaleType", 1));
	
	ParseJsonForView(csJson, pView);
	CSJsonDictionary* pSubJson = csJson.getSubDictionary("ImageColor");
	pView->setColor(ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));
	return pView;
}

CAView* CAStudioViewParser::ParseJsonForLabel(CSJsonDictionary& csJson, CALabel* pView)
{
	ParseJsonForView(csJson, pView);

	pView->setText(csJson.getItemStringValue("Text"));
	pView->setWordWrap(csJson.getItemBoolvalue("WordWrap", false));
	pView->setLineSpacing(csJson.getItemIntValue("LineSpacing", 0));
	pView->setNumberOfLine(csJson.getItemIntValue("NumberOfLine", 0));

	pView->setFontName(csJson.getItemStringValue("FontName"));
	pView->setFontSize(csJson.getItemIntValue("FontSize", 20));
	pView->setBold(csJson.getItemBoolvalue("Bold", false));
	pView->setUnderLine(csJson.getItemBoolvalue("UnderLine", false));
	pView->setItalics(csJson.getItemBoolvalue("Italics", false));
	pView->setVerticalTextAlignmet((CAVerticalTextAlignment)csJson.getItemIntValue("VerticalTextAlignment", 1));
	pView->setTextAlignment((CATextAlignment)csJson.getItemIntValue("TextAlignment", 0));

	CSJsonDictionary* pSubJson = csJson.getSubDictionary("FontColor");
	pView->setColor(ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));
	return pView;
}

CAView* CAStudioViewParser::ParseJsonForProgress(CSJsonDictionary& csJson, CAProgress* pView)
{
	ParseJsonForView(csJson, pView);
	
	pView->setProgressTintImage(CAImage::create(csJson.getItemStringValue("ProgressTintImage")));
	CSJsonDictionary* pSubJson = csJson.getSubDictionary("ProgressTintColor");
	pView->setProgressTintColor(ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));

	pView->setProgressTrackImage(CAImage::create(csJson.getItemStringValue("ProgressTrackImage")));
	pSubJson = csJson.getSubDictionary("ProgressTrackColor");
	pView->setProgressTrackColor(ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));

	pView->setProgress(0.5f);

	return pView;
	
}

CAView* CAStudioViewParser::ParseJsonForButton(CSJsonDictionary& csJson, CAButton* pView)
{
	pView->setBackGroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create(csJson.getItemStringValue("BackImage_normal"))));
	pView->setBackGroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create(csJson.getItemStringValue("BackImage_highlighted"))));
	pView->setBackGroundViewForState(CAControlStateDisabled, CAScale9ImageView::createWithImage(CAImage::create(csJson.getItemStringValue("BackImage_disabled"))));
	pView->setBackGroundViewForState(CAControlStateSelected, CAScale9ImageView::createWithImage(CAImage::create(csJson.getItemStringValue("BackImage_selected"))));

	pView->setTitleForState(CAControlStateNormal, csJson.getItemStringValue("Text_normal"));
	pView->setTitleForState(CAControlStateHighlighted, csJson.getItemStringValue("Text_highlighted"));
	pView->setTitleForState(CAControlStateDisabled, csJson.getItemStringValue("Text_disabled"));
	pView->setTitleForState(CAControlStateSelected, csJson.getItemStringValue("Text_selected"));
	
	CSJsonDictionary* pSubJson = csJson.getSubDictionary("Color_normal");
	pView->getBackGroundViewForState(CAControlStateNormal)->setColor(ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));

	pSubJson = csJson.getSubDictionary("Color_highlighted");
	pView->getBackGroundViewForState(CAControlStateHighlighted)->setColor(ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));

	pSubJson = csJson.getSubDictionary("Color_disabled");
	pView->getBackGroundViewForState(CAControlStateDisabled)->setColor(ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));

	pSubJson = csJson.getSubDictionary("Color_selected");
	pView->getBackGroundViewForState(CAControlStateSelected)->setColor(ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));

	pSubJson = csJson.getSubDictionary("Text_color_normal");
	pView->setTitleColorForState(CAControlStateNormal, ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));

	pSubJson = csJson.getSubDictionary("Text_color_highlighted");
	pView->setTitleColorForState(CAControlStateHighlighted, ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));


	pSubJson = csJson.getSubDictionary("Text_color_disabled");
	pView->setTitleColorForState(CAControlStateDisabled, ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));

	pSubJson = csJson.getSubDictionary("Text_color_selected");
	pView->setTitleColorForState(CAControlStateSelected, ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));

	pView->setTitleFontName(csJson.getItemStringValue("Fontname"));
	return ParseJsonForControl(csJson, pView);
}

CAView* CAStudioViewParser::ParseJsonForSegmentedControl(CSJsonDictionary& csJson, CASegmentedControl* pView)
{
	return pView;
}

CAView* CAStudioViewParser::ParseJsonForSlider(CSJsonDictionary& csJson, CASlider* pView)
{
	pView->setValue(csJson.getItemFloatValue("Value", 0));
	pView->setMinValue(csJson.getItemFloatValue("MinValue", 0));
	pView->setMaxValue(csJson.getItemFloatValue("MaxValue", 100.0f));
	pView->setTrackHeight(csJson.getItemFloatValue("TrackHeigth", 3.0f));
	pView->setMinTrackTintImage(CAImage::create(csJson.getItemStringValue("MinTrackTintImage")));
	pView->setMaxTrackTintImage(CAImage::create(csJson.getItemStringValue("MaxTrackTintImage")));
	pView->setThumbTintImage(CAImage::create(csJson.getItemStringValue("ThumbTintImage")));
	return ParseJsonForControl(csJson, pView);
}

CAView* CAStudioViewParser::ParseJsonForStepper(CSJsonDictionary& csJson, CAStepper* pView)
{
	pView->setContinuous(csJson.getItemBoolvalue("Continuous", true));
	pView->setAutoRepeat(csJson.getItemBoolvalue("AutoRepeat", true));
	pView->setTouchEffect(csJson.getItemBoolvalue("TouchEffect", false));
	pView->setWraps(csJson.getItemBoolvalue("Wraps", false));
	pView->setValue(csJson.getItemFloatValue("Value", 0));
	pView->setMinValue(csJson.getItemFloatValue("MinValue", 0));
	pView->setMaxValue(csJson.getItemFloatValue("MaxValue", 100.0f));
	pView->setStepValue(csJson.getItemFloatValue("StepValue", 1.0f));
	
	pView->setBackgroundImage(CAImage::create(csJson.getItemStringValue("BackgroundImage")), CAControlStateAll);
	pView->setIncrementImage(CAImage::create(csJson.getItemStringValue("IncrementImage")), CAControlStateAll);
	pView->setDecrementImage(CAImage::create(csJson.getItemStringValue("DecrementImage")), CAControlStateAll);
	pView->setDividerImage(CAImage::create(csJson.getItemStringValue("DividerImage")), CAControlStateAll);
	return ParseJsonForControl(csJson, pView);
}

CAView* CAStudioViewParser::ParseJsonForSwitch(CSJsonDictionary& csJson, CASwitch* pView)
{
	pView->initWithFrame(CCRect(0, 0, 1, 1));
	pView->setIsOn(csJson.getItemBoolvalue("IsOpen", false), false);
	pView->setOnImage(CAImage::create(csJson.getItemStringValue("OnImage")));
	pView->setOffImage(CAImage::create(csJson.getItemStringValue("OffImage")));
	pView->setThumbTintImage(CAImage::create(csJson.getItemStringValue("ThumbTintImage")));

	return ParseJsonForControl(csJson, pView);
}

CAView* CAStudioViewParser::ParseJsonForTextField(CSJsonDictionary& csJson, CATextField* pView)
{
	pView->initWithFrame(CCRect(0, 0, 1, 1));
	pView->setPlaceHolder(csJson.getItemStringValue("PromptInfo"));

	CSJsonDictionary* pSubJson = csJson.getSubDictionary("PrompInfoColor");
	pView->setSpaceHolderColor(ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));

	pView->setText(csJson.getItemStringValue("TextInfo"));

	pSubJson = csJson.getSubDictionary("TextColor");
	pView->setTextColor(ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));
	pView->setFontSize(pSubJson->getItemIntValue("FontSize", 18));

	pSubJson = csJson.getSubDictionary("CursorColor");
	pView->setCursorColor(ccc4(
		pSubJson->getItemIntValue("R", 0),
		pSubJson->getItemIntValue("G", 0),
		pSubJson->getItemIntValue("B", 0),
		pSubJson->getItemIntValue("A", 0)));

	pView->setInputType((eKeyBoardInputType)csJson.getItemIntValue("KeyBoardInputType", 0));
	pView->setKeyboardType((eKeyBoardType)csJson.getItemIntValue("KeyBoardType", 0));
	pView->setKeyboardReturnType((eKeyBoardReturnType)csJson.getItemIntValue("KeyBoardReturnType", 0));
	pView->setBackgroundView(CAScale9ImageView::createWithImage(CAImage::create(csJson.getItemStringValue("BackgroundView"))));
	return ParseJsonForView(csJson, pView);
}

CAView* CAStudioViewParser::ParseJsonForControl(CSJsonDictionary& csJson, CAControl* pView)
{
	pView->setControlState((CAControlState)csJson.getItemIntValue("ControlStart", 0));
	pView->setControlStateLocked(csJson.getItemBoolvalue("ControlStateLocked", true));
	return ParseJsonForView(csJson, pView);
}


NS_CC_EXT_END
