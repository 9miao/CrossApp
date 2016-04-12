
#ifndef __Test__WaterfallViewTest__
#define __Test__WaterfallViewTest__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class WaterfallViewTest: public CAViewController, CAWaterfallViewDelegate, CAWaterfallViewDataSource, CAScrollViewDelegate
{
    
public:

	WaterfallViewTest();
    
	virtual ~WaterfallViewTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
public:
    
    CAWaterfallView* Waterfall;
    
    std::vector<CAColor4B> colorArr;
    
    std::vector<std::string> m_vTitle;
    
    CAWaterfallViewCell* p_Cell;
    
    CAPullToRefreshView* headerRefreshView;
    CAPullToRefreshView* footerRefreshView;
    
public:
    
    void refreshData1(float interval);
    
    void refreshData2(float interval);
    
    void scrollViewHeaderBeginRefreshing(CAScrollView* view);
    
    void scrollViewFooterBeginRefreshing(CAScrollView* view);
    
    virtual void waterfallViewDidSelectCellAtIndexPath(CAWaterfallView *waterfallView, unsigned int itemIndex);
    
    virtual void waterfallViewDidDeselectCellAtIndexPath(CAWaterfallView *waterfallView, unsigned int itemIndex);
    
    //Necessary
    virtual CAWaterfallViewCell* waterfallCellAtIndex(CAWaterfallView *waterfallView, const DSize& cellSize, unsigned int itemIndex);
    
    //Necessary
    virtual unsigned int waterfallViewHeightForItemAtIndex(CAWaterfallView *waterfallView, unsigned int itemIndex);
        
    //Necessary
    virtual unsigned int numberOfItems(CAWaterfallView *waterfallView);
    
    virtual CAView* waterfallViewSectionViewForHeader(CAWaterfallView *waterfallView, const DSize& viewSize);
    
    virtual unsigned int waterfallViewHeightForHeader(CAWaterfallView *waterfallView);
    
    virtual CAView* waterfallViewSectionViewForFooter(CAWaterfallView *waterfallView, const DSize& viewSize);
    
    virtual unsigned int waterfallViewHeightForFooter(CAWaterfallView *waterfallView);
    
    virtual void waterfallViewWillDisplayCellAtIndex(CAWaterfallView* waterfallView, CAWaterfallViewCell* cell, unsigned int itemIndex);
    
};


#endif /* defined(__HelloCpp__ViewController__) */


