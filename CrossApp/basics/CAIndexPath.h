//
//  CAIndexPath.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-7-27.
//  Copyright (c) 2014å¹?http://9miao.com All rights reserved.
//

#ifndef __CrossApp__CAIndexPath__
#define __CrossApp__CAIndexPath__

#include <iostream>
#include "CAObject.h"

NS_CC_BEGIN

struct CC_DLL CAIndexPath2E
{
    unsigned int section;
    unsigned int row;
    
    CAIndexPath2E();
    CAIndexPath2E(unsigned int section, unsigned int row);
    
    bool operator!= (const CAIndexPath2E& other) const;
    bool operator== (const CAIndexPath2E& other) const;
    bool operator< (const CAIndexPath2E& other) const;
    bool operator> (const CAIndexPath2E& other) const;
};

struct CC_DLL CAIndexPath3E
{
    unsigned int section;
    unsigned int row;
    unsigned int item;
    
    CAIndexPath3E();
    CAIndexPath3E(unsigned int section, unsigned int row, unsigned int item);
    
    bool operator!= (const CAIndexPath3E& other) const;
    bool operator== (const CAIndexPath3E& other) const;
    bool operator< (const CAIndexPath3E& other) const;
    bool operator> (const CAIndexPath3E& other) const;
};

const CAIndexPath2E CAIndexPath2EZero = CAIndexPath2E();
const CAIndexPath3E CAIndexPath3EZero = CAIndexPath3E();

NS_CC_END

#endif /* defined(__CrossApp__CAIndexPath__) */
