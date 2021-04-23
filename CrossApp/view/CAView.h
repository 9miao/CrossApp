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
#include "ccTypes.h"
#include "basics/CALayout.h"
#include "dispatcher/CAProtocols.h"
#include "basics/CAResponder.h"
#include "images/CAImageCache.h"
#include "math/CAAffineTransform.h"
#include "math/CAMath.h"
#include "math/TransformUtils.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/CCGLProgramStateCache.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCRenderState.h"
#include "renderer/CCFrameBuffer.h"
#include "renderer/CCTrianglesCommand.h"
#include "renderer/CCCustomCommand.h"

NS_CC_BEGIN

class DPoint;
class CATouch;
class CARGBAProtocol;
class CCComponent;
class CAImage;
class CAContentContainer;
class CAScrollView;
class CAViewAnimation;
class CARenderImage;
class CAApplication;
class CAThemeManager;

class CC_DLL CAView
:public CAResponder
,public CARGBAProtocol
,public CAImageProtocol
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

    virtual void setPointZ(float PointZ);

    virtual float getPointZ();

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
    
    virtual void setTag(int tag);
    
    virtual int getTag();

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

    void setOnEnterCallback(const std::function<void()>& callback) { m_obOnEnterCallback = callback; }
    
    const std::function<void()>& getOnEnterCallback() const { return m_obOnEnterCallback; }
    
    void setOnExitCallback(const std::function<void()>& callback) { m_obOnExitCallback = callback; }
    
    const std::function<void()>& getOnExitCallback() const { return m_obOnExitCallback; }
    
    void setonEnterTransitionDidFinishCallback(const std::function<void()>& callback) { m_obOnEnterTransitionDidFinishCallback = callback; }
    
    const std::function<void()>& getonEnterTransitionDidFinishCallback() const { return m_obOnEnterTransitionDidFinishCallback; }
    
    void setonExitTransitionDidStartCallback(const std::function<void()>& callback) { m_obOnExitTransitionDidStartCallback = callback; }
    
    const std::function<void()>& getonExitTransitionDidStartCallback() const { return m_obOnExitTransitionDidStartCallback; }
    
    virtual void onEnter();

    virtual void onEnterTransitionDidFinish();

    virtual void onExit();

    virtual void onExitTransitionDidStart();

    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
    virtual void draw() final;

    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
    virtual void visit() final;

    virtual void visitEve(void);
    
    virtual CAResponder* nextResponder();
    
    virtual CAView* copy();
    
    virtual void enabledLeftShadow(bool var);

    virtual void enabledRightShadow(bool var);
    
    virtual void enabledTopShadow(bool var);
    
    virtual void enabledBottomShadow(bool var);
    
public:
    
    bool isVisitableByVisitingCamera() const;
    Mat4 transform(const Mat4& parentTransform);
    uint32_t processParentFlags(const Mat4& parentTransform, uint32_t parentFlags);
    
    virtual void updateTransform();

    virtual const Mat4& getViewToSuperviewTransform() const;
    virtual AffineTransform getViewToSuperviewAffineTransform() const;
    
    virtual Mat4 getViewToSuperviewTransform(CAView* ancestor) const;
    
    virtual AffineTransform getViewToSuperviewAffineTransform(CAView* ancestor) const;
    
    virtual void setViewToSuperviewTransform(const Mat4& transform);
    
    virtual const Mat4& getSuperviewToViewTransform() const;
    virtual AffineTransform getSuperviewToViewAffineTransform() const;
    
    
    virtual Mat4 getViewToWorldTransform() const;
    virtual AffineTransform getViewToWorldAffineTransform() const;
    
    virtual Mat4 getWorldToViewTransform() const;
    virtual AffineTransform getWorldToViewAffineTransform() const;
    
    void setAdditionalTransform(const Mat4* additionalTransform);
    void setAdditionalTransform(const AffineTransform& additionalTransform);

    DRect convertRectToNodeSpace(const DRect& worldRect);

    DRect convertRectToWorldSpace(const DRect& nodeRect);

    DPoint convertToNodeSpace(const DPoint& worldPoint);

    DPoint convertToWorldSpace(const DPoint& nodePoint);

    DSize convertToNodeSize(const DSize& worldSize);
    
    DSize convertToWorldSize(const DSize& nodeSize);
    
    DPoint convertTouchToNodeSpace(CATouch * touch);
    
    virtual void setOrderOfArrival(unsigned int uOrderOfArrival);
    
    virtual unsigned int getOrderOfArrival();

    GLProgramState *getGLProgramState();

    virtual void setGLProgramState(GLProgramState *glProgramState);

    virtual void setShaderProgram(GLProgram *pShaderProgram);

    virtual GLProgram* getShaderProgram();
    
