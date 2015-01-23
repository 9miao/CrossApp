//
//  CASegmentedControl.cpp
//  CrossApp
//
//  Created by qiaoxin on 15-1-20.
//  Copyright (c) 2015年 CrossApp. All rights reserved.
//

#include "CASegmentedControl.h"
#include "CAButton.h"
#include "view/CAScale9ImageView.h"
#include "dispatcher/CATouch.h"
#include "basics/CAApplication.h"
#include "view/CAClippingView.h"
#include "draw_nodes/CCDrawNode.h"
#include "view/CARenderImage.h"

using namespace std;
NS_CC_BEGIN

CASegmentedControl::CASegmentedControl(unsigned int itemsCount)
    : CAControl()
    , m_nSelectedIndex(-1)
    , m_nClickToForbidSelectedAtIndex(-1)
    , m_nItemsCount(itemsCount)
    , m_pBackgroundView(NULL)
    , m_sTitleFontName("")
    , m_pBackGroundImageView(NULL)
    , m_sSeparateImagePath("")
    , m_fSeparateWidth(1)
{
    m_vSeparateImage.clear();
}

CASegmentedControl::~CASegmentedControl()
{
    this->removeAllSegments();
}

void CASegmentedControl::onExitTransitionDidStart()
{
    CAControl::onExitTransitionDidStart();
}

void CASegmentedControl::onEnterTransitionDidFinish()
{
    CAControl::onEnterTransitionDidFinish();
}

CASegmentedControl* CASegmentedControl::createWithFrame(const CCRect& rect, unsigned int itemsCount)
{
    CASegmentedControl* segmentedControl = new CASegmentedControl(itemsCount);
    if (segmentedControl && segmentedControl->initWithFrame(rect))
    {
        segmentedControl->autorelease();
        return segmentedControl;
    }
    
    CC_SAFE_DELETE(segmentedControl);
    return NULL;
}

CASegmentedControl* CASegmentedControl::createWithCenter(const CCRect& rect, unsigned int itemsCount)
{
    CASegmentedControl* segmentedControl = new CASegmentedControl(itemsCount);
    if (segmentedControl && segmentedControl->initWithCenter(rect))
    {
        segmentedControl->autorelease();
        return segmentedControl;
    }
    
    CC_SAFE_DELETE(segmentedControl);
    return NULL;
}

bool CASegmentedControl::initWithFrame(const CCRect& rect)
{
    if (!CAControl::initWithFrame(rect))
    {
        return false;
    }
    this->setBackgroundImageView( CAImage::create("source_material/btn_rounded_normal.png") );
    m_pBackGroundImageView->setVisible(false);
    this->removeAllSegments();
    const float elemWidth = this->getBounds().size.width / m_nItemsCount;
    m_itemSize = CCSizeMake(elemWidth, this->getBounds().size.height);
    CCRect elemFrame = CCRectMake(0, 0, m_itemSize.width, m_itemSize.height);
    for (int i = 0; i < m_nItemsCount; ++i)
    {
        CAButton *btn = this->createDefaultSegment();
        if (btn)
        {
            btn->setFrame(elemFrame);
            m_segments.push_back(btn);
            this->insertSubview(btn, 1);
        }
        elemFrame.origin.x += elemWidth;
    }
    setSegmentImage(CAControlStateSelected,"source_material/btn_square_highlighted.png");
    setSegmentImage(CAControlStateHighlighted,"source_material/btn_square_highlighted.png");
    createSeparate();
    return true;
}

