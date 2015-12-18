//
//  CAUIEditorParser.hpp
//  CrossApp
//
//  Created by 栗元峰 on 15/11/26.
//  Copyright © 2015年 http://www.9miao.com. All rights reserved.
//

#ifndef CAUIEditorParser_h
#define CAUIEditorParser_h

#include "basics/CAObject.h"
#include "basics/CASTLContainer.h"
NS_CC_BEGIN

CC_DLL class CAUIEditorParser : public CAObject
{
public:
    
    CAUIEditorParser();
    
    virtual ~CAUIEditorParser();
    
    bool initWithPath(const std::string& filePath, CAView* superview);
    
    CAMap<std::string, CAView*> m_mViews;
    
protected:
    
    CAView* m_pSupverview;
};

NS_CC_END
#endif /* CAUIEditorParser_h */
