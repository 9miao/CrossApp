#ifndef __cocos2dx_h__
#define __cocos2dx_h__

#include "jsapi.h"
#include "jsfriendapi.h"


extern JSClass  *jsb_CCAction_class;
extern JSObject *jsb_CCAction_prototype;

JSBool js_cocos2dx_CCAction_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCAction_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCAction(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCAction_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAction_setOriginalTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAction_setTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAction_getOriginalTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAction_stop(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAction_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAction_getTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAction_step(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAction_setTag(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAction_getTag(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAction_isDone(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAction_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAction_CCAction(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFiniteTimeAction_class;
extern JSObject *jsb_CCFiniteTimeAction_prototype;

JSBool js_cocos2dx_CCFiniteTimeAction_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFiniteTimeAction_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFiniteTimeAction(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFiniteTimeAction_setDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFiniteTimeAction_getDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFiniteTimeAction_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFiniteTimeAction_CCFiniteTimeAction(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCSpeed_class;
extern JSObject *jsb_CCSpeed_prototype;

JSBool js_cocos2dx_CCSpeed_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCSpeed_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCSpeed(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCSpeed_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpeed_setInnerAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpeed_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpeed_stop(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpeed_step(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpeed_setSpeed(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpeed_initWithAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpeed_getInnerAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpeed_isDone(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpeed_getSpeed(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpeed_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpeed_CCSpeed(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFollow_class;
extern JSObject *jsb_CCFollow_prototype;

JSBool js_cocos2dx_CCFollow_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFollow_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFollow(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFollow_initWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFollow_stop(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFollow_setBoudarySet(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFollow_step(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFollow_isDone(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFollow_isBoundarySet(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFollow_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFollow_CCFollow(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCGLProgram_class;
extern JSObject *jsb_CCGLProgram_prototype;

JSBool js_cocos2dx_CCGLProgram_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCGLProgram_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCGLProgram(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCGLProgram_fragmentShaderLog(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_addAttribute(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_setUniformLocationWithMatrix4fv(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_getUniformLocationForName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_use(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_vertexShaderLog(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_initWithVertexShaderByteArray(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_initWithVertexShaderFilename(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_setUniformsForBuiltins(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_setUniformLocationWith3i(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_setUniformLocationWith3iv(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_updateUniforms(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_setUniformLocationWith4iv(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_link(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_setUniformLocationWith2iv(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_reset(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_setUniformLocationWith4i(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_setUniformLocationWith1i(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_setUniformLocationWith2i(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGLProgram_CCGLProgram(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTouch_class;
extern JSObject *jsb_CCTouch_prototype;

JSBool js_cocos2dx_CCTouch_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTouch_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTouch(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTouch_getPreviousLocationInView(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTouch_getLocation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTouch_getDelta(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTouch_getStartLocationInView(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTouch_getStartLocation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTouch_getID(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTouch_setTouchInfo(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTouch_getLocationInView(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTouch_getPreviousLocation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTouch_CCTouch(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCSet_class;
extern JSObject *jsb_CCSet_prototype;

JSBool js_cocos2dx_CCSet_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCSet_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCSet(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCSet_count(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSet_addObject(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSet_mutableCopy(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSet_anyObject(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSet_removeAllObjects(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSet_removeObject(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSet_copy(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSet_containsObject(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSet_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTexture2D_class;
extern JSObject *jsb_CCTexture2D_prototype;

JSBool js_cocos2dx_CCTexture2D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTexture2D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTexture2D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTexture2D_getShaderProgram(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_initWithETCFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_stringForFormat(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_initWithImage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_setShaderProgram(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_getMaxS(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_hasPremultipliedAlpha(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_getPixelsHigh(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_bitsPerPixelForFormat(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_getName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_initWithString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_setMaxT(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_drawInRect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_getContentSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_getMaxT(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_setAliasTexParameters(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_setAntiAliasTexParameters(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_generateMipmap(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_getPixelFormat(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_getContentSizeInPixels(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_getPixelsWide(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_drawAtPoint(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_hasMipmaps(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_initWithPVRFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_setMaxS(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_setDefaultAlphaPixelFormat(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_defaultAlphaPixelFormat(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_PVRImagesHavePremultipliedAlpha(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTexture2D_CCTexture2D(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCNode_class;
extern JSObject *jsb_CCNode_prototype;

JSBool js_cocos2dx_CCNode_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCNode_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCNode(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCNode_addChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_removeComponent(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_removeAllComponents(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getShaderProgram(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getChildren(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getScriptHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_convertToWorldSpaceAR(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_isIgnoreAnchorPointForPosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setRotation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setZOrder(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setScaleY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setScaleX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_unregisterScriptHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getTag(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_nodeToWorldTransform(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_removeChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_convertToWorldSpace(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setSkewX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setSkewY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_convertTouchToNodeSpace(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_removeAllChildrenWithCleanup(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getRotationX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getRotationY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setParent(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_nodeToParentTransform(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_convertToNodeSpace(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getGrid(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setPosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_stopActionByTag(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_reorderChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setPositionY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setPositionX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getAnchorPoint(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_numberOfRunningActions(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_updateTransform(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_isVisible(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getChildrenCount(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_convertToNodeSpaceAR(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_addComponent(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_visit(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setShaderProgram(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getRotation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_resumeSchedulerAndActions(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getZOrder(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getAnchorPointInPoints(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_runAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_transform(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setVertexZ(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setScheduler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_stopAllActions(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getSkewX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getSkewY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_ignoreAnchorPointForPosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getActionByTag(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setRotationX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setRotationY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setAdditionalTransform(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getScheduler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getOrderOfArrival(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setActionManager(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getPosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_isRunning(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getParent(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getPositionY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getPositionX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_removeChildByTag(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setVisible(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_parentToNodeTransform(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_pauseSchedulerAndActions(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getVertexZ(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode__setZOrder(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setScale(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getChildByTag(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setOrderOfArrival(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getScaleY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getScaleX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_cleanup(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getComponent(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getContentSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_boundingBox(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_transformAncestors(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setUserObject(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_registerScriptHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_removeFromParentAndCleanup(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_convertTouchToNodeSpaceAR(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_sortAllChildren(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_worldToNodeTransform(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getScale(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getCamera(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_setTag(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_stopAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_getActionManager(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNode_CCNode(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCNodeRGBA_class;
extern JSObject *jsb_CCNodeRGBA_prototype;

JSBool js_cocos2dx_CCNodeRGBA_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCNodeRGBA_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCNodeRGBA(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCNodeRGBA_updateDisplayedColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_setColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_isCascadeOpacityEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_getColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_getDisplayedOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_setCascadeColorEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_setOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_setOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_setCascadeOpacityEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_updateDisplayedOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_getOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_isOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_isCascadeColorEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_getDisplayedColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCNodeRGBA_CCNodeRGBA(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCSpriteFrame_class;
extern JSObject *jsb_CCSpriteFrame_prototype;

JSBool js_cocos2dx_CCSpriteFrame_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCSpriteFrame_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCSpriteFrame(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCSpriteFrame_setRotated(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_setTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_getOffset(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_setRectInPixels(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_getTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_getRect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_setOffsetInPixels(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_getRectInPixels(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_setOriginalSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_getOriginalSizeInPixels(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_setOriginalSizeInPixels(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_setOffset(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_initWithTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_isRotated(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_initWithTextureFilename(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_setRect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_getOffsetInPixels(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_getOriginalSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrame_createWithTexture(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCAnimationFrame_class;
extern JSObject *jsb_CCAnimationFrame_prototype;

JSBool js_cocos2dx_CCAnimationFrame_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCAnimationFrame_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCAnimationFrame(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCAnimationFrame_setSpriteFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationFrame_getUserInfo(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationFrame_setDelayUnits(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationFrame_getSpriteFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationFrame_getDelayUnits(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationFrame_setUserInfo(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationFrame_initWithSpriteFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationFrame_CCAnimationFrame(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCAnimation_class;
extern JSObject *jsb_CCAnimation_prototype;

JSBool js_cocos2dx_CCAnimation_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCAnimation_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCAnimation(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCAnimation_getLoops(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_setFrames(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_getFrames(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_addSpriteFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_setRestoreOriginalFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_setDelayPerUnit(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_initWithAnimationFrames(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_initWithSpriteFrames(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_setLoops(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_addSpriteFrameWithFileName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_getTotalDelayUnits(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_getDelayPerUnit(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_getRestoreOriginalFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_getDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_addSpriteFrameWithTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimation_CCAnimation(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCActionInterval_class;
extern JSObject *jsb_CCActionInterval_prototype;

JSBool js_cocos2dx_CCActionInterval_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCActionInterval_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCActionInterval(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCActionInterval_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionInterval_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionInterval_setAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionInterval_getAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionInterval_step(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionInterval_getElapsed(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionInterval_isDone(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionInterval_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionInterval_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCSequence_class;
extern JSObject *jsb_CCSequence_prototype;

JSBool js_cocos2dx_CCSequence_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCSequence_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCSequence(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCSequence_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSequence_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSequence_stop(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSequence_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSequence_initWithTwoActions(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCRepeat_class;
extern JSObject *jsb_CCRepeat_prototype;

JSBool js_cocos2dx_CCRepeat_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCRepeat_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCRepeat(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCRepeat_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeat_setInnerAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeat_stop(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeat_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeat_initWithAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeat_getInnerAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeat_isDone(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeat_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeat_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCRepeatForever_class;
extern JSObject *jsb_CCRepeatForever_prototype;

JSBool js_cocos2dx_CCRepeatForever_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCRepeatForever_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCRepeatForever(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCRepeatForever_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeatForever_setInnerAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeatForever_step(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeatForever_initWithAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeatForever_getInnerAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeatForever_isDone(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeatForever_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeatForever_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRepeatForever_CCRepeatForever(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCSpawn_class;
extern JSObject *jsb_CCSpawn_prototype;

JSBool js_cocos2dx_CCSpawn_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCSpawn_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCSpawn(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCSpawn_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpawn_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpawn_stop(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpawn_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpawn_initWithTwoActions(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCRotateTo_class;
extern JSObject *jsb_CCRotateTo_prototype;

JSBool js_cocos2dx_CCRotateTo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCRotateTo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCRotateTo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCRotateTo_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRotateTo_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRotateTo_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRotateTo_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCRotateBy_class;
extern JSObject *jsb_CCRotateBy_prototype;

JSBool js_cocos2dx_CCRotateBy_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCRotateBy_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCRotateBy(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCRotateBy_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRotateBy_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRotateBy_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRotateBy_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRotateBy_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCMoveBy_class;
extern JSObject *jsb_CCMoveBy_prototype;

JSBool js_cocos2dx_CCMoveBy_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCMoveBy_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCMoveBy(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCMoveBy_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMoveBy_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMoveBy_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMoveBy_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMoveBy_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCMoveTo_class;
extern JSObject *jsb_CCMoveTo_prototype;

JSBool js_cocos2dx_CCMoveTo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCMoveTo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCMoveTo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCMoveTo_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMoveTo_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMoveTo_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCSkewTo_class;
extern JSObject *jsb_CCSkewTo_prototype;

JSBool js_cocos2dx_CCSkewTo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCSkewTo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCSkewTo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCSkewTo_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSkewTo_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSkewTo_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSkewTo_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSkewTo_CCSkewTo(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCSkewBy_class;
extern JSObject *jsb_CCSkewBy_prototype;

JSBool js_cocos2dx_CCSkewBy_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCSkewBy_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCSkewBy(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCSkewBy_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSkewBy_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSkewBy_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSkewBy_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCJumpBy_class;
extern JSObject *jsb_CCJumpBy_prototype;

JSBool js_cocos2dx_CCJumpBy_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCJumpBy_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCJumpBy(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCJumpBy_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCJumpBy_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCJumpBy_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCJumpBy_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCJumpBy_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCJumpTo_class;
extern JSObject *jsb_CCJumpTo_prototype;

JSBool js_cocos2dx_CCJumpTo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCJumpTo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCJumpTo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCJumpTo_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCJumpTo_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCBezierBy_class;
extern JSObject *jsb_CCBezierBy_prototype;

JSBool js_cocos2dx_CCBezierBy_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCBezierBy_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCBezierBy(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCBezierBy_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCBezierBy_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCBezierBy_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCBezierBy_update(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCBezierTo_class;
extern JSObject *jsb_CCBezierTo_prototype;

JSBool js_cocos2dx_CCBezierTo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCBezierTo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCBezierTo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCBezierTo_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCBezierTo_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCScaleTo_class;
extern JSObject *jsb_CCScaleTo_prototype;

JSBool js_cocos2dx_CCScaleTo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCScaleTo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCScaleTo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCScaleTo_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCScaleTo_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCScaleTo_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCScaleTo_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCScaleBy_class;
extern JSObject *jsb_CCScaleBy_prototype;

JSBool js_cocos2dx_CCScaleBy_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCScaleBy_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCScaleBy(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCScaleBy_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCScaleBy_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCScaleBy_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCBlink_class;
extern JSObject *jsb_CCBlink_prototype;

JSBool js_cocos2dx_CCBlink_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCBlink_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCBlink(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCBlink_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCBlink_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCBlink_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCBlink_stop(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCBlink_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCBlink_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFadeIn_class;
extern JSObject *jsb_CCFadeIn_prototype;

JSBool js_cocos2dx_CCFadeIn_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFadeIn_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFadeIn(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFadeIn_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeIn_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeIn_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFadeOut_class;
extern JSObject *jsb_CCFadeOut_prototype;

JSBool js_cocos2dx_CCFadeOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFadeOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFadeOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFadeOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFadeTo_class;
extern JSObject *jsb_CCFadeTo_prototype;

JSBool js_cocos2dx_CCFadeTo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFadeTo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFadeTo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFadeTo_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeTo_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeTo_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeTo_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTintTo_class;
extern JSObject *jsb_CCTintTo_prototype;

JSBool js_cocos2dx_CCTintTo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTintTo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTintTo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTintTo_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTintTo_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTintTo_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTintTo_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTintBy_class;
extern JSObject *jsb_CCTintBy_prototype;

JSBool js_cocos2dx_CCTintBy_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTintBy_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTintBy(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTintBy_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTintBy_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTintBy_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTintBy_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTintBy_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCDelayTime_class;
extern JSObject *jsb_CCDelayTime_prototype;

JSBool js_cocos2dx_CCDelayTime_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCDelayTime_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCDelayTime(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCDelayTime_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDelayTime_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDelayTime_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCAnimate_class;
extern JSObject *jsb_CCAnimate_prototype;

JSBool js_cocos2dx_CCAnimate_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCAnimate_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCAnimate(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCAnimate_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimate_getAnimation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimate_stop(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimate_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimate_initWithAnimation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimate_setAnimation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimate_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimate_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimate_CCAnimate(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTargetedAction_class;
extern JSObject *jsb_CCTargetedAction_prototype;

JSBool js_cocos2dx_CCTargetedAction_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTargetedAction_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTargetedAction(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTargetedAction_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTargetedAction_setForcedTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTargetedAction_initWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTargetedAction_stop(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTargetedAction_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTargetedAction_getForcedTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTargetedAction_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTargetedAction_CCTargetedAction(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCActionCamera_class;
extern JSObject *jsb_CCActionCamera_prototype;

JSBool js_cocos2dx_CCActionCamera_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCActionCamera_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCActionCamera(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCActionCamera_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionCamera_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionCamera_CCActionCamera(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCOrbitCamera_class;
extern JSObject *jsb_CCOrbitCamera_prototype;

JSBool js_cocos2dx_CCOrbitCamera_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCOrbitCamera_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCOrbitCamera(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCOrbitCamera_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCOrbitCamera_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCOrbitCamera_sphericalRadius(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCOrbitCamera_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCOrbitCamera_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCOrbitCamera_CCOrbitCamera(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCActionManager_class;
extern JSObject *jsb_CCActionManager_prototype;

JSBool js_cocos2dx_CCActionManager_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCActionManager_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCActionManager(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCActionManager_getActionByTag(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionManager_removeActionByTag(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionManager_removeAllActions(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionManager_addAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionManager_resumeTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionManager_pauseTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionManager_removeAllActionsFromTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionManager_resumeTargets(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionManager_removeAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionManager_numberOfRunningActionsInTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionManager_pauseAllRunningActions(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionManager_CCActionManager(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCActionEase_class;
extern JSObject *jsb_CCActionEase_prototype;

JSBool js_cocos2dx_CCActionEase_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCActionEase_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCActionEase(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCActionEase_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionEase_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionEase_stop(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionEase_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionEase_initWithAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionEase_getInnerAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionEase_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseRateAction_class;
extern JSObject *jsb_CCEaseRateAction_prototype;

JSBool js_cocos2dx_CCEaseRateAction_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseRateAction_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseRateAction(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseRateAction_setRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseRateAction_initWithAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseRateAction_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseRateAction_getRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseRateAction_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseIn_class;
extern JSObject *jsb_CCEaseIn_prototype;

JSBool js_cocos2dx_CCEaseIn_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseIn_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseIn(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseIn_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseIn_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseIn_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseOut_class;
extern JSObject *jsb_CCEaseOut_prototype;

JSBool js_cocos2dx_CCEaseOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseInOut_class;
extern JSObject *jsb_CCEaseInOut_prototype;

JSBool js_cocos2dx_CCEaseInOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseInOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseInOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseInOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseInOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseInOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseExponentialIn_class;
extern JSObject *jsb_CCEaseExponentialIn_prototype;

JSBool js_cocos2dx_CCEaseExponentialIn_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseExponentialIn_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseExponentialIn(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseExponentialIn_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseExponentialIn_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseExponentialIn_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseExponentialOut_class;
extern JSObject *jsb_CCEaseExponentialOut_prototype;

JSBool js_cocos2dx_CCEaseExponentialOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseExponentialOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseExponentialOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseExponentialOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseExponentialOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseExponentialOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseExponentialInOut_class;
extern JSObject *jsb_CCEaseExponentialInOut_prototype;

JSBool js_cocos2dx_CCEaseExponentialInOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseExponentialInOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseExponentialInOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseExponentialInOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseExponentialInOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseExponentialInOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseSineIn_class;
extern JSObject *jsb_CCEaseSineIn_prototype;

JSBool js_cocos2dx_CCEaseSineIn_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseSineIn_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseSineIn(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseSineIn_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseSineIn_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseSineIn_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseSineOut_class;
extern JSObject *jsb_CCEaseSineOut_prototype;

JSBool js_cocos2dx_CCEaseSineOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseSineOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseSineOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseSineOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseSineOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseSineOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseSineInOut_class;
extern JSObject *jsb_CCEaseSineInOut_prototype;

JSBool js_cocos2dx_CCEaseSineInOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseSineInOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseSineInOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseSineInOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseSineInOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseSineInOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseElastic_class;
extern JSObject *jsb_CCEaseElastic_prototype;

JSBool js_cocos2dx_CCEaseElastic_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseElastic_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseElastic(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseElastic_setPeriod(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseElastic_initWithAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseElastic_getPeriod(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseElastic_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseElastic_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseElasticIn_class;
extern JSObject *jsb_CCEaseElasticIn_prototype;

JSBool js_cocos2dx_CCEaseElasticIn_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseElasticIn_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseElasticIn(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseElasticIn_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseElasticIn_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseElasticIn_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseElasticOut_class;
extern JSObject *jsb_CCEaseElasticOut_prototype;

JSBool js_cocos2dx_CCEaseElasticOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseElasticOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseElasticOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseElasticOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseElasticOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseElasticOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseElasticInOut_class;
extern JSObject *jsb_CCEaseElasticInOut_prototype;

JSBool js_cocos2dx_CCEaseElasticInOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseElasticInOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseElasticInOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseElasticInOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseElasticInOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseElasticInOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseBounce_class;
extern JSObject *jsb_CCEaseBounce_prototype;

JSBool js_cocos2dx_CCEaseBounce_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseBounce_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseBounce(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseBounce_bounceTime(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBounce_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBounce_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseBounceIn_class;
extern JSObject *jsb_CCEaseBounceIn_prototype;

JSBool js_cocos2dx_CCEaseBounceIn_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseBounceIn_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseBounceIn(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseBounceIn_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBounceIn_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBounceIn_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseBounceOut_class;
extern JSObject *jsb_CCEaseBounceOut_prototype;

JSBool js_cocos2dx_CCEaseBounceOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseBounceOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseBounceOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseBounceOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBounceOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBounceOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseBounceInOut_class;
extern JSObject *jsb_CCEaseBounceInOut_prototype;

JSBool js_cocos2dx_CCEaseBounceInOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseBounceInOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseBounceInOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseBounceInOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBounceInOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBounceInOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseBackIn_class;
extern JSObject *jsb_CCEaseBackIn_prototype;

JSBool js_cocos2dx_CCEaseBackIn_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseBackIn_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseBackIn(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseBackIn_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBackIn_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBackIn_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseBackOut_class;
extern JSObject *jsb_CCEaseBackOut_prototype;

JSBool js_cocos2dx_CCEaseBackOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseBackOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseBackOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseBackOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBackOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBackOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEaseBackInOut_class;
extern JSObject *jsb_CCEaseBackInOut_prototype;

JSBool js_cocos2dx_CCEaseBackInOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEaseBackInOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEaseBackInOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEaseBackInOut_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBackInOut_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEaseBackInOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCActionInstant_class;
extern JSObject *jsb_CCActionInstant_prototype;

JSBool js_cocos2dx_CCActionInstant_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCActionInstant_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCActionInstant(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCActionInstant_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionInstant_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionInstant_step(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionInstant_isDone(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionInstant_CCActionInstant(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCShow_class;
extern JSObject *jsb_CCShow_prototype;

JSBool js_cocos2dx_CCShow_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCShow_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCShow(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCShow_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShow_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShow_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShow_CCShow(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCHide_class;
extern JSObject *jsb_CCHide_prototype;

JSBool js_cocos2dx_CCHide_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCHide_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCHide(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCHide_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCHide_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCHide_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCHide_CCHide(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCToggleVisibility_class;
extern JSObject *jsb_CCToggleVisibility_prototype;

JSBool js_cocos2dx_CCToggleVisibility_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCToggleVisibility_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCToggleVisibility(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCToggleVisibility_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCToggleVisibility_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCToggleVisibility_CCToggleVisibility(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFlipX_class;
extern JSObject *jsb_CCFlipX_prototype;

JSBool js_cocos2dx_CCFlipX_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFlipX_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFlipX(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFlipX_initWithFlipX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFlipX_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFlipX_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFlipX_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFlipX_CCFlipX(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFlipY_class;
extern JSObject *jsb_CCFlipY_prototype;

JSBool js_cocos2dx_CCFlipY_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFlipY_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFlipY(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFlipY_initWithFlipY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFlipY_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFlipY_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFlipY_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFlipY_CCFlipY(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCPlace_class;
extern JSObject *jsb_CCPlace_prototype;

JSBool js_cocos2dx_CCPlace_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCPlace_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCPlace(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCPlace_initWithPosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCPlace_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCPlace_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCPlace_CCPlace(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCCallFunc_class;
extern JSObject *jsb_CCCallFunc_prototype;

JSBool js_cocos2dx_CCCallFunc_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCCallFunc_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCCallFunc(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCCallFunc_execute(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCallFunc_initWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCallFunc_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCallFunc_getTargetCallback(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCallFunc_getScriptHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCallFunc_setTargetCallback(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCallFunc_CCCallFunc(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCGridAction_class;
extern JSObject *jsb_CCGridAction_prototype;

JSBool js_cocos2dx_CCGridAction_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCGridAction_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCGridAction(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCGridAction_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridAction_getGrid(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridAction_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridAction_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridAction_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCGrid3DAction_class;
extern JSObject *jsb_CCGrid3DAction_prototype;

JSBool js_cocos2dx_CCGrid3DAction_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCGrid3DAction_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCGrid3DAction(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCGrid3DAction_setVertex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGrid3DAction_getGrid(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGrid3DAction_vertex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGrid3DAction_originalVertex(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTiledGrid3DAction_class;
extern JSObject *jsb_CCTiledGrid3DAction_prototype;

JSBool js_cocos2dx_CCTiledGrid3DAction_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTiledGrid3DAction_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTiledGrid3DAction(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTiledGrid3DAction_tile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTiledGrid3DAction_setTile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTiledGrid3DAction_originalTile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTiledGrid3DAction_getGrid(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCStopGrid_class;
extern JSObject *jsb_CCStopGrid_prototype;

JSBool js_cocos2dx_CCStopGrid_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCStopGrid_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCStopGrid(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCStopGrid_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCStopGrid_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCReuseGrid_class;
extern JSObject *jsb_CCReuseGrid_prototype;

JSBool js_cocos2dx_CCReuseGrid_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCReuseGrid_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCReuseGrid(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCReuseGrid_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCReuseGrid_initWithTimes(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCReuseGrid_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCWaves3D_class;
extern JSObject *jsb_CCWaves3D_prototype;

JSBool js_cocos2dx_CCWaves3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCWaves3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCWaves3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCWaves3D_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWaves3D_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWaves3D_getAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWaves3D_setAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWaves3D_getAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWaves3D_setAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWaves3D_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFlipX3D_class;
extern JSObject *jsb_CCFlipX3D_prototype;

JSBool js_cocos2dx_CCFlipX3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFlipX3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFlipX3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFlipX3D_initWithSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFlipX3D_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFlipX3D_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFlipX3D_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFlipY3D_class;
extern JSObject *jsb_CCFlipY3D_prototype;

JSBool js_cocos2dx_CCFlipY3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFlipY3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFlipY3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFlipY3D_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFlipY3D_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCLens3D_class;
extern JSObject *jsb_CCLens3D_prototype;

JSBool js_cocos2dx_CCLens3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCLens3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCLens3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCLens3D_setConcave(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLens3D_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLens3D_setLensEffect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLens3D_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLens3D_getLensEffect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLens3D_getPosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLens3D_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCRipple3D_class;
extern JSObject *jsb_CCRipple3D_prototype;

JSBool js_cocos2dx_CCRipple3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCRipple3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCRipple3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCRipple3D_setAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRipple3D_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRipple3D_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRipple3D_getAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRipple3D_setAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRipple3D_getAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRipple3D_getPosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRipple3D_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCShaky3D_class;
extern JSObject *jsb_CCShaky3D_prototype;

JSBool js_cocos2dx_CCShaky3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCShaky3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCShaky3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCShaky3D_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShaky3D_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShaky3D_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCLiquid_class;
extern JSObject *jsb_CCLiquid_prototype;

JSBool js_cocos2dx_CCLiquid_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCLiquid_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCLiquid(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCLiquid_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLiquid_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLiquid_getAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLiquid_setAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLiquid_getAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLiquid_setAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLiquid_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCWaves_class;
extern JSObject *jsb_CCWaves_prototype;

JSBool js_cocos2dx_CCWaves_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCWaves_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCWaves(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCWaves_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWaves_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWaves_getAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWaves_setAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWaves_getAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWaves_setAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWaves_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTwirl_class;
extern JSObject *jsb_CCTwirl_prototype;

JSBool js_cocos2dx_CCTwirl_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTwirl_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTwirl(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTwirl_setAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTwirl_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTwirl_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTwirl_getAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTwirl_setAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTwirl_getAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTwirl_getPosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTwirl_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCPageTurn3D_class;
extern JSObject *jsb_CCPageTurn3D_prototype;

JSBool js_cocos2dx_CCPageTurn3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCPageTurn3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCPageTurn3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCPageTurn3D_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCPageTurn3D_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCProgressTo_class;
extern JSObject *jsb_CCProgressTo_prototype;

JSBool js_cocos2dx_CCProgressTo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCProgressTo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCProgressTo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCProgressTo_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTo_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTo_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTo_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCProgressFromTo_class;
extern JSObject *jsb_CCProgressFromTo_prototype;

JSBool js_cocos2dx_CCProgressFromTo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCProgressFromTo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCProgressFromTo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCProgressFromTo_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressFromTo_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressFromTo_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressFromTo_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressFromTo_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCShakyTiles3D_class;
extern JSObject *jsb_CCShakyTiles3D_prototype;

JSBool js_cocos2dx_CCShakyTiles3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCShakyTiles3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCShakyTiles3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCShakyTiles3D_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShakyTiles3D_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShakyTiles3D_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCShatteredTiles3D_class;
extern JSObject *jsb_CCShatteredTiles3D_prototype;

JSBool js_cocos2dx_CCShatteredTiles3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCShatteredTiles3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCShatteredTiles3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCShatteredTiles3D_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShatteredTiles3D_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShatteredTiles3D_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCShuffleTiles_class;
extern JSObject *jsb_CCShuffleTiles_prototype;

JSBool js_cocos2dx_CCShuffleTiles_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCShuffleTiles_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCShuffleTiles(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCShuffleTiles_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShuffleTiles_placeTile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShuffleTiles_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShuffleTiles_getDelta(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShuffleTiles_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShuffleTiles_shuffle(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShuffleTiles_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFadeOutTRTiles_class;
extern JSObject *jsb_CCFadeOutTRTiles_prototype;

JSBool js_cocos2dx_CCFadeOutTRTiles_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFadeOutTRTiles_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFadeOutTRTiles(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFadeOutTRTiles_turnOnTile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeOutTRTiles_turnOffTile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeOutTRTiles_transformTile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeOutTRTiles_testFunc(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeOutTRTiles_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeOutTRTiles_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFadeOutBLTiles_class;
extern JSObject *jsb_CCFadeOutBLTiles_prototype;

JSBool js_cocos2dx_CCFadeOutBLTiles_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFadeOutBLTiles_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFadeOutBLTiles(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFadeOutBLTiles_testFunc(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeOutBLTiles_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFadeOutUpTiles_class;
extern JSObject *jsb_CCFadeOutUpTiles_prototype;

JSBool js_cocos2dx_CCFadeOutUpTiles_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFadeOutUpTiles_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFadeOutUpTiles(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFadeOutUpTiles_transformTile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeOutUpTiles_testFunc(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeOutUpTiles_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFadeOutDownTiles_class;
extern JSObject *jsb_CCFadeOutDownTiles_prototype;

JSBool js_cocos2dx_CCFadeOutDownTiles_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFadeOutDownTiles_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFadeOutDownTiles(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFadeOutDownTiles_testFunc(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFadeOutDownTiles_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTurnOffTiles_class;
extern JSObject *jsb_CCTurnOffTiles_prototype;

JSBool js_cocos2dx_CCTurnOffTiles_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTurnOffTiles_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTurnOffTiles(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTurnOffTiles_turnOnTile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTurnOffTiles_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTurnOffTiles_turnOffTile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTurnOffTiles_shuffle(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTurnOffTiles_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTurnOffTiles_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTurnOffTiles_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCWavesTiles3D_class;
extern JSObject *jsb_CCWavesTiles3D_prototype;

JSBool js_cocos2dx_CCWavesTiles3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCWavesTiles3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCWavesTiles3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCWavesTiles3D_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWavesTiles3D_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWavesTiles3D_getAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWavesTiles3D_setAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWavesTiles3D_getAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWavesTiles3D_setAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCWavesTiles3D_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCJumpTiles3D_class;
extern JSObject *jsb_CCJumpTiles3D_prototype;

JSBool js_cocos2dx_CCJumpTiles3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCJumpTiles3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCJumpTiles3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCJumpTiles3D_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCJumpTiles3D_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCJumpTiles3D_getAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCJumpTiles3D_setAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCJumpTiles3D_getAmplitude(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCJumpTiles3D_setAmplitudeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCJumpTiles3D_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCSplitRows_class;
extern JSObject *jsb_CCSplitRows_prototype;

JSBool js_cocos2dx_CCSplitRows_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCSplitRows_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCSplitRows(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCSplitRows_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSplitRows_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSplitRows_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSplitRows_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCSplitCols_class;
extern JSObject *jsb_CCSplitCols_prototype;

JSBool js_cocos2dx_CCSplitCols_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCSplitCols_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCSplitCols(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCSplitCols_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSplitCols_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSplitCols_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSplitCols_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCActionTween_class;
extern JSObject *jsb_CCActionTween_prototype;

JSBool js_cocos2dx_CCActionTween_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCActionTween_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCActionTween(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCActionTween_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionTween_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionTween_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionTween_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCActionTween_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCCardinalSplineTo_class;
extern JSObject *jsb_CCCardinalSplineTo_prototype;

JSBool js_cocos2dx_CCCardinalSplineTo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCCardinalSplineTo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCCardinalSplineTo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCCardinalSplineTo_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCardinalSplineTo_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCardinalSplineTo_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCardinalSplineTo_getPoints(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCardinalSplineTo_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCardinalSplineTo_updatePosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCardinalSplineTo_CCCardinalSplineTo(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCCardinalSplineBy_class;
extern JSObject *jsb_CCCardinalSplineBy_prototype;

JSBool js_cocos2dx_CCCardinalSplineBy_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCCardinalSplineBy_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCCardinalSplineBy(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCCardinalSplineBy_startWithTarget(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCardinalSplineBy_updatePosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCardinalSplineBy_reverse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCardinalSplineBy_CCCardinalSplineBy(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCCatmullRomTo_class;
extern JSObject *jsb_CCCatmullRomTo_prototype;

JSBool js_cocos2dx_CCCatmullRomTo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCCatmullRomTo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCCatmullRomTo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCCatmullRomTo_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCCatmullRomBy_class;
extern JSObject *jsb_CCCatmullRomBy_prototype;

JSBool js_cocos2dx_CCCatmullRomBy_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCCatmullRomBy_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCCatmullRomBy(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCCatmullRomBy_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCAtlasNode_class;
extern JSObject *jsb_CCAtlasNode_prototype;

JSBool js_cocos2dx_CCAtlasNode_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCAtlasNode_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCAtlasNode(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCAtlasNode_setTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_initWithTileFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_setColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_setOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_setTextureAtlas(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_getTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_getTextureAtlas(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_setOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_getQuadsToDraw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_updateAtlasValues(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_getColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_initWithTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_isOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_setQuadsToDraw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAtlasNode_CCAtlasNode(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCDrawNode_class;
extern JSObject *jsb_CCDrawNode_prototype;

JSBool js_cocos2dx_CCDrawNode_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCDrawNode_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCDrawNode(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCDrawNode_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDrawNode_clear(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDrawNode_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDrawNode_drawDot(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDrawNode_drawSegment(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDrawNode_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDrawNode_CCDrawNode(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCCamera_class;
extern JSObject *jsb_CCCamera_prototype;

JSBool js_cocos2dx_CCCamera_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCCamera_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCCamera(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCCamera_locate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCamera_restore(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCamera_setEyeXYZ(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCamera_setCenterXYZ(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCamera_isDirty(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCamera_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCamera_setDirty(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCamera_setUpXYZ(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCamera_getZEye(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCCamera_CCCamera(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCLabelAtlas_class;
extern JSObject *jsb_CCLabelAtlas_prototype;

JSBool js_cocos2dx_CCLabelAtlas_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCLabelAtlas_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCLabelAtlas(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCLabelAtlas_setString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelAtlas_initWithString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelAtlas_updateAtlasValues(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelAtlas_getString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelAtlas_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelAtlas_CCLabelAtlas(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCDirector_class;
extern JSObject *jsb_CCDirector_prototype;

JSBool js_cocos2dx_CCDirector_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCDirector_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCDirector(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCDirector_pause(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setDelegate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setContentScaleFactor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getContentScaleFactor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getWinSizeInPixels(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getDeltaTime(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setGLDefaultValues(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setActionManager(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setAlphaBlending(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_popToRootScene(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getNotificationNode(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getWinSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_end(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_isSendCleanupToScene(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getVisibleOrigin(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_mainLoop(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setDepthTest(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getSecondsPerFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_convertToUI(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setAccelerometer(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setDefaultValues(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setScheduler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_reshapeProjection(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_startAnimation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getRunningScene(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setViewport(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_stopAnimation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setKeypadDispatcher(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_popToSceneStackLevel(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_resume(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setTouchDispatcher(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_isNextDeltaTimeZero(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getDelegate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setOpenGLView(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_convertToGL(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_purgeCachedData(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getTotalFrames(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_runWithScene(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setNotificationNode(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_drawScene(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_popScene(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_isDisplayStats(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setProjection(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getZEye(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setNextDeltaTimeZero(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getVisibleSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getScheduler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_pushScene(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getAnimationInterval(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_isPaused(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setDisplayStats(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_replaceScene(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_setAnimationInterval(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_getActionManager(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCDirector_sharedDirector(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCGridBase_class;
extern JSObject *jsb_CCGridBase_prototype;

JSBool js_cocos2dx_CCGridBase_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCGridBase_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCGridBase(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCGridBase_setGridSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_calculateVertexPoints(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_afterDraw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_beforeDraw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_isTextureFlipped(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_getGridSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_getStep(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_set2DProjection(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_setStep(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_setTextureFlipped(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_blit(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_setActive(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_getReuseGrid(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_initWithSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_setReuseGrid(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_isActive(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_reuse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGridBase_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCGrid3D_class;
extern JSObject *jsb_CCGrid3D_prototype;

JSBool js_cocos2dx_CCGrid3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCGrid3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCGrid3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCGrid3D_calculateVertexPoints(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGrid3D_setVertex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGrid3D_reuse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGrid3D_vertex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGrid3D_blit(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGrid3D_originalVertex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGrid3D_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCGrid3D_CCGrid3D(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTiledGrid3D_class;
extern JSObject *jsb_CCTiledGrid3D_prototype;

JSBool js_cocos2dx_CCTiledGrid3D_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTiledGrid3D_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTiledGrid3D(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTiledGrid3D_calculateVertexPoints(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTiledGrid3D_reuse(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTiledGrid3D_originalTile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTiledGrid3D_tile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTiledGrid3D_setTile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTiledGrid3D_blit(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTiledGrid3D_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTiledGrid3D_CCTiledGrid3D(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCSprite_class;
extern JSObject *jsb_CCSprite_prototype;

JSBool js_cocos2dx_CCSprite_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCSprite_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCSprite(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCSprite_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_addChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_isFlipX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_getTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setScaleY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setScale(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setDisplayFrameWithAnimationName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setRotationY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_getBatchNode(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_isTextureRectRotated(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_isOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setTextureRect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_initWithFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setVertexZ(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_getTextureRect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_updateDisplayedOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_initWithSpriteFrameName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_isFrameDisplayed(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_getAtlasIndex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setRotation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setDisplayFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_getOffsetPosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setBatchNode(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setRotationX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setScaleX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_initWithTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setTextureAtlas(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setFlipY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setFlipX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_removeAllChildrenWithCleanup(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_sortAllChildren(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setAtlasIndex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setVertexRect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setDirty(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_isDirty(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_reorderChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_ignoreAnchorPointForPosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_getTextureAtlas(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_initWithSpriteFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_removeChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_updateTransform(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_isFlipY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_updateDisplayedColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setSkewX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setSkewY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_setVisible(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_createWithTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_createWithSpriteFrameName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_createWithSpriteFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSprite_CCSprite(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCLabelTTF_class;
extern JSObject *jsb_CCLabelTTF_prototype;

JSBool js_cocos2dx_CCLabelTTF_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCLabelTTF_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCLabelTTF(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCLabelTTF_enableShadow(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_setDimensions(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_getFontSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_getString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_setTextDefinition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_setFontName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_getHorizontalAlignment(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_initWithStringAndTextDefinition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_setString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_initWithString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_setFontFillColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_enableStroke(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_getDimensions(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_setVerticalAlignment(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_setFontSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_getVerticalAlignment(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_getTextDefinition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_getFontName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_setHorizontalAlignment(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_disableShadow(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_disableStroke(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_createWithFontDefinition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelTTF_CCLabelTTF(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCSpriteBatchNode_class;
extern JSObject *jsb_CCSpriteBatchNode_prototype;

JSBool js_cocos2dx_CCSpriteBatchNode_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCSpriteBatchNode_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCSpriteBatchNode(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCSpriteBatchNode_appendChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_reorderBatch(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_addChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_visit(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_removeAllChildrenWithCleanup(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_setTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_removeChildAtIndex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_removeSpriteFromAtlas(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_atlasIndexForChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_getTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_increaseAtlasCapacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_insertChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_lowestAtlasIndexInChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_initWithTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_setTextureAtlas(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_sortAllChildren(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_reorderChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_rebuildIndexInOrder(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_getTextureAtlas(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_getDescendants(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_removeChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_highestAtlasIndexInChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_createWithTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteBatchNode_CCSpriteBatchNode(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCLabelBMFont_class;
extern JSObject *jsb_CCLabelBMFont_prototype;

JSBool js_cocos2dx_CCLabelBMFont_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCLabelBMFont_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCLabelBMFont(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCLabelBMFont_createFontChars(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_getString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_getConfiguration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setScale(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setCascadeOpacityEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_getFntFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_updateLabel(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setWidth(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_isOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_isCascadeOpacityEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_initWithString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setCascadeColorEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_updateDisplayedOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setFntFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_getOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setLineBreakWithoutSpace(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setScaleY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setScaleX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_getColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_getDisplayedOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_isCascadeColorEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setCString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_getDisplayedColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_updateDisplayedColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_setAlignment(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_purgeCachedData(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLabelBMFont_CCLabelBMFont(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCLayer_class;
extern JSObject *jsb_CCLayer_prototype;

JSBool js_cocos2dx_CCLayer_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCLayer_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCLayer(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCLayer_unregisterScriptTouchHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_keyBackClicked(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_ccTouchBegan(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_setAccelerometerInterval(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_ccTouchesCancelled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_unregisterScriptAccelerateHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_ccTouchesMoved(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_registerScriptAccelerateHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_getTouchMode(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_setAccelerometerEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_isTouchEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_getScriptAccelerateHandlerEntry(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_getScriptKeypadHandlerEntry(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_ccTouchMoved(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_setTouchEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_unregisterScriptKeypadHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_isKeypadEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_ccTouchesEnded(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_setTouchMode(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_isAccelerometerEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_ccTouchEnded(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_registerScriptTouchHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_ccTouchCancelled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_getScriptTouchHandlerEntry(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_ccTouchesBegan(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_setTouchPriority(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_getTouchPriority(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_setKeypadEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_registerWithTouchDispatcher(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_keyMenuClicked(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_registerScriptKeypadHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayer_CCLayer(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCLayerRGBA_class;
extern JSObject *jsb_CCLayerRGBA_prototype;

JSBool js_cocos2dx_CCLayerRGBA_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCLayerRGBA_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCLayerRGBA(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCLayerRGBA_updateDisplayedColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_setColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_isCascadeOpacityEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_getColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_getDisplayedOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_setCascadeColorEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_setOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_setOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_setCascadeOpacityEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_updateDisplayedOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_getOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_isOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_isCascadeColorEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_getDisplayedColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerRGBA_CCLayerRGBA(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCLayerColor_class;
extern JSObject *jsb_CCLayerColor_prototype;

JSBool js_cocos2dx_CCLayerColor_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCLayerColor_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCLayerColor(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCLayerColor_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerColor_setColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerColor_changeWidthAndHeight(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerColor_setOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerColor_initWithColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerColor_changeWidth(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerColor_changeHeight(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerColor_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerColor_CCLayerColor(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCLayerGradient_class;
extern JSObject *jsb_CCLayerGradient_prototype;

JSBool js_cocos2dx_CCLayerGradient_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCLayerGradient_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCLayerGradient(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCLayerGradient_getStartColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_isCompressedInterpolation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_getStartOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_setVector(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_setStartOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_setCompressedInterpolation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_setEndOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_getVector(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_initWithColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_setEndColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_getEndColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_getEndOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_setStartColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerGradient_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCLayerMultiplex_class;
extern JSObject *jsb_CCLayerMultiplex_prototype;

JSBool js_cocos2dx_CCLayerMultiplex_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCLayerMultiplex_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCLayerMultiplex(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCLayerMultiplex_initWithArray(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerMultiplex_switchToAndReleaseMe(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerMultiplex_addLayer(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerMultiplex_switchTo(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCLayerMultiplex_CCLayerMultiplex(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCScene_class;
extern JSObject *jsb_CCScene_prototype;

JSBool js_cocos2dx_CCScene_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCScene_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCScene(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCScene_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCScene_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCScene_CCScene(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionEaseScene_class;
extern JSObject *jsb_CCTransitionEaseScene_prototype;

JSBool js_cocos2dx_CCTransitionEaseScene_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionEaseScene_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionEaseScene(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionEaseScene_easeActionWithAction(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionScene_class;
extern JSObject *jsb_CCTransitionScene_prototype;

JSBool js_cocos2dx_CCTransitionScene_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionScene_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionScene(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionScene_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionScene_finish(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionScene_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionScene_cleanup(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionScene_hideOutShowIn(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionScene_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionScene_CCTransitionScene(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionSceneOriented_class;
extern JSObject *jsb_CCTransitionSceneOriented_prototype;

JSBool js_cocos2dx_CCTransitionSceneOriented_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionSceneOriented_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionSceneOriented(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionSceneOriented_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSceneOriented_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSceneOriented_CCTransitionSceneOriented(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionRotoZoom_class;
extern JSObject *jsb_CCTransitionRotoZoom_prototype;

JSBool js_cocos2dx_CCTransitionRotoZoom_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionRotoZoom_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionRotoZoom(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionRotoZoom_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionRotoZoom_CCTransitionRotoZoom(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionJumpZoom_class;
extern JSObject *jsb_CCTransitionJumpZoom_prototype;

JSBool js_cocos2dx_CCTransitionJumpZoom_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionJumpZoom_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionJumpZoom(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionJumpZoom_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionJumpZoom_CCTransitionJumpZoom(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionMoveInL_class;
extern JSObject *jsb_CCTransitionMoveInL_prototype;

JSBool js_cocos2dx_CCTransitionMoveInL_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionMoveInL_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionMoveInL(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionMoveInL_action(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionMoveInL_easeActionWithAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionMoveInL_initScenes(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionMoveInL_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionMoveInL_CCTransitionMoveInL(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionMoveInR_class;
extern JSObject *jsb_CCTransitionMoveInR_prototype;

JSBool js_cocos2dx_CCTransitionMoveInR_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionMoveInR_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionMoveInR(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionMoveInR_initScenes(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionMoveInR_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionMoveInR_CCTransitionMoveInR(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionMoveInT_class;
extern JSObject *jsb_CCTransitionMoveInT_prototype;

JSBool js_cocos2dx_CCTransitionMoveInT_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionMoveInT_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionMoveInT(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionMoveInT_initScenes(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionMoveInT_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionMoveInT_CCTransitionMoveInT(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionMoveInB_class;
extern JSObject *jsb_CCTransitionMoveInB_prototype;

JSBool js_cocos2dx_CCTransitionMoveInB_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionMoveInB_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionMoveInB(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionMoveInB_initScenes(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionMoveInB_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionMoveInB_CCTransitionMoveInB(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionSlideInL_class;
extern JSObject *jsb_CCTransitionSlideInL_prototype;

JSBool js_cocos2dx_CCTransitionSlideInL_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionSlideInL_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionSlideInL(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionSlideInL_action(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInL_easeActionWithAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInL_initScenes(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInL_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInL_CCTransitionSlideInL(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionSlideInR_class;
extern JSObject *jsb_CCTransitionSlideInR_prototype;

JSBool js_cocos2dx_CCTransitionSlideInR_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionSlideInR_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionSlideInR(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionSlideInR_action(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInR_initScenes(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInR_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInR_CCTransitionSlideInR(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionSlideInB_class;
extern JSObject *jsb_CCTransitionSlideInB_prototype;

JSBool js_cocos2dx_CCTransitionSlideInB_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionSlideInB_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionSlideInB(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionSlideInB_action(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInB_initScenes(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInB_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInB_CCTransitionSlideInB(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionSlideInT_class;
extern JSObject *jsb_CCTransitionSlideInT_prototype;

JSBool js_cocos2dx_CCTransitionSlideInT_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionSlideInT_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionSlideInT(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionSlideInT_action(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInT_initScenes(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInT_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSlideInT_CCTransitionSlideInT(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionShrinkGrow_class;
extern JSObject *jsb_CCTransitionShrinkGrow_prototype;

JSBool js_cocos2dx_CCTransitionShrinkGrow_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionShrinkGrow_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionShrinkGrow(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionShrinkGrow_easeActionWithAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionShrinkGrow_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionShrinkGrow_CCTransitionShrinkGrow(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionFlipX_class;
extern JSObject *jsb_CCTransitionFlipX_prototype;

JSBool js_cocos2dx_CCTransitionFlipX_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionFlipX_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionFlipX(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionFlipX_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFlipX_CCTransitionFlipX(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionFlipY_class;
extern JSObject *jsb_CCTransitionFlipY_prototype;

JSBool js_cocos2dx_CCTransitionFlipY_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionFlipY_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionFlipY(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionFlipY_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFlipY_CCTransitionFlipY(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionFlipAngular_class;
extern JSObject *jsb_CCTransitionFlipAngular_prototype;

JSBool js_cocos2dx_CCTransitionFlipAngular_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionFlipAngular_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionFlipAngular(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionFlipAngular_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFlipAngular_CCTransitionFlipAngular(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionZoomFlipX_class;
extern JSObject *jsb_CCTransitionZoomFlipX_prototype;

JSBool js_cocos2dx_CCTransitionZoomFlipX_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionZoomFlipX_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionZoomFlipX(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionZoomFlipX_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionZoomFlipX_CCTransitionZoomFlipX(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionZoomFlipY_class;
extern JSObject *jsb_CCTransitionZoomFlipY_prototype;

JSBool js_cocos2dx_CCTransitionZoomFlipY_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionZoomFlipY_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionZoomFlipY(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionZoomFlipY_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionZoomFlipY_CCTransitionZoomFlipY(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionZoomFlipAngular_class;
extern JSObject *jsb_CCTransitionZoomFlipAngular_prototype;

JSBool js_cocos2dx_CCTransitionZoomFlipAngular_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionZoomFlipAngular_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionZoomFlipAngular(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionZoomFlipAngular_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionZoomFlipAngular_CCTransitionZoomFlipAngular(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionFade_class;
extern JSObject *jsb_CCTransitionFade_prototype;

JSBool js_cocos2dx_CCTransitionFade_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionFade_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionFade(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionFade_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFade_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFade_CCTransitionFade(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionCrossFade_class;
extern JSObject *jsb_CCTransitionCrossFade_prototype;

JSBool js_cocos2dx_CCTransitionCrossFade_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionCrossFade_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionCrossFade(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionCrossFade_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionCrossFade_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionCrossFade_CCTransitionCrossFade(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionTurnOffTiles_class;
extern JSObject *jsb_CCTransitionTurnOffTiles_prototype;

JSBool js_cocos2dx_CCTransitionTurnOffTiles_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionTurnOffTiles_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionTurnOffTiles(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionTurnOffTiles_easeActionWithAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionTurnOffTiles_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionTurnOffTiles_CCTransitionTurnOffTiles(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionSplitCols_class;
extern JSObject *jsb_CCTransitionSplitCols_prototype;

JSBool js_cocos2dx_CCTransitionSplitCols_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionSplitCols_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionSplitCols(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionSplitCols_action(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSplitCols_easeActionWithAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSplitCols_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSplitCols_CCTransitionSplitCols(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionSplitRows_class;
extern JSObject *jsb_CCTransitionSplitRows_prototype;

JSBool js_cocos2dx_CCTransitionSplitRows_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionSplitRows_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionSplitRows(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionSplitRows_action(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSplitRows_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionSplitRows_CCTransitionSplitRows(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionFadeTR_class;
extern JSObject *jsb_CCTransitionFadeTR_prototype;

JSBool js_cocos2dx_CCTransitionFadeTR_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionFadeTR_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionFadeTR(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionFadeTR_easeActionWithAction(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFadeTR_actionWithSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFadeTR_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFadeTR_CCTransitionFadeTR(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionFadeBL_class;
extern JSObject *jsb_CCTransitionFadeBL_prototype;

JSBool js_cocos2dx_CCTransitionFadeBL_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionFadeBL_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionFadeBL(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionFadeBL_actionWithSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFadeBL_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFadeBL_CCTransitionFadeBL(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionFadeUp_class;
extern JSObject *jsb_CCTransitionFadeUp_prototype;

JSBool js_cocos2dx_CCTransitionFadeUp_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionFadeUp_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionFadeUp(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionFadeUp_actionWithSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFadeUp_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFadeUp_CCTransitionFadeUp(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionFadeDown_class;
extern JSObject *jsb_CCTransitionFadeDown_prototype;

JSBool js_cocos2dx_CCTransitionFadeDown_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionFadeDown_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionFadeDown(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionFadeDown_actionWithSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFadeDown_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionFadeDown_CCTransitionFadeDown(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionPageTurn_class;
extern JSObject *jsb_CCTransitionPageTurn_prototype;

JSBool js_cocos2dx_CCTransitionPageTurn_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionPageTurn_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionPageTurn(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionPageTurn_actionWithSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionPageTurn_initWithDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionPageTurn_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionPageTurn_CCTransitionPageTurn(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionProgress_class;
extern JSObject *jsb_CCTransitionProgress_prototype;

JSBool js_cocos2dx_CCTransitionProgress_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionProgress_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionProgress(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionProgress_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTransitionProgress_CCTransitionProgress(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionProgressRadialCCW_class;
extern JSObject *jsb_CCTransitionProgressRadialCCW_prototype;

JSBool js_cocos2dx_CCTransitionProgressRadialCCW_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionProgressRadialCCW_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionProgressRadialCCW(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionProgressRadialCCW_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionProgressRadialCW_class;
extern JSObject *jsb_CCTransitionProgressRadialCW_prototype;

JSBool js_cocos2dx_CCTransitionProgressRadialCW_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionProgressRadialCW_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionProgressRadialCW(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionProgressRadialCW_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionProgressHorizontal_class;
extern JSObject *jsb_CCTransitionProgressHorizontal_prototype;

JSBool js_cocos2dx_CCTransitionProgressHorizontal_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionProgressHorizontal_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionProgressHorizontal(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionProgressHorizontal_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionProgressVertical_class;
extern JSObject *jsb_CCTransitionProgressVertical_prototype;

JSBool js_cocos2dx_CCTransitionProgressVertical_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionProgressVertical_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionProgressVertical(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionProgressVertical_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionProgressInOut_class;
extern JSObject *jsb_CCTransitionProgressInOut_prototype;

JSBool js_cocos2dx_CCTransitionProgressInOut_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionProgressInOut_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionProgressInOut(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionProgressInOut_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTransitionProgressOutIn_class;
extern JSObject *jsb_CCTransitionProgressOutIn_prototype;

JSBool js_cocos2dx_CCTransitionProgressOutIn_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTransitionProgressOutIn_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTransitionProgressOutIn(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTransitionProgressOutIn_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCMenuItem_class;
extern JSObject *jsb_CCMenuItem_prototype;

JSBool js_cocos2dx_CCMenuItem_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCMenuItem_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCMenuItem(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCMenuItem_setEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItem_activate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItem_unregisterScriptTapHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItem_isEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItem_selected(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItem_getScriptTapHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItem_isSelected(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItem_registerScriptTapHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItem_unselected(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItem_rect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItem_CCMenuItem(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCMenuItemLabel_class;
extern JSObject *jsb_CCMenuItemLabel_prototype;

JSBool js_cocos2dx_CCMenuItemLabel_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCMenuItemLabel_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCMenuItemLabel(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCMenuItemLabel_setEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemLabel_setLabel(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemLabel_activate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemLabel_getDisabledColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemLabel_setString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemLabel_selected(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemLabel_setDisabledColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemLabel_getLabel(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemLabel_unselected(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemLabel_CCMenuItemLabel(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCMenuItemAtlasFont_class;
extern JSObject *jsb_CCMenuItemAtlasFont_prototype;

JSBool js_cocos2dx_CCMenuItemAtlasFont_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCMenuItemAtlasFont_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCMenuItemAtlasFont(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCMenuItemAtlasFont_CCMenuItemAtlasFont(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCMenuItemFont_class;
extern JSObject *jsb_CCMenuItemFont_prototype;

JSBool js_cocos2dx_CCMenuItemFont_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCMenuItemFont_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCMenuItemFont(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCMenuItemFont_setFontNameObj(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemFont_fontNameObj(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemFont_setFontSizeObj(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemFont_fontSizeObj(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemFont_setFontName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemFont_fontName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemFont_fontSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemFont_setFontSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemFont_CCMenuItemFont(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCMenuItemSprite_class;
extern JSObject *jsb_CCMenuItemSprite_prototype;

JSBool js_cocos2dx_CCMenuItemSprite_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCMenuItemSprite_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCMenuItemSprite(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCMenuItemSprite_setEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemSprite_selected(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemSprite_setNormalImage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemSprite_setDisabledImage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemSprite_setSelectedImage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemSprite_getDisabledImage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemSprite_getSelectedImage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemSprite_getNormalImage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemSprite_unselected(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemSprite_CCMenuItemSprite(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCMenuItemImage_class;
extern JSObject *jsb_CCMenuItemImage_prototype;

JSBool js_cocos2dx_CCMenuItemImage_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCMenuItemImage_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCMenuItemImage(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCMenuItemImage_setDisabledSpriteFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemImage_setSelectedSpriteFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemImage_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemImage_setNormalSpriteFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemImage_CCMenuItemImage(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCMenuItemToggle_class;
extern JSObject *jsb_CCMenuItemToggle_prototype;

JSBool js_cocos2dx_CCMenuItemToggle_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCMenuItemToggle_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCMenuItemToggle(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCMenuItemToggle_setSubItems(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemToggle_initWithItem(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemToggle_setSelectedIndex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemToggle_setEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemToggle_getSelectedIndex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemToggle_addSubItem(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemToggle_selected(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemToggle_activate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemToggle_unselected(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemToggle_selectedItem(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenuItemToggle_CCMenuItemToggle(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCMenu_class;
extern JSObject *jsb_CCMenu_prototype;

JSBool js_cocos2dx_CCMenu_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCMenu_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCMenu(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCMenu_initWithArray(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_addChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_alignItemsVertically(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_ccTouchBegan(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_ccTouchEnded(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_isOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_isEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_setOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_setHandlerPriority(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_alignItemsHorizontallyWithPadding(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_alignItemsHorizontally(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_setEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_ccTouchMoved(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_ccTouchCancelled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_removeChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_alignItemsVerticallyWithPadding(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_registerWithTouchDispatcher(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMenu_CCMenu(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCClippingNode_class;
extern JSObject *jsb_CCClippingNode_prototype;

JSBool js_cocos2dx_CCClippingNode_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCClippingNode_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCClippingNode(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCClippingNode_setInverted(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCClippingNode_visit(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCClippingNode_setStencil(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCClippingNode_getAlphaThreshold(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCClippingNode_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCClippingNode_getStencil(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCClippingNode_setAlphaThreshold(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCClippingNode_isInverted(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCClippingNode_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCMotionStreak_class;
extern JSObject *jsb_CCMotionStreak_prototype;

JSBool js_cocos2dx_CCMotionStreak_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCMotionStreak_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCMotionStreak(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCMotionStreak_reset(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_setTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_isOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_getTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_tintWithColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_setOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_initWithFade(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_isFastMode(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_setOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_getOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_setPosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_setStartingPositionInitialized(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_setFastMode(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_isStartingPositionInitialized(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCMotionStreak_CCMotionStreak(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCProgressTimer_class;
extern JSObject *jsb_CCProgressTimer_prototype;

JSBool js_cocos2dx_CCProgressTimer_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCProgressTimer_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCProgressTimer(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCProgressTimer_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_isReverseDirection(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_setBarChangeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_setColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_getBarChangeRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_getPercentage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_setSprite(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_getType(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_setOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_getSprite(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_setMidpoint(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_getMidpoint(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_getColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_getOpacity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_setReverseDirection(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_initWithSprite(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_setPercentage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_setType(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCProgressTimer_CCProgressTimer(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCRenderTexture_class;
extern JSObject *jsb_CCRenderTexture_prototype;

JSBool js_cocos2dx_CCRenderTexture_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCRenderTexture_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCRenderTexture(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCRenderTexture_clearStencil(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_begin(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_listenToForeground(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_getClearDepth(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_getClearStencil(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_end(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_setClearStencil(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_initWithWidthAndHeight(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_visit(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_getSprite(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_isAutoDraw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_setClearFlags(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_saveToFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_setAutoDraw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_setClearColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_endToLua(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_beginWithClear(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_clearDepth(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_getClearColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_listenToBackground(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_clear(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_getClearFlags(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_newCCImage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_setClearDepth(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_setSprite(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCRenderTexture_CCRenderTexture(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleBatchNode_class;
extern JSObject *jsb_CCParticleBatchNode_prototype;

JSBool js_cocos2dx_CCParticleBatchNode_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleBatchNode_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleBatchNode(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleBatchNode_removeChildAtIndex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_addChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_draw(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_setTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_initWithFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_disableParticle(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_getTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_visit(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_removeAllChildrenWithCleanup(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_getTextureAtlas(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_removeChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_insertChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_initWithTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_reorderChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_setTextureAtlas(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_createWithTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleBatchNode_CCParticleBatchNode(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleSystem_class;
extern JSObject *jsb_CCParticleSystem_prototype;

JSBool js_cocos2dx_CCParticleSystem_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleSystem_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleSystem(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleSystem_getStartSizeVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_isFull(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getBatchNode(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getStartColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getPositionType(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setPosVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getEndSpin(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setRotatePerSecondVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getStartSpinVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getEndSpinVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_stopSystem(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getEndSizeVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setRotation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setTangentialAccel(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setScaleY(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setScaleX(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getRadialAccel(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setStartRadius(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setRotatePerSecond(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setEndSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getGravity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getTangentialAccel(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setEndRadius(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getAngle(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getSpeed(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setEndColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setStartSpin(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_initWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getPosVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_updateWithNoTime(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_isBlendAdditive(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getAngleVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setPositionType(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getEndRadius(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getSourcePosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setLifeVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setEndColorVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_updateQuadWithParticle(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getAtlasIndex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getStartSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setStartSpinVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_resetSystem(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setAtlasIndex(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setTangentialAccelVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setEndRadiusVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_isActive(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setRadialAccelVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setStartSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setSpeed(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getStartSpin(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getRotatePerSecond(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_initParticle(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setEmitterMode(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getDuration(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setSourcePosition(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getRadialAccelVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setBlendAdditive(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setLife(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setAngleVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setRotationIsDir(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setEndSizeVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setAngle(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setBatchNode(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getTangentialAccelVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getEmitterMode(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setEndSpinVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_initWithFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getSpeedVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setStartColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getRotatePerSecondVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getEndSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getLife(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setSpeedVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setAutoRemoveOnFinish(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setGravity(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_postStep(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setEmissionRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getEndColorVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getRotationIsDir(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setScale(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getEmissionRate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getEndColor(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getLifeVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setStartSizeVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_addParticle(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getOpacityModifyRGB(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getStartRadius(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getParticleCount(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getStartRadiusVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setStartColorVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setEndSpin(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setRadialAccel(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_initWithDictionary(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_isAutoRemoveOnFinish(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_setStartRadiusVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getEndRadiusVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_getStartColorVar(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystem_CCParticleSystem(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleSystemQuad_class;
extern JSObject *jsb_CCParticleSystemQuad_prototype;

JSBool js_cocos2dx_CCParticleSystemQuad_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleSystemQuad_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleSystemQuad(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleSystemQuad_initTexCoordsWithRect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystemQuad_setTextureWithRect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystemQuad_initIndices(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystemQuad_setDisplayFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystemQuad_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystemQuad_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSystemQuad_CCParticleSystemQuad(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleFire_class;
extern JSObject *jsb_CCParticleFire_prototype;

JSBool js_cocos2dx_CCParticleFire_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleFire_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleFire(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleFire_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleFire_initWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleFire_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleFire_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleFire_CCParticleFire(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleFireworks_class;
extern JSObject *jsb_CCParticleFireworks_prototype;

JSBool js_cocos2dx_CCParticleFireworks_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleFireworks_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleFireworks(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleFireworks_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleFireworks_initWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleFireworks_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleFireworks_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleFireworks_CCParticleFireworks(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleSun_class;
extern JSObject *jsb_CCParticleSun_prototype;

JSBool js_cocos2dx_CCParticleSun_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleSun_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleSun(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleSun_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSun_initWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSun_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSun_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSun_CCParticleSun(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleGalaxy_class;
extern JSObject *jsb_CCParticleGalaxy_prototype;

JSBool js_cocos2dx_CCParticleGalaxy_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleGalaxy_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleGalaxy(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleGalaxy_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleGalaxy_initWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleGalaxy_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleGalaxy_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleGalaxy_CCParticleGalaxy(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleFlower_class;
extern JSObject *jsb_CCParticleFlower_prototype;

JSBool js_cocos2dx_CCParticleFlower_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleFlower_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleFlower(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleFlower_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleFlower_initWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleFlower_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleFlower_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleFlower_CCParticleFlower(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleMeteor_class;
extern JSObject *jsb_CCParticleMeteor_prototype;

JSBool js_cocos2dx_CCParticleMeteor_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleMeteor_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleMeteor(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleMeteor_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleMeteor_initWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleMeteor_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleMeteor_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleMeteor_CCParticleMeteor(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleSpiral_class;
extern JSObject *jsb_CCParticleSpiral_prototype;

JSBool js_cocos2dx_CCParticleSpiral_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleSpiral_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleSpiral(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleSpiral_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSpiral_initWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSpiral_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSpiral_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSpiral_CCParticleSpiral(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleExplosion_class;
extern JSObject *jsb_CCParticleExplosion_prototype;

JSBool js_cocos2dx_CCParticleExplosion_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleExplosion_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleExplosion(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleExplosion_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleExplosion_initWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleExplosion_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleExplosion_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleExplosion_CCParticleExplosion(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleSmoke_class;
extern JSObject *jsb_CCParticleSmoke_prototype;

JSBool js_cocos2dx_CCParticleSmoke_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleSmoke_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleSmoke(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleSmoke_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSmoke_initWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSmoke_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSmoke_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSmoke_CCParticleSmoke(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleSnow_class;
extern JSObject *jsb_CCParticleSnow_prototype;

JSBool js_cocos2dx_CCParticleSnow_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleSnow_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleSnow(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleSnow_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSnow_initWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSnow_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSnow_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleSnow_CCParticleSnow(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParticleRain_class;
extern JSObject *jsb_CCParticleRain_prototype;

JSBool js_cocos2dx_CCParticleRain_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParticleRain_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParticleRain(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParticleRain_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleRain_initWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleRain_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleRain_createWithTotalParticles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParticleRain_CCParticleRain(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCFileUtils_class;
extern JSObject *jsb_CCFileUtils_prototype;

JSBool js_cocos2dx_CCFileUtils_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCFileUtils_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCFileUtils(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCFileUtils_isFileExist(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_removeSearchPath(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_purgeCachedEntries(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_fullPathFromRelativeFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_getFileData(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_isPopupNotify(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_setFilenameLookupDictionary(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_addSearchResolutionsOrder(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_getFileDataFromZip(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_removeAllPaths(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_fullPathForFilename(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_isAbsolutePath(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_getWritablePath(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_addSearchPath(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_setPopupNotify(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_loadFilenameLookupDictionaryFromFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_purgeFileUtils(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCFileUtils_sharedFileUtils(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCApplication_class;
extern JSObject *jsb_CCApplication_prototype;

JSBool js_cocos2dx_CCApplication_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCApplication_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCApplication(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCApplication_getTargetPlatform(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCApplication_setAnimationInterval(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCApplication_getCurrentLanguage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCApplication_sharedApplication(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEGLViewProtocol_class;
extern JSObject *jsb_CCEGLViewProtocol_prototype;

JSBool js_cocos2dx_CCEGLViewProtocol_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEGLViewProtocol_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEGLViewProtocol(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEGLViewProtocol_getVisibleOrigin(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEGLViewProtocol_setDesignResolutionSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEGLViewProtocol_getVisibleSize(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCEGLView_class;
extern JSObject *jsb_CCEGLView_prototype;

JSBool js_cocos2dx_CCEGLView_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCEGLView_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCEGLView(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCEGLView_setIMEKeyboardState(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEGLView_isOpenGLReady(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCEGLView_sharedOpenGLView(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCShaderCache_class;
extern JSObject *jsb_CCShaderCache_prototype;

JSBool js_cocos2dx_CCShaderCache_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCShaderCache_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCShaderCache(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCShaderCache_reloadDefaultShaders(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShaderCache_addProgram(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShaderCache_programForKey(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShaderCache_loadDefaultShaders(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShaderCache_sharedShaderCache(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShaderCache_purgeSharedShaderCache(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCShaderCache_CCShaderCache(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCAnimationCache_class;
extern JSObject *jsb_CCAnimationCache_prototype;

JSBool js_cocos2dx_CCAnimationCache_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCAnimationCache_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCAnimationCache(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCAnimationCache_animationByName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationCache_addAnimationsWithFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationCache_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationCache_addAnimationsWithDictionary(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationCache_removeAnimationByName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationCache_addAnimation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationCache_purgeSharedAnimationCache(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationCache_sharedAnimationCache(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCAnimationCache_CCAnimationCache(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCSpriteFrameCache_class;
extern JSObject *jsb_CCSpriteFrameCache_prototype;

JSBool js_cocos2dx_CCSpriteFrameCache_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCSpriteFrameCache_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCSpriteFrameCache(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCSpriteFrameCache_addSpriteFramesWithFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrameCache_addSpriteFrame(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrameCache_removeUnusedSpriteFrames(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrameCache_spriteFrameByName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrameCache_removeSpriteFramesFromFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrameCache_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrameCache_removeSpriteFrames(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrameCache_removeSpriteFramesFromTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrameCache_removeSpriteFrameByName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrameCache_purgeSharedSpriteFrameCache(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCSpriteFrameCache_sharedSpriteFrameCache(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTextFieldTTF_class;
extern JSObject *jsb_CCTextFieldTTF_prototype;

JSBool js_cocos2dx_CCTextFieldTTF_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTextFieldTTF_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTextFieldTTF(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTextFieldTTF_getCharCount(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_getDelegate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_setSecureTextEntry(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_getString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_getColorSpaceHolder(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_initWithPlaceHolder(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_getPlaceHolder(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_setColorSpaceHolder(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_detachWithIME(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_setDelegate(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_setPlaceHolder(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_isSecureTextEntry(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_setString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_attachWithIME(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_textFieldWithPlaceHolder(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextFieldTTF_CCTextFieldTTF(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTextureCache_class;
extern JSObject *jsb_CCTextureCache_prototype;

JSBool js_cocos2dx_CCTextureCache_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTextureCache_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTextureCache(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTextureCache_dumpCachedTextureInfo(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_reloadTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_addETCImage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_addUIImage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_removeTextureForKey(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_textureForKey(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_snapshotTextures(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_addPVRImage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_addImage(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_removeAllTextures(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_removeUnusedTextures(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_removeTexture(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_purgeSharedTextureCache(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_reloadAllTextures(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_sharedTextureCache(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTextureCache_CCTextureCache(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCParallaxNode_class;
extern JSObject *jsb_CCParallaxNode_prototype;

JSBool js_cocos2dx_CCParallaxNode_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCParallaxNode_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCParallaxNode(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCParallaxNode_addChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParallaxNode_visit(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParallaxNode_removeAllChildrenWithCleanup(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParallaxNode_removeChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParallaxNode_getParallaxArray(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParallaxNode_setParallaxArray(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParallaxNode_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCParallaxNode_CCParallaxNode(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTMXObjectGroup_class;
extern JSObject *jsb_CCTMXObjectGroup_prototype;

JSBool js_cocos2dx_CCTMXObjectGroup_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTMXObjectGroup_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTMXObjectGroup(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTMXObjectGroup_setPositionOffset(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXObjectGroup_objectNamed(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXObjectGroup_getPositionOffset(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXObjectGroup_getObjects(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXObjectGroup_setGroupName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXObjectGroup_getProperties(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXObjectGroup_getGroupName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXObjectGroup_setProperties(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXObjectGroup_propertyNamed(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXObjectGroup_setObjects(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXObjectGroup_CCTMXObjectGroup(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTMXLayerInfo_class;
extern JSObject *jsb_CCTMXLayerInfo_prototype;

JSBool js_cocos2dx_CCTMXLayerInfo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTMXLayerInfo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTMXLayerInfo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTMXLayerInfo_setProperties(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayerInfo_getProperties(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayerInfo_CCTMXLayerInfo(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTMXTilesetInfo_class;
extern JSObject *jsb_CCTMXTilesetInfo_prototype;

JSBool js_cocos2dx_CCTMXTilesetInfo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTMXTilesetInfo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTMXTilesetInfo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTMXTilesetInfo_rectForGID(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTilesetInfo_CCTMXTilesetInfo(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTMXMapInfo_class;
extern JSObject *jsb_CCTMXMapInfo_prototype;

JSBool js_cocos2dx_CCTMXMapInfo_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTMXMapInfo_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTMXMapInfo(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTMXMapInfo_getTileProperties(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setObjectGroups(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setTileSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_initWithTMXFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getOrientation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setTMXFileName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setLayers(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setStoringCharacters(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getStoringCharacters(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getParentElement(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getLayerAttribs(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getLayers(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getTilesets(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getParentGID(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setParentElement(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setProperties(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setParentGID(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_parseXMLString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getTileSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getObjectGroups(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setLayerAttribs(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getTMXFileName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setCurrentString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_initWithXML(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setOrientation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setTileProperties(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setMapSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_parseXMLFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getMapSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_setTilesets(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getProperties(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_getCurrentString(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_formatWithTMXFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_formatWithXML(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXMapInfo_CCTMXMapInfo(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTMXLayer_class;
extern JSObject *jsb_CCTMXLayer_prototype;

JSBool js_cocos2dx_CCTMXLayer_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTMXLayer_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTMXLayer(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTMXLayer_addChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_tileGIDAt(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_positionAt(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_setLayerOrientation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_releaseMap(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_setTiles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_getLayerSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_setMapTileSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_getLayerOrientation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_setProperties(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_setLayerName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_removeTileAt(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_initWithTilesetInfo(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_setupTiles(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_setTileGID(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_getMapTileSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_propertyNamed(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_setLayerSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_getLayerName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_setTileSet(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_removeChild(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_getTileSet(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_getProperties(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_tileAt(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXLayer_CCTMXLayer(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTMXTiledMap_class;
extern JSObject *jsb_CCTMXTiledMap_prototype;

JSBool js_cocos2dx_CCTMXTiledMap_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTMXTiledMap_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTMXTiledMap(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTMXTiledMap_setObjectGroups(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_setTileSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_setMapSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_getTileSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_propertiesForGID(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_getObjectGroups(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_initWithXML(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_initWithTMXFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_objectGroupNamed(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_getMapSize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_getProperties(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_setMapOrientation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_setProperties(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_layerNamed(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_getMapOrientation(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_propertyNamed(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_createWithXML(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTMXTiledMap_CCTMXTiledMap(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTileMapAtlas_class;
extern JSObject *jsb_CCTileMapAtlas_prototype;

JSBool js_cocos2dx_CCTileMapAtlas_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTileMapAtlas_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTileMapAtlas(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTileMapAtlas_initWithTileFile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTileMapAtlas_releaseMap(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTileMapAtlas_getTGAInfo(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTileMapAtlas_tileAt(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTileMapAtlas_setTile(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTileMapAtlas_setTGAInfo(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTileMapAtlas_create(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTileMapAtlas_CCTileMapAtlas(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCTimer_class;
extern JSObject *jsb_CCTimer_prototype;

JSBool js_cocos2dx_CCTimer_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCTimer_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCTimer(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCTimer_getInterval(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTimer_setInterval(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTimer_initWithScriptHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTimer_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTimer_getScriptHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTimer_timerWithScriptHandler(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCTimer_CCTimer(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCScheduler_class;
extern JSObject *jsb_CCScheduler_prototype;

JSBool js_cocos2dx_CCScheduler_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCScheduler_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCScheduler(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCScheduler_setTimeScale(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCScheduler_getTimeScale(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCScheduler_CCScheduler(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_CCComponent_class;
extern JSObject *jsb_CCComponent_prototype;

JSBool js_cocos2dx_CCComponent_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_CCComponent_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_CCComponent(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_CCComponent_setEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCComponent_setName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCComponent_isEnabled(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCComponent_serialize(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCComponent_update(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCComponent_getOwner(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCComponent_init(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCComponent_setOwner(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCComponent_getName(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_CCComponent_create(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_SimpleAudioEngine_class;
extern JSObject *jsb_SimpleAudioEngine_prototype;

JSBool js_cocos2dx_SimpleAudioEngine_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_cocos2dx_SimpleAudioEngine_finalize(JSContext *cx, JSObject *obj);
void js_register_cocos2dx_SimpleAudioEngine(JSContext *cx, JSObject *global);
void register_all_cocos2dx(JSContext* cx, JSObject* obj);
JSBool js_cocos2dx_SimpleAudioEngine_stopAllEffects(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_getBackgroundMusicVolume(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_isBackgroundMusicPlaying(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_getEffectsVolume(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_setBackgroundMusicVolume(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_stopEffect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_stopBackgroundMusic(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_playBackgroundMusic(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_pauseAllEffects(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_preloadBackgroundMusic(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_resumeBackgroundMusic(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_playEffect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_rewindBackgroundMusic(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_willPlayBackgroundMusic(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_unloadEffect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_preloadEffect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_setEffectsVolume(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_pauseEffect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_resumeAllEffects(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_pauseBackgroundMusic(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_resumeEffect(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_end(JSContext *cx, uint32_t argc, jsval *vp);
JSBool js_cocos2dx_SimpleAudioEngine_sharedEngine(JSContext *cx, uint32_t argc, jsval *vp);
#endif