bool CASegmentedControl::initWithCenter(const CCRect& rect)
{
    if (!CAControl::initWithCenter(rect))
    {
        return false;
    }
    this->setBackgroundImageView( CAImage::create("source_material/btn_rounded_normal.png") );
    m_pBackGroundImageView->setVisible(false);
    this->removeAllSegments();
    const float elemWidth = this->getBounds().size.width / m_nItemsCount;
    m_itemSize = CCSizeMake(elemWidth, this->getBounds().size.height);
    CCRect elemFrame = CCRectMake(0, 0, m_itemSize.width, m_itemSize.height);
    for (int i = 0; i < m_nItemsCount; ++i)
    {
        CAButton *btn = this->createDefaultSegment();
        if (btn)
        {
            btn->setFrame(elemFrame);
            m_segments.push_back(btn);
            this->insertSubview(btn, 1);
        }
        elemFrame.origin.x += elemWidth;
    }
    setSegmentImage(CAControlStateSelected,"source_material/btn_square_highlighted.png");
    setSegmentImage(CAControlStateHighlighted,"source_material/btn_square_highlighted.png");
    createSeparate();
    return true;
}

bool CASegmentedControl::insertSegmentWithTitle(const char* title, int index, const CAControlState& controlState)
{
    const int curItemCount = m_segments.size();
    if (index < 0)
    {
        index = 0;
    }
    else if (index >= curItemCount)
    {
        index = curItemCount;
    }
    
    CAButton *newBtn = this->createDefaultSegment();
    if (NULL == newBtn)
    {
        return false;
    }
    
    newBtn->setTitleForState(controlState, title);
    newBtn->setTitleFontName(m_sTitleFontName);
    m_segments.insert(m_segments.begin() + index, newBtn);
    this->addSubview(newBtn);
    this->layoutSubviews();
    
    setSegmentImage(CAControlStateSelected,"source_material/btn_square_highlighted.png");
    setSegmentImage(CAControlStateHighlighted,"source_material/btn_square_highlighted.png");
    cleanAllSeparate();
    createSeparate();
    return true;
}

bool CASegmentedControl::insertSegmentWithBackgroundImage(CAImage *image, int index, const CAControlState& controlState)
{
    const int curItemCount = m_segments.size();
    if (index < 0)
    {
        index = 0;
    }
    else if (index >= curItemCount)
    {
        index = curItemCount;
    }
    
    CAButton *newBtn = this->createDefaultSegment();
    if (NULL == newBtn)
    {
        return false;
    }
    
    newBtn->setBackGroundViewForState(controlState, CAScale9ImageView::createWithImage(image));
    m_segments.insert(m_segments.begin() + index, newBtn);
    this->addSubview(newBtn);
    this->layoutSubviews();
    cleanAllSeparate();
    createSeparate();
    return true;
}

bool CASegmentedControl::insertSegmentWithImage(CAImage *image, int index, const CAControlState& controlState)
{
    const int curItemCount = m_segments.size();
    if (index < 0)
    {
        index = 0;
    }
    else if (index >= curItemCount)
    {
        index = curItemCount;
    }
    
    CAButton *newBtn = this->createDefaultSegment();
    if (NULL == newBtn)
    {
        return false;
    }
    
    newBtn->setImageForState(controlState, image);
    m_segments.insert(m_segments.begin() + index, newBtn);
    this->addSubview(newBtn);
    this->layoutSubviews();
    cleanAllSeparate();
    createSeparate();
    return true;
}

void CASegmentedControl::removeSegmentAtIndex(int index)
{
    if (!this->indexIsValid(index))
    {
        return ;
    }
    
    vector<CAButton *>::iterator vi = m_segments.begin() + index;
    (*vi)->removeFromSuperview();
    CC_SAFE_RELEASE(*vi);
    m_segments.erase(vi);
    this->layoutSubviews();
    setSegmentImage(CAControlStateSelected,"source_material/btn_square_highlighted.png");
    setSegmentImage(CAControlStateHighlighted,"source_material/btn_square_highlighted.png");
    cleanAllSeparate();
    createSeparate();
}

void CASegmentedControl::removeAllSegments()
{
    for (std::vector<CAButton *>::iterator vi = m_segments.begin(); vi != m_segments.end(); ++vi)
    {
        (*vi)->removeFromSuperview();
    }
    m_segments.clear();
    cleanAllSeparate();
}

void CASegmentedControl::setForbidSelectedAtIndex(int index)
{
    m_nForbidSelectedIndexs.insert(index);
}

