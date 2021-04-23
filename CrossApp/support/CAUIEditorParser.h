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
#include "tinyxml2/tinyxml2.h"


NS_CC_BEGIN
class CAViewController;
class CC_DLL CAUIEditorParser : public CAObject
{
public:
    
    CAUIEditorParser();
    
    virtual ~CAUIEditorParser();
    
    bool initWithPath(const std::string& filePath, CAView* superview);
    
	void parseViewControllItems(CAViewController* viewController);

    CAMap<std::string, CAView*> m_mViews;
    
protected:
    
    CAView* m_pSupverview;

	CAViewController* m_pViewController;

	tinyxml2::XMLDocument* m_pMyDocument;
};

NS_CC_END
#endif /* CAUIEditorParser_h */
