//
//  CAThemeManager.cpp
//  CrossApp
//
//  Created by 栗元峰 on 16/11/15.
//  Copyright © 2015年 http://www.9miao.com. All rights reserved.
//

#include "CAThemeManager.h"
#include "platform/CAFileUtils.h"
#include "view/CAAlertView.h"

NS_CC_BEGIN

static CAThemeManager::stringMap s_map_null = CAThemeManager::stringMap(std::map<std::string, std::string>());

CAThemeManager* CAThemeManager::create(const std::string& filePath)
{
    CAThemeManager* themeManager = new CAThemeManager(filePath);
    themeManager->init();
    themeManager->autorelease();
    return  themeManager;
}

CAThemeManager::CAThemeManager(const std::string& filePath)
:m_pMyDocument(nullptr)
,m_sThemePath(filePath)
{
    if (m_sThemePath.back() != '/')
    {
        m_sThemePath += "/";
    }

    unsigned long size = 0;
    const char* data = (const char*)FileUtils::getInstance()->getFileData(m_sThemePath + "theme.style", "rb", &size);    
	
	if (size > 0)
    {
        m_pMyDocument = new tinyxml2::XMLDocument();
        m_pMyDocument->Parse(data, size);
        free((void*)data);
        tinyxml2::XMLElement* rootElement = m_pMyDocument->RootElement();
        
        tinyxml2::XMLElement* controlXml = ((tinyxml2::XMLElement*)rootElement->FirstChildElement());

        while (controlXml)
        {
            std::map<std::string, std::string> map;
            const tinyxml2::XMLElement* pathXml = ((tinyxml2::XMLElement*)controlXml->FirstChildElement());;
            while (pathXml)
            {
                std::string pathType = pathXml->Attribute("path");
                if (pathType.compare("0")==0)
                {
                    map.insert(std::make_pair(pathXml->Attribute("name"), pathXml->Attribute("value")));
                }
                else if(pathType.compare("1")==0)
                {
                    map.insert(std::make_pair(pathXml->Attribute("name"), (m_sThemePath + pathXml->Attribute("value"))));
                }
                pathXml = (tinyxml2::XMLElement*)pathXml->NextSiblingElement();
            }
            
            m_mPathss.insert(std::make_pair(controlXml->Attribute("name"), stringMap(map)));
            controlXml = controlXml->NextSiblingElement();
        }
    }
    else
    {
        free((void*)data);
        CAAlertView::create("警告", "主题配置文件缺失")->show();
        return;
    }
    
//    for (auto& var : m_mPathss)
//    {
//        std::map<std::string, std::string>& map = var.second.map;
//        for (auto& it : map)
//        {
//            CCLog("name: %s, value: %s", it.first.c_str(), it.second.c_str());
//        }
//        CCLog("control   name: %s", var.first.c_str());
//    }
}

CAThemeManager::~CAThemeManager()
{
	CC_SAFE_DELETE(m_pMyDocument);
}

const CAThemeManager::stringMap& CAThemeManager::getThemeMap(const std::string& key)
{
    if (m_mPathss.count(key) == 0)
    {
        return s_map_null;
    }
    return m_mPathss.at(key);
}

NS_CC_END