void CASegmentedControl::setAllowSelectedAtIndex(int index)
{
    m_nForbidSelectedIndexs.erase(index);
}

bool CASegmentedControl::setTitleAtIndex(const char* title, int index, const CAControlState& controlState)
{
    if (!this->indexIsValid(index))
    {
        return false;
    }
    
    CAButton *btn = m_segments.at(index);
    if (NULL == btn)
    {
        return false;
    }
    btn->setTitleForState(controlState, title);
    
    return true;
}

bool CASegmentedControl::setTitleColorAtIndex(const CAColor4B& color, int index, const CAControlState& controlState)
{
    if (!this->indexIsValid(index))
    {
        return false;
    }
    
    CAButton *btn = m_segments.at(index);
    if (NULL == btn)
    {
        return false;
    }
    btn->setTitleColorForState(controlState, color);
    
    return true;
}

bool CASegmentedControl::setBackgroundImageAtIndex(CAImage *image, int index, const CAControlState& controlState)
{
    if (!this->indexIsValid(index))
    {
        return false;
    }
    
    CAButton *btn = m_segments.at(index);
    if (NULL == btn)
    {
        return false;
    }
    CAScale9ImageView* bg = CAScale9ImageView::createWithImage(image);
    if (image)
    {
        CCSize size = image->getContentSize();
        bg->setCapInsets(CCRect(size.width / 2 - 1.0f, size.height / 2 - 1.0f, 2.0f, 2.0f));
    }
    btn->setBackGroundViewForState(controlState, bg);
    
    return true;
}

bool CASegmentedControl::setBackgroundViewAtIndex(CAView *view, int index, const CAControlState& controlState)
{
    if (!this->indexIsValid(index))
    {
        return false;
    }
    
    CAButton *btn = m_segments.at(index);
    if (NULL == btn)
    {
        return false;
    }
    btn->setBackGroundViewForState(controlState, view);
    
    return true;
}

bool CASegmentedControl::setImageAtIndex(CAImage *image, int index, const CAControlState& controlState)
{
    if (!this->indexIsValid(index))
    {
        return false;
    }
    
    CAButton *btn = m_segments.at(index);
    if (NULL == btn)
    {
        return false;
    }
    btn->setImageForState(controlState, image);
    
    return true;
}

bool CASegmentedControl::setImageColorAtIndex(const CAColor4B& color, int index, const CAControlState& controlState)
{
    if (!this->indexIsValid(index))
    {
        return false;
    }
    
    CAButton *btn = m_segments.at(index);
    if (NULL == btn)
    {
        return false;
    }
    btn->setImageColorForState(controlState, color);
    
    return true;
}

void CASegmentedControl::setTitleFontName(const std::string& var)
{
    CC_RETURN_IF(m_sTitleFontName.compare(var) == 0);
    m_sTitleFontName = var;
    for (std::vector<CAButton *>::iterator itr=m_segments.begin(); itr!=m_segments.end(); itr++)
    {
        (*itr)->setTitleFontName(m_sTitleFontName);
    }
}

const std::string& CASegmentedControl::getTitleFontName()
{
    return m_sTitleFontName;
}

CAView* CASegmentedControl::getDefaultNormalBackgroundView()
{
    return CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_square_normal.png"));
}

CAView* CASegmentedControl::getDefaultHighlightedBackgroundView()
{
    return CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_square_highlighted.png"));
}

CAView* CASegmentedControl::getDefaultSelectedBackgroundView()
{
    return CAScale9ImageView::createWithImage(CAImage::create("source_material/btn_square_highlighted.png"));
}

std::string CASegmentedControl::getDefaultmSeparateImagePath()
{
    return "source_material/stepper_divider.png";
}

