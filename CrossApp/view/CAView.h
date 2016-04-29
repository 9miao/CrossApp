//
//  CAView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-12.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CAView__
#define __CAView__

#include <iostream>
#include "ccMacros.h"
#include "CCGL.h"
#include "shaders/CATransformation.h"
#include "basics/CALayout.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CAGLProgram.h"
#include "kazmath/kazmath.h"
#include "dispatcher/CAProtocols.h"
#include "platform/CAAccelerometerDelegate.h"
#include "basics/CAResponder.h"
#include "images/CAImageCache.h"


#ifdef EMSCRIPTEN
#include "base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

class CACamera;
class DPoint;
class CATouch;
class CARGBAProtocol;
class CCComponent;
class CAImage;
class CAContentContainer;
class CABatchView;
class CAScrollView;
class CAViewAnimation;
class CARenderImage;
struct transformValues_;


enum {
    kCAViewOnEnter,
    kCAViewOnExit,
    kCAViewOnEnterTransitionDidFinish,
    kCAViewOnExitTransitionDidStart,
    kCAViewOnCleanup
};

typedef enum
{
    CALayoutLinearHorizontal = 0,
    CALayoutLinearVertical
}
CALayoutLinearType;

#define kCAViewPointInvalid DPoint(FLT_MIN, FLT_MIN)
#define kCAViewSizeInvalid DPoint(0, 0)
#define kCAViewRectInvalid DRect(0, 0, 0, 0)