/*
 
 #include "WaterfallViewTest.h"
 
 //static const char* iconTag[29] =
 //{
 //    "image/AlertView.png",
 //    "image/button.png",
 //    "image/SegmentedControl.png",
 //
 //    "image/ImageView.png",
 //    "image/Scale9ImageView.png",
 //    "image/indicatorView.png",
 //
 //    "image/Progress.png",
 //    "image/slider.png",
 //    "image/Switch.png",
 //
 //    "image/Stepper.png",
 //    "image/Label.png",
 //    "image/TextField.png",
 //
 //    "image/TextView.png",
 //    "image/TabBar.png",
 //    "image/PageView.png",
 //
 //    "image/ScrollView.png",
 //    "image/CollectionView.png",
 //    "image/CollectionView.png",
 //
 //    "image/CollectionView.png",
 //    "image/ListView.png",
 //    "image/TableView.png",
 //
 //    "image/PickerView.png",
 //    "image/WebView.png",
 //    "image/GifView.png",
 //
 //    "image/FlashView.png",
 //    "image/Video.png",
 //    "image/RenderImage.png",
 //
 //    "image/Video.png",
 //    "image/CollectionView.png"
 //
 //
 //};
 
 WaterfallViewTest::WaterfallViewTest()
 {
 m_vTitle.push_back("AlertView");
 m_vTitle.push_back("Button");
 m_vTitle.push_back("SegmentedControl");
 
 m_vTitle.push_back("ImageView");
 m_vTitle.push_back("Scale9ImageView");
 m_vTitle.push_back("IndicatorView");
 
 m_vTitle.push_back("Progress");
 m_vTitle.push_back("Slider");
 m_vTitle.push_back("Switch");
 
 m_vTitle.push_back("Stepper");
 m_vTitle.push_back("Label");
 m_vTitle.push_back("TextField");
 
 m_vTitle.push_back("TextView");
 m_vTitle.push_back("TabBar");
 m_vTitle.push_back("PageView");
 
 m_vTitle.push_back("ScrollView");
 m_vTitle.push_back("CollectionView");
 m_vTitle.push_back("AutoCollectionView");
 
 m_vTitle.push_back("WaterfallView");
 m_vTitle.push_back("ListView");
 m_vTitle.push_back("TableView");
 
 m_vTitle.push_back("PickerView");
 m_vTitle.push_back("WebView");
 m_vTitle.push_back("GifView");
 
 m_vTitle.push_back("FlashView");
 m_vTitle.push_back("VideoPlayer");
 m_vTitle.push_back("RenderImage");
 
 m_vTitle.push_back("Animation");
 }
 
 WaterfallViewTest::~WaterfallViewTest()
 {
 
 }
 
 void WaterfallViewTest::viewDidLoad()
 {
 for (int i = 0; i < 40; i++)
 {
 char r = CCRANDOM_0_1() * 255;
 char g = CCRANDOM_0_1() * 255;
 char b = CCRANDOM_0_1() * 255;
 colorArr.push_back(ccc4(r, g, b, 255));
 }
 
 Waterfall = CAWaterfallView::createWithLayout(DRectLayout(0,0,0,0,DRectLayout::L_R_T_B));
 Waterfall->setItemMargin(10);
 Waterfall->setColumnMargin(10);
 Waterfall->setAllowsSelection(true);
 Waterfall->setWaterfallViewDelegate(this);
 Waterfall->setWaterfallViewDataSource(this);
 this->getView()->addSubview(Waterfall);
 }
 
 void WaterfallViewTest::viewDidUnload()
 {
 // Release any retained subviews of the main view.
 // e.g. self.myOutlet = nil;
 }
 
 void WaterfallViewTest::waterfallViewDidSelectCellAtIndexPath(CAWaterfallView *waterfallView, unsigned int itemIndex)
 {
 CCLog("WaterfallViewTest%d", itemIndex);
 }
 
 void WaterfallViewTest::waterfallViewDidDeselectCellAtIndexPath(CAWaterfallView *waterfallView, unsigned int itemIndex)
 {
 CCLog("CellAtIndexPath%d", itemIndex);
 }
 
 //Necessary
 CAWaterfallViewCell* WaterfallViewTest::waterfallCellAtIndex(CAWaterfallView *waterfallView, const DSize& cellSize, unsigned int itemIndex)
 {
 p_Cell = Waterfall->dequeueReusableCellWithIdentifier("CrossApp");
 if (p_Cell == NULL)
 {
 Height = CCRANDOM_0_1() * 50 + 100;
 CCLog("p_Cell.height = %d", Height);
 p_Cell = CAWaterfallViewCell::create("CrossApp");
 p_Cell->setAllowsSelected(false);
 p_Cell->setBackgroundView(CAView::create());
 
 //        CAView* itemImage = CAView::createWithLayout(DRectLayout(0, 0, 0, Height,DRectLayout::L_R_T_H));
 //        itemImage->setTag(99);
 //        p_Cell->addSubview(itemImage);
 
 CALabel* itemText = CALabel::createWithLayout(DRectLayout(0, 0, 0, 0,DRectLayout::L_R_T_B));
 itemText->setTag(100);
 itemText->setFontSize(24);
 itemText->setTextAlignment(CATextAlignmentCenter);
 itemText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
 p_Cell->addSubview(itemText);
 }
 
 //    CAView* itemImageView = p_Cell->getSubviewByTag(99);
 //    itemImageView->setColor(colorArr.at(itemIndex));
 
 p_Cell->getBackgroundView()->setColor(colorArr.at(itemIndex));
 
 char pos[20] = "";
 sprintf(pos, "(%d)", itemIndex);
 
 CALabel* itemText = (CALabel*)p_Cell->getSubviewByTag(100);
 itemText->setText(pos);
 
 return  p_Cell;
 
 }
 //Necessary
 unsigned int WaterfallViewTest::waterfallViewHeightForItemAtIndex(CAWaterfallView *waterfallView, unsigned int itemIndex)
 {
 return Height;
 }
 //Necessary
 unsigned int WaterfallViewTest::numberOfItems(CAWaterfallView *waterfallView)
 {
 return 12;
 }
 
 CAView* WaterfallViewTest::waterfallViewSectionViewForHeader(CAWaterfallView *waterfallView, const DSize& viewSize)
 {
 CAImageView* image = CAImageView::createWithImage(CAImage::create("image/WebView.png"));
 CAView* view = CAView::createWithFrame(DRect(0,0,100,100));
 view->addSubview(image);
 //    view->setColor(CAColor_red);
 return view;
 }
 
 unsigned int WaterfallViewTest::waterfallViewHeightForHeader(CAWaterfallView *waterfallView)
 {
 return 50;
 }
 
 CAView* WaterfallViewTest::waterfallViewSectionViewForFooter(CAWaterfallView *waterfallView, const DSize& viewSize)
 {
 CAImageView* image = CAImageView::createWithImage(CAImage::create("image/WebView.png"));
 CAView* view = CAView::createWithFrame(DRect(0,0,100,100));
 view->addSubview(image);
 //    view->setColor(CAColor_red);
 return view;
 }
 
 unsigned int WaterfallViewTest::waterfallViewHeightForFooter(CAWaterfallView *waterfallView)
 {
 return 100;
 }
 
 void WaterfallViewTest::waterfallViewWillDisplayCellAtIndex(CAWaterfallView* waterfallView, CAWaterfallViewCell* cell, unsigned int itemIndex)
 {
 
 }
*/
