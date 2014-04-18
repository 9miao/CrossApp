//
//  MainInterfaceData.h
//  project
//
//  Created by 栗元峰 on 14-3-11.
//
//

#ifndef __project__MainInterfaceData__
#define __project__MainInterfaceData__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class MainInterfaceData: public CCObject
{
public:

    int ID;
    
    int catID;
    
    string title;
    
    string text;
    
    string username;
    
};

#endif /* defined(__project__MainInterfaceData__) */
