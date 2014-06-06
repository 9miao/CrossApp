//
//  CAView.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-5-12.
//  Copyright (c) 2014年 厦门雅基软件有限公司. All rights reserved.
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

struct transformValues_;


enum {
    kCAViewOnEnter,
    kCAViewOnExit,
    kCAViewOnEnterTransitionDidFinish,
    kCAViewOnExitTransitionDidStart,
    kCAViewOnCleanup
};

#define kCAViewTagInvalid -1
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

    CAView();
    
    virtual ~CAView();
    
    /**
     *  Initializes the instance of CAView
     *  @return Whether the initialization was successful.
     */
    virtual bool init();
	/**
     * Allocates and initializes a node.
     * @return A initialized node which is marked as "autorelease".
     */
    
    virtual bool initWithFrame(const CCRect& rect);
    
    virtual bool initWithFrame(const CCRect& rect, const ccColor4B& color4B);
    
    virtual bool initWithCenter(const CCRect& rect);
    
    virtual bool initWithCenter(const CCRect& rect, const ccColor4B& color4B);
    
    virtual bool initWithColor(const ccColor4B& color4B);
    
    static CAView * create(void);
    
    static CAView* createWithFrame(const CCRect& rect);
    
    static CAView* createWithFrame(const CCRect& rect, const ccColor4B& color4B);
    
    static CAView* createWithCenter(const CCRect& rect);
    
    static CAView* createWithCenter(const CCRect& rect, const ccColor4B& color4B);
    
    static CAView* createWithColor(const ccColor4B& color4B);
    
    /**
     * Gets the description string. It makes debugging easier.
     * @return A string terminated with '\0'
     * @js NA
     */
    const char* description(void);
    
    /// @} end of initializers
    
    
    
    /// @{
    /// @name Setters & Getters for Graphic Peroperties
    
    /**
     * Sets the Z order which stands for the drawing order, and reorder this node in its parent's children array.
     *
     * The Z order of node is relative to its "brothers": children of the same parent.
     * It's nothing to do with OpenGL's z vertex. This one only affects the draw order of nodes in cocos2d.
     * The larger number it is, the later this node will be drawn in each message loop.
     * Please refer to setVertexZ(float) for the difference.
     *
     * @param nZOrder   Z order of this node.
     */
    virtual void setZOrder(int zOrder);
    /**
     * Sets the z order which stands for the drawing order
     *
     * This is an internal method. Don't call it outside the framework.
     * The difference between setZOrder(int) and _setOrder(int) is:
     * - _setZOrder(int) is a pure setter for m_nZOrder memeber variable
     * - setZOrder(int) firstly changes m_nZOrder, then recorder this node in its parent's chilren array.
     */
    virtual void _setZOrder(int z);
    /**
     * Gets the Z order of this node.
     *
     * @see setZOrder(int)
     *
     * @return The Z order.
     */
    virtual int getZOrder();
    
    
    /**
     * Sets the real OpenGL Z vertex.
     *
     * Differences between openGL Z vertex and cocos2d Z order:
     * - OpenGL Z modifies the Z vertex, and not the Z order in the relation between parent-children
     * - OpenGL Z might require to set 2D projection
     * - cocos2d Z order works OK if all the nodes uses the same openGL Z vertex. eg: vertexZ = 0
     *
     * @warning Use it at your own risk since it might break the cocos2d parent-children z order
     *
     * @param fVertexZ  OpenGL Z vertex of this node.
     */
    virtual void setVertexZ(float vertexZ);
    /**
     * Gets OpenGL Z vertex of this node.
     *
     * @see setVertexZ(float)
     *
     * @return OpenGL Z vertex of this node
     */
    virtual float getVertexZ();
    
    
    /**
     * Changes the scale factor on X axis of this node
     *
     * The deafult value is 1.0 if you haven't changed it before
     *
     * @param fScaleX   The scale factor on X axis.
     */
    virtual void setScaleX(float fScaleX);
    /**
     * Returns the scale factor on X axis of this node
     *
     * @see setScaleX(float)
     *
     * @return The scale factor on X axis.
     */
    virtual float getScaleX();
    
    
    /**
     * Changes the scale factor on Y axis of this node
     *
     * The Default value is 1.0 if you haven't changed it before.
     *
     * @param fScaleY   The scale factor on Y axis.
     */
    virtual void setScaleY(float fScaleY);
    /**
     * Returns the scale factor on Y axis of this node
     *
     * @see setScaleY(float)
     *
     * @return The scale factor on Y axis.
     */
    virtual float getScaleY();
    
    
    /**
     * Changes both X and Y scale factor of the node.
     *
     * 1.0 is the default scale factor. It modifies the X and Y scale at the same time.
     *
     * @param scale     The scale factor for both X and Y axis.
     */
    virtual void setScale(float scale);
    /**
     * Gets the scale factor of the node,  when X and Y have the same scale factor.
     *
     * @warning Assert when m_fScaleX != m_fScaleY.
     * @see setScale(float)
     *
     * @return The scale factor of the node.
     */
    virtual float getScale();
    
    
    /**
     * Changes both X and Y scale factor of the node.
     *
     * 1.0 is the default scale factor. It modifies the X and Y scale at the same time.
     *
     * @param fScaleX     The scale factor on X axis.
     * @param fScaleY     The scale factor on Y axis.
     */
    virtual void setScale(float fScaleX,float fScaleY);
    
    
    /**
     * Changes the position (x,y) of the node in OpenGL coordinates
     *
     * Usually we use ccp(x,y) to compose CCPoint object.
     * The original point (0,0) is at the left-bottom corner of screen.
     * For example, this codesnip sets the node in the center of screen.
     * @code
     * CCSize size = CCDirector::sharedDirector()->getWinSize();
     * node->setPosition( ccp(size.width/2, size.height/2) )
     * @endcode
     *
     * @param position  The position (x,y) of the node in OpenGL coordinates
     * @js NA
     */
    virtual void setPosition(const CCPoint &position);
    /**
     * Gets the position (x,y) of the node in OpenGL coordinates
     *
     * @see setPosition(const CCPoint&)
     *
     * @return The position (x,y) of the node in OpenGL coordinates
     */
    virtual const CCPoint& getPosition();

    /**
     * Gets position in a more efficient way, returns two number instead of a CCPoint object
     *
     * @see setPosition(float, float)
     */
    virtual void getPosition(float* x, float* y);

    /**
     * Changes the X skew angle of the node in degrees.
     *
     * This angle describes the shear distortion in the X direction.
     * Thus, it is the angle between the Y axis and the left edge of the shape
     * The default skewX angle is 0. Positive values distort the node in a CW direction.
     *
     * @param fSkewX The X skew angle of the node in degrees.
     */
    virtual void setSkewX(float fSkewX);
    /**
     * Returns the X skew angle of the node in degrees.
     *
     * @see setSkewX(float)
     *
     * @return The X skew angle of the node in degrees.
     */
    virtual float getSkewX();
    
    
    /**
     * Changes the Y skew angle of the node in degrees.
     *
     * This angle describes the shear distortion in the Y direction.
     * Thus, it is the angle between the X axis and the bottom edge of the shape
     * The default skewY angle is 0. Positive values distort the node in a CCW direction.
     *
     * @param fSkewY    The Y skew angle of the node in degrees.
     */
    virtual void setSkewY(float fSkewY);
    /**
     * Returns the Y skew angle of the node in degrees.
     *
     * @see setSkewY(float)
     *
     * @return The Y skew angle of the node in degrees.
     */
    virtual float getSkewY();
    
    
    /**
     * Sets the anchor point in percent.
     *
     * anchorPoint is the point around which all transformations and positioning manipulations take place.
     * It's like a pin in the node where it is "attached" to its parent.
     * The anchorPoint is normalized, like a percentage. (0,0) means the bottom-left corner and (1,1) means the top-right corner.
     * But you can use values higher than (1,1) and lower than (0,0) too.
     * The default anchorPoint is (0.5,0.5), so it starts in the center of the node.
     *
     * @param anchorPoint   The anchor point of node.
     */
    void setAnchorPoint(const CCPoint& anchorPoint);
    /**
     * Returns the anchor point in percent.
     *
     * @see setAnchorPoint(const CCPoint&)
     *
     * @return The anchor point of node.
     */
    const CCPoint& getAnchorPoint();
    /**
     * Returns the anchorPoint in absolute pixels.
     *
     * @warning You can only read it. If you wish to modify it, use anchorPoint instead.
     * @see getAnchorPoint()
     *
     * @return The anchor point in absolute pixels.
     */
    
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

    /**
     * Sets whether the node is visible
     *
     * The default value is true, a node is default to visible
     *
     * @param visible   true if the node is visible, false if the node is hidden.
     */
    virtual void setVisible(bool visible);
    /**
     * Determines if the node is visible
     *
     * @see setVisible(bool)
     *
     * @return true if the node is visible, false if the node is hidden.
     */
    virtual bool isVisible();
    
    
    /**
     * Sets the rotation (angle) of the node in degrees.
     *
     * 0 is the default rotation angle.
     * Positive values rotate node clockwise, and negative values for anti-clockwise.
     *
     * @param fRotation     The roration of the node in degrees.
     */
    virtual void setRotation(float fRotation);
    /**
     * Returns the rotation of the node in degrees.
     *
     * @see setRotation(float)
     *
     * @return The rotation of the node in degrees.
     */
    virtual float getRotation();
    
    
    /**
     * Sets the X rotation (angle) of the node in degrees which performs a horizontal rotational skew.
     *
     * 0 is the default rotation angle.
     * Positive values rotate node clockwise, and negative values for anti-clockwise.
     *
     * @param fRotationX    The X rotation in degrees which performs a horizontal rotational skew.
     */
    virtual void setRotationX(float fRotaionX);
    /**
     * Gets the X rotation (angle) of the node in degrees which performs a horizontal rotation skew.
     *
     * @see setRotationX(float)
     *
     * @return The X rotation in degrees.
     */
    virtual float getRotationX();
    
    
    /**
     * Sets the Y rotation (angle) of the node in degrees which performs a vertical rotational skew.
     *
     * 0 is the default rotation angle.
     * Positive values rotate node clockwise, and negative values for anti-clockwise.
     *
     * @param fRotationY    The Y rotation in degrees.
     */
    virtual void setRotationY(float fRotationY);
    /**
     * Gets the Y rotation (angle) of the node in degrees which performs a vertical rotational skew.
     *
     * @see setRotationY(float)
     *
     * @return The Y rotation in degrees.
     */
    virtual float getRotationY();
    
    
    /**
     * Sets the arrival order when this node has a same ZOrder with other children.
     *
     * A node which called addChild subsequently will take a larger arrival order,
     * If two children have the same Z order, the child with larger arrival order will be drawn later.
     *
     * @warning This method is used internally for zOrder sorting, don't change this manually
     *
     * @param uOrderOfArrival   The arrival order.
     */
    virtual void setOrderOfArrival(unsigned int uOrderOfArrival);
    /**
     * Returns the arrival order, indecates which children is added previously.
     *
     * @see setOrderOfArrival(unsigned int)
     *
     * @return The arrival order.
     */
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
     * Sets whether the anchor point will be (0,0) when you position this node.
     *
     * This is an internal method, only used by CCLayer and CCScene. Don't call it outside framework.
     * The default value is false, while in CCLayer and CCScene are true
     *
     * @param ignore    true if anchor point will be (0,0) when you position this node
     * @todo This method shoud be renamed as setIgnoreAnchorPointForPosition(bool) or something with "set"
     */
    virtual void ignoreAnchorPointForPosition(bool ignore);
    /**
     * Gets whether the anchor point will be (0,0) when you position this node.
     *
     * @see ignoreAnchorPointForPosition(bool)
     *
     * @return true if the anchor point will be (0,0) when you position this node.
     */
    virtual bool isIgnoreAnchorPointForPosition();
    
    /// @}  end of Setters & Getters for Graphic Peroperties
    
    
    /// @{
    /// @name Children and Parent
    
    /**
     * Adds a child to the container with z-order as 0.
     *
     * If the child is added to a 'running' node, then 'onEnter' and 'onEnterTransitionDidFinish' will be called immediately.
     *
     * @param child A child node
     */
    virtual void addSubview(CAView * child);
    
    /// helper that reorder a child
    virtual void insertSubview(CAView* subview, int z);
    
    /*
     * Gets a child from the container with its tag
     *
     * @param tag   An identifier to find the child node.
     *
     * @return a CAView object whose tag equals to the input parameter
     */
    
    CAView * getSubviewByTag(int tag);
    /**
     * Return an array of children
     *
     * Composing a "tree" structure is a very important feature of CAView
     * Here's a sample code of traversing children array:
     * @code
     * CAView* node = NULL;
     * CCARRAY_FOREACH(parent->getChildren(), node)
     * {
     *     node->setPosition(0,0);
     * }
     * @endcode
     * This sample code traverses all children nodes, and set theie position to (0,0)
     *
     * @return An array of children
     */
    virtual CCArray* getSubviews();
    
    /**
     * Get the amount of children.
     *
     * @return The amount of children.
     */
    unsigned int getSubviewsCount(void) const;
    
    /**
     * Sets the parent node
     *
     * @param parent    A pointer to the parnet node
     */
    
    CC_PROPERTY(CAView*, m_pSuperview, Superview);
    
    
    virtual void removeFromSuperview();
    
    /**
     * Removes a child from the container with a cleanup
     *
     * @see removeChild(CAView, bool)
     *
     * @param child     The child node which will be removed.
     */
    virtual void removeSubview(CAView* subview);
    /**
     * Removes a child from the container by tag value with a cleanup.
     *
     * @see removeChildByTag(int, bool)
     *
     * @param tag       An interger number that identifies a child node
     */
    virtual void removeSubviewByTag(int tag);
    /**
     * Removes all children from the container with a cleanup.
     *
     * @see removeAllChildrenWithCleanup(bool)
     */
    virtual void removeAllSubviews();
    
    /**
     * Reorders a child according to a new z value.
     *
     * @param child     An already added child node. It MUST be already added.
     * @param zOrder    Z order for drawing priority. Please refer to setZOrder(int)
     */
    virtual void reorderSubview(CAView * child, int zOrder);
    
    /**
     * Sorts the children array once before drawing, instead of every time when a child is added or reordered.
     * This appraoch can improves the performance massively.
     * @note Don't call this manually unless a child added needs to be removed in the same frame
     */
    virtual void sortAllSubviews();
    
    /// @} end of Children and Parent
    
    
    
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
    
    
    /// @{
    /// @name Tag & User data
    
    /**
     * Returns a tag that is used to identify the node easily.
     *
     * You can set tags to node then identify them easily.
     * @code
     * #define TAG_PLAYER  1
     * #define TAG_MONSTER 2
     * #define TAG_BOSS    3
     * // set tags
     * node1->setTag(TAG_PLAYER);
     * node2->setTag(TAG_MONSTER);
     * node3->setTag(TAG_BOSS);
     * parent->addChild(node1);
     * parent->addChild(node2);
     * parent->addChild(node3);
     * // identify by tags
     * CAView* node = NULL;
     * CCARRAY_FOREACH(parent->getChildren(), node)
     * {
     *     switch(node->getTag())
     *     {
     *         case TAG_PLAYER:
     *             break;
     *         case TAG_MONSTER:
     *             break;
     *         case TAG_BOSS:
     *             break;
     *     }
     * }
     * @endcode
     *
     * @return A interger that identifies the node.
     */
    virtual int getTag() const;
    /**
     * Changes the tag that is used to identify the node easily.
     *
     * Please refer to getTag for the sample code.
     *
     * @param A interger that indentifies the node.
     */
    virtual void setTag(int nTag);
    
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
    
    /// @} end of Tag & User Data
    
    
    /// @{
    /// @name Shader Program
    /**
     * Return the shader program currently used for this node
     *
     * @return The shader program currelty used for this node
     */
    virtual CCGLProgram* getShaderProgram();
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
    /// @} end of Shader Program
    
    
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
     * Returns whether or not the node accepts event callbacks.
     *
     * Running means the node accept event callbacks like onEnter(), onExit(), update()
     *
     * @return Whether or not the node is running.
     */
    virtual bool isRunning();
    
    /// @}  end Script Bindings
    
    
    /// @{
    /// @name Event Callbacks
    
    /**
     * Event callback that is invoked every time when CAView enters the 'stage'.
     * If the CAView enters the 'stage' with a transition, this event is called when the transition starts.
     * During onEnter you can't access a "sister/brother" node.
     * If you override onEnter, you shall call its parent's one, e.g., CAView::onEnter().
     * @js NA
     * @lua NA
     */
    virtual void onEnter();
    
    /** Event callback that is invoked when the CAView enters in the 'stage'.
     * If the CAView enters the 'stage' with a transition, this event is called when the transition finishes.
     * If you override onEnterTransitionDidFinish, you shall call its parent's one, e.g. CAView::onEnterTransitionDidFinish()
     * @js NA
     * @lua NA
     */
    virtual void onEnterTransitionDidFinish();
    
    /**
     * Event callback that is invoked every time the CAView leaves the 'stage'.
     * If the CAView leaves the 'stage' with a transition, this event is called when the transition finishes.
     * During onExit you can't access a sibling node.
     * If you override onExit, you shall call its parent's one, e.g., CAView::onExit().
     * @js NA
     * @lua NA
     */
    virtual void onExit();
    
    /**
     * Event callback that is called every time the CAView leaves the 'stage'.
     * If the CAView leaves the 'stage' with a transition, this callback is called when the transition starts.
     * @js NA
     * @lua NA
     */
    virtual void onExitTransitionDidStart();
    
    /// @} end of event callbacks.
    
    
    /**
     * Stops all running actions and schedulers
     */
    virtual void cleanup(void);
    
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
    
    /**
     * Visits this node's children and draw them recursively.
     */
    virtual void visit(void);
    
    virtual void rendering();
    
    /**
     * Returns a "local" axis aligned bounding box of the node.
     * The returned box is relative only to its parent.
     *
     * @note This method returns a temporaty variable, so it can't returns const CCRect&
     * @todo Rename to getBoundingBox() in the future versions.
     *
     * @return A "local" axis aligned boudning box of the node.
     * @js getBoundingBox
     */
    CCRect boundingBox(void);
    
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
    
    virtual bool isDisplayRange();
    
    virtual void setDisplayRange(bool value);

    virtual GLubyte getOpacity();
    
    virtual GLubyte getDisplayedOpacity();
    
    virtual void setOpacity(GLubyte opacity);
    
    virtual void updateDisplayedOpacity(GLubyte parentOpacity);
    
    virtual bool isCascadeOpacityEnabled();
    
    virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled);
    
    virtual const ccColor3B& getColor(void);
    
    virtual const ccColor3B& getDisplayedColor();
    
    virtual void setColor(const ccColor3B& color);
    
    virtual void updateDisplayedColor(const ccColor3B& parentColor);
    
    virtual bool isCascadeColorEnabled();
    
    virtual void setCascadeColorEnabled(bool cascadeColorEnabled);
    
    virtual void setOpacityModifyRGB(bool bValue);
    
    virtual bool isOpacityModifyRGB();
    
	/**
     *  Sets the additional transform.
     *
     *  @note The additional transform will be concatenated at the end of nodeToParentTransform.
     *        It could be used to simulate `parent-child` relationship between two nodes (e.g. one is in BatchNode, another isn't).
     *  @code
     // create a batchNode
     CCSpriteBatchNode* batch= CCSpriteBatchNode::create("Icon-114.png");
     this->addChild(batch);
     
     // create two sprites, spriteA will be added to batchNode, they are using different textures.
     CAImageView* spriteA = CAImageView::createWithImage(batch->getImage());
     CAImageView* spriteB = CAImageView::create("Icon-72.png");
     
     batch->addChild(spriteA);
     
     // We can't make spriteB as spriteA's child since they use different textures. So just add it to layer.
     // But we want to simulate `parent-child` relationship for these two node.
     this->addChild(spriteB);
     
     //position
     spriteA->setPosition(ccp(200, 200));
     
     // Gets the spriteA's transform.
     CCAffineTransform t = spriteA->nodeToParentTransform();
     
     // Sets the additional transform to spriteB, spriteB's postion will based on its pseudo parent i.e. spriteA.
     spriteB->setAdditionalTransform(t);
     
     //scale
     spriteA->setScale(2);
     
     // Gets the spriteA's transform.
     t = spriteA->nodeToParentTransform();
     
     // Sets the additional transform to spriteB, spriteB's scale will based on its pseudo parent i.e. spriteA.
     spriteB->setAdditionalTransform(t);
     
     //rotation
     spriteA->setRotation(20);
     
     // Gets the spriteA's transform.
     t = spriteA->nodeToParentTransform();
     
     // Sets the additional transform to spriteB, spriteB's rotation will based on its pseudo parent i.e. spriteA.
     spriteB->setAdditionalTransform(t);
     *  @endcode
     */
    void setAdditionalTransform(const CCAffineTransform& additionalTransform);

    CCComponent* getComponent(const char *pName) const;

    virtual bool addComponent(CCComponent *pComponent);

    virtual bool removeComponent(const char *pName);

    virtual void removeAllComponents();
    
    virtual void update(float fDelta);
    
    void reViewlayout();
    
    
