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
#include "cocoa/CCAffineTransform.h"
#include "cocoa/CCArray.h"
#include "CCGL.h"
#include "shaders/ccGLStateCache.h"
#include "shaders/CCGLProgram.h"
#include "kazmath/kazmath.h"
#include "CCProtocols.h"
#include "platform/CCAccelerometerDelegate.h"
#include "keypad_dispatcher/CCKeypadDelegate.h"
#include "cocoa/CAResponder.h"
#include "images/CAImageAtlas.h"

#ifdef EMSCRIPTEN
#include "base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

class CCCamera;
class CCGridBase;
class CCPoint;
class CCTouch;
class CCAction;
class CCRGBAProtocol;
class CCLabelProtocol;
class CAScheduler;
class CCActionManager;
class CCComponent;
class CCDictionary;
class CCComponentContainer;
class CAImage;
class CAViewDelegate;
class CABatchView;
struct transformValues_;


enum {
    kCAViewOnEnter,
    kCAViewOnExit,
    kCAViewOnEnterTransitionDidFinish,
    kCAViewOnExitTransitionDidStart,
    kCAViewOnCleanup
};

#define kCAViewPointInvalid CCPoint(-0xffffffff, -0xffffffff)
#define kCAViewSizeInvalid CCPoint(0, 0)
#define kCAViewRectInvalid CCRect(0, 0, 0, 0)