CAButton* CASegmentedControl::createDefaultSegment()
{
    CCRect elemFrame = CCRectMake(0, 0, m_itemSize.width, m_itemSize.height);
    
    CAButton *newBtn = CAButton::createWithFrame(elemFrame, CAButtonTypeCustom);
    if (newBtn)
    {
        //CAView* normalBG = this->getDefaultNormalBackgroundView();
        CAView* selectedBG = this->getDefaultSelectedBackgroundView();
        //CAView* selectedBG = this->getTailorImageAtIndex(1, "source_material/btn_square_highlighted.png");
        CAView* highlightedBG = this->getDefaultHighlightedBackgroundView();
        newBtn->setBackGroundViewForState(CAControlStateNormal, NULL);
        newBtn->setBackGroundViewForState(CAControlStateSelected, selectedBG);
        newBtn->setBackGroundViewForState(CAControlStateHighlighted, highlightedBG);
        newBtn->setAllowsSelected(true);
        newBtn->setTouchEnabled(false);
    }
    return newBtn;
}

void CASegmentedControl::setHighlightedAtIndex(int index)
{
    if (!this->indexIsValid(index))
    {
        return ;
    }
    
    this->setHighlightedNormal();
    if (index != m_nSelectedIndex)
    {
        m_segments.at(index)->setControlStateHighlighted();
    }
}

void CASegmentedControl::setHighlightedNormal()
{
    for (int i=0; i<m_segments.size(); i++)
    {
        if (m_segments.at(i)->getControlState() != CAControlStateSelected)
        {
            m_segments.at(i)->setControlStateNormal();
        }
    }
}

void CASegmentedControl::setSelectedHighlighted()
{
    int index = m_nSelectedIndex;

    for (int i=0; i<m_segments.size(); i++)
    {
        if (m_segments.at(i)->getControlState() == CAControlStateHighlighted)
        {
            index = i;
        }
    }
    
    if (m_nForbidSelectedIndexs.count(index) == 0)
    {
        for (int i=0; i<m_segments.size(); i++)
        {
            if (m_segments.at(i)->getControlState() == CAControlStateSelected)
            {
                m_segments.at(i)->setControlStateNormal();
            }
        }
        
        m_segments.at(index)->setControlStateSelected();
        
        if (m_nSelectedIndex != index)
        {
            int lastSelectedIndex = m_nSelectedIndex;
            
            m_nSelectedIndex = index;
            
            CC_RETURN_IF(lastSelectedIndex < 0);
            
            if (m_pTarget[CAControlEventTouchValueChanged] && m_selTouch[CAControlEventTouchValueChanged])
            {
                ((CAObject *)m_pTarget[CAControlEventTouchValueChanged]->*m_selTouch[CAControlEventTouchValueChanged])(this, CCPointZero);
            }
        }
    }
    else
    {
        m_segments.at(index)->setControlStateNormal();
        m_nClickToForbidSelectedAtIndex = index;
        
        if (m_pTarget[CAControlEventTouchUpInSide] && m_selTouch[CAControlEventTouchUpInSide])
        {
            ((CAObject *)m_pTarget[CAControlEventTouchUpInSide]->*m_selTouch[CAControlEventTouchUpInSide])(this, CCPointZero);
        }
    }
}

void CASegmentedControl::setSelectedAtIndex(int index)
{
    this->setHighlightedAtIndex(index);
    this->setSelectedHighlighted();
}

int CASegmentedControl::getItemCount() const
{
    return m_segments.size();
}

void CASegmentedControl::layoutSubviews()
{
    const CCSize controlSize = this->getBounds().size;
    const int totalCount = this->getItemCount();
    const float elemWidth = controlSize.width / totalCount;
    m_itemSize = CCSize(elemWidth, controlSize.height);
    CCRect elemFrame = CCRect(0, 0, m_itemSize.width, m_itemSize.height);
    
    for (int i = 0; i < totalCount; ++i)
    {
        CAButton *btn = m_segments.at(i);
        if (btn)
        {
            btn->setFrame(elemFrame);
            btn->setControlState((m_nSelectedIndex == i) ? CAControlStateSelected : CAControlStateNormal);
        }
        elemFrame.origin.x += elemWidth;
    }
}

