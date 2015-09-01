//
//  MainScenes.hpp
//  CAUIEDITOR
//
//  Created by renhongguang on 15/8/13.
//
//

#ifndef MainScenes_cpp
#define MainScenes_cpp

#include "CrossApp.h"
#include "AutoView.h"
#include "CanvasView.h"
#include "CanvasScenes.h"
#include "CAMenuView.h"
#include "CAListMenuView.h"
#include "PropertyView.h"

static const char* iconTag[25] =
{
    "image/button.png",
    "image/ImageView.png",
    "image/Scale9ImageView.png",
    "image/indicatorView.png",
    "image/Label.png",
    "image/Progress.png",
    "image/slider.png",
    "image/Switch.png",
    "image/TextField.png",
    "image/TextView.png",
    "image/SegmentedControl.png",
    "image/TabBar.png",
    "image/PageView.png",
    "image/TableView.png",
    "image/ListView.png",
    "image/CollectionView.png",
    "image/ScrollView.png",
    "image/WebView.png",
    "image/FlashView.png",
    "image/GifView.png",
    "image/PickerView.png",
    "image/Stepper.png",
    "image/RenderImage.png",
    "image/Video.png",
    "image/Video.png"
};

class MainScenes: public CAViewController, CAAutoCollectionViewDelegate, CAAutoCollectionViewDataSource,AutoViewDelegate,CAMenuViewDelegate
{
    
public:
    
    MainScenes();
    
    virtual ~MainScenes();
    
protected:
    
    void viewDidLoad();
    void viewDidUnload();
    
    void initMenuBar();
    virtual void collectionViewDidSelectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual void collectionViewDidDeselectCellAtIndexPath(CAAutoCollectionView *collectionView, unsigned int section, unsigned int item);
    
    virtual CACollectionViewCell* collectionCellAtIndex(CAAutoCollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int item);
    
    virtual CCSize collectionViewSizeForItemAtIndexPath(CAAutoCollectionView* collectionView, unsigned int section, unsigned int item);
    virtual unsigned int numberOfItemsInSection(CAAutoCollectionView *collectionView, unsigned int section);
    virtual unsigned int numberOfSections(CAAutoCollectionView *collectionView);

    void addControl(CAControl* control, CCPoint point);
    void movedControl(CAControl* control, CCPoint point);
    void putDownControl(CAControl* control, CCPoint point);
    void selectedControl(CAControl* control, CCPoint point);
    void cancelControl(CAControl* control, CCPoint point);
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void AutoViewChangeFrameStart();
    virtual void AutoViewChangeFrameEnd();
    virtual void AutoViewMoveToPosition(CCPoint& p);
    
    virtual void selectedMenuViewItem(CAMenuView* menuView, int tag, std::string titleName);
public:
    CADipSize                   winsize;
    std::vector<std::string>    m_vTitle;
    CAScrollView*               p_ScrollView;
    CanvasScenes*               scence_view;
    CanvasView*                 p_view;
    CAView*                     p_canvas;
    CAView*                     p_control;
    PropertyView*               p_property;
    AutoView*                   p_tempView;
    CAAutoCollectionView*       p_Conllection;
    CAMenuView*                 p_MenuView;
    CAListMenuView*             p_listMenuView;
};
#endif /* MainScenes_cpp */
