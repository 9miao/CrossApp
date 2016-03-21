
#include "RenderImageTest.h"


RenderImageTest::RenderImageTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(false);
}

RenderImageTest::~RenderImageTest()
{
    CADrawerController* drawer = (CADrawerController*)CAApplication::getApplication()->getRootWindow()->getRootViewController();
    drawer->setTouchMoved(true);
}

void RenderImageTest::viewDidLoad()
{
    winSize = this->getView()->getBounds().size;
    winRect = this->getView()->getBounds();
    this->getView()->setColor(CAColor_gray);
    
    dle_ren_index = 0;
    
    CAButton* btn = CAButton::create(CAButtonTypeRoundedRect);
    btn->setLayout(DRectLayout(300, 300, 600, 600, DRectLayout::L_R_T_B));
    btn->setTitleForState(CAControlStateNormal, "Click");
    btn->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    btn->setTag(1);
    btn->addTarget(this, CAControl_selector(RenderImageTest::renderCallBack), CAControlEventTouchUpInSide);
    this->getView()->addSubview(btn);
}

void RenderImageTest::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void RenderImageTest::renderCallBack(CAControl* control, DPoint point)
{
    CAButton* button = (CAButton*)control;
    if (button->getTag()==1)
    {
        CADevice::openAlbum(this);
    }
    else if(button->getTag()==2)
    {
        m_clvImage->setClippingEnabled(true);
        CARenderImage* rm = CARenderImage::create(winSize.width-100, winSize.width-100);//123456
        rm->printscreenWithView(m_clvImage);
        
//        renderImage = CAView::createWithFrame(DRect(50,winSize.height/4,winSize.width-100,winSize.width-100));
        renderImage = CAView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
        this->getView()->addSubview(renderImage);
        
        m_clvImage->setClippingEnabled(false);
        
        if (m_clv!=NULL)
        {
            this->getView()->removeSubview(m_clv);
            m_clv = NULL;
            this->getView()->removeSubview(m_clvImage);
            m_clvImage = NULL;
            this->getView()->removeSubview(render_btn);
            render_btn = NULL;
        }
        
        CAImageView* imageView = CAImageView::createWithFrame(DRect(0,0,winSize.width-100,winSize.width-100));
        imageView->setImage(rm->getImageView()->getImage());
        renderImage->addSubview(imageView);
        
        CAScheduler::schedule(schedule_selector(RenderImageTest::scheduleFuck), this, 3);
    }
    
}

CADrawView* getStencil(const DSize& size, int index)
{
    if (index == 0)
    {
        DPoint ver[4];
        ver[0] = DPoint(0, 0);
        ver[1] = DPoint(0, size.height);
        ver[2] = DPoint(size.width, size.height);
        ver[3] = DPoint(size.width, 0);
        CADrawView* stencil = CADrawView::create();
        stencil->drawPolygon(ver, 4, ccc4f(255, 0, 0, 0), 2, ccc4f(255, 0, 0, 0));
        stencil->setFrameOrigin(DPoint(0, size.height));
        return stencil;
    }
    else if (index == 1)
    {
        DPoint cir[720];
        for (int i=0; i<720; i++)
        {
            float x = cosf(i * M_PI/180.f) * size.width/2;
            float y = sinf(i * M_PI/180.f) * size.width/2;
            cir[i] = DPoint(x, y);
        }
        CADrawView* stencil = CADrawView::create();
        stencil->drawPolygon(cir, 720, ccc4f(1, 1, 1, 0.5), 0, ccc4f(1, 1, 1, 0));
        stencil->setCenterOrigin(DPoint(size.width/2, size.height/2));
        return stencil;
    }
    return NULL;
}

