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

	static MyTableViewCell* create(const std::string& identifier);

public:
	void initCell();
	void cellBtnCallback(CAControl* btn, CCPoint point);

private:

};

#endif