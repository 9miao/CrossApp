
#ifndef __Test__WaterfallViewTest__
#define __Test__WaterfallViewTest__

#include "RootWindow.h"

class WaterfallViewTest: public CAViewController
{
    
public:

	WaterfallViewTest();
    
	virtual ~WaterfallViewTest();
    
    CREATE_FUNC(WaterfallViewTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    void scrollViewHeaderBeginRefreshing();
    
    void scrollViewFooterBeginRefreshing();
    
    void waterfallViewDidSelectCellAtIndexPath(unsigned int itemIndex);
    
    void waterfallViewDidDeselectCellAtIndexPath(unsigned int itemIndex);
    
    //Necessary
    CAWaterfallViewCell* waterfallCellAtIndex(const DSize& cellSize, unsigned int itemIndex);
    
    //Necessary
    unsigned int waterfallViewHeightForItemAtIndex(unsigned int itemIndex);
        
    //Necessary
    unsigned int numberOfItems();
    
private:
    
    CAWaterfallView* m_pWaterfallView;
    
    std::vector<CAColor4B> m_vColors;
    std::vector<unsigned int> m_vHeights;
};


#endif /* defined(__Test__WaterfallViewTest__) */
