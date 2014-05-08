//
//  CAUIHelper.h
//  iBook
//
//  Created by lh on 14-4-25.
//
//

#ifndef __iBook__CAUIHelper__
#define __iBook__CAUIHelper__

#include <iostream>
#include "Json/DictionaryHelper.h"
#include "Json/CSContentJsonDictionary.h"

NS_CC_EXT_BEGIN

#define CAHELPER CAUIHelper::getInstance()
class CAUIHelper
{
public:
    
    CAUIHelper();
    
    ~CAUIHelper();
    
    void init();
    
    CCDictionary *createCAWidgetWithJsonFile(const char *filename);
    
    static CAUIHelper* getInstance();
private:
    CCObject *addButton(CSJson::Value jsonValue, CCRect &uiRect);
    
    CCObject *addLabel(CSJson::Value jsonValue, CCRect &uiRect);
    
    CCObject *addImage(CSJson::Value jsonValue, CCRect &uiRect);
    
    CCObject *addScrollView(CSJson::Value jsonValue,CCRect &uiRect);
    
    CCObject *addTableView(CSJson::Value jsonValue, CCRect &uiRect);
    
    
protected:
    
    CCDictionary *m_UIDictionary;
    
};

NS_CC_EXT_END
#endif /* defined(__iBook__CAUIHelper__) */
