//
//  CAUIEditorParser.hpp
//  CrossApp
//
//  Created by 栗元峰 on 15/11/26.
//  Copyright © 2015年 http://www.9miao.com. All rights reserved.
//

#ifndef CAThemeManager_h
#define CAThemeManager_h

#include "basics/CAObject.h"
#include "tinyxml2/tinyxml2.h"
#include <map>

NS_CC_BEGIN

class CC_DLL CAThemeManager : public CAObject
{
public:
    
	struct stringMap
    {
        stringMap(std::map<std::string, std::string> var)
        {
            map = var;
        }
        
        const std::string& at(const std::string& key) const
        {
            if (map.find(key) == map.end())
            {
                return string_null;
            }
            return map.at(key);
        }
        
        std::map<std::string, std::string> map;
        
        std::string string_null;
    };
    
public:
    
    CAThemeManager(const std::string& filePath);
    
    virtual ~CAThemeManager();
    
    static CAThemeManager *create(const std::string& filePath);
    
    const stringMap& getThemeMap(const std::string& key);

protected:
    
    std::map<std::string, stringMap> m_mPathss;

	tinyxml2::XMLDocument* m_pMyDocument;
    
    std::string m_sThemePath;
};

NS_CC_END
#endif /* CAThemeManager_h */
