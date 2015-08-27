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

class MainScenes: public CAViewController, CACollectionViewDelegate, CACollectionViewDataSource,AutoViewDelegate
{
    
public:
    
    MainScenes();
    
    virtual ~MainScenes();
    
protected:
    
    void viewDidLoad();
    void viewDidUnload();
    
    virtual void collectionViewDidSelectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    virtual void collectionViewDidDeselectCellAtIndexPath(CACollectionView *collectionView, unsigned int section, unsigned int row, unsigned int item);
    virtual CACollectionViewCell* collectionCellAtIndex(CACollectionView *collectionView, const CCSize& cellSize, unsigned int section, unsigned int row, unsigned int item);
    virtual unsigned int numberOfSections(CACollectionView *collectionView);
    virtual unsigned int numberOfRowsInSection(CACollectionView *collectionView, unsigned int section);
    virtual unsigned int numberOfItemsInRowsInSection(CACollectionView *collectionView, unsigned int section, unsigned int row);
    virtual unsigned int collectionViewHeightForRowAtIndexPath(CACollectionView* collectionView, unsigned int section, unsigned int row);

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
public:
    CADipSize winsize;
    std::vector<std::string> m_vTitle;
    CAScrollView* p_ScrollView;
    CanvasScenes* scence_view;
    CanvasView* p_view;
    CAView* p_canvas;
    AutoView* p_tempView;
    CACollectionView* p_Conllection;
};
#endif /* MainScenes_cpp */