class CC_DLL CAView
:public CAResponder
,public CARGBAProtocol
,public CAImageProtocol
#ifdef EMSCRIPTEN
,public CCGLBufferedNode
#endif // EMSCRIPTEN
{
    
public:

    static CAView * create(void);
    
    static CAView* createWithFrame(const DRect& rect);
    
    static CAView* createWithFrame(const DRect& rect, const CAColor4B& color4B);
    
    static CAView* createWithCenter(const DRect& rect);
    
    static CAView* createWithCenter(const DRect& rect, const CAColor4B& color4B);
    
    static CAView* createWithLayout(const DLayout& layout);
    
    static CAView* createWithLayout(const DLayout& layout, const CAColor4B& color4B);
    
    static CAView* createWithColor(const CAColor4B& color4B);
    
    CAView();
    
    virtual ~CAView();

    virtual bool init();

    virtual bool initWithFrame(const DRect& rect);
    
    virtual bool initWithCenter(const DRect& rect);
    
    virtual bool initWithLayout(const DLayout& layout);
    
    virtual bool initWithColor(const CAColor4B& color4B);

    const char* description(void);

    virtual void setZOrder(int zOrder);

    virtual void _setZOrder(int z);

    virtual int getZOrder();

    virtual void setVertexZ(float vertexZ);

    virtual float getVertexZ();

    virtual void setScaleX(float fScaleX);

    virtual float getScaleX();

    virtual void setScaleY(float fScaleY);

    virtual float getScaleY();

    virtual void setScale(float scale);

    virtual float getScale();

    virtual void setScale(float fScaleX,float fScaleY);

    virtual void setSkewX(float fSkewX);

    virtual float getSkewX();

    virtual void setSkewY(float fSkewY);

    virtual float getSkewY();

    void setAnchorPoint(const DPoint& anchorPoint);

    const DPoint& getAnchorPoint();

    void setAnchorPointInPoints(const DPoint& anchorPointInPoints);
    
    const DPoint& getAnchorPointInPoints();
    
    virtual void setFrame(const DRect& rect);
    
    virtual const DRect& getFrame();
    
    virtual void setFrameOrigin(const DPoint& point);
    
    virtual const DPoint& getFrameOrigin();
    
    virtual void setCenter(const DRect& rect);
    
    virtual const DRect&  getCenter();
    
    virtual void setBounds(const DRect& rect);
    
    virtual const DRect& getBounds();

    virtual void setCenterOrigin(const DPoint& point);
    
    virtual const DPoint& getCenterOrigin();

    virtual void setLayout(const DLayout& layout);
    
    const DLayout& getLayout();
    
    virtual void setVisible(bool visible);

    virtual bool isVisible();

    virtual void setRotation(int fRotation);

    virtual int getRotation();

    virtual void setRotationX(int fRotaionX);

    virtual int getRotationX();

    virtual void setRotationY(int fRotationY);
 
    virtual int getRotationY();

    virtual void addSubview(CAView * child);

    virtual void insertSubview(CAView* subview, int z);

    virtual CAView * getSubviewByTag(int tag);

    virtual CAView * getSubviewByTextTag(const std::string& textTag);
    
    virtual const CAVector<CAView*>& getSubviews();

    virtual unsigned int getSubviewsCount(void) const;

    virtual void setSuperview(CAView* superview);
    
    virtual CAView* getSuperview();
    
    virtual void removeFromSuperview();

    virtual void removeSubview(CAView* subview);

    virtual void removeSubviewByTag(int tag);

    virtual void removeSubviewByTextTag(const std::string& textTag);
    
    virtual void removeAllSubviews();

    virtual void reorderSubview(CAView * child, int zOrder);

    virtual void sortAllSubviews();

    virtual bool isRunning();

    virtual void onEnter();

    virtual void onEnterTransitionDidFinish();

    virtual void onExit();

    virtual void onExitTransitionDidStart();

    virtual CACamera* getCamera();

    virtual void draw(void);

    virtual void visit(void);

    virtual void visitEve(void);
    
    virtual CAResponder* nextResponder();
    
    virtual CAView* copy();
    
public:
    
    void transform(void);
    
    void transformAncestors(void);

    virtual void updateTransform(void);

    virtual CATransformation viewToSuperviewTransform(void);

    DRect convertRectToNodeSpace(const DRect& worldRect);

    DRect convertRectToWorldSpace(const DRect& nodeRect);

    DPoint convertToNodeSpace(const DPoint& worldPoint);

    DPoint convertToWorldSpace(const DPoint& nodePoint);

    DPoint convertToNodeSize(const DSize& worldSize);
    
    DPoint convertToWorldSize(const DSize& nodeSize);
    
    DPoint convertTouchToNodeSpace(CATouch * touch);

    DPoint convertTouchToNodeSpaceAR(CATouch * touch);
    
    virtual void setOrderOfArrival(unsigned int uOrderOfArrival);
    
    virtual unsigned int getOrderOfArrival();

    virtual void setGLServerState(ccGLServerState glServerState);

    virtual ccGLServerState getGLServerState();

    virtual void setShaderProgram(CAGLProgram *pShaderProgram);

    virtual CAGLProgram* getShaderProgram();
        
public:
    
    virtual bool isDisplayRange();
    
    virtual void setDisplayRange(bool value);

    virtual float getAlpha();
    
    virtual void setAlpha(float alpha);
    
    virtual float getDisplayedAlpha();
    
    virtual void updateDisplayedAlpha(float parentOpacity);
    
    virtual const CAColor4B& getColor(void);
    
    virtual const CAColor4B& getDisplayedColor();
    
    virtual void setColor(const CAColor4B& color);
    
    virtual void updateDisplayedColor(const CAColor4B& parentColor);

    virtual void update(float fDelta);
    
    virtual void reViewlayout(const DSize& contentSize, bool allowAnimation = false);
    
    inline void setBlendFunc(ccBlendFunc blendFunc) { m_sBlendFunc = blendFunc; }
    
    inline ccBlendFunc getBlendFunc(void) { return m_sBlendFunc; }
    
    inline virtual bool isDirty(void) { return m_bDirty; }
    
    inline virtual void setDirty(bool bDirty) { m_bDirty = bDirty; }
    
    inline ccV3F_C4B_T2F_Quad getQuad(void) { return m_sQuad; }
    
    inline bool isImageRectRotated(void) { return m_bRectRotated; }
    
    inline unsigned int getAtlasIndex(void) { return m_uAtlasIndex; }
    
    inline void setAtlasIndex(unsigned int uAtlasIndex) { m_uAtlasIndex = uAtlasIndex; }
    
    inline const DRect& getImageRect(void) { return m_obRect; }
    
    bool isFlipX(void);
    
    void setFlipX(bool bFlipX);
    
    bool isFlipY(void);
    
    void setFlipY(bool bFlipY);
    
    void setBatch(CABatchView* batchView);
    
public:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
protected:
    
    void detachSubview(CAView *subview);
    
    void updateDraw();
    
    void updateColor(void);
    
    virtual void setPoint(const DPoint &point);
    
    virtual void setContentSize(const DSize& contentSize);
    
    virtual void setImage(CAImage* image);
    
    virtual CAImage* getImage(void);
    
    virtual void setImageRect(const DRect& rect);
    
    virtual void setImageCoords(DRect rect);
    
    virtual void setVertexRect(const DRect& rect);
    
    virtual void setReorderChildDirtyRecursively(void);
    
    virtual void setDirtyRecursively(bool bValue);

    virtual void updateBlendFunc(void);
    
    virtual void updateImageRect();
    
protected:
 
    CC_SYNTHESIZE(CAContentContainer*, m_pContentContainer, ContentContainer);
    
    CC_SYNTHESIZE_READONLY(int, m_eLayoutType, LayoutType);
    
    CC_SYNTHESIZE_READONLY(CABatchView*, m_pobBatchView, Batch);
    
    CC_SYNTHESIZE(CAImageAtlas*, m_pobImageAtlas, ImageAtlas);
    
    float                       m_fSkewX;
    float                       m_fSkewY;
    
    int                         m_fRotationX;
    int                         m_fRotationY;
    
    float                       m_fScaleX;
    float                       m_fScaleY;
    
    float                       m_fVertexZ;
    
    DPoint                      m_obAnchorPointInPoints;
    DPoint                      m_obAnchorPoint;
    
    DPoint                      m_obPoint;
    DSize                       m_obContentSize;
    
    DLayout                     m_obLayout;
    DRect                       m_obReturn;
    
    
    CATransformation            m_sAdditionalTransform;
    CATransformation            m_sTransform;
    
    CACamera*                   m_pCamera;

    int                         m_nZOrder;
    
    CAVector<CAView*>           m_obSubviews;
    CAView*                     m_pSuperview;

    
    CAGLProgram*                m_pShaderProgram;
    
    
    ccGLServerState             m_eGLServerState;
    
    unsigned int                m_uOrderOfArrival;
    
    bool                        m_bRunning;
    
    bool                        m_bTransformDirty;
    bool                        m_bInverseDirty;
    bool                        m_bVisible;

    bool                        m_bReorderChildDirty;
    
    float                       _displayedAlpha;
    float                       _realAlpha;
	CAColor4B                   _displayedColor;
    CAColor4B                   _realColor;
    
    bool                        m_bDisplayRange;

    unsigned int                m_uAtlasIndex;
    
    bool                        m_bDirty;
    bool                        m_bRecursiveDirty;
    
    bool                        m_bHasChildren;
    bool                        m_bShouldBeHidden;
    
    CATransformation            m_transformToBatch;

    DRect                       m_obRect;
    bool                        m_bRectRotated;
    

    bool                        m_bIsAnimation;
    
    ccV3F_C4B_T2F_Quad          m_sQuad;
    
    bool                        m_bFlipX;
    bool                        m_bFlipY;
    
    ccBlendFunc                 m_sBlendFunc;
    
    CAImage*                    m_pobImage;
    
    friend class                CARenderImage;
    
    friend class                CAScrollView;
    
    friend class                CAViewAnimation;
};

class CC_DLL CAContentContainer: public CAResponder
{
public:
    
    virtual ~CAContentContainer(){};
    
    virtual void viewOnEnterTransitionDidFinish() = 0;
    
    virtual void viewOnExitTransitionDidStart() = 0;
    
    virtual void viewOnSizeTransitionDidChanged() = 0;
};

static bool compareSubviewZOrder(CAView* one, CAView* two)
{
    if (one->getZOrder() < two->getZOrder())
    {
        return true;
    }
    else if (one->getZOrder() == two->getZOrder())
    {
        return (bool)(one->getOrderOfArrival() < two->getOrderOfArrival());
    }
    return false;
}

NS_CC_END


#endif
