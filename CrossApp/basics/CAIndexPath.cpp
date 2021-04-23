//
//  CAIndexPath.cpp
//  CrossApp
//
//  Created by MAC on 14-7-27.
//  Copyright (c) 2014�?厦门雅基软件有限公司. All rights reserved.
//

#include "CAIndexPath.h"

NS_CC_BEGIN

#pragma CAIndexPath2E


CAIndexPath2E::CAIndexPath2E()
:section(UINT_NONE)
,row(UINT_NONE)
{

}

CAIndexPath2E::CAIndexPath2E(unsigned int section, unsigned int row)
:section(section)
,row(row)
{
    
}

bool CAIndexPath2E::operator!= (const CAIndexPath2E& other) const
{
    return (this->section != other.section)
            || (this->row != other.row);
}

bool CAIndexPath2E::operator== (const CAIndexPath2E& other) const
{
    return (this->section == other.section)
            && (this->row == other.row);
}

bool CAIndexPath2E::operator< (const CAIndexPath2E& other) const
{
    if (this->section != other.section)
        return (this->section < other.section);
    
    return (this->row < other.row);
}

bool CAIndexPath2E::operator> (const CAIndexPath2E& other) const
{
    if (this->section != other.section)
        return (this->section > other.section);
    
    return (this->row > other.row);
}



#pragma CAIndexPath3E

CAIndexPath3E::CAIndexPath3E()
:section(UINT_NONE)
,row(UINT_NONE)
,item(UINT_NONE)
{

}

CAIndexPath3E::CAIndexPath3E(unsigned int section, unsigned int row, unsigned int item)
:section(section)
,row(row)
,item(item)
{
    
}


bool CAIndexPath3E::operator!= (const CAIndexPath3E& other) const
{
    return (this->section != other.section)
            || (this->row != other.row)
            || (this->item != other.item);
}

bool CAIndexPath3E::operator== (const CAIndexPath3E& other) const
{
    return (this->section == other.section)
            && (this->row == other.row)
            && (this->item == other.item);
}

bool CAIndexPath3E::operator< (const CAIndexPath3E& other) const
{
    if (this->section != other.section)
        return (this->section < other.section);
    
    if (this->row != other.row)
        return (this->row < other.row);
    
    return (this->item < other.item);
}

bool CAIndexPath3E::operator> (const CAIndexPath3E& other) const
{
    if (this->section != other.section)
        return (this->section > other.section);
    
    if (this->row != other.row)
        return (this->row > other.row);
    
    return (this->item > other.item);
}







NS_CC_END