private:
    /// lazy allocs
    void childrenAlloc(void);
    
    /// Removes a child, call child->onExit(), do cleanup, remove it from children array.
    void detachSubview(CAView *subview);
    
    /** Convert cocos2d coordinates to UI windows coordinate.
     * @js NA
     * @lua NA
     */
    CCPoint convertToWindowSpace(const CCPoint& nodePoint);
    
    void updateDraw();
    
    
protected:
    float m_fRotationX;                 ///< rotation angle on x-axis
    float m_fRotationY;                 ///< rotation angle on y-axis
    
    float m_fScaleX;                    ///< scaling factor on x-axis
    float m_fScaleY;                    ///< scaling factor on y-axis
    
    float m_fVertexZ;                   ///< OpenGL real Z vertex
    
    CCPoint m_obPosition;               ///< position of the node
    
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
    
    int m_nTag;                         ///< a tag. Can be any number you assigned just to identify this node
    
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
    
    bool m_bIgnoreAnchorPointForPosition; ///< true if the Anchor Point will be (0,0) when you position the CAView, false otherwise.
    ///< Used by CCLayer and CCScene.
    
    bool m_bReorderChildDirty;          ///< children order dirty flag
    
    
    CCComponentContainer *m_pComponentContainer;        ///< Dictionary of components
    
    GLubyte		_displayedOpacity;
    GLubyte     _realOpacity;
	ccColor3B	_displayedColor;
    ccColor3B   _realColor;
	bool		_cascadeColorEnabled;
    bool        _cascadeOpacityEnabled;
    
    bool m_bDisplayRange;
    