public:
    
    virtual bool isDisplayRange();
    
    virtual void setDisplayRange(bool value);

    virtual float getAlpha();
    
    virtual void setAlpha(float alpha);
    
    virtual float getDisplayedAlpha();
    
    virtual void updateDisplayedAlpha(float superviewAlpha);
    
    virtual const CAColor4B& getColor(void);
    
    virtual const CAColor4B& getDisplayedColor();
    
    virtual void setColor(const CAColor4B& color);
    
    virtual void updateDisplayedColor(const CAColor4B& superviewColor);

    inline void setBlendFunc(BlendFunc blendFunc) { m_sBlendFunc = blendFunc; }
    
    inline BlendFunc getBlendFunc(void) { return m_sBlendFunc; }
    
    virtual void setOpacityModifyRGB(bool modify);
    
    virtual bool isOpacityModifyRGB(void);
    
    virtual void update(float fDelta);
    
    virtual void reViewlayout(const DSize& contentSize, bool allowAnimation = false);
    
    inline ccV3F_C4B_T2F_Quad getQuad(void) { return m_sQuad; }
    
    inline bool isImageRectRotated(void) { return m_bRectRotated; }
    
    inline const DRect& getImageRect(void) { return m_obRect; }
    
    bool isFlipX(void);
    
    void setFlipX(bool bFlipX);
    
    bool isFlipY(void);
    
    void setFlipY(bool bFlipY);
    
    unsigned short getCameraMask() const { return m_iCameraMask; }

    virtual void setCameraMask(unsigned short mask, bool applyChildren = true);
    
public:
    
    virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
    
    virtual void ccTouchCancelled(CATouch *pTouch, CAEvent *pEvent);
    
protected:
    
    void detachSubview(CAView *subview);
    
    virtual void updateDraw();
    
    virtual void updateColor(void);
    
    virtual void setPoint(const DPoint &point);
    
    virtual void setContentSize(const DSize& contentSize);
    
    virtual void setImage(CAImage* image);
    
    virtual CAImage* getImage(void);
    
    virtual void setImageRect(const DRect& rect);
    
    virtual void setImageCoords(DRect rect);
    
    virtual void setVertexRect(const DRect& rect);
    
    virtual void updateBlendFunc(void);
    
    virtual void updateImageRect();
    
    void updateRotationQuat();
    
    void checkInsideBounds(Renderer* renderer, const Mat4 &transform, uint32_t flags);