bool CASegmentedControl::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    m_nClickToForbidSelectedAtIndex = -1;
    this->setHighlightedAtIndex((int)(point.x / m_itemSize.width));
    
    return true;
}

void CASegmentedControl::ccTouchMoved(CrossApp::CATouch *pTouch, CrossApp::CAEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    if (getBounds().containsPoint(point))
    {
        this->setHighlightedAtIndex((int)(point.x / m_itemSize.width));
    }
    else
    {
        this->setHighlightedNormal();
    }
}

void CASegmentedControl::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
    CCPoint point = pTouch->getLocation();
    point = this->convertToNodeSpace(point);
    
    this->setSelectedHighlighted();
}

void CASegmentedControl::ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent)
{
    this->setHighlightedNormal();
}

bool CASegmentedControl::indexIsValid(int index)
{
    return index >= 0 && index < m_segments.size();
}

void CASegmentedControl::setContentSize(const CrossApp::CCSize &var)
{
    CCSize size = var;
    size.height = MAX(size.height, _px(58));
    size.width = MAX(size.width, size.height * 2);
    CAControl::setContentSize(size);
    this->layoutSubviews();
    if (m_pBackgroundView)
    {
        m_pBackgroundView->setFrame(this->getBounds());
    }
    if(m_pBackGroundImageView)
    {
        m_pBackGroundImageView->setFrame(this->getBounds());
    }
    
    this->layoutSubviews();
}

void CASegmentedControl::addTarget(CAObject* target, SEL_CAControl selector)
{
    this->addTarget(target, selector, CAControlEventTouchValueChanged);
}

void CASegmentedControl::removeTarget(CAObject* target, SEL_CAControl selector)
{
    this->removeTarget(target, selector, CAControlEventTouchValueChanged);
}

void CASegmentedControl::addTargetAtForbidSelected(CAObject* target, SEL_CAControl selector)
{
    this->addTarget(target, selector, CAControlEventTouchUpInSide);
}

void CASegmentedControl::removeTargetAtForbidSelected(CAObject* target, SEL_CAControl selector)
{
    this->removeTarget(target, selector, CAControlEventTouchUpInSide);
}

void CASegmentedControl::setBackgroundView(CrossApp::CAView *view)
{
    CC_SAFE_RETAIN(view);
    CC_SAFE_RELEASE_NULL(m_pBackgroundView);
    m_pBackgroundView = view;
    m_pBackgroundView->setFrame(this->getBounds());
    this->insertSubview(m_pBackgroundView, -1);
}

void CASegmentedControl::setBackgroundImageView(CAImage* image)
{
    CC_SAFE_RELEASE_NULL(m_pBackGroundImageView);

    m_pBackGroundImageView = CAScale9ImageView::createWithFrame(this->getBounds());
    m_pBackGroundImageView->setImage(image);
    m_pBackGroundImageView->setVisible(true);
    this->insertSubview(m_pBackGroundImageView, -1);
}

void CASegmentedControl::createSeparate()
{
    const float elemWidth = this->getBounds().size.width / m_segments.size();
    m_itemSize = CCSizeMake(elemWidth, this->getBounds().size.height);
    CCRect elemFrame = CCRectMake(m_itemSize.width - m_fSeparateWidth/2, 0, m_fSeparateWidth, m_itemSize.height);
    
    int count = m_segments.size();
    std::vector<CAButton *>::iterator itr = m_segments.begin();
    for(; itr != m_segments.end() && count != 1; ++ itr, --count)
    {
        CAScale9ImageView* separateImage = CAScale9ImageView::createWithFrame(elemFrame);
        if(m_sSeparateImagePath.empty())
        {
            separateImage->setImage(CAImage::create(getDefaultmSeparateImagePath().c_str()));
        }
        else
        {
            separateImage->setImage(CAImage::create(m_sSeparateImagePath.c_str()));
        }
        elemFrame.origin.x += elemWidth;
        this->insertSubview(separateImage, 100);
        m_vSeparateImage.push_back(separateImage);
    }
}

