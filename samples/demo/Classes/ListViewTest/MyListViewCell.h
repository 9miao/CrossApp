#ifndef _My_ListViewCell_h_
#define _My_ListViewCell_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class MyListViewCell :public CAListViewCell
{
public:
	MyListViewCell();
	virtual ~MyListViewCell();

	static MyListViewCell* create(const std::string& identifier,const CADipRect& _rect=CADipRectZero);

public:
	void initWithCell();
	void cellBtnCallback(CAControl* btn, CCPoint point);

protected:
	virtual void normalListViewCell();
	virtual void highlightedListViewCell();
	virtual void selectedListViewCell();
	virtual void disabledListViewCell();

private:

};

#endif