//
//  CAUIHelper.cpp
//  iBook
//
//  Created by lh on 14-4-25.
//
//

#include "CAUIHelper.h"
#include "view//CALabelStyle.h"

NS_CC_EXT_BEGIN

static CAUIHelper *helperInstance=NULL;
typedef enum _CAType
{
    CATypeButton = 1,
    CATypeLabel,
    CATypeImage
}CAType;
typedef enum _mCAButtonType{
    ButtonColor = 1,
    ButtonImage,
    ButtonGrid9,
    ButtonNone =0
}mCAButtonType;

CAUIHelper::CAUIHelper()
{
    m_UIDictionary = CCDictionary::create();
    m_UIDictionary->retain();
}
CAUIHelper::~CAUIHelper()
{
    CC_SAFE_DELETE(m_UIDictionary);
}
CAUIHelper *CAUIHelper::getInstance()
{
    if (!helperInstance)
    {
        helperInstance= new CAUIHelper();
    }
    return  helperInstance;
    
}

CCDictionary* CAUIHelper::createCAWidgetWithJsonFile(const char *filename)
{
    std::string fullpath= CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);

    unsigned long size = 0;
    char *des = (char*) CCFileUtils::sharedFileUtils()->getFileData(filename, "r", &size);
    CSJson::Reader reader;
    CSJson::Value root;
    CSJson::Value description;
    CSJson::Value rect;
    CAObject *ui = new CAObject();
    
    if (!reader.parse(des, root))
    {
        return NULL;
    }

    for (int i = 0; i<root.size(); i++)
    {
        description = root[i];
        CAType type = (CAType)description["typeMode"].asInt();
        rect = description["rect"];
        std::string name = description["name"].asString();
        float width = rect["width"].asFloat();
        float height = rect["height"].asFloat();
        float x = rect["x"].asFloat();
        float y = rect ["y"].asFloat();
        CCRect uirect = CCRectMake(x, y, width, height);
        // ui=CAButton::createWithFrame(CCRectMake(x, y, width, height));
        if (type == CATypeButton)
        {
            ui = addButton(description, uirect);
        }
        else if(type == CATypeLabel)
        {
            ui = addLabel(description, uirect);
        }
        else if(type == CATypeImage)
        {
            
        }
        m_UIDictionary->setObject(ui, name.c_str());
        
    }
    ui->release();
    return m_UIDictionary;
}

CAObject* CAUIHelper::addButton(CSJson::Value jsonValue, CrossApp::CCRect &uiRect)
{
    CAButton *btn = CAButton::createWithFrame(uiRect, CAButtonTypeRoundedRect);
    int tag = jsonValue["tag"].asInt();
//    bool isEffect = jsonValue["isEffect"].asBool();
//    bool isClick = jsonValue["isClick"].asBool();
//    bool isTouchSuspend = jsonValue["isTouchSuspend"].asBool();
    CAControlState  btnState = (CAControlState)jsonValue["btnState"].asInt();

    int alpha = jsonValue["alpha"].asInt();
    std::string imageRes = jsonValue["backgroundImage"].asString();
    mCAButtonType btntype = (mCAButtonType)jsonValue["btnType"].asInt();
    
    btn->setTag(tag);
    btn->setAlpha(alpha);
    btn->setControlState(btnState);
    if (btntype == ButtonColor)
    {
        
    }
    else if (btntype == ButtonGrid9)
    {
        CAScale9ImageView *image = CAScale9ImageView::createWithImage(CAImage::create(imageRes.c_str()));
    
        btn->setBackGroundViewForState(CAControlStateNormal, image);
        
    }
    else if (btntype == ButtonImage)
    {
        
    }
    else if (btntype == ButtonNone)
    {

    }
    return btn;
    
    
}
CAObject* CAUIHelper::addLabel(CSJson::Value jsonValue, CCRect &uiRect)
{

    return NULL;
}
CAObject* CAUIHelper::addImage(CSJson::Value jsonValue, CCRect &uiRect)
{
    CAImageView *imageview = CAImageView::createWithImage(CAImage::create(""));
    imageview->setFrame(uiRect);
    return NULL;
}

bool CAUIHelper::loadLabelStyle(const string& sFileName)
{
	unsigned long size = 0;
	char *des = (char*)CCFileUtils::sharedFileUtils()->getFileData(sFileName.c_str(), "r", &size);
	CSJson::Reader reader;
	CSJson::Value root;

	if (!reader.parse(des, root))
	{
		return false;
	}

	if (!root.isArray())
		return false;

	for (size_t i = 0; i < root.size(); i++)
	{
		CSJson::Value& style = root[i];
		string sStyleName = style["StyleName"].asString();
		string sFontName = style["FontName"].asString();
		int nFontSize = style["FontSize"].asInt();
		int nLineSpace = style["LineSpace"].asInt();
		string sColor = style["FontColor"].asString();
		int r = 0, g = 0, b = 0, a = 0xff;
		sscanf(sColor.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
		CAColor4B cFontColor = ccc4(r, g, b, a);
		bool bBold = style["Bold"].asBool();
		bool bItalics = style["Italics"].asBool();
		bool bWordWrap = style["WordWrap"].asBool();

		CALabelStyleCache::sharedStyleCache()->addStyle(sStyleName, sFontName, _px(nFontSize), _px(nLineSpace), cFontColor, bBold, bItalics, bWordWrap);
	}

	return true;
}

NS_CC_EXT_END