protected:
 
    CC_SYNTHESIZE(CAContentContainer*, m_pContentContainer, ContentContainer);
    
    CC_SYNTHESIZE_READONLY(int, m_eLayoutType, LayoutType);
    
    float                       m_fSkewX;
    float                       m_fSkewY;
    
    int                         m_fRotationX;
    int                         m_fRotationY;
    int                         m_fRotationZ;
    Quaternion                  m_obRotationQuat;      ///rotation using quaternion, if _rotationZ_X == _rotationZ_Y, _rotationQuat = RotationZ_X * RotationY * RotationX, else _rotationQuat = RotationY * RotationX
    
    float                       m_fScaleX;
    float                       m_fScaleY;
    float                       m_fScaleZ;
    
    float                       m_fPointZ;
    
    DPoint                      m_obAnchorPointInPoints;
    DPoint                      m_obAnchorPoint;
    
    DPoint                      m_obPoint;
    DSize                       m_obContentSize;
    bool                        m_bContentSizeDirty;
    
    DLayout                     m_obLayout;
    DRect                       m_obReturn;

    int                         m_nZOrder;
    
    CAVector<CAView*>           m_obSubviews;
    CAView*                     m_pSuperview;

    // "cache" variables are allowed to be mutable
    mutable Mat4                m_tTransform;      ///< transform
    mutable bool                m_bTransformDirty;   ///< transform dirty flag
    mutable Mat4                m_tInverse;        ///< inverse transform
    mutable bool                m_bInverseDirty;     ///< inverse transform dirty flag
    mutable Mat4*               m_pAdditionalTransform; ///< transform
    mutable bool                m_bAdditionalTransformDirty;   ///< The flag to check whether the additional transform is dirty
    bool                        m_bTransformUpdated;         ///< Whether or not the Transform object was updated since the last
    
    Mat4                        m_tModelViewTransform;       ///< ModelView transform of the Node.
    
    unsigned int                m_uOrderOfArrival;
    
    GLProgramState              *m_pGlProgramState;

    bool                        m_bRunning;
    
    bool                        m_bVisible;

    float                       _displayedAlpha;
    float                       _realAlpha;
	CAColor4B                   _displayedColor;
    CAColor4B                   _realColor;
    
    unsigned short              m_iCameraMask;
    
    std::function<void()>       m_obOnEnterCallback;
    std::function<void()>       m_obOnExitCallback;
    std::function<void()>       m_obOnEnterTransitionDidFinishCallback;
    std::function<void()>       m_obOnExitTransitionDidStartCallback;
    
    bool                        m_bReorderSubviewDirty;
    
    bool                        m_bDisplayRange;
    bool                        m_bScissorRestored;
    DRect                       m_obSupviewScissorRect;
    
    DRect                       m_obRect;
    bool                        m_bRectRotated;
    
    bool                        m_bIsAnimation;
    
    ccV3F_C4B_T2F_Quad          m_sQuad;

    bool                        m_bFlipX;
    bool                        m_bFlipY;
    
    BlendFunc                   m_sBlendFunc;
    bool                        m_bOpacityModifyRGB;
    
    bool                        m_bInsideBounds;
    CAImage*                    m_pobImage;
    
    TrianglesCommand            m_obTrianglesCommand;
    
    CustomCommand               m_obBeforeDrawCommand;
    CustomCommand               m_obAfterDrawCommand;

    CAApplication*              m_pApplication;
    
protected:
    
    struct Shadow
    {
        Shadow();
        ~Shadow();
        CAImage*            image;
        ccV3F_C4B_T2F_Quad  quad;
        BlendFunc           blendFunc;
        TrianglesCommand    trianglesCommand;
        GLProgramState*     glProgramState;
        void updateQuad(CAView* view, GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat left, GLfloat right, GLfloat top, GLfloat bottom);
    };
    
    void drawAllShadow(Renderer* renderer, const Mat4 &transform, uint32_t flags);

    void drawShadow(Renderer* renderer, const Mat4 &transform, uint32_t flags, CAView::Shadow* shadow);
    
    void autoEnabledCornerShadow();
    
    CAView::Shadow*             m_pLeftShadow;
    CAView::Shadow*             m_pRightShadow;
    CAView::Shadow*             m_pTopShadow;
    CAView::Shadow*             m_pBottomShadow;
    
    CAView::Shadow*             m_pLeftTopShadow;
    CAView::Shadow*             m_pRightTopShadow;
    CAView::Shadow*             m_pLeftBottomShadow;
    CAView::Shadow*             m_pRightBottomShadow;

    
protected:
    
    friend class                CARenderImage;
    
    friend class                CAScrollView;
    
    friend class                CAViewAnimation;
    
    friend class                CAViewModel;

};

class CC_DLL CAContentContainer
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
