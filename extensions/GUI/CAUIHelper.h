//
//  CAUIHelper.h
//  iBook
//
//  Created by lh on 14-4-25.
//
//

#ifndef __iBook__CAUIHelper__
#define __iBook__CAUIHelper__

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
    CAObject *addButton(CSJson::Value jsonValue, CCRect &uiRect);
    
    CAObject *addLabel(CSJson::Value jsonValue, CCRect &uiRect);
    
    CAObject *addImage(CSJson::Value jsonValue, CCRect &uiRect);
    
    CAObject *addScrollView(CSJson::Value jsonValue,CCRect &uiRect);
    
    CAObject *addTableView(CSJson::Value jsonValue, CCRect &uiRect);
    
    
protected:
    
    CCDictionary *m_UIDictionary;
    
};

NS_CC_EXT_END
#endif /* defined(__iBook__CAUIHelper__) */