class CC_DLL CAView
:public CAResponder
,public CCRGBAProtocol
,public CCTextureProtocol
#ifdef EMSCRIPTEN
,public CCGLBufferedNode
#endif // EMSCRIPTEN
{
    
public:

    static CAView * create(void);
    
    static CAView* createWithFrame(const CCRect& rect);
    
    static CAView* createWithFrame(const CCRect& rect, const CAColor4B& color4B);
    
    static CAView* createWithCenter(const CCRect& rect);
    
    static CAView* createWithCenter(const CCRect& rect, const CAColor4B& color4B);
    
    static CAView* createWithColor(const CAColor4B& color4B);
    
    CAView();
    
    virtual ~CAView();

    virtual bool init();

    virtual bool initWithFrame(const CCRect& rect);
    
    virtual bool initWithFrame(const CCRect& rect, const CAColor4B& color4B);
    
    virtual bool initWithCenter(const CCRect& rect);
    
    virtual bool initWithCenter(const CCRect& rect, const CAColor4B& color4B);
    
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

    void setAnchorPoint(const CCPoint& anchorPoint);

    const CCPoint& getAnchorPoint();

    void setAnchorPointInPoints(const CCPoint& anchorPointInPoints);
    
    const CCPoint& getAnchorPointInPoints();
    
    virtual void setFrame(const CCRect& rect);
    
    virtual const CCRect& getFrame() const;
    
    virtual void setFrameOrigin(const CCPoint& point);
    
    virtual const CCPoint& getFrameOrigin();
    
    virtual void setBounds(const CCRect& rect);
    
    virtual CCRect getBounds() const;
    
    virtual void setCenter(CCRect rect);
    
    virtual CCRect getCenter();
    
    virtual void setCenterOrigin(const CCPoint& point);
    
    virtual CCPoint getCenterOrigin();

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

    CAView * getSubviewByTag(int tag);

    virtual CCArray* getSubviews();

    unsigned int getSubviewsCount(void) const;

    virtual void removeFromSuperview();

    virtual void removeSubview(CAView* subview);

    virtual void removeSubviewByTag(int tag);

    virtual void removeAllSubviews();

    virtual void reorderSubview(CAView * child, int zOrder);

    virtual void sortAllSubviews();

    virtual bool isRunning();

    virtual void onEnter();

    virtual void onEnterTransitionDidFinish();

    virtual void onExit();

    virtual void onExitTransitionDidStart();

    virtual void cleanup(void);
    
    /// @{
    /// @name Grid object for effects
    
    /**
     * Returns a grid object that is used when applying effects
     *
     * @return A CCGrid object that is used when applying effects
     * @js NA
     */
    virtual CCGridBase* getGrid();
    /**
     * Changes a grid object that is used when applying effects
     *
     * @param A CCGrid object that is used when applying effects
     */
    virtual void setGrid(CCGridBase *pGrid);
    
    /// @} end of Grid

    /**
     * Returns a custom user data pointer
     *
     * You can set everything in UserData pointer, a data block, a structure or an object.
     *
     * @return A custom user data pointer
     * @js NA
     */
    virtual void* getUserData();
    /**
     * Sets a custom user data pointer
     *
     * You can set everything in UserData pointer, a data block, a structure or an object, etc.
     * @warning Don't forget to release the memroy manually,
     *          especially before you change this data pointer, and before this node is autoreleased.
     *
     * @return A custom user data pointer
     * @js NA
     */
    virtual void setUserData(void *pUserData);
    
    /**
     * Returns a user assigned CCObject
     *
     * Similar to userData, but instead of holding a void* it holds an object
     *
     * @return A user assigned CCObject
     * @js NA
     */
    virtual CCObject* getUserObject();
    /**
     * Returns a user assigned CCObject
     *
     * Similar to UserData, but instead of holding a void* it holds an object.
     * The UserObject will be retained once in this method,
     * and the previous UserObject (if existed) will be relese.
     * The UserObject will be released in CAView's destructure.
     *
     * @param A user assigned CCObject
     */
    virtual void setUserObject(CCObject *pUserObject);
    /**
     * Returns a camera object that lets you move the node using a gluLookAt
     *
     * @code
     * CCCamera* camera = node->getCamera();
     * camera->setEyeXYZ(0, 0, 415/2);
     * camera->setCenterXYZ(0, 0, 0);
     * @endcode
     *
     * @return A CCCamera object that lets you move the node using a gluLookAt
     */
    virtual CCCamera* getCamera();
    /**
     * Override this method to draw your own node.
     * The following GL states will be enabled by default:
     * - glEnableClientState(GL_VERTEX_ARRAY);
     * - glEnableClientState(GL_COLOR_ARRAY);
     * - glEnableClientState(GL_TEXTURE_COORD_ARRAY);
     * - glEnable(GL_TEXTURE_2D);
     * AND YOU SHOULD NOT DISABLE THEM AFTER DRAWING YOUR NODE
     * But if you enable any other GL state, you should disable it after drawing your node.
     */
    virtual void draw(void);

    virtual void visit(void);
    
    virtual void rendering();
    
public:
    
    /// @{
    /// @name Actions
    
    /**
     * Sets the CCActionManager object that is used by all actions.
     *
     * @warning If you set a new CCActionManager, then previously created actions will be removed.
     *
     * @param actionManager     A CCActionManager object that is used by all actions.
     */
    virtual void setActionManager(CCActionManager* actionManager);
    /**
     * Gets the CCActionManager object that is used by all actions.
     * @see setActionManager(CCActionManager*)
     * @return A CCActionManager object.
     */
    virtual CCActionManager* getActionManager();
    
    /**
     * Executes an action, and returns the action that is executed.
     *
     * This node becomes the action's target. Refer to CCAction::getTarget()
     * @warning Actions don't retain their target.
     *
     * @return An Action pointer
     */
    CCAction* runAction(CCAction* action);
    
    /**
     * Stops and removes all actions from the running action list .
     */
    void stopAllActions(void);
    
    /**
     * Stops and removes an action from the running action list.
     *
     * @param An action object to be removed.
     */
    void stopAction(CCAction* action);
    
    /**
     * Removes an action from the running action list by its tag.
     *
     * @param A tag that indicates the action to be removed.
     */
    void stopActionByTag(int tag);
    
    /**
     * Gets an action from the running action list by its tag.
     *
     * @see setTag(int), getTag().
     *
     * @return The action object with the given tag.
     */
    CCAction* getActionByTag(int tag);
    
    /**
     * Returns the numbers of actions that are running plus the ones that are schedule to run (actions in actionsToAdd and actions arrays).
     *
     * Composable actions are counted as 1 action. Example:
     *    If you are running 1 Sequence of 7 actions, it will return 1.
     *    If you are running 7 Sequences of 2 actions, it will return 7.
     * @todo Rename to getNumberOfRunningActions()
     *
     * @return The number of actions that are running plus the ones that are schedule to run
     */
    unsigned int numberOfRunningActions(void);
    
public:
    
     void transform(void);
    /**
     * Performs OpenGL view-matrix transformation of it's ancestors.
     * Generally the ancestors are already transformed, but in certain cases (eg: attaching a FBO)
     * It's necessary to transform the ancestors again.
     */
    void transformAncestors(void);
    /**
     * Calls children's updateTransform() method recursively.
     *
     * This method is moved from CAImageView, so it's no longer specific to CCSprite.
     * As the result, you apply CCSpriteBatchNode's optimization on your customed CAView.
     * e.g., batchNode->addChild(myCustomNode), while you can only addChild(sprite) before.
     */
    virtual void updateTransform(void);
    
    /**
     * Returns the matrix that transform the node's (local) space coordinates into the parent's space coordinates.
     * The matrix is in Pixels.
     */
    virtual CCAffineTransform nodeToParentTransform(void);
    
    /**
     * Returns the matrix that transform parent's space coordinates to the node's (local) space coordinates.
     * The matrix is in Pixels.
     */
    virtual CCAffineTransform parentToNodeTransform(void);
    
    /**
     * Returns the world affine transform matrix. The matrix is in Pixels.
     */
    virtual CCAffineTransform nodeToWorldTransform(void);
    
    /**
     * Returns the inverse world affine transform matrix. The matrix is in Pixels.
     */
    virtual CCAffineTransform worldToNodeTransform(void);
    
    /// @} end of Transformations
    
    
    /// @{
    /// @name Coordinate Converters
    
    /**
     * Converts a Point to node (local) space coordinates. The result is in Points.
     */
    CCRect convertRectToNodeSpace(const CCRect& worldRect);
    
    /**
     * Converts a Point to world space coordinates. The result is in Points.
     */
    CCRect convertRectToWorldSpace(const CCRect& nodeRect);
    
    /**
     * Converts a Point to node (local) space coordinates. The result is in Points.
     */
    CCPoint convertToNodeSpace(const CCPoint& worldPoint);
    
    /**
     * Converts a Point to world space coordinates. The result is in Points.
     */
    CCPoint convertToWorldSpace(const CCPoint& nodePoint);
    
    /**
     * Converts a Point to node (local) space coordinates. The result is in Points.
     * treating the returned/received node point as anchor relative.
     */
    CCPoint convertToNodeSpaceAR(const CCPoint& worldPoint);
    
    /**
     * Converts a local Point to world space coordinates.The result is in Points.
     * treating the returned/received node point as anchor relative.
     */
    CCPoint convertToWorldSpaceAR(const CCPoint& nodePoint);
    
    /**
     * convenience methods which take a CCTouch instead of CCPoint
     */
    CCPoint convertTouchToNodeSpace(CCTouch * touch);
    
    /**
     * converts a CCTouch (world coordinates) into a local coordinate. This method is AR (Anchor Relative).
     */
    CCPoint convertTouchToNodeSpaceAR(CCTouch * touch);
    
    virtual void setOrderOfArrival(unsigned int uOrderOfArrival);
    
    virtual unsigned int getOrderOfArrival();
    
    /**
     * Sets the state of OpenGL server side.
     *
     * @param glServerState     The state of OpenGL server side.
     * @js NA
     */
    virtual void setGLServerState(ccGLServerState glServerState);
    /**
     * Returns the state of OpenGL server side.
     *
     * @return The state of OpenGL server side.
     * @js NA
     */
    virtual ccGLServerState getGLServerState();
    /**
     * Sets the shader program for this node
     *
     * Since v2.0, each rendering node must set its shader program.
     * It should be set in initialize phase.
     * @code
     * node->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
     * @endcode
     *
     * @param The shader program which fetchs from CCShaderCache.
     */
    virtual void setShaderProgram(CCGLProgram *pShaderProgram);
    /// @{
    /// @name Shader Program
    /**
     * Return the shader program currently used for this node
     *
     * @return The shader program currelty used for this node
     */
    virtual CCGLProgram* getShaderProgram();
    
    void setAdditionalTransform(const CCAffineTransform& additionalTransform);
    
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
    
    CCComponent* getComponent(const char *pName) const;

    virtual bool addComponent(CCComponent *pComponent);

    virtual bool removeComponent(const char *pName);

    virtual void removeAllComponents();
    
    virtual void update(float fDelta);
    
    void reViewlayout();
    
    virtual void setImage(CAImage* image);
    
    virtual CAImage* getImage(void);
    
    inline void setBlendFunc(ccBlendFunc blendFunc) { m_sBlendFunc = blendFunc; }
    
    inline ccBlendFunc getBlendFunc(void) { return m_sBlendFunc; }
    
    virtual void setImageRect(const CCRect& rect);
    
    virtual void setImageRect(const CCRect& rect, bool rotated, const CCSize& untrimmedSize);
    
    virtual void setVertexRect(const CCRect& rect);
    
    inline virtual bool isDirty(void) { return m_bDirty; }
    
    inline virtual void setDirty(bool bDirty) { m_bDirty = bDirty; }
    
    inline ccV3F_C4B_T2F_Quad getQuad(void) { return m_sQuad; }
    
    inline bool isImageRectRotated(void) { return m_bRectRotated; }
    
    inline unsigned int getAtlasIndex(void) { return m_uAtlasIndex; }
    
    inline void setAtlasIndex(unsigned int uAtlasIndex) { m_uAtlasIndex = uAtlasIndex; }
    
    inline const CCRect& getImageRect(void) { return m_obRect; }
    
    bool isFlipX(void);
    
    void setFlipX(bool bFlipX);
    
    bool isFlipY(void);
    
    void setFlipY(bool bFlipY);
    
    void setBatch(CABatchView* batchView);
    
public:
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:

    void childrenAlloc(void);

    void detachSubview(CAView *subview);

    void updateDraw();

    void updateColor(void);
    
    void setPoint(const CCPoint &point);
    
    virtual void setContentSize(const CCSize& contentSize);
    
    virtual void setImageCoords(CCRect rect);
    
    virtual void setReorderChildDirtyRecursively(void);
    
    virtual void setDirtyRecursively(bool bValue);

    virtual void updateBlendFunc(void);
    
    virtual void updateImageRect();
    
protected:
    
    CC_PROPERTY(CAView*, m_pSuperview, Superview);
    
    CC_SYNTHESIZE(CAViewDelegate*, m_pViewDelegate, ViewDelegate);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bFrame, Frame);
    
    CC_SYNTHESIZE_READONLY(CABatchView*, m_pobBatchView, Batch);
    
    CC_SYNTHESIZE(CAImageAtlas*, m_pobImageAtlas, ImageAtlas);

    float m_fRotationX;                 ///< rotation angle on x-axis
    float m_fRotationY;                 ///< rotation angle on y-axis
    
    float m_fScaleX;                    ///< scaling factor on x-axis
    float m_fScaleY;                    ///< scaling factor on y-axis
    
    float m_fVertexZ;                   ///< OpenGL real Z vertex
    
    CCPoint m_obPoint;               ///< position of the node
    
    float m_fSkewX;                     ///< skew angle on x-axis
    float m_fSkewY;                     ///< skew angle on y-axis
    
    CCPoint m_obAnchorPointInPoints;    ///< anchor point in points
    CCPoint m_obAnchorPoint;            ///< anchor point normalized (NOT in points)
    
    CCSize m_obContentSize;             ///< untransformed size of the node
    
    CCRect m_obFrameRect;
    
    CCAffineTransform m_sAdditionalTransform; ///< transform
    CCAffineTransform m_sTransform;     ///< transform
    CCAffineTransform m_sInverse;       ///< transform
    
    CCCamera *m_pCamera;                ///< a camera
    
    CCGridBase *m_pGrid;                ///< a grid
    
    int m_nZOrder;                      ///< z-order value that affects the draw order
    
    CCArray *m_pSubviews;               ///< array of children nodes              ///< weak reference to parent node
    
    void *m_pUserData;                  ///< A user assingned void pointer, Can be point to any cpp object
    CCObject *m_pUserObject;            ///< A user assigned CCObject
    
    CCGLProgram *m_pShaderProgram;      ///< OpenGL shader
    
    ccGLServerState m_eGLServerState;   ///< OpenGL servier side state
    
    unsigned int m_uOrderOfArrival;     ///< used to preserve sequence while sorting children with the same zOrder
    
    CCActionManager *m_pActionManager;  ///< a pointer to ActionManager singleton, which is used to handle all the actions
    
    bool m_bRunning;                    ///< is running
    
    bool m_bTransformDirty;             ///< transform dirty flag
    bool m_bInverseDirty;               ///< transform dirty flag
    bool m_bAdditionalTransformDirty;   ///< The flag to check whether the additional transform is dirty
    bool m_bVisible;                    ///< is this node visible

    bool m_bReorderChildDirty;          ///< children order dirty flag
    
    
    CCComponentContainer *m_pComponentContainer;        ///< Dictionary of components
    
    float		_displayedAlpha;
    float     _realAlpha;
	CAColor4B	_displayedColor;
    CAColor4B   _realColor;
    
    bool m_bDisplayRange;

    unsigned int        m_uAtlasIndex;          /// Absolute (real) Index on the SpriteSheet
    
    bool                m_bDirty;               /// Whether the sprite needs to be updated
    bool                m_bRecursiveDirty;      /// Whether all of the sprite's children needs to be
    
    bool                m_bHasChildren;         /// Whether the sprite contains children
    bool                m_bShouldBeHidden;      /// should not be drawn because one of the ancestors
    
    CCAffineTransform   m_transformToBatch;

    CCRect m_obRect;                            /// Retangle of CAImage
    bool   m_bRectRotated;                      /// Whether the Image is rotated
    

    CCPoint m_obUnflippedOffsetPositionFromCenter;
    
    // vertex coords, Image coords and color info
    ccV3F_C4B_T2F_Quad m_sQuad;
    
    // image is flipped
    bool m_bFlipX;                              /// Whether the sprite is flipped horizaontally or not.
    bool m_bFlipY;                              /// Whether the sprite is flipped vertically or not.
    
    ccBlendFunc        m_sBlendFunc;            /// It's required for CCTextureProtocol inheritance
    
    CAImage*       m_pobImage;            /// CAImage object that is used to render the sprite
};

class CAViewDelegate
{
public:
    
    virtual void getSuperViewRect(const CCRect& rect) = 0;
    
    virtual void viewOnEnterTransitionDidFinish() = 0;
    
    virtual void viewOnExitTransitionDidStart() = 0;
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){ return false; };
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){};
    
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){};
};

NS_CC_END


#endif
