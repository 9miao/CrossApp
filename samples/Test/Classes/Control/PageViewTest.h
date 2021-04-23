
#ifndef __Test__PageViewTest__
#define __Test__PageViewTest__

#include "RootWindow.h"

class PageViewTest: public CAViewController
{
    
public:

	PageViewTest();
    
	virtual ~PageViewTest();
    
    CREATE_FUNC(PageViewTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void pageViewDidBeginTurning();
    
    void pageViewDidEndTurning();
    
    void pageViewDidSelectPageAtIndex(unsigned int index, const DPoint& point);
    
private:
    
    CAPageView* m_pPageView;
    
    CAPageControl* m_pPageControl;

};


#endif /* defined(__HelloCpp__ViewController__) */
