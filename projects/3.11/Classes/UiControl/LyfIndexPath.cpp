//  出自九秒社区 http://www.9miao.com
//
//  LyfIndexPath.cpp
//  DDHero
//
//  Created by LI YUANFENG on 13-3-25.
//
//

#include "LyfIndexPath.h"

LyfIndexPath::~LyfIndexPath(){
    row=0;
    page=0;
    maxRowOfPage=0;
}

LyfIndexPath::LyfIndexPath(int row, int page, int maxRowOfPage)
:maxRowOfPage(maxRowOfPage)
,row(row)
,page(page)
{
    
}

LyfIndexPath::LyfIndexPath(int index, int maxRowOfPage)
:maxRowOfPage(maxRowOfPage)
,row(index%maxRowOfPage)
,page(index/maxRowOfPage)
{
    
}

LyfIndexPath::LyfIndexPath(const LyfIndexPath& otherIndexPath)
:maxRowOfPage(otherIndexPath.maxRowOfPage)
,row(otherIndexPath.row)
,page(otherIndexPath.page)
{

}

LyfIndexPath& LyfIndexPath::operator= (const LyfIndexPath& other)
{
    maxRowOfPage=other.maxRowOfPage;
    row=other.row;
    page=other.page;
    return *this;
}


int LyfIndexPath::getIndex()
{
    return (row+page*maxRowOfPage);
}

bool LyfIndexPath::isEqual(LyfIndexPath otherPath)
{
    return (row==otherPath.row && page==otherPath.page && maxRowOfPage==otherPath.maxRowOfPage);
}