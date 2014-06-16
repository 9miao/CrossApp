//
//  CAUIHelper.cpp
//  iBook
//
//  Created by lh on 14-4-25.
//
//

#include "CAUIHelper.h"

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
    CCLog("%s",fullpath.c_str());
    unsigned long size = 0;
    char *des = (char*) CCFileUtils::sharedFileUtils()->getFileData(filename, "r", &size);
    CSJson::Reader reader;
    CSJson::Value root;
    CSJson::Value description;
    CSJson::Value rect;
    CCObject *ui = new CCObject();
    
    if (!reader.parse(des, root))
    {
        CCLog("error");
        return NULL;
    }
    CCLog("%s",des);
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

CCObject* CAUIHelper::addButton(CSJson::Value jsonValue, cocos2d::CCRect &uiRect)
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
CCObject* CAUIHelper::addLabel(CSJson::Value jsonValue, CCRect &uiRect)
{
    CCLabelTTF *label=CCLabelTTF::create("", "黑体", 30);
    label->setFrame(uiRect);
    return NULL;
}
CCObject* CAUIHelper::addImage(CSJson::Value jsonValue, CCRect &uiRect)
{
    CAImageView *imageview = CAImageView::createWithImage(CAImage::create(""));
    imageview->setFrame(uiRect);
    return NULL;
}

NS_CC_EXT_END
