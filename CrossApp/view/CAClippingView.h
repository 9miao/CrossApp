
#ifndef __MISCNODE_CCCLIPPING_NODE_H__
#define __MISCNODE_CCCLIPPING_NODE_H__

#include "CAView.h"
#include "CCGL.h"

NS_CC_BEGIN


class CC_DLL CAClippingView : public CAView
{
protected:
    CAView* m_pStencil;
    GLfloat m_fAlphaThreshold;
    bool    m_bInverted;
    
public:
    /** Creates and initializes a clipping node without a stencil.
     */
    static CAClippingView* create();
    
    /** Creates and initializes a clipping node with an other node as its stencil.
     The stencil node will be retained.
     */
    static CAClippingView* create(CAView *pStencil);
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CAClippingView();
    
    /** Initializes a clipping node without a stencil.
     */
    virtual bool init();
    
    /** Initializes a clipping node with an other node as its stencil.
     The stencil node will be retained, and its parent will be set to this clipping node.
     */
    virtual bool init(CAView *pStencil);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual void onEnter();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual void onEnterTransitionDidFinish();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual void onExitTransitionDidStart();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual void onExit();
    virtual void visit();
    
    /** The CAView to use as a stencil to do the clipping.
     The stencil node will be retained.
     This default to nil.
     */
    CAView* getStencil() const;
    void setStencil(CAView *pStencil);
    
    /** The alpha threshold.
     The content is drawn only where the stencil have pixel with alpha greater than the alphaThreshold.
     Should be a float between 0 and 1.
     This default to 1 (so alpha test is disabled).
     */
    GLfloat getAlphaThreshold() const;
    void setAlphaThreshold(GLfloat fAlphaThreshold);
    
    /** Inverted. If this is set to YES,
     the stencil is inverted, so the content is drawn where the stencil is NOT drawn.
     This default to NO.
     */
    bool isInverted() const;
    void setInverted(bool bInverted);
    
protected:
    CAClippingView();
};

NS_CC_END

#endif // __MISCNODE_CCCLIPPING_NODE_H__