void CASegmentedControl::cleanAllSeparate()
{
    std::vector<CAScale9ImageView *>::iterator itr = m_vSeparateImage.begin();
    for(; itr != m_vSeparateImage.end(); ++itr)
    {
        (*itr)->removeFromSuperview();
    }
    m_vSeparateImage.clear();
}

void CASegmentedControl::setSeparateImage(const std::string &file)
{
    cleanAllSeparate();
    m_sSeparateImagePath = file;
    createSeparate();
    setSeparateImageVisible(true);
}
void CASegmentedControl::setSeparateImageVisible(bool isVisible)
{
    std::vector<CAScale9ImageView *>::iterator itr = m_vSeparateImage.begin();
    for(; itr != m_vSeparateImage.end(); ++itr)
    {
        (*itr)->setVisible(isVisible);
    }
    m_vSeparateImage.clear();

}

void CASegmentedControl::setSeparateWidth(float width)
{
    cleanAllSeparate();
    m_fSeparateWidth = width;
    createSeparate();
}

float CASegmentedControl::getSeparateWidth()
{
    return m_fSeparateWidth;
}

void CASegmentedControl::setSegmentImage(const CAControlState& controlState, std::string imagePath)
{
    if(m_pBackGroundImageView && m_pBackGroundImageView->isVisible())
    {
        std::vector<CAButton *>::iterator itr = m_segments.begin();
        
        for (int i=0; itr != m_segments.end(); ++itr, ++i) {
            
            (*itr)->setBackGroundViewForState(controlState,getTailorImageAtIndex(i, imagePath));
        }
    }
    else if(m_pBackGroundImageView && !m_pBackGroundImageView->isVisible())
    {
        m_pBackGroundImageView->setVisible(true);
        std::vector<CAButton *>::iterator itr = m_segments.begin();
        
        for (int i=0; itr != m_segments.end(); ++itr, ++i) {
            
            (*itr)->setBackGroundViewForState(controlState,getTailorImageAtIndex(i, imagePath));
        }
        m_pBackGroundImageView->setVisible(false);
    }

}

CAView* CASegmentedControl::getTailorImageAtIndex(int index, std::string imagePath)
{
    CAImageView* imageView = NULL;
    if (NULL ==m_pBackGroundImageView) {
        imageView = CAImageView::createWithImage(CAImage::create(NULL));
        return imageView;
    }
    CAScale9ImageView* clipNode = CAScale9ImageView::createWithFrame(m_pBackGroundImageView->getBounds());
    clipNode->setImage(CAImage::create(imagePath));
    CAClippingView* clip = CAClippingView::create(m_pBackGroundImageView);
    clip->addSubview(clipNode);
    clip->setInverted(false);
    clip->setAlphaThreshold(0);
    
    float x = 0;
    
    for(int i=0; i<index; ++i)
    {
        x +=m_segments.at(i)->getBounds().size.width;
    }
    
    CAButton* button = m_segments.at(index);
    if(button)
    {
        clip->setFrame(this->getBounds());
        this->addSubview(clip);
        CARenderImage* render = CARenderImage::create(m_pBackGroundImageView->getBounds().size.width, m_pBackGroundImageView->getBounds().size.height, kCAImagePixelFormat_RGBA8888);
        render->beginWithClear(0, 0, 0, 0);
        clip->visit();
        render->end();
        clip->removeFromSuperview();
        
        CCRect rect = CCRectMake( index * button->getBounds().size.width, 0, button->getBounds().size.width, button->getBounds().size.height);
        imageView = CAImageView::createWithImage(render->getSprite()->getImage());
        imageView->setRotationX(180.0f);
        imageView->setFrame(rect);
        imageView->setImageRect(rect, false, rect.size);
    }
    return imageView;
}

CAView* CASegmentedControl::getBackgroundView()
{
    return m_pBackgroundView;
}

NS_CC_END
