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
#include "shaders/CATransformation.h"
#include "CCGL.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CAGLProgram.h"
#include "kazmath/kazmath.h"
#include "dispatcher/CAProtocols.h"
#include "platform/CCAccelerometerDelegate.h"
#include "basics/CAResponder.h"
#include "basics/CASTLContainer.h"
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
class CAViewDelegate;
class CABatchView;
class CAViewAnimation;
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
    
    static CAView* createWithColor(const CAColor4B& color4B);
    
    CAView();
    
    virtual ~CAView();

    virtual bool init();

    virtual bool initWithFrame(const DRect& rect);
    
    virtual bool initWithFrame(const DRect& rect, const CAColor4B& color4B);
    
    virtual bool initWithCenter(const DRect& rect);
    
    virtual bool initWithCenter(const DRect& rect, const CAColor4B& color4B);
    
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
    
    virtual const DRect& getFrame() const;
    
    virtual void setFrameOrigin(const DPoint& point);
    
    virtual const DPoint& getFrameOrigin();
    
    virtual void setCenter(const DRect& rect);
    
    virtual DRect getCenter();
    
    virtual void setBounds(const DRect& rect);
    
    virtual DRect getBounds() const;

    virtual void setCenterOrigin(const DPoint& point);
    
    virtual DPoint getCenterOrigin();

    virtual void setVisible(bool visible);

    virtual bool isVisible();

    virtual void setRotation(float fRotation);

    virtual float getRotation();

    virtual void setRotationX(float fRotaionX);

    virtual float getRotationX();

    virtual void setRotationY(float fRotationY);
 
    virtual float getRotationY();

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

    virtual CAResponder* nextResponder();
    
    virtual CAView* copy();
    
public:
    
    void transform(void);
    
    void transformAncestors(void);

    virtual void updateTransform(void);

    virtual CATransformation nodeToParentTransform(void);

    virtual CATransformation parentToNodeTransform(void);

    virtual CATransformation nodeToWorldTransform(void);

    virtual CATransformation worldToNodeTransform(void);

    DRect convertRectToNodeSpace(const DRect& worldRect);

    DRect convertRectToWorldSpace(const DRect& nodeRect);

    DPoint convertToNodeSpace(const DPoint& worldPoint);

    DPoint convertToWorldSpace(const DPoint& nodePoint);

    DPoint convertToNodeSpaceAR(const DPoint& worldPoint);

    DPoint convertToWorldSpaceAR(const DPoint& nodePoint);

    DPoint convertTouchToNodeSpace(CATouch * touch);

    DPoint convertTouchToNodeSpaceAR(CATouch * touch);
    
    virtual void setOrderOfArrival(unsigned int uOrderOfArrival);
    
    virtual unsigned int getOrderOfArrival();

    virtual void setGLServerState(ccGLServerState glServerState);

    virtual ccGLServerState getGLServerState();

    virtual void setShaderProgram(CAGLProgram *pShaderProgram);

    virtual CAGLProgram* getShaderProgram();
    
    void setAdditionalTransform(const CATransformation& additionalTransform);
    
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
    
    void reViewlayout();
    
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

    CC_SYNTHESIZE(CAViewDelegate*, m_pViewDelegate, ViewDelegate);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bFrame, Frame);
    
    CC_SYNTHESIZE_READONLY(CABatchView*, m_pobBatchView, Batch);
    
    CC_SYNTHESIZE(CAImageAtlas*, m_pobImageAtlas, ImageAtlas);
    
    float m_fRotationX;                 ///< rotation angle on x-axis
    float m_fRotationY;                 ///< rotation angle on y-axis
    
    float m_fScaleX;                    ///< scaling factor on x-axis
    float m_fScaleY;                    ///< scaling factor on y-axis
    
    float m_fVertexZ;                   ///< OpenGL real Z vertex
    
    DPoint m_obPoint;               ///< position of the node
    
    float m_fSkewX;                     ///< skew angle on x-axis
    float m_fSkewY;                     ///< skew angle on y-axis
    
    DPoint m_obAnchorPointInPoints;    ///< anchor point in points
    DPoint m_obAnchorPoint;            ///< anchor point normalized (NOT in points)
    
    DSize m_obContentSize;             ///< untransformed size of the node
    
    DRect m_obFrameRect;
    
    CATransformation m_sAdditionalTransform; ///< transform
    CATransformation m_sTransform;     ///< transform
    CATransformation m_sInverse;       ///< transform
    
    CACamera *m_pCamera;                ///< a camera

    int m_nZOrder;                      ///< z-order value that affects the draw order
    
    CAVector<CAView*> m_obSubviews;               ///< array of children nodes              ///< weak reference to parent node
    CAView* m_pSuperview;
    
    CAGLProgram *m_pShaderProgram;      ///< OpenGL shader
    
    ccGLServerState m_eGLServerState;   ///< OpenGL servier side state
    
    unsigned int m_uOrderOfArrival;     ///< used to preserve sequence while sorting children with the same zOrder
    
    bool m_bRunning;                    ///< is running
    
    bool m_bTransformDirty;             ///< transform dirty flag
    bool m_bInverseDirty;               ///< transform dirty flag
    bool m_bAdditionalTransformDirty;   ///< The flag to check whether the additional transform is dirty
    bool m_bVisible;                    ///< is this node visible

    bool m_bReorderChildDirty;          ///< children order dirty flag
    
    float		_displayedAlpha;
    float       _realAlpha;
	CAColor4B	_displayedColor;
    CAColor4B   _realColor;
    
    bool m_bDisplayRange;

    unsigned int        m_uAtlasIndex;          /// Absolute (real) Index on the SpriteSheet
    
    bool                m_bDirty;               /// Whether the sprite needs to be updated
    bool                m_bRecursiveDirty;      /// Whether all of the sprite's children needs to be
    
    bool                m_bHasChildren;         /// Whether the sprite contains children
    bool                m_bShouldBeHidden;      /// should not be drawn because one of the ancestors
    
    CATransformation   m_transformToBatch;

    DRect m_obRect;                            /// Retangle of CAImage
    bool   m_bRectRotated;                      /// Whether the Image is rotated
    

    bool m_bIsAnimation;
    
    // vertex coords, Image coords and color info
    ccV3F_C4B_T2F_Quad m_sQuad;
    
    // image is flipped
    bool m_bFlipX;                              /// Whether the sprite is flipped horizaontally or not.
    bool m_bFlipY;                              /// Whether the sprite is flipped vertically or not.
    
    ccBlendFunc        m_sBlendFunc;            /// It's required for CAImageProtocol inheritance
    
    CAImage*       m_pobImage;            /// CAImage object that is used to render the sprite
    
    friend class CAViewAnimation;
};

class CC_DLL CAViewDelegate
{
public:
    
    virtual ~CAViewDelegate(){};
    
    virtual void getSuperViewRect(const DRect& rect) = 0;
    
    virtual void viewOnEnterTransitionDidFinish() = 0;
    
    virtual void viewOnExitTransitionDidStart() = 0;
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
