//
//  CAIndexPath.cpp
//  CrossApp
//
//  Created by MAC on 14-7-27.
//  Copyright (c) 2014. All rights reserved.
//

#include "CAIndexPath.h"

NS_CC_BEGIN

const unsigned int _max_unsigned_int = 0xffffffff;

CAIndexPath2E::CAIndexPath2E()
:section(_max_unsigned_int)
,row(_max_unsigned_int)
{

}

CAIndexPath2E::CAIndexPath2E(unsigned int section, unsigned int row)
:section(section)
,row(row)
{
    
}

CAIndexPath2E::CAIndexPath2E(const CAIndexPath2E& other)
:section(other.section)
,row(other.row)
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



CAIndexPath3E::CAIndexPath3E()
:section(_max_unsigned_int)
,row(_max_unsigned_int)
,item(_max_unsigned_int)
{

}

CAIndexPath3E::CAIndexPath3E(unsigned int section, unsigned int row, unsigned int item)
:section(section)
,row(row)
,item(item)
{
    
}

CAIndexPath3E::CAIndexPath3E(const CAIndexPath3E& other)
:section(other.section)
,row(other.row)
,item(other.item)
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