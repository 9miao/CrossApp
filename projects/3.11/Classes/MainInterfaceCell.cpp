//
//  MainInterfaceCell.cpp
//  project
//
//  Created by 栗元峰 on 14-3-11.
//
//

#include "MainInterfaceCell.h"

MainInterfaceCell::MainInterfaceCell(MainInterfaceData* data)
:m_pData(data)
{

}

MainInterfaceCell* MainInterfaceCell::create(MainInterfaceData* data)
{
    MainInterfaceCell* cell = new MainInterfaceCell(data);
    if (cell && cell->init())
    {
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

bool MainInterfaceCell::init()
{
    if (!CCSprite::init())
    {
        return false;
    }
    
    this->setContentSize(CCSize(CCDirector::sharedDirector()->getWinSize().width, 100));
    
    m_pBg = CCScale9Sprite::create("tableView_cell.png");
    m_pBg->setPreferredSize(m_obContentSize);
    m_pBg->setFrame(CCRectZero);
    this->addChild(m_pBg, -1);
    
    CCLabelTTF* ttf = CCLabelTTF::create("", "Arial", 30);
    ttf->setPosition(ccpAdd(this->getContentSize()/2, CCPoint(0, 10)));
    this->addChild(ttf);
    ttf->setColor(ccBLACK);
    if (m_pData)
    {
        ttf->setString(m_pData->title.c_str());
    }
    
    CCLabelTTF* userNameTTF = CCLabelTTF::create("", "Arial", 20);
    userNameTTF->setAnchorPoint(CCPoint(0.0f, 0.0f));
    userNameTTF->setPosition(CCPoint(m_obContentSize.width - 200, 10));
    this->addChild(userNameTTF);
    userNameTTF->setColor(ccBLACK);
    if (m_pData)
    {
        string userName = "作者：" + m_pData->username;
        userNameTTF->setString(userName.c_str());
    }
    
    return true;
}

void MainInterfaceCell::setTouchedEffect()
{
    m_pBg->setColor(ccBLUE);
}

void MainInterfaceCell::setNoTouchedEffect()
{
    m_pBg->setColor(ccWHITE);
}