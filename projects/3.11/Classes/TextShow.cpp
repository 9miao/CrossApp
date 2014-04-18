//
//  TextShow.cpp
//  project
//
//  Created by 栗元峰 on 14-3-11.
//
//

#include "TextShow.h"
#include "LUrlSprite.h"
#include "LUrlHyperLink.h"
#define WINSIZE CCDirector::sharedDirector()->getWinSize()

#define X_URL "*#$*"
#define X_HYPERLINK "#!&#"

TextShow::TextShow(MainInterfaceData* data)
:m_pData(data)
,m_nUrlIndex(0)
,m_nHyperlinkIndex(0)
{
    m_pArray = new CCArray();
    
}

TextShow::~TextShow()
{
    delete m_pArray;
}

void TextShow::onEnterTransitionDidFinish()
{
    LRootLayer::onEnterTransitionDidFinish();
    
    MySocket::shareMySocket()->addDelegate(100, this);
    map<string, string> m;
    m["id"] = CCString::createWithFormat("%d",m_pData->ID)->getCString();
    m["catid"] = CCString::createWithFormat("%d",m_pData->catID)->getCString();
    MySocket::shareMySocket()->send(&m, 100);
}

void TextShow::onExitTransitionDidStart()
{
    LRootLayer::onExitTransitionDidStart();
    
    MySocket::shareMySocket()->removeDelegate(100, this);
}

