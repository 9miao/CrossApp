
#ifndef __Test__RenderImageTest__
#define __Test__RenderImageTest__

#include "CrossApp.h"

USING_NS_CC;

class RenderImageTest: public CAViewController, public CAScrollViewDelegate, public CAMediaDelegate
{
    
public:

	RenderImageTest();
    
	virtual ~RenderImageTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    DSize winSize;
    DRect winRect;
    
    CAView* renderImage;
    CAClippingView* m_clvImage;
    CAClippingView* m_clv;
    CAButton* render_btn;
    int dle_ren_index;
    
public:
    
    void renderCallBack(CAControl* control, DPoint point);

    void scheduleFuck(float dt);
    
    virtual void getSelectedImage(CAImage *image);
};


#endif /* defined(__HelloCpp__ViewController__) */
