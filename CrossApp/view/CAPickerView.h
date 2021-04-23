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

    virtual const char* titleForRow(CAPickerView* pickerView, unsigned int row, unsigned int component) {return NULL;}

    virtual CAView* viewForRow(CAPickerView* pickerView, unsigned int row, unsigned int component) {return NULL;}

    virtual CAView* viewForSelect(CAPickerView* pickerView, unsigned int component, const DSize& size) {return NULL;}
};

class CC_DLL CAPickerView : public CAView, public CATableViewDataSource , public CATableViewDelegate
{
public:
    // event listeners. If these functions are set, the corresponding function of CAPickerViewDataSource will fail.
    CC_LISTENING_FUNCTION(unsigned int(), NumberOfComponents);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int component), NumberOfRowsInComponent);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int component), WidthForComponent);
    
    CC_LISTENING_FUNCTION(unsigned int(unsigned int component), HeightForComponent);
    
    CC_LISTENING_FUNCTION(std::string(unsigned int row, unsigned int component), TitleForRow);
    
    CC_LISTENING_FUNCTION(CAView*(unsigned int row, unsigned int component), ViewForRow);
    
    CC_LISTENING_FUNCTION(CAView*(unsigned int component, DSize size), ViewForSelected);
    
    // event listeners. If these functions are set, the corresponding function of CAPickerViewDelegate will fail.
    CC_LISTENING_FUNCTION(void(unsigned int row, unsigned int component), DidSelectRow);
    
public:
    
    CAPickerView();
    virtual ~CAPickerView();
    
    static CAPickerView* create();
    static CAPickerView* createWithFrame(const DRect& rect);
    static CAPickerView* createWithCenter(const DRect& rect);
    static CAPickerView* createWithLayout(const DLayout& layout);
    
    virtual bool init();

	virtual void onEnterTransitionDidFinish();

	virtual void onExitTransitionDidStart();

    virtual void visitEve();
    

    // Reloading whole view or single component
    virtual void reloadAllComponents();
    virtual void reloadComponent(unsigned int row,unsigned int component, bool bReload = true);
    
    // selection. in this case, it means showing the appropriate row in the middle
    // animated: scrolls the specified row to center. default is false
    virtual void selectRow(unsigned int row, unsigned int component, bool animated = false);
    
    // returns selected row. -1 if nothing selected
    virtual int selectedRowInComponent(unsigned int component);

	void setBackgroundImage(CAImage* image, bool isScale9 = false);

	CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CAPickerViewDelegate*, m_delegate, PickerViewDelegate);
	CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(CAPickerViewDataSource*, m_dataSource, PickerViewDataSource);
    
    CC_SYNTHESIZE(float, m_fontSizeNormal, FontSizeNormal);
    CC_SYNTHESIZE(float, m_fontSizeSelected, FontSizeSelected);
    CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_fontColorNormal, FontColorNormal);
    CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_fontColorSelected, FontColorSelected);
    CC_SYNTHESIZE_PASS_BY_REF(CAColor4B, m_separateColor, SeparateColor);

private:
    float calcTotalWidth(unsigned int component);
    CAView* viewForRowInComponent(int component, int row, DSize size);
    
protected:
    virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
    virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
    virtual void setContentSize(const DSize& size);
private:

	CAVector<CATableView*> m_tableViews;
    std::vector<int> m_selected;
    std::vector< std::vector<int> > m_componentsIndex;
    std::vector<float> m_componentOffsetX;
    std::vector<int> m_displayRow;
    CAView* m_pBackgroundView;
};

NS_CC_END

#endif /* defined(__CrossApp__CAPickerView__) */