public:
    
    virtual void setImage(CAImage* image);
    
    virtual CAImage* getImage(void);
    
    inline void setBlendFunc(ccBlendFunc blendFunc) { m_sBlendFunc = blendFunc; }

    inline ccBlendFunc getBlendFunc(void) { return m_sBlendFunc; }
    
    /// @{
    /// @name Image methods
    
    /**
     * Updates the Image rect of the CAImageView in points.
     * It will call setImageRect:rotated:untrimmedSize with rotated = NO, and utrimmedSize = rect.size.
     */
    virtual void setImageRect(const CCRect& rect);
    
    /**
     * Sets the Image rect, rectRotated and untrimmed size of the CAImageView in points.
     * It will update the Image coordinates and the vertex rectangle.
     */
    virtual void setImageRect(const CCRect& rect, bool rotated, const CCSize& untrimmedSize);
    
    /**
     * Sets the vertex rect.
     * It will be called internally by setImageRect.
     * Useful if you want to create 2x images from SD images in Retina Display.
     * Do not call it manually. Use setImageRect instead.
     */
    virtual void setVertexRect(const CCRect& rect);
    
    /// @{
    /// @name Sprite Properties' setter/getters
    
    /**
     * Whether or not the Sprite needs to be updated in the Atlas.
     *
     * @return true if the sprite needs to be updated in the Atlas, false otherwise.
     */
    inline virtual bool isDirty(void) { return m_bDirty; }
    
    /**
     * Makes the Sprite to be updated in the Atlas.
     */
    inline virtual void setDirty(bool bDirty) { m_bDirty = bDirty; }
    
    /**
     * Returns the quad (tex coords, vertex coords and color) information.
     * @js NA
     */
    inline ccV3F_C4B_T2F_Quad getQuad(void) { return m_sQuad; }
    
    /**
     * Returns whether or not the Image rectangle is rotated.
     */
    inline bool isImageRectRotated(void) { return m_bRectRotated; }
    
    /**
     * Returns the index used on the TextureAtlas.
     */
    inline unsigned int getAtlasIndex(void) { return m_uAtlasIndex; }
    
    /**
     * Sets the index used on the TextureAtlas.
     * @warning Don't modify this value unless you know what you are doing
     */
    inline void setAtlasIndex(unsigned int uAtlasIndex) { m_uAtlasIndex = uAtlasIndex; }
    
    /**
     * Returns the rect of the CAImageView in points
     */
    inline const CCRect& getImageRect(void) { return m_obRect; }
    
    /**
     * Gets the offset position of the sprite. Calculated automatically by editors like Zwoptex.
     */
    inline const CCPoint& getOffsetPosition(void) { return m_obOffsetPosition; }
    
    
    /**
     * Returns the flag which indicates whether the sprite is flipped horizontally or not.
     *
     * It only flips the Image of the sprite, and not the Image of the sprite's children.
     * Also, flipping the Image doesn't alter the anchorPoint.
     * If you want to flip the anchorPoint too, and/or to flip the children too use:
     * sprite->setScaleX(sprite->getScaleX() * -1);
     *
     * @return true if the sprite is flipped horizaontally, false otherwise.
     * @js isFlippedX
     */
    bool isFlipX(void);
    /**
     * Sets whether the sprite should be flipped horizontally or not.
     *
     * @param bFlipX true if the sprite should be flipped horizaontally, false otherwise.
     */
    void setFlipX(bool bFlipX);
    
    /**
     * Return the flag which indicates whether the sprite is flipped vertically or not.
     *
     * It only flips the Image of the sprite, and not the Image of the sprite's children.
     * Also, flipping the Image doesn't alter the anchorPoint.
     * If you want to flip the anchorPoint too, and/or to flip the children too use:
     * sprite->setScaleY(sprite->getScaleY() * -1);
     *
     * @return true if the sprite is flipped vertically, flase otherwise.
     * @js isFlippedY
     */
    bool isFlipY(void);
    /**
     * Sets whether the sprite should be flipped vertically or not.
     *
     * @param bFlipY true if the sprite should be flipped vertically, flase otherwise.
     */
    void setFlipY(bool bFlipY);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    
    /**
     * Sets the untransformed size of the node.
     *
     * The contentSize remains the same no matter the node is scaled or rotated.
     * All nodes has a size. Layer and Scene has the same size of the screen.
     *
     * @param contentSize   The untransformed size of the node.
     */
    virtual void setContentSize(const CCSize& contentSize);
    /**
     * Returns the untransformed size of the node.
     *
     * @see setContentSize(const CCSize&)
     *
     * @return The untransformed size of the node.
     */
    virtual const CCSize& getContentSize() const;
    
    void updateColor(void);
    virtual void setImageCoords(CCRect rect);
    virtual void updateBlendFunc(void);
    virtual void setReorderChildDirtyRecursively(void);
    virtual void setDirtyRecursively(bool bValue);
    
    unsigned int        m_uAtlasIndex;          /// Absolute (real) Index on the SpriteSheet
    
    bool                m_bDirty;               /// Whether the sprite needs to be updated
    bool                m_bRecursiveDirty;      /// Whether all of the sprite's children needs to be updated
    bool                m_bHasChildren;         /// Whether the sprite contains children
    bool                m_bShouldBeHidden;      /// should not be drawn because one of the ancestors is not visible
    CCAffineTransform   m_transformToBatch;
    
    //
    // Data used when the sprite is self-rendered
    //
    
    //
    // Shared data
    //
    
    // texture
    CCRect m_obRect;                            /// Retangle of CAImage
    bool   m_bRectRotated;                      /// Whether the Image is rotated
    
    // Offset Position (used by Zwoptex)
    CCPoint m_obOffsetPosition;
    CCPoint m_obUnflippedOffsetPositionFromCenter;
    
    // vertex coords, Image coords and color info
    ccV3F_C4B_T2F_Quad m_sQuad;
    
    // opacity and RGB protocol
    bool m_bOpacityModifyRGB;
    
    // image is flipped
    bool m_bFlipX;                              /// Whether the sprite is flipped horizaontally or not.
    bool m_bFlipY;                              /// Whether the sprite is flipped vertically or not.
    
    ccBlendFunc        m_sBlendFunc;            /// It's required for CCTextureProtocol inheritance
    CAImage*       m_pobImage;            /// CAImage object that is used to render the sprite
    
    
    CC_SYNTHESIZE(CAViewDelegate*, m_pViewDelegate, ViewDelegate);
    
    CC_SYNTHESIZE_IS_READONLY(bool, m_bFrame, Frame);
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


#endif /* defined(__cocos2dx__CAView_) */