void RenderImageTest::getSelectedImage(CAImage *image)
{
    int index = 0;
    DRect scrollRect;
    scrollRect.origin.x = 50;
    scrollRect.origin.y = winSize.height/4;
    scrollRect.size.width = winSize.width-100;
    scrollRect.size.height = scrollRect.size.width;
    
    DRectLayout scrollLayout;
    scrollLayout.left = 50;
    scrollLayout.top = winSize.height/4;
    scrollLayout.width = winSize.width - 100;
    scrollLayout.height = scrollLayout.width;
    
    m_clvImage = CAClippingView::create();
    m_clvImage->setStencil(getStencil(scrollRect.size, index));
    m_clvImage->setFrame(scrollRect);
//    m_clvImage->setLayout(scrollLayout);
    m_clvImage->setInverted(false);
    m_clvImage->setClippingEnabled(false);
    this->getView()->addSubview(m_clvImage);
    
    float temp_mini = 0;
    if (image->getContentSize().width>image->getContentSize().height)
    {
        temp_mini = scrollRect.size.height/image->getContentSize().height;
    }
    else
    {
        temp_mini = scrollRect.size.width/image->getContentSize().width;
    }
    
//    CAScrollView* scrollView = CAScrollView::createWithFrame(m_clvImage->getBounds());
    CAScrollView* scrollView = CAScrollView::createWithLayout(m_clvImage->getLayout());
    scrollView->setViewSize(DSize(image->getContentSize()));
    scrollView->setContentOffset(DPoint(0,winSize.height/4), false);
    scrollView->setMinimumZoomScale(temp_mini);
    scrollView->setMaximumZoomScale(2.5f);
    scrollView->setBackgroundColor(CAColor_clear);
    scrollView->setShowsScrollIndicators(false);
    scrollView->setBounces(false);
    scrollView->setScrollViewDelegate(this);
    scrollView->setDisplayRange(true);
    m_clvImage->addSubview(scrollView);
    
    DRect rect;
    rect.origin = DPointZero;
    rect.size = scrollView->getViewSize();
    DRectLayout Layout;
    Layout.width = scrollView->getViewSize().width;
    Layout.height = scrollView->getViewSize().height;
//    CAImageView* imv = CAImageView::createWithFrame(rect);
    CAImageView* imv = CAImageView::createWithLayout(Layout);
    imv->setImage(image);
    imv->setImageViewScaleType(CAImageViewScaleTypeFitImageInside);
    scrollView->addSubview(imv);
    
    m_clv = CAClippingView::create();
    m_clv->setStencil(getStencil(scrollRect.size, index));
//    m_clv->setFrame(scrollRect);
    m_clv->setLayout(scrollLayout);
    m_clv->setInverted(true);
    m_clv->setTouchEnabled(false);
    this->getView()->addSubview(m_clv);
    
    DRect ivRect;
    ivRect.size = winSize;
    ivRect.origin = ccpMult(scrollRect.origin, -1);
    CAView* iv = CAView::createWithColor(ccc4(0,0,0,128));
    iv->setFrame(ivRect);
    m_clv->addSubview(iv);
    
    render_btn = CAButton::create(CAButtonTypeSquareRect);
    render_btn->setCenter(DRect(winSize.width/2, winSize.height-100, 100, 50));
    render_btn->setTitleForState(CAControlStateNormal, "Click");
    render_btn->setTitleColorForState(CAControlStateNormal, ccc4(51,204,255,255));
    render_btn->addTarget(this, CAControl_selector(RenderImageTest::renderCallBack), CAControlEventTouchUpInSide);
    render_btn->setTag(2);
    this->getView()->addSubview(render_btn);
}

void RenderImageTest::scheduleFuck(float dt)
{
    if (dle_ren_index>=1) {
        CAScheduler::unschedule(schedule_selector(RenderImageTest::scheduleFuck), this);
        if (renderImage!=NULL)
        {
            this->getView()->removeSubview(renderImage);
            renderImage = NULL;
        }
        dle_ren_index = 0;
    }else{
        dle_ren_index++;
    }
}
