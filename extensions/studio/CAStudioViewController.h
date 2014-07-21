//
//  CAStudioManager.h
//  Demo
//
//  Created by lh on 14-6-30.
//
//

#ifndef __Demo__CAStudioManager__
#define __Demo__CAStudioManager__

#include <iostream>
#include "CrossApp.h"
#include "Json/CSContentJsonDictionary.h"
#include "Json/DictionaryHelper.h"
USING_NS_CC;
enum ITEMSTYPE{
CA_TYPE_BUTTON=1,
CA_TYPE_LABEL,
CA_TYPE_IMAGE,
CA_TYPE_VIEW,
CA_TYPE_SCROLLVIEW,
CA_TYPE_TABLEVIEW,
CA_TYPE_PROGRESS,
CA_TYPE_TEXTFIELD,
CA_TYPE_SWITCH,
};
class BaseAnalyze:public CAObject
{
public:
    std::string name;
    float width;
    float height;
    CCArray *controlsArr;
    BaseAnalyze()
    {
        name = "";
        width = 0.0f;
        height = 0.0f;
        controlsArr = NULL;
    }
};
class CATableViewController :public CATableViewDataSource
{
public:
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section)
	{
		return 5;
	}

	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
	{
		CATableViewCell *cell = CATableViewCell::create("");
		return cell;
	}

	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
	{
		return 100;
	}
};
class CAStudioViewController :public CAViewController, public CATableViewController
{
public:
	CAStudioViewController();
    
	~CAStudioViewController();
    
public:
	virtual void viewDidLoad();
	virtual void viewDidUnload();
	virtual bool init();
	virtual bool initWithJson(CSJson::Value var);
	void sendStudioJson(CSJson::Value var);
	CAView* getViewByName(std::string viewName);
	CAView* getViewByTag(int tag);
	virtual void reshapeViewRectDidFinish();
    CC_PROPERTY(CATableViewDataSource*, m_pDataSourceTarget, DataSourceTarget);
    CC_PROPERTY(CATableViewDelegate*, m_pDelegateTarget, DelegateTarget);
	CC_SYNTHESIZE(bool, m_bNavFlag, NavFlag);
	CC_SYNTHESIZE(bool, m_bTabFlag, TabFlag);
	std::string m_sJsonName;
private:
    void getItemType(CSJson::Value sender,std::string itemName);
	void setPropertiesForBaseObject(CAObject *sender, CSJson::Value dic);
	void setPropertiesForButton(CAObject *sender, CSJson::Value dic);
	void setPropertiesForLabel(CAObject *sender, CSJson::Value dic);
	void setPropertiesForView(CAObject *sender, CSJson::Value dic);
	void setPropertiesForScrollView(CAObject *sender, CSJson::Value dic);
	void setPropertiesForTableView(CAObject *sender, CSJson::Value dic);
	void setPropertiesForSwitch(CAObject *sender, CSJson::Value dic);
	void setPropertiesForProgress(CAObject *sender, CSJson::Value dic);
	void setPropertiesForTextField(CAObject *sender, CSJson::Value dic);
	void setPropertiesForImageView(CAObject *sender, CSJson::Value dic);

	CCDictionary *m_pViewDicByName;
	CCDictionary *m_pViewDicByTag;
	CSJson::Value::Members names;
	CSJson::Value controls;
	
};
#endif /* defined(__Demo__CAStudioManager__) */
