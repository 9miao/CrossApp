
//
//  LyfTableViewCell.h
//  LTabelView
//
//  Created by LI YUANFENG on 13-3-11.
//
//

#ifndef __LTabelView__LyfTableViewCell__
#define __LTabelView__LyfTableViewCell__

#include <iostream>
#include "cocos2d.h"
#include "LyfIndexPath.h"
using namespace cocos2d;

#define dvalue(x) x*2 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#define TOUCH_PROTECTION_ACCURACY 0
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define TOUCH_PROTECTION_ACCURACY dvalue(2)
#endif

class LyfTableViewCell;
typedef enum {
    tNone = 0,
    tCell = 1,
    tLead = 2,
    tDel = 3
}tTouchModel;

typedef enum {
    tTop = 0,
    tCenter = 1,
    tBottom = 2
    
}tVerticalText;

class LyfTableViewCellDelegate {
    
public:
    virtual void setTouchPitchOn(LyfTableViewCell* cell) = 0;
    
    virtual void setTouchImageView(LyfTableViewCell* cell) = 0;
    
    virtual void removeTableCell(LyfTableViewCell* cell) = 0;
    
    virtual void joinEditing(LyfTableViewCell* cell) = 0;
    
    virtual bool isEditing() = 0;
    
    virtual bool isCanEdit() = 0;
    
    virtual void closeEditing() = 0;
    
    virtual bool isCanSelected() = 0;
    
    void removeDelegate(LyfTableViewCell* cell);
};


class LyfTableViewCell: public CCSprite
{
    
public:
      
    LyfTableViewCell();
    
    virtual ~LyfTableViewCell();
    
    static LyfTableViewCell* create(const char *fileName);
    
    static LyfTableViewCell* createWithSpriteFrameName(const char *fileName);
    
    static LyfTableViewCell* createTmp();
    
    static LyfTableViewCell* createWithTexture(CCTexture2D *pTexture);
    
    void setDelegate(LyfTableViewCellDelegate * tableView);
    
public:
    
    //点击特效
    virtual void setTouchedEffect(){};
    
    virtual void setNoTouchedEffect(){};
    
    //选中特效
    virtual void setPitchOnEffect(){};
    
    virtual void setNoPitchOnEffect(){};
    
    //不可点特效
    virtual void setClickEffect(){};
    
    virtual void setNoClickEffect(){};
    
    //设置触摸优先级
    virtual void setPriority(int nPriority);
    
public:
    
    void setImageView(CCSprite* sprite);
    
    CCSprite* getImageView();
    
    void setTextLabel(CCLabelTTF* textLabel);
    
    CCLabelTTF* getTextLabel();
    
    void setDetailTextLabel(CCLabelTTF* detailTextLabel);
    
    CCLabelTTF* getDetailTextLabel();
    
    void setTouchModel(tTouchModel tEnum);
    
    tTouchModel m_touchModel;
    
    void showDelBtn();
    
    void removeDelBtnParent();
    
public:
    
    //点击特效开关
    CC_SYNTHESIZE(bool, m_specially, TouchSpecially);
    
    //string型标记
    CC_SYNTHESIZE(std::string, m_textTag, TextTag);
    
    //属性
    CC_SYNTHESIZE(LyfIndexPath, m_sign, Sign);
    
    //触摸开关
    CC_PROPERTY(bool, m_bIsTouchEnabled, TouchEnabled);
    
    //是否高亮
    CC_SYNTHESIZE_READONLY(bool, m_touchBegin, TouchBegin);
    
    //是否选中
    CC_PROPERTY(bool, m_pitchOn, PitchOn);
    
    bool isTextTagEqual(const char* text);
    
    //传入相对于cell的point，判断point是否在tableView可视范围内
    bool isInTheRangeOfTableView(CCPoint point);
    
    void setOpacity(GLubyte opacity);
    
    void removeThisAndActions();
    
    void removeThis();
    
protected:
    
    virtual void onExit();
    
    virtual void onEnter();
    
    CCSprite* m_imageView;
    
    CCLabelTTF* m_textLabel;
    
    CCLabelTTF* m_detailTextLabel;
    
    LyfTableViewCellDelegate* m_delegate;
    
    int m_nTouchPriority;
    
    bool m_IsRehisterTouchDispatcher;
    
    CCSprite* m_delBtn;
    
    CCRect m_delRect;
    
    void delShowAction();
    
    void delHideAction();
    
protected:
    
    bool isOverstep(CCPoint point);
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void registerWithTouchDispatcher();
    
    void unRegisterWithTouchDispatcher();
    
    inline CCRect getRect();
    
    inline CCRect getBtnRect(CCSprite* sprite);
    
    inline CCRect getDelRect();
    
    CCPoint m_touchBeginUIPoint;
    
};

#endif /* defined(__LTabelView__LyfTableViewCell__) */