TextShow* TextShow::create(MainInterfaceData* data)
{
    TextShow* layer = new TextShow(data);
    if (layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

bool TextShow::init()
{
    if (!LRootLayer::init())
    {
        return false;
    }
    
    CCLayerColor* layerColor = CCLayerColor::createWithFrame(CCRect(0, 0, m_obContentSize.width, m_obContentSize.height));
    this->addChild(layerColor);

    return true;
}

unsigned int TextShow::numberfRowsInTableView(LyfTableView *table)
{
    return 2 + m_nTextVec.size() + m_nPostReviewVec.size();
}

LyfTableViewCell* TextShow::tableCellAtIndex(LyfTableView *table, unsigned int idx)
{
    LyfTableViewCell* cell = LyfTableViewCell::createTmp();
    cell->setTouchEnabled(false);
    
    int width = WINSIZE.width - 15;
    int height = 0;
    
    if (idx == 0)
    {
        CCLabelTTF* ttf = CCLabelTTF::create("", "Arial", 30);
        ttf->setColor(ccBLACK);
        
        if (m_pData)
        {
            ttf->setString(m_pData->title.c_str());
        }
        
        height = ttf->getContentSize().height + 10;
        
        ttf->setPosition(CCPoint(width/2, height/2));
        cell->addChild(ttf);
    }
    else if (idx == 1)
    {
        CCLabelTTF* userNameTTF = CCLabelTTF::create("", "Arial", 20);
        userNameTTF->setAnchorPoint(CCPoint(0.0f, 0.0f));
        userNameTTF->setColor(ccBLACK);
        
        if (m_pData)
        {
            string userName = "作者：" + m_pData->username;
            userNameTTF->setString(userName.c_str());
        }
        
        height = userNameTTF->getContentSize().height + 10;
        
        userNameTTF->setPosition(CCPoint(m_obContentSize.width - 200, 10));
        cell->addChild(userNameTTF);
    }
    else  if (2 <= idx && idx - 2 < m_nTextVec.size())
    {
        unsigned int index = idx - 2;
        
        CCLog("---%d::: %s", index, m_nTextVec.at(index).c_str());
        
        if (m_nTextVec.at(index).compare(X_URL) == 0)
        {
            if (m_nUrlIndex >= m_nUrlVec.size())
                return cell;
            
            LUrlSprite* sprite = LUrlSprite::create(m_nUrlVec.at(m_nUrlIndex).c_str());
            sprite->setAnchorPoint(CCPoint(0.5, 0));
            sprite->setPosition(CCPoint(width/2, 0));
            cell->addChild(sprite);
            
            m_nUrlIndex++;
            
            height = sprite->getContentSize().height * sprite->getScaleY();
        }
        else if (m_nTextVec.at(index).compare(X_HYPERLINK) == 0)
        {
            if (m_nHyperlinkIndex >= m_nHyperlinkVec.size())
                return cell;
            
            LUrlHyperLink* urlHyperLink = LUrlHyperLink::create(m_nHyperlinkVec.at(m_nHyperlinkIndex).c_str(), width);
            urlHyperLink->setAnchorPoint(CCPointZero);
            urlHyperLink->setPosition(CCPoint(5, 0));
            cell->addChild(urlHyperLink);
            
            m_nHyperlinkIndex++;
            
            height = urlHyperLink->getContentSize().height;
        }
        else
        {
            string str = "　　";
            str += m_nTextVec.at(index);
            
            CCLabelTTF* ttf = CCLabelTTF::create(str.c_str(),
                                                 "Arial",
                                                 25,
                                                 CCSize(width, 0),
                                                 kCCTextAlignmentLeft);
            ttf->setAnchorPoint(CCPointZero);
            ttf->setPosition(CCPoint(5, 0));
            ttf->setColor(ccBLACK);
            cell->addChild(ttf);
            
            height = ttf->getContentSize().height;
        }
    }
    else
    {
        int index = idx - 2 - m_nTextVec.size();
        
        string contentStr = "　　" + m_nPostReviewVec.at(index).content;

        CCLabelTTF* contentTTF = CCLabelTTF::create(contentStr.c_str(),
                                             "Arial",
                                             25,
                                             CCSize(width, 0),
                                             kCCTextAlignmentLeft);
        contentTTF->setAnchorPoint(CCPointZero);
        contentTTF->setPosition(CCPoint(5, 5));
        contentTTF->setColor(ccBLACK);
        cell->addChild(contentTTF);

        const char* userNameStr = m_nPostReviewVec.at(index).username.c_str();
        CCLabelTTF* userNameTTF = CCLabelTTF::create(userNameStr, "Arial", 20);
        userNameTTF->setAnchorPoint(CCPointZero);
        userNameTTF->setPosition(CCPoint(5, contentTTF->getContentSize().height + 10));
        userNameTTF->setColor(ccBLUE);
        cell->addChild(userNameTTF);
        
        time_t rawtime = m_nPostReviewVec.at(index).creat_at;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime (&rawtime);
        
        char timeStr[32];
        
        sprintf(timeStr, "    %04d-%02d-%02d %02d:%02d",
                timeinfo->tm_year+1900,
                timeinfo->tm_mon,
                timeinfo->tm_mday,
                timeinfo->tm_hour,
                timeinfo->tm_min);
        
        CCLabelTTF* timeTTF = CCLabelTTF::create(timeStr, "Arial", 20);
        timeTTF->setAnchorPoint(CCPointZero);
        timeTTF->setPosition(CCPoint(5 + userNameTTF->getContentSize().width + 5, userNameTTF->getPositionY()));
        timeTTF->setColor(ccGRAY);
        cell->addChild(timeTTF);
        
        height = contentTTF->getContentSize().height + userNameTTF->getContentSize().height + 15;
        
    }
    
    
    cell->setContentSize(CCSize(WINSIZE.width-5, height+5));

    return cell;
}

void TextShow::show()
{
    m_pTableView = LyfTableView::create(this->getContentSize().height);
    m_pTableView->setTableViewDataSource(this);
    m_pTableView->setPosition(CCPointZero);
    this->addChild(m_pTableView);
    m_pTableView->setIndicatorPointOff(-10);
}

static bool comparePostReviewVec(PostReviewData a, PostReviewData b)
{
    if (a.creat_at < b.creat_at)
    {
        return false;
    }
    return true;
}

void TextShow::callBackWithSocket(int msgID, const CSJson::Value& root)
{
    string text = root["content"][0].asString();

    while (1)
    {
        int pos = text.find("\r\n", 0);
        CC_BREAK_IF(pos == string::npos);
        text.erase(pos, 2);
    }
        
    int pos = 0;
    while (1)
    {
        int p = text.find(X_URL, pos);
        int q = text.find(X_HYPERLINK, pos);
        if (p == string::npos && q == string::npos)
        {
            string s1 = text.substr(pos, text.length());
            m_nTextVec.push_back(s1);
            break;
        }
        else if (p != string::npos && q != string::npos)
        {
            if (p < q)
            {
                string s1 = text.substr(pos, p - pos);
                string s2 = text.substr(p, 4);
                m_nTextVec.push_back(s1);
                m_nTextVec.push_back(s2);
                pos = p + 4;
            }
            else
            {
                string s1 = text.substr(pos, q - pos);
                string s2 = text.substr(q, 4);
                m_nTextVec.push_back(s1);
                m_nTextVec.push_back(s2);
                pos = q + 4;
            }
        }
        else if (p != string::npos && q == string::npos)
        {
            string s1 = text.substr(pos, p - pos);
            string s2 = text.substr(p, 4);
            m_nTextVec.push_back(s1);
            m_nTextVec.push_back(s2);
            pos = p + 4;
        }
        else if (p == string::npos && q != string::npos)
        {
            string s1 = text.substr(pos, q - pos);
            string s2 = text.substr(q, 4);
            m_nTextVec.push_back(s1);
            m_nTextVec.push_back(s2);
            pos = q + 4;
        }
    }
    
    int urlSize = root["content"][1].size();
    for (int i=0; i<urlSize; i++)
    {
        string url = root["content"][1][i].asString();
        m_nUrlVec.push_back(url);
    }
    
    int hyperlinkSize = root["content"][2].size();
    for (int i=0; i<hyperlinkSize; i++)
    {
        string hyperlink = root["content"][2][i].asString();
        m_nHyperlinkVec.push_back(hyperlink);
    }

    CSJson::Value _postReview = root["data"];
    
    for (unsigned int i=0; i<_postReview.size(); i++)
    {
        PostReviewData data;
        data.username = _postReview[i]["username"].asString();
        data.status = _postReview[i]["status"].asInt();
        data.direction = _postReview[i]["direction"].asInt();
        data.uID = _postReview[i]["uid"].asInt();
        data.ip = _postReview[i]["ip"].asString();
        data.support = _postReview[i]["support"].asInt();
        data.userID = _postReview[i]["userid"].asInt();
        data.uptime = _postReview[i]["uptime"].asInt();
        data.content = _postReview[i]["content"].asString();
        data.siteID = _postReview[i]["siteid"].asInt();
        data.creat_at = _postReview[i]["creat_at"].asInt();
        data.reply = _postReview[i]["reply"].asInt();
        data.vote = _postReview[i]["vote"].asInt();
        data.commentID = _postReview[i]["commentid"].asString();
        data.ID = _postReview[i]["id"].asInt();
        data.pID = _postReview[i]["pid"].asInt();
        m_nPostReviewVec.push_back(data);
    }

    //sort(m_nPostReviewVec.begin(), m_nPostReviewVec.end(), comparePostReviewVec);
}
