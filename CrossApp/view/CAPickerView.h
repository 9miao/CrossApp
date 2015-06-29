//
//  CAPickerView.h
//  CrossApp
//
//  Created by dai xinping on 14-7-22.
//  Copyright (c) 2014年 www.9miao.com. All rights reserved.
//

#ifndef __CrossApp__CAPickerView__
#define __CrossApp__CAPickerView__

#include "CATableView.h"

NS_CC_BEGIN

class CAPickerView;

class CC_DLL CAPickerViewDelegate {

public:
    virtual void didSelectRow(CAPickerView* pickerView, unsigned int row, unsigned int component) {}
};

class CC_DLL CAPickerViewDataSource {
    
public:
    virtual ~CAPickerViewDataSource() {};
    
    virtual unsigned int numberOfComponentsInPickerView(CAPickerView* pickerView) = 0; 

    virtual unsigned int numberOfRowsInComponent(CAPickerView* pickerView, unsigned int component) = 0;

    virtual float widthForComponent(CAPickerView* pickerView, unsigned int component) {return 0;}
    
    virtual float rowHeightForComponent(CAPickerView* pickerView, unsigned int component) {return 0;}

    virtual CCString* titleForRow(CAPickerView* pickerView, unsigned int row, unsigned int component) {return NULL;}

    virtual CAView* viewForRow(CAPickerView* pickerView, unsigned int row, unsigned int component) {return NULL;}
    
    virtual CAView* viewForSelect(CAPickerView* pickerView, unsigned int component, const CCSize& size) {return NULL;}
};

class CC_DLL CAPickerView : public CAView, public CATableViewDataSource , public CATableViewDelegate
{
public:
    static CAPickerView* create();
    static CAPickerView* createWithFrame(const CCRect& rect);
    static CAPickerView* createWithCenter(const CCRect& rect);
    
    CAPickerView();
    virtual ~CAPickerView();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
	
	virtual void onEnterTransitionDidFinish();

	virtual void onExitTransitionDidStart();

    virtual void visit();
    
    virtual bool initWithFrame(const CCRect& rect);
    virtual bool initWithCenter(const CCRect& rect);
    
    // info that was fetched and cached from the data source and delegate.
    // -1 if does not implement CAPickerViewDataSource
    virtual int numberOfComponents();
    virtual int numberOfRowsInComponent(unsigned int component);
    virtual CCSize rowSizeForComponent(unsigned int component);
    
    // returns the view provided by the delegate via viewForRow
    // or NULL if the row/component is not visible or the delegate does not implement viewForRow
    virtual CAView* viewForRow(unsigned int row, unsigned int component);
    
    // Reloading whole view or single component
    virtual void reloadAllComponents();
    virtual void reloadComponent(unsigned int row,unsigned int component, bool bReload = true);
    
    // selection. in this case, it means showing the appropriate row in the middle
    // animated: scrolls the specified row to center. default is false
    virtual void selectRow(unsigned int row, unsigned int component, bool animated = false);
    
    // returns selected row. -1 if nothing selected
    virtual int selectedRowInComponent(unsigned int component);

	virtual void setBackgroundColor(const CAColor4B& color);

	CC_SYNTHESIZE(CAPickerViewDelegate*, m_delegate, PickerViewDelegate);
	CC_SYNTHESIZE(CAPickerViewDataSource*, m_dataSource, PickerViewDataSource);
    
    CC_SYNTHESIZE(float, m_fontSizeNormal, FontSizeNormal);
    CC_SYNTHESIZE(float, m_fontSizeSelected, FontSizeSelected);
    CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_fontColorNormal, FontColorNormal);
    CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_fontColorSelected, FontColorSelected);
    CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_separateColor, SeparateColor);

private:
    float calcTotalWidth(unsigned int component);
    CAView* viewForRowInComponent(int component, int row, CCSize size);
    
protected:
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
private:

	CAVector<CATableView*> m_tableViews;
    std::vector<int> m_selected;
    std::vector< std::vector<int> > m_componentsIndex;
    std::vector<float> m_componentOffsetX;
    std::vector<int> m_displayRow;
};

NS_CC_END

#endif /* defined(__CrossApp__CAPickerView__) */
