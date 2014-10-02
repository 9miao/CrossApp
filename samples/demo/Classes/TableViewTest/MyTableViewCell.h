#ifndef _My_TableViewCell_h_
#define _My_TableViewCell_h_

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class MyTableViewCell:public CATableViewCell
{
public:
	MyTableViewCell();
	virtual ~MyTableViewCell();

	static MyTableViewCell* create(const std::string& identifier, const CADipRect& _rect = CADipRectZero);

public:
	void initWithCell();
	void cellBtnCallback(CAControl* btn, CCPoint point);

protected:
	virtual void normalTableViewCell();
	virtual void highlightedTableViewCell();
	virtual void selectedTableViewCell();
	virtual void disabledTableViewCell();

private:

};

#endif