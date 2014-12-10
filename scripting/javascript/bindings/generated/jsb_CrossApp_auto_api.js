/**
 * @module CrossApp
 */
var ca = ca || {};

/**
 * @class CAImage
 */
ca.Image = {

/**
 * @method getShaderProgram
 * @return A value converted from C/C++ "CrossApp::CAGLProgram*"
 */
getShaderProgram : function () {},

/**
 * @method initWithETCFile
 * @return A value converted from C/C++ "bool"
 * @param {const char*}
 */
initWithETCFile : function () {},

/**
 * @method stringForFormat
 * @return A value converted from C/C++ "const char*"
 */
stringForFormat : function () {},

/**
 * @method initWithImage
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CCImage*}
 */
initWithImage : function () {},

/**
 * @method setShaderProgram
 * @param {CrossApp::CAGLProgram*}
 */
setShaderProgram : function () {},

/**
 * @method getMaxS
 * @return A value converted from C/C++ "float"
 */
getMaxS : function () {},

/**
 * @method getAspectRatio
 * @return A value converted from C/C++ "float"
 */
getAspectRatio : function () {},

/**
 * @method releaseData
 * @param {void*}
 */
releaseData : function () {},

/**
 * @method hasPremultipliedAlpha
 * @return A value converted from C/C++ "bool"
 */
hasPremultipliedAlpha : function () {},

/**
 * @method getPixelsHigh
 * @return A value converted from C/C++ "unsigned int"
 */
getPixelsHigh : function () {},

/**
 * @method setTexParameters
 * @param {ccTexParams*}
 */
setTexParameters : function () {},

/**
 * @method getPixelsWide
 * @return A value converted from C/C++ "unsigned int"
 */
getPixelsWide : function () {},

/**
 * @method getName
 * @return A value converted from C/C++ "unsigned int"
 */
getName : function () {},

/**
 * @method setMaxT
 * @param {float}
 */
setMaxT : function () {},

/**
 * @method drawInRect
 * @param {CrossApp::CCRect}
 */
drawInRect : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "CrossApp::CCSize"
 */
getContentSize : function () {},

/**
 * @method getMaxT
 * @return A value converted from C/C++ "float"
 */
getMaxT : function () {},

/**
 * @method getData
 * @return A value converted from C/C++ "unsigned char*"
 */
getData : function () {},

/**
 * @method setAliasTexParameters
 */
setAliasTexParameters : function () {},

/**
 * @method setAntiAliasTexParameters
 */
setAntiAliasTexParameters : function () {},

/**
 * @method generateMipmap
 */
generateMipmap : function () {},

/**
 * @method saveToFile
 * @return A value converted from C/C++ "bool"
 * @param {std::string}
 */
saveToFile : function () {},

/**
 * @method description
 * @return A value converted from C/C++ "const char*"
 */
description : function () {},

/**
 * @method isMonochrome
 * @return A value converted from C/C++ "bool"
 */
isMonochrome : function () {},

/**
 * @method getPixelFormat
 * @return A value converted from C/C++ "CrossApp::CAImagePixelFormat"
 */
getPixelFormat : function () {},

/**
 * @method keepData
 * @return A value converted from C/C++ "void*"
 * @param {void*}
 * @param {unsigned int}
 */
keepData : function () {},

/**
 * @method getContentSizeInPixels
 * @return A value converted from C/C++ "CrossApp::CCSize"
 */
getContentSizeInPixels : function () {},

/**
 * @method copy
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
copy : function () {},

/**
 * @method getImageFileType
 * @return A value converted from C/C++ "const char*"
 */
getImageFileType : function () {},

/**
 * @method drawAtPoint
 * @param {CrossApp::CCPoint}
 */
drawAtPoint : function () {},

/**
 * @method hasMipmaps
 * @return A value converted from C/C++ "bool"
 */
hasMipmaps : function () {},

/**
 * @method getDataLenght
 * @return A value converted from C/C++ "unsigned long"
 */
getDataLenght : function () {},

/**
 * @method setMaxS
 * @param {float}
 */
setMaxS : function () {},

/**
 * @method CC_WHITE_IMAGE
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
CC_WHITE_IMAGE : function () {},

/**
 * @method getFontHeight
 * @return A value converted from C/C++ "int"
 * @param {const char*}
 * @param {unsigned long}
 */
getFontHeight : function () {},

/**
 * @method createWithDataNoCache
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 * @param {void*}
 * @param {int}
 */
createWithDataNoCache : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 * @param {std::string}
 */
create : function () {},

/**
 * @method cutStringByWidth
 * @return A value converted from C/C++ "int"
 * @param {const char*}
 * @param {unsigned long}
 * @param {std::string}
 * @param {int}
 * @param {}
 */
cutStringByWidth : function () {},

/**
 * @method setDefaultAlphaPixelFormat
 * @param {CrossApp::CAImagePixelFormat}
 */
setDefaultAlphaPixelFormat : function () {},

/**
 * @method getStringHeight
 * @return A value converted from C/C++ "int"
 * @param {const char*}
 * @param {unsigned long}
 * @param {std::string}
 * @param {int}
 * @param {int}
 * @param {bool}
 */
getStringHeight : function () {},

/**
 * @method createWithData
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 * @param {void*}
 * @param {int}
 * @param {std::string}
 */
createWithData : function () {},

/**
 * @method getStringWidth
 * @return A value converted from C/C++ "int"
 * @param {const char*}
 * @param {unsigned long}
 * @param {std::string}
 */
getStringWidth : function () {},

/**
 * @method defaultAlphaPixelFormat
 * @return A value converted from C/C++ "CrossApp::CAImagePixelFormat"
 */
defaultAlphaPixelFormat : function () {},

/**
 * @method createWithString
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 * @param {const char*}
 * @param {const char*}
 * @param {float}
 * @param {CrossApp::CCSize}
 * @param {CrossApp::CATextAlignment}
 * @param {CrossApp::CAVerticalTextAlignment}
 * @param {bool}
 * @param {int}
 * @param {bool}
 * @param {bool}
 * @param {bool}
 */
createWithString : function () {},

/**
 * @method CAImage
 * @constructor
 */
CAImage : function () {},

};

/**
 * @class CAResponder
 */
ca.Responder = {

/**
 * @method isTouchMovedListenHorizontal
 * @return A value converted from C/C++ "bool"
 */
isTouchMovedListenHorizontal : function () {},

/**
 * @method isSlidingMinY
 * @return A value converted from C/C++ "bool"
 */
isSlidingMinY : function () {},

/**
 * @method isSlidingMinX
 * @return A value converted from C/C++ "bool"
 */
isSlidingMinX : function () {},

/**
 * @method resignFirstResponder
 * @return A value converted from C/C++ "bool"
 */
resignFirstResponder : function () {},

/**
 * @method isSlidingMaxY
 * @return A value converted from C/C++ "bool"
 */
isSlidingMaxY : function () {},

/**
 * @method setTouchMovedListenHorizontal
 * @param {bool}
 */
setTouchMovedListenHorizontal : function () {},

/**
 * @method isFirstResponder
 * @return A value converted from C/C++ "bool"
 */
isFirstResponder : function () {},

/**
 * @method isTouchEnabled
 * @return A value converted from C/C++ "bool"
 */
isTouchEnabled : function () {},

/**
 * @method isTouchMovedStopSubviews
 * @return A value converted from C/C++ "bool"
 */
isTouchMovedStopSubviews : function () {},

/**
 * @method isSlidingMaxX
 * @return A value converted from C/C++ "bool"
 */
isSlidingMaxX : function () {},

/**
 * @method ccTouchCancelled
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchCancelled : function () {},

/**
 * @method setHaveNextResponder
 * @param {bool}
 */
setHaveNextResponder : function () {},

/**
 * @method becomeFirstResponder
 * @return A value converted from C/C++ "bool"
 */
becomeFirstResponder : function () {},

/**
 * @method ccTouchEnded
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchEnded : function () {},

/**
 * @method setTouchMovedListenVertical
 * @param {bool}
 */
setTouchMovedListenVertical : function () {},

/**
 * @method isTouchMovedListenVertical
 * @return A value converted from C/C++ "bool"
 */
isTouchMovedListenVertical : function () {},

/**
 * @method ccTouchMoved
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchMoved : function () {},

/**
 * @method setTouchEnabled
 * @param {bool}
 */
setTouchEnabled : function () {},

/**
 * @method ccTouchBegan
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchBegan : function () {},

/**
 * @method isHaveNextResponder
 * @return A value converted from C/C++ "bool"
 */
isHaveNextResponder : function () {},

};

/**
 * @class CAView
 */
ca.View = {

/**
 * @method removeFromSuperview
 */
removeFromSuperview : function () {},

/**
 * @method nodeToWorldTransform
 * @return A value converted from C/C++ "CrossApp::CATransformation"
 */
nodeToWorldTransform : function () {},

/**
 * @method getShaderProgram
 * @return A value converted from C/C++ "CrossApp::CAGLProgram*"
 */
getShaderProgram : function () {},

/**
 * @method getUserObject
 * @return A value converted from C/C++ "CrossApp::CAObject*"
 */
getUserObject : function () {},

/**
 * @method ccTouchEnded
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchEnded : function () {},

/**
 * @method getSuperview
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
getSuperview : function () {},

/**
 * @method sortAllSubviews
 */
sortAllSubviews : function () {},

/**
 * @method copy
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
copy : function () {},

/**
 * @method convertToWorldSpaceAR
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 * @param {CrossApp::CCPoint}
 */
convertToWorldSpaceAR : function () {},

/**
 * @method updateDisplayedAlpha
 * @param {float}
 */
updateDisplayedAlpha : function () {},

/**
 * @method isFlipX
 * @return A value converted from C/C++ "bool"
 */
isFlipX : function () {},

/**
 * @method isFlipY
 * @return A value converted from C/C++ "bool"
 */
isFlipY : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method ccTouchMoved
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchMoved : function () {},

/**
 * @method setRotation
 * @param {float}
 */
setRotation : function () {},

/**
 * @method setZOrder
 * @param {int}
 */
setZOrder : function () {},

/**
 * @method ccTouchBegan
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchBegan : function () {},

/**
 * @method setScaleY
 * @param {float}
 */
setScaleY : function () {},

/**
 * @method setScaleX
 * @param {float}
 */
setScaleX : function () {},

/**
 * @method removeSubviewByTag
 * @param {int}
 */
removeSubviewByTag : function () {},

/**
 * @method getSubviewByTag
 * @return A value converted from C/C++ "CrossApp::CAView*"
 * @param {int}
 */
getSubviewByTag : function () {},

/**
 * @method setCenter
 * @param {CrossApp::CCRect}
 */
setCenter : function () {},

/**
 * @method convertRectToWorldSpace
 * @return A value converted from C/C++ "CrossApp::CCRect"
 * @param {CrossApp::CCRect}
 */
convertRectToWorldSpace : function () {},

/**
 * @method onExit
 */
onExit : function () {},

/**
 * @method setSuperview
 * @param {CrossApp::CAView*}
 */
setSuperview : function () {},

/**
 * @method setRotationX
 * @param {float}
 */
setRotationX : function () {},

/**
 * @method convertToWorldSpace
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 * @param {CrossApp::CCPoint}
 */
convertToWorldSpace : function () {},

/**
 * @method getAlpha
 * @return A value converted from C/C++ "float"
 */
getAlpha : function () {},

/**
 * @method setSkewX
 * @param {float}
 */
setSkewX : function () {},

/**
 * @method setSkewY
 * @param {float}
 */
setSkewY : function () {},

/**
 * @method removeSubview
 * @param {CrossApp::CAView*}
 */
removeSubview : function () {},

/**
 * @method onEnterTransitionDidFinish
 */
onEnterTransitionDidFinish : function () {},

/**
 * @method removeAllSubviews
 */
removeAllSubviews : function () {},

/**
 * @method convertTouchToNodeSpace
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 * @param {CrossApp::CATouch*}
 */
convertTouchToNodeSpace : function () {},

/**
 * @method reViewlayout
 */
reViewlayout : function () {},

/**
 * @method getRotationX
 * @return A value converted from C/C++ "float"
 */
getRotationX : function () {},

/**
 * @method getRotationY
 * @return A value converted from C/C++ "float"
 */
getRotationY : function () {},

/**
 * @method convertRectToNodeSpace
 * @return A value converted from C/C++ "CrossApp::CCRect"
 * @param {CrossApp::CCRect}
 */
convertRectToNodeSpace : function () {},

/**
 * @method setUserObject
 * @param {CrossApp::CAObject*}
 */
setUserObject : function () {},

/**
 * @method getUserData
 * @return A value converted from C/C++ "void*"
 */
getUserData : function () {},

/**
 * @method getFrame
 * @return A value converted from C/C++ "CrossApp::CCRect"
 */
getFrame : function () {},

/**
 * @method getImageRect
 * @return A value converted from C/C++ "CrossApp::CCRect"
 */
getImageRect : function () {},

/**
 * @method getAtlasIndex
 * @return A value converted from C/C++ "unsigned int"
 */
getAtlasIndex : function () {},

/**
 * @method setBatch
 * @param {CrossApp::CABatchView*}
 */
setBatch : function () {},

/**
 * @method nodeToParentTransform
 * @return A value converted from C/C++ "CrossApp::CATransformation"
 */
nodeToParentTransform : function () {},

/**
 * @method nextResponder
 * @return A value converted from C/C++ "CrossApp::CAResponder*"
 */
nextResponder : function () {},

/**
 * @method numberOfRunningActions
 * @return A value converted from C/C++ "unsigned int"
 */
numberOfRunningActions : function () {},

/**
 * @method getFrameOrigin
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 */
getFrameOrigin : function () {},

/**
 * @method setDirty
 * @param {bool}
 */
setDirty : function () {},

/**
 * @method stopActionByTag
 * @param {int}
 */
stopActionByTag : function () {},

/**
 * @method setGLServerState
 * @param {CrossApp::ccGLServerState}
 */
setGLServerState : function () {},

/**
 * @method isImageRectRotated
 * @return A value converted from C/C++ "bool"
 */
isImageRectRotated : function () {},

/**
 * @method setColor
 * @param {CrossApp::CAColor4B}
 */
setColor : function () {},

/**
 * @method getDisplayedColor
 * @return A value converted from C/C++ "CrossApp::CAColor4B"
 */
getDisplayedColor : function () {},

/**
 * @method getAnchorPoint
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 */
getAnchorPoint : function () {},

/**
 * @method updateTransform
 */
updateTransform : function () {},

/**
 * @method setViewDelegate
 * @param {CrossApp::CAViewDelegate*}
 */
setViewDelegate : function () {},

/**
 * @method getAnchorPointInPoints
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 */
getAnchorPointInPoints : function () {},

/**
 * @method setAnchorPoint
 * @param {CrossApp::CCPoint}
 */
setAnchorPoint : function () {},

/**
 * @method onEnter
 */
onEnter : function () {},

/**
 * @method convertToNodeSpaceAR
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 * @param {CrossApp::CCPoint}
 */
convertToNodeSpaceAR : function () {},

/**
 * @method setAtlasIndex
 * @param {unsigned int}
 */
setAtlasIndex : function () {},

/**
 * @method visit
 */
visit : function () {},

/**
 * @method getCenterOrigin
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 */
getCenterOrigin : function () {},

/**
 * @method setShaderProgram
 * @param {CrossApp::CAGLProgram*}
 */
setShaderProgram : function () {},

/**
 * @method getRotation
 * @return A value converted from C/C++ "float"
 */
getRotation : function () {},

/**
 * @method setCenterOrigin
 * @param {CrossApp::CCPoint}
 */
setCenterOrigin : function () {},

/**
 * @method getZOrder
 * @return A value converted from C/C++ "int"
 */
getZOrder : function () {},

/**
 * @method getSubviewsCount
 * @return A value converted from C/C++ "unsigned int"
 */
getSubviewsCount : function () {},

/**
 * @method runAction
 * @return A value converted from C/C++ "CrossApp::CCAction*"
 * @param {CrossApp::CCAction*}
 */
runAction : function () {},

/**
 * @method transform
 */
transform : function () {},

/**
 * @method setVertexZ
 * @param {float}
 */
setVertexZ : function () {},

/**
 * @method stopAllActions
 */
stopAllActions : function () {},

/**
 * @method getSkewX
 * @return A value converted from C/C++ "float"
 */
getSkewX : function () {},

/**
 * @method getSkewY
 * @return A value converted from C/C++ "float"
 */
getSkewY : function () {},

/**
 * @method getActionByTag
 * @return A value converted from C/C++ "CrossApp::CCAction*"
 * @param {int}
 */
getActionByTag : function () {},

/**
 * @method setBounds
 * @param {CrossApp::CCRect}
 */
setBounds : function () {},

/**
 * @method setRotationY
 * @param {float}
 */
setRotationY : function () {},

/**
 * @method insertSubview
 * @param {CrossApp::CAView*}
 * @param {int}
 */
insertSubview : function () {},

/**
 * @method setAdditionalTransform
 * @param {CrossApp::CATransformation}
 */
setAdditionalTransform : function () {},

/**
 * @method setFlipY
 * @param {bool}
 */
setFlipY : function () {},

/**
 * @method setFlipX
 * @param {bool}
 */
setFlipX : function () {},

/**
 * @method getCenter
 * @return A value converted from C/C++ "CrossApp::CCRect"
 */
getCenter : function () {},

/**
 * @method isDirty
 * @return A value converted from C/C++ "bool"
 */
isDirty : function () {},

/**
 * @method getViewDelegate
 * @return A value converted from C/C++ "CrossApp::CAViewDelegate*"
 */
getViewDelegate : function () {},

/**
 * @method getOrderOfArrival
 * @return A value converted from C/C++ "unsigned int"
 */
getOrderOfArrival : function () {},

/**
 * @method setActionManager
 * @param {CrossApp::CCActionManager*}
 */
setActionManager : function () {},

/**
 * @method isRunning
 * @return A value converted from C/C++ "bool"
 */
isRunning : function () {},

/**
 * @method setBlendFunc
 * @param {ccBlendFunc}
 */
setBlendFunc : function () {},

/**
 * @method getQuad
 * @return A value converted from C/C++ "ccV3F_C4B_T2F_Quad"
 */
getQuad : function () {},

/**
 * @method updateDisplayedColor
 * @param {CrossApp::CAColor4B}
 */
updateDisplayedColor : function () {},

/**
 * @method setVisible
 * @param {bool}
 */
setVisible : function () {},

/**
 * @method reorderSubview
 * @param {CrossApp::CAView*}
 * @param {int}
 */
reorderSubview : function () {},

/**
 * @method getVertexZ
 * @return A value converted from C/C++ "float"
 */
getVertexZ : function () {},

/**
 * @method _setZOrder
 * @param {int}
 */
_setZOrder : function () {},

/**
 * @method isDisplayRange
 * @return A value converted from C/C++ "bool"
 */
isDisplayRange : function () {},

/**
 * @method isFrame
 * @return A value converted from C/C++ "bool"
 */
isFrame : function () {},

/**
 * @method getDisplayedAlpha
 * @return A value converted from C/C++ "float"
 */
getDisplayedAlpha : function () {},

/**
 * @method setOrderOfArrival
 * @param {unsigned int}
 */
setOrderOfArrival : function () {},

/**
 * @method getImageAtlas
 * @return A value converted from C/C++ "CrossApp::CAImageAtlas*"
 */
getImageAtlas : function () {},

/**
 * @method ccTouchCancelled
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchCancelled : function () {},

/**
 * @method setFrameOrigin
 * @param {CrossApp::CCPoint}
 */
setFrameOrigin : function () {},

/**
 * @method getScaleY
 * @return A value converted from C/C++ "float"
 */
getScaleY : function () {},

/**
 * @method getScaleX
 * @return A value converted from C/C++ "float"
 */
getScaleX : function () {},

/**
 * @method addSubview
 * @param {CrossApp::CAView*}
 */
addSubview : function () {},

/**
 * @method setAnchorPointInPoints
 * @param {CrossApp::CCPoint}
 */
setAnchorPointInPoints : function () {},

/**
 * @method setDisplayRange
 * @param {bool}
 */
setDisplayRange : function () {},

/**
 * @method setAlpha
 * @param {float}
 */
setAlpha : function () {},

/**
 * @method getSubviews
 * @return A value converted from C/C++ "CrossApp::CCArray*"
 */
getSubviews : function () {},

/**
 * @method draw
 */
draw : function () {},

/**
 * @method getBlendFunc
 * @return A value converted from C/C++ "ccBlendFunc"
 */
getBlendFunc : function () {},

/**
 * @method transformAncestors
 */
transformAncestors : function () {},

/**
 * @method description
 * @return A value converted from C/C++ "const char*"
 */
description : function () {},

/**
 * @method convertTouchToNodeSpaceAR
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 * @param {CrossApp::CATouch*}
 */
convertTouchToNodeSpaceAR : function () {},

/**
 * @method isVisible
 * @return A value converted from C/C++ "bool"
 */
isVisible : function () {},

/**
 * @method update
 * @param {float}
 */
update : function () {},

/**
 * @method convertToNodeSpace
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 * @param {CrossApp::CCPoint}
 */
convertToNodeSpace : function () {},

/**
 * @method onExitTransitionDidStart
 */
onExitTransitionDidStart : function () {},

/**
 * @method getBounds
 * @return A value converted from C/C++ "CrossApp::CCRect"
 */
getBounds : function () {},

/**
 * @method getScale
 * @return A value converted from C/C++ "float"
 */
getScale : function () {},

/**
 * @method worldToNodeTransform
 * @return A value converted from C/C++ "CrossApp::CATransformation"
 */
worldToNodeTransform : function () {},

/**
 * @method setImageAtlas
 * @param {CrossApp::CAImageAtlas*}
 */
setImageAtlas : function () {},

/**
 * @method setUserData
 * @param {void*}
 */
setUserData : function () {},

/**
 * @method initWithColor
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CAColor4B}
 */
initWithColor : function () {},

/**
 * @method setFrame
 * @param {CrossApp::CCRect}
 */
setFrame : function () {},

/**
 * @method parentToNodeTransform
 * @return A value converted from C/C++ "CrossApp::CATransformation"
 */
parentToNodeTransform : function () {},

/**
 * @method getCamera
 * @return A value converted from C/C++ "CrossApp::CACamera*"
 */
getCamera : function () {},

/**
 * @method getBatch
 * @return A value converted from C/C++ "CrossApp::CABatchView*"
 */
getBatch : function () {},

/**
 * @method getColor
 * @return A value converted from C/C++ "CrossApp::CAColor4B"
 */
getColor : function () {},

/**
 * @method getGLServerState
 * @return A value converted from C/C++ "CrossApp::ccGLServerState"
 */
getGLServerState : function () {},

/**
 * @method stopAction
 * @param {CrossApp::CCAction*}
 */
stopAction : function () {},

/**
 * @method getActionManager
 * @return A value converted from C/C++ "CrossApp::CCActionManager*"
 */
getActionManager : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
create : function () {},

/**
 * @method createWithColor
 * @return A value converted from C/C++ "CrossApp::CAView*"
 * @param {CrossApp::CAColor4B}
 */
createWithColor : function () {},

/**
 * @method CAView
 * @constructor
 */
CAView : function () {},

};

/**
 * @class CALabel
 */
ca.Label = {

/**
 * @method setTextAlignment
 * @param {CrossApp::CATextAlignment}
 */
setTextAlignment : function () {},

/**
 * @method setDimensions
 * @param {CrossApp::CCSize}
 */
setDimensions : function () {},

/**
 * @method getFontSize
 * @return A value converted from C/C++ "unsigned int"
 */
getFontSize : function () {},

/**
 * @method getItalics
 * @return A value converted from C/C++ "bool"
 */
getItalics : function () {},

/**
 * @method sizeToFit
 */
sizeToFit : function () {},

/**
 * @method getBold
 * @return A value converted from C/C++ "bool"
 */
getBold : function () {},

/**
 * @method getText
 * @return A value converted from C/C++ "std::string"
 */
getText : function () {},

/**
 * @method initWithCenter
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CCRect}
 */
initWithCenter : function () {},

/**
 * @method setUnderLine
 * @param {bool}
 */
setUnderLine : function () {},

/**
 * @method setFontName
 * @param {std::string}
 */
setFontName : function () {},

/**
 * @method setLineSpacing
 * @param {int}
 */
setLineSpacing : function () {},

/**
 * @method setBold
 * @param {bool}
 */
setBold : function () {},

/**
 * @method visit
 */
visit : function () {},

/**
 * @method getLabelSize
 * @return A value converted from C/C++ "CrossApp::CCSize"
 */
getLabelSize : function () {},

/**
 * @method setWordWrap
 * @param {bool}
 */
setWordWrap : function () {},

/**
 * @method getFontName
 * @return A value converted from C/C++ "std::string"
 */
getFontName : function () {},

/**
 * @method getLineSpacing
 * @return A value converted from C/C++ "int"
 */
getLineSpacing : function () {},

/**
 * @method setVerticalTextAlignmet
 * @param {CrossApp::CAVerticalTextAlignment}
 */
setVerticalTextAlignmet : function () {},

/**
 * @method getUnderLine
 * @return A value converted from C/C++ "bool"
 */
getUnderLine : function () {},

/**
 * @method getVerticalTextAlignmet
 * @return A value converted from C/C++ "CrossApp::CAVerticalTextAlignment"
 */
getVerticalTextAlignmet : function () {},

/**
 * @method initWithFrame
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CCRect}
 */
initWithFrame : function () {},

/**
 * @method setItalics
 * @param {bool}
 */
setItalics : function () {},

/**
 * @method getDimensions
 * @return A value converted from C/C++ "CrossApp::CCSize"
 */
getDimensions : function () {},

/**
 * @method getNumberOfLine
 * @return A value converted from C/C++ "unsigned int"
 */
getNumberOfLine : function () {},

/**
 * @method setFontSize
 * @param {unsigned int}
 */
setFontSize : function () {},

/**
 * @method unsizeToFit
 */
unsizeToFit : function () {},

/**
 * @method setText
 * @param {std::string}
 */
setText : function () {},

/**
 * @method getWordWrap
 * @return A value converted from C/C++ "bool"
 */
getWordWrap : function () {},

/**
 * @method getTextAlignment
 * @return A value converted from C/C++ "CrossApp::CATextAlignment"
 */
getTextAlignment : function () {},

/**
 * @method setNumberOfLine
 * @param {unsigned int}
 */
setNumberOfLine : function () {},

/**
 * @method onEnterTransitionDidFinish
 */
onEnterTransitionDidFinish : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CrossApp::CALabel*"
 * @param {CrossApp::CCRect}
 */
createWithFrame : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CrossApp::CALabel*"
 * @param {CrossApp::CCRect}
 */
createWithCenter : function () {},

/**
 * @method CALabel
 * @constructor
 */
CALabel : function () {},

};

/**
 * @class CAApplication
 */
ca.Application = {

/**
 * @method setDelegate
 * @param {CrossApp::CAApplicationDelegate*}
 */
setDelegate : function () {},

/**
 * @method setContentScaleFactor
 * @param {float}
 */
setContentScaleFactor : function () {},

/**
 * @method getContentScaleFactor
 * @return A value converted from C/C++ "float"
 */
getContentScaleFactor : function () {},

/**
 * @method getWinSizeInPixels
 * @return A value converted from C/C++ "CrossApp::CCSize"
 */
getWinSizeInPixels : function () {},

/**
 * @method getDeltaTime
 * @return A value converted from C/C++ "float"
 */
getDeltaTime : function () {},

/**
 * @method setGLDefaultValues
 */
setGLDefaultValues : function () {},

/**
 * @method setActionManager
 * @param {CrossApp::CCActionManager*}
 */
setActionManager : function () {},

/**
 * @method setAlphaBlending
 * @param {bool}
 */
setAlphaBlending : function () {},

/**
 * @method updateDraw
 */
updateDraw : function () {},

/**
 * @method getKeypadDispatcher
 * @return A value converted from C/C++ "CrossApp::CAKeypadDispatcher*"
 */
getKeypadDispatcher : function () {},

/**
 * @method getNotificationNode
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
getNotificationNode : function () {},

/**
 * @method getWinSize
 * @return A value converted from C/C++ "CrossApp::CCSize"
 */
getWinSize : function () {},

/**
 * @method end
 */
end : function () {},

/**
 * @method runWindow
 * @param {CrossApp::CAWindow*}
 */
runWindow : function () {},

/**
 * @method isSendCleanupToScene
 * @return A value converted from C/C++ "bool"
 */
isSendCleanupToScene : function () {},

/**
 * @method getVisibleOrigin
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 */
getVisibleOrigin : function () {},

/**
 * @method mainLoop
 */
mainLoop : function () {},

/**
 * @method setDepthTest
 * @param {bool}
 */
setDepthTest : function () {},

/**
 * @method getSecondsPerFrame
 * @return A value converted from C/C++ "float"
 */
getSecondsPerFrame : function () {},

/**
 * @method convertToUI
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 * @param {CrossApp::CCPoint}
 */
convertToUI : function () {},

/**
 * @method setAccelerometer
 * @param {CrossApp::CCAccelerometer*}
 */
setAccelerometer : function () {},

/**
 * @method setDefaultValues
 */
setDefaultValues : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method reshapeProjection
 * @param {CrossApp::CCSize}
 */
reshapeProjection : function () {},

/**
 * @method startAnimation
 */
startAnimation : function () {},

/**
 * @method getAdaptationRatio
 * @return A value converted from C/C++ "float"
 */
getAdaptationRatio : function () {},

/**
 * @method setViewport
 */
setViewport : function () {},

/**
 * @method stopAnimation
 */
stopAnimation : function () {},

/**
 * @method setKeypadDispatcher
 * @param {CrossApp::CAKeypadDispatcher*}
 */
setKeypadDispatcher : function () {},

/**
 * @method run
 * @param {float}
 */
run : function () {},

/**
 * @method resume
 */
resume : function () {},

/**
 * @method setTouchDispatcher
 * @param {CrossApp::CATouchDispatcher*}
 */
setTouchDispatcher : function () {},

/**
 * @method isNextDeltaTimeZero
 * @return A value converted from C/C++ "bool"
 */
isNextDeltaTimeZero : function () {},

/**
 * @method getDelegate
 * @return A value converted from C/C++ "CrossApp::CAApplicationDelegate*"
 */
getDelegate : function () {},

/**
 * @method setOpenGLView
 * @param {CrossApp::CCEGLView*}
 */
setOpenGLView : function () {},

/**
 * @method convertToGL
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 * @param {CrossApp::CCPoint}
 */
convertToGL : function () {},

/**
 * @method purgeCachedData
 */
purgeCachedData : function () {},

/**
 * @method getTotalFrames
 * @return A value converted from C/C++ "unsigned int"
 */
getTotalFrames : function () {},

/**
 * @method pause
 */
pause : function () {},

/**
 * @method setNotificationNode
 * @param {CrossApp::CAView*}
 */
setNotificationNode : function () {},

/**
 * @method drawScene
 */
drawScene : function () {},

/**
 * @method isDisplayStats
 * @return A value converted from C/C++ "bool"
 */
isDisplayStats : function () {},

/**
 * @method setProjection
 * @param {CrossApp::ccDirectorProjection}
 */
setProjection : function () {},

/**
 * @method getTouchDispatcher
 * @return A value converted from C/C++ "CrossApp::CATouchDispatcher*"
 */
getTouchDispatcher : function () {},

/**
 * @method getZEye
 * @return A value converted from C/C++ "float"
 */
getZEye : function () {},

/**
 * @method setNextDeltaTimeZero
 * @param {bool}
 */
setNextDeltaTimeZero : function () {},

/**
 * @method getVisibleSize
 * @return A value converted from C/C++ "CrossApp::CCSize"
 */
getVisibleSize : function () {},

/**
 * @method getRootWindow
 * @return A value converted from C/C++ "CrossApp::CAWindow*"
 */
getRootWindow : function () {},

/**
 * @method getAnimationInterval
 * @return A value converted from C/C++ "double"
 */
getAnimationInterval : function () {},

/**
 * @method isPaused
 * @return A value converted from C/C++ "bool"
 */
isPaused : function () {},

/**
 * @method setDisplayStats
 * @param {bool}
 */
setDisplayStats : function () {},

/**
 * @method setAnimationInterval
 * @param {double}
 */
setAnimationInterval : function () {},

/**
 * @method getActionManager
 * @return A value converted from C/C++ "CrossApp::CCActionManager*"
 */
getActionManager : function () {},

/**
 * @method getApplication
 * @return A value converted from C/C++ "CrossApp::CAApplication*"
 */
getApplication : function () {},

};

/**
 * @class CCTimer
 */
ca.CCTimer = {

/**
 * @method getInterval
 * @return A value converted from C/C++ "float"
 */
getInterval : function () {},

/**
 * @method setInterval
 * @param {float}
 */
setInterval : function () {},

/**
 * @method initWithScriptHandler
 * @return A value converted from C/C++ "bool"
 * @param {int}
 * @param {float}
 */
initWithScriptHandler : function () {},

/**
 * @method update
 * @param {float}
 */
update : function () {},

/**
 * @method getScriptHandler
 * @return A value converted from C/C++ "int"
 */
getScriptHandler : function () {},

/**
 * @method timerWithScriptHandler
 * @return A value converted from C/C++ "CrossApp::CCTimer*"
 * @param {int}
 * @param {float}
 */
timerWithScriptHandler : function () {},

/**
 * @method CCTimer
 * @constructor
 */
CCTimer : function () {},

};

/**
 * @class CAScheduler
 */
ca.Scheduler = {

/**
 * @method getScheduler
 * @return A value converted from C/C++ "CrossApp::CAScheduler*"
 */
getScheduler : function () {},

};

/**
 * @class CAControl
 */
ca.Control = {

/**
 * @method setControlStateSelected
 */
setControlStateSelected : function () {},

/**
 * @method setControlStateHighlighted
 */
setControlStateHighlighted : function () {},

/**
 * @method getControlState
 * @return A value converted from C/C++ "CrossApp::CAControlState"
 */
getControlState : function () {},

/**
 * @method setControlStateNormal
 */
setControlStateNormal : function () {},

/**
 * @method setControlState
 * @param {CrossApp::CAControlState}
 */
setControlState : function () {},

/**
 * @method setControlStateLocked
 * @param {bool}
 */
setControlStateLocked : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method getControlStateLocked
 * @return A value converted from C/C++ "bool"
 */
getControlStateLocked : function () {},

/**
 * @method setControlStateDisabled
 */
setControlStateDisabled : function () {},

/**
 * @method CAControl
 * @constructor
 */
CAControl : function () {},

};

/**
 * @class CAButton
 */
ca.Button = {

/**
 * @method getBackGroundViewForState
 * @return A value converted from C/C++ "CrossApp::CAView*"
 * @param {CrossApp::CAControlState}
 */
getBackGroundViewForState : function () {},

/**
 * @method setBackGroundViewForState
 * @param {CrossApp::CAControlState}
 * @param {CrossApp::CAView*}
 */
setBackGroundViewForState : function () {},

/**
 * @method ccTouchBegan
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchBegan : function () {},

/**
 * @method ccTouchEnded
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchEnded : function () {},

/**
 * @method setTitleForState
 * @param {CrossApp::CAControlState}
 * @param {std::string}
 */
setTitleForState : function () {},

/**
 * @method isAllowsSelected
 * @return A value converted from C/C++ "bool"
 */
isAllowsSelected : function () {},

/**
 * @method isTouchClick
 * @return A value converted from C/C++ "bool"
 */
isTouchClick : function () {},

/**
 * @method isSelected
 * @return A value converted from C/C++ "bool"
 */
isSelected : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method setImageColorForState
 * @param {CrossApp::CAControlState}
 * @param {CrossApp::CAColor4B}
 */
setImageColorForState : function () {},

/**
 * @method ccTouchMoved
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchMoved : function () {},

/**
 * @method isCloseTapSound
 * @return A value converted from C/C++ "bool"
 */
isCloseTapSound : function () {},

/**
 * @method setTitleFontName
 * @param {std::string}
 */
setTitleFontName : function () {},

/**
 * @method onExitTransitionDidStart
 */
onExitTransitionDidStart : function () {},

/**
 * @method setTitleColorForState
 * @param {CrossApp::CAControlState}
 * @param {CrossApp::CAColor4B}
 */
setTitleColorForState : function () {},

/**
 * @method setCloseTapSound
 * @param {bool}
 */
setCloseTapSound : function () {},

/**
 * @method interruptTouchState
 */
interruptTouchState : function () {},

/**
 * @method setControlState
 * @param {CrossApp::CAControlState}
 */
setControlState : function () {},

/**
 * @method setImageForState
 * @param {CrossApp::CAControlState}
 * @param {CrossApp::CAImage*}
 */
setImageForState : function () {},

/**
 * @method ccTouchCancelled
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchCancelled : function () {},

/**
 * @method setAllowsSelected
 * @param {bool}
 */
setAllowsSelected : function () {},

/**
 * @method onEnterTransitionDidFinish
 */
onEnterTransitionDidFinish : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "CrossApp::CAButton*"
 * @param {CrossApp::CAButtonType}
 */
create : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CrossApp::CAButton*"
 * @param {CrossApp::CCRect}
 * @param {CrossApp::CAButtonType}
 */
createWithFrame : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CrossApp::CAButton*"
 * @param {CrossApp::CCRect}
 * @param {CrossApp::CAButtonType}
 */
createWithCenter : function () {},

};

/**
 * @class CABarItem
 */
ca.BarItem = {

/**
 * @method setTouchEnabled
 * @param {bool}
 */
setTouchEnabled : function () {},

/**
 * @method setImage
 * @param {CrossApp::CAImage*}
 */
setImage : function () {},

/**
 * @method setTitle
 * @param {std::string}
 */
setTitle : function () {},

/**
 * @method isTouchEnabled
 * @return A value converted from C/C++ "bool"
 */
isTouchEnabled : function () {},

/**
 * @method getTitle
 * @return A value converted from C/C++ "std::string"
 */
getTitle : function () {},

/**
 * @method getImage
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
getImage : function () {},

/**
 * @method CABarItem
 * @constructor
 */
CABarItem : function () {},

};

/**
 * @class CATabBarItem
 */
ca.TabBarItem = {

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 * @param {std::string}
 * @param {CrossApp::CAImage*}
 * @param {CrossApp::CAImage*}
 */
init : function () {},

/**
 * @method setSelectedImage
 * @param {CrossApp::CAImage*}
 */
setSelectedImage : function () {},

/**
 * @method getSelectedImage
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
getSelectedImage : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "CrossApp::CATabBarItem*"
 * @param {std::string}
 * @param {CrossApp::CAImage*}
 * @param {CrossApp::CAImage*}
 */
create : function () {},

/**
 * @method CATabBarItem
 * @constructor
 */
CATabBarItem : function () {},

};

/**
 * @class CAImageView
 */
ca.ImageView = {

/**
 * @method copy
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
copy : function () {},

/**
 * @method setImage
 * @param {CrossApp::CAImage*}
 */
setImage : function () {},

/**
 * @method initWithImage
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CAImage*}
 */
initWithImage : function () {},

/**
 * @method setImageAsyncWithFile
 * @param {std::string}
 */
setImageAsyncWithFile : function () {},

/**
 * @method getImageViewScaleType
 * @return A value converted from C/C++ "CrossApp::CAImageViewScaleType"
 */
getImageViewScaleType : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method setImageViewScaleType
 * @param {CrossApp::CAImageViewScaleType}
 */
setImageViewScaleType : function () {},

/**
 * @method getImage
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
getImage : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CrossApp::CAImageView*"
 * @param {CrossApp::CCRect}
 */
createWithFrame : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "CrossApp::CAImageView*"
 */
create : function () {},

/**
 * @method createWithImage
 * @return A value converted from C/C++ "CrossApp::CAImageView*"
 * @param {CrossApp::CAImage*}
 */
createWithImage : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CrossApp::CAImageView*"
 * @param {CrossApp::CCRect}
 */
createWithCenter : function () {},

/**
 * @method CAImageView
 * @constructor
 */
CAImageView : function () {},

};

/**
 * @class CAScale9ImageView
 */
ca.Scale9ImageView = {

/**
 * @method setInsetBottom
 * @param {float}
 */
setInsetBottom : function () {},

/**
 * @method getOriginalSize
 * @return A value converted from C/C++ "CrossApp::CCSize"
 */
getOriginalSize : function () {},

/**
 * @method draw
 */
draw : function () {},

/**
 * @method getImage
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
getImage : function () {},

/**
 * @method setColor
 * @param {CrossApp::CAColor4B}
 */
setColor : function () {},

/**
 * @method setImage
 * @param {CrossApp::CAImage*}
 */
setImage : function () {},

/**
 * @method updateDisplayedAlpha
 * @param {float}
 */
updateDisplayedAlpha : function () {},

/**
 * @method initWithImage
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CAImage*}
 */
initWithImage : function () {},

/**
 * @method getInsetTop
 * @return A value converted from C/C++ "float"
 */
getInsetTop : function () {},

/**
 * @method setInsetLeft
 * @param {float}
 */
setInsetLeft : function () {},

/**
 * @method setCapInsets
 * @param {CrossApp::CCRect}
 */
setCapInsets : function () {},

/**
 * @method getCapInsets
 * @return A value converted from C/C++ "CrossApp::CCRect"
 */
getCapInsets : function () {},

/**
 * @method setInsetTop
 * @param {float}
 */
setInsetTop : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method getInsetBottom
 * @return A value converted from C/C++ "float"
 */
getInsetBottom : function () {},

/**
 * @method getColor
 * @return A value converted from C/C++ "CrossApp::CAColor4B"
 */
getColor : function () {},

/**
 * @method getInsetLeft
 * @return A value converted from C/C++ "float"
 */
getInsetLeft : function () {},

/**
 * @method setInsetRight
 * @param {float}
 */
setInsetRight : function () {},

/**
 * @method copy
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
copy : function () {},

/**
 * @method getInsetRight
 * @return A value converted from C/C++ "float"
 */
getInsetRight : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CrossApp::CAScale9ImageView*"
 * @param {CrossApp::CCRect}
 */
createWithFrame : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "CrossApp::CAScale9ImageView*"
 */
create : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CrossApp::CAScale9ImageView*"
 * @param {CrossApp::CCRect}
 */
createWithCenter : function () {},

/**
 * @method CAScale9ImageView
 * @constructor
 */
CAScale9ImageView : function () {},

};

/**
 * @class CAProgress
 */
ca.Progress = {

/**
 * @method setProgressTintImage
 * @param {CrossApp::CAImage*}
 */
setProgressTintImage : function () {},

/**
 * @method getProgressTintImage
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
getProgressTintImage : function () {},

/**
 * @method setProgresstrackColor
 * @param {CrossApp::CAColor4B}
 */
setProgresstrackColor : function () {},

/**
 * @method getProgresstrackColor
 * @return A value converted from C/C++ "CrossApp::CAColor4B"
 */
getProgresstrackColor : function () {},

/**
 * @method getProgressStyle
 * @return A value converted from C/C++ "CrossApp::CAProgressStyle"
 */
getProgressStyle : function () {},

/**
 * @method getProgressTintColor
 * @return A value converted from C/C++ "CrossApp::CAColor4B"
 */
getProgressTintColor : function () {},

/**
 * @method getProgressTrackImage
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
getProgressTrackImage : function () {},

/**
 * @method setProgressTrackImage
 * @param {CrossApp::CAImage*}
 */
setProgressTrackImage : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method onExitTransitionDidStart
 */
onExitTransitionDidStart : function () {},

/**
 * @method getProgress
 * @return A value converted from C/C++ "float"
 */
getProgress : function () {},

/**
 * @method setProgress
 * @param {float}
 * @param {bool}
 */
setProgress : function () {},

/**
 * @method setProgressStyle
 * @param {CrossApp::CAProgressStyle}
 */
setProgressStyle : function () {},

/**
 * @method onEnterTransitionDidFinish
 */
onEnterTransitionDidFinish : function () {},

/**
 * @method setProgressTintColor
 * @param {CrossApp::CAColor4B}
 */
setProgressTintColor : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "CrossApp::CAProgress*"
 */
create : function () {},

/**
 * @method CAProgress
 * @constructor
 */
CAProgress : function () {},

};

/**
 * @class CASwitch
 */
ca.Switch = {

/**
 * @method getOnImage
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
getOnImage : function () {},

/**
 * @method setOffImage
 * @param {CrossApp::CAImage*}
 */
setOffImage : function () {},

/**
 * @method isTouchClick
 * @return A value converted from C/C++ "bool"
 */
isTouchClick : function () {},

/**
 * @method initWithFrame
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CCRect}
 */
initWithFrame : function () {},

/**
 * @method getThumbTintImage
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
getThumbTintImage : function () {},

/**
 * @method setIsOn
 * @param {bool}
 * @param {bool}
 */
setIsOn : function () {},

/**
 * @method initWithCenter
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CCRect}
 */
initWithCenter : function () {},

/**
 * @method setOnImage
 * @param {CrossApp::CAImage*}
 */
setOnImage : function () {},

/**
 * @method onExitTransitionDidStart
 */
onExitTransitionDidStart : function () {},

/**
 * @method getOffImage
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
getOffImage : function () {},

/**
 * @method isOn
 * @return A value converted from C/C++ "bool"
 */
isOn : function () {},

/**
 * @method onEnterTransitionDidFinish
 */
onEnterTransitionDidFinish : function () {},

/**
 * @method setThumbTintImage
 * @param {CrossApp::CAImage*}
 */
setThumbTintImage : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CrossApp::CASwitch*"
 * @param {CrossApp::CCRect}
 */
createWithFrame : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CrossApp::CASwitch*"
 * @param {CrossApp::CCRect}
 */
createWithCenter : function () {},

/**
 * @method CASwitch
 * @constructor
 */
CASwitch : function () {},

};

/**
 * @class CASlider
 */
ca.Slider = {

/**
 * @method ccTouchBegan
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchBegan : function () {},

/**
 * @method initWithCenter
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CCRect}
 */
initWithCenter : function () {},

/**
 * @method ccTouchEnded
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchEnded : function () {},

/**
 * @method getTrackHeight
 * @return A value converted from C/C++ "float"
 */
getTrackHeight : function () {},

/**
 * @method setMinValue
 * @param {float}
 */
setMinValue : function () {},

/**
 * @method getMinValue
 * @return A value converted from C/C++ "float"
 */
getMinValue : function () {},

/**
 * @method ccTouchMoved
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchMoved : function () {},

/**
 * @method getMaxValue
 * @return A value converted from C/C++ "float"
 */
getMaxValue : function () {},

/**
 * @method setMaxTrackTintImage
 * @param {CrossApp::CAImage*}
 */
setMaxTrackTintImage : function () {},

/**
 * @method setValue
 * @param {float}
 */
setValue : function () {},

/**
 * @method isTouchClick
 * @return A value converted from C/C++ "bool"
 */
isTouchClick : function () {},

/**
 * @method initWithFrame
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CCRect}
 */
initWithFrame : function () {},

/**
 * @method getMaxTrackTintImage
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
getMaxTrackTintImage : function () {},

/**
 * @method setThumbTintImage
 * @param {CrossApp::CAImage*}
 */
setThumbTintImage : function () {},

/**
 * @method getMinTrackTintImage
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
getMinTrackTintImage : function () {},

/**
 * @method setTrackHeight
 * @param {float}
 */
setTrackHeight : function () {},

/**
 * @method getThumbTintImage
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 */
getThumbTintImage : function () {},

/**
 * @method setMaxValue
 * @param {float}
 */
setMaxValue : function () {},

/**
 * @method getValue
 * @return A value converted from C/C++ "float"
 */
getValue : function () {},

/**
 * @method onExitTransitionDidStart
 */
onExitTransitionDidStart : function () {},

/**
 * @method setMinTrackTintImage
 * @param {CrossApp::CAImage*}
 */
setMinTrackTintImage : function () {},

/**
 * @method onEnterTransitionDidFinish
 */
onEnterTransitionDidFinish : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CrossApp::CASlider*"
 * @param {CrossApp::CCRect}
 */
createWithFrame : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CrossApp::CASlider*"
 * @param {CrossApp::CCRect}
 */
createWithCenter : function () {},

/**
 * @method CASlider
 * @constructor
 */
CASlider : function () {},

};

/**
 * @class CATextField
 */
ca.TextField = {

/**
 * @method updateImageRect
 */
updateImageRect : function () {},

/**
 * @method getFontSize
 * @return A value converted from C/C++ "int"
 */
getFontSize : function () {},

/**
 * @method getKeyboardReturnType
 * @return A value converted from C/C++ "int"
 */
getKeyboardReturnType : function () {},

/**
 * @method setKeyboardType
 * @param {CrossApp::eKeyBoardType}
 */
setKeyboardType : function () {},

/**
 * @method getText
 * @return A value converted from C/C++ "std::string"
 */
getText : function () {},

/**
 * @method getTextColor
 * @return A value converted from C/C++ "CAColor4B"
 */
getTextColor : function () {},

/**
 * @method setImageRect
 * @param {CrossApp::CCRect}
 */
setImageRect : function () {},

/**
 * @method getVertMargins
 * @return A value converted from C/C++ "int"
 */
getVertMargins : function () {},

/**
 * @method getPlaceHolder
 * @return A value converted from C/C++ "std::string"
 */
getPlaceHolder : function () {},

/**
 * @method setKeyboardReturnType
 * @param {CrossApp::eKeyBoardReturnType}
 */
setKeyboardReturnType : function () {},

/**
 * @method setSpaceHolderColor
 * @param {CAColor4B}
 */
setSpaceHolderColor : function () {},

/**
 * @method getKeyboardType
 * @return A value converted from C/C++ "int"
 */
getKeyboardType : function () {},

/**
 * @method analyzeString
 * @param {const char*}
 * @param {int}
 */
analyzeString : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method setInputType
 * @param {CrossApp::eKeyBoardInputType}
 */
setInputType : function () {},

/**
 * @method setBackgroundView
 * @param {CrossApp::CAView*}
 */
setBackgroundView : function () {},

/**
 * @method getCharCount
 * @return A value converted from C/C++ "int"
 */
getCharCount : function () {},

/**
 * @method getInputType
 * @return A value converted from C/C++ "CrossApp::eKeyBoardInputType"
 */
getInputType : function () {},

/**
 * @method setHoriMargins
 * @param {int}
 */
setHoriMargins : function () {},

/**
 * @method setVertMargins
 * @param {int}
 */
setVertMargins : function () {},

/**
 * @method getBackgroundView
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
getBackgroundView : function () {},

/**
 * @method onExitTransitionDidStart
 */
onExitTransitionDidStart : function () {},

/**
 * @method setFontSize
 * @param {int}
 */
setFontSize : function () {},

/**
 * @method setPlaceHolder
 * @param {std::string}
 */
setPlaceHolder : function () {},

/**
 * @method setTextColor
 * @param {CAColor4B}
 */
setTextColor : function () {},

/**
 * @method getCursorColor
 * @return A value converted from C/C++ "CAColor4B"
 */
getCursorColor : function () {},

/**
 * @method resignFirstResponder
 * @return A value converted from C/C++ "bool"
 */
resignFirstResponder : function () {},

/**
 * @method setText
 * @param {std::string}
 */
setText : function () {},

/**
 * @method becomeFirstResponder
 * @return A value converted from C/C++ "bool"
 */
becomeFirstResponder : function () {},

/**
 * @method getSpaceHolderColor
 * @return A value converted from C/C++ "CAColor4B"
 */
getSpaceHolderColor : function () {},

/**
 * @method setCursorColor
 * @param {CAColor4B}
 */
setCursorColor : function () {},

/**
 * @method getHoriMargins
 * @return A value converted from C/C++ "int"
 */
getHoriMargins : function () {},

/**
 * @method onEnterTransitionDidFinish
 */
onEnterTransitionDidFinish : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CrossApp::CATextField*"
 * @param {CrossApp::CCRect}
 */
createWithFrame : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CrossApp::CATextField*"
 * @param {CrossApp::CCRect}
 */
createWithCenter : function () {},

/**
 * @method CATextField
 * @constructor
 */
CATextField : function () {},

};

/**
 * @class CAStepper
 */
ca.Stepper = {

/**
 * @method getDividerImageForState
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 * @param {CrossApp::CAControlState}
 */
getDividerImageForState : function () {},

/**
 * @method setMinValue
 * @param {double}
 */
setMinValue : function () {},

/**
 * @method getBackgroundImageForState
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 * @param {CrossApp::CAControlState}
 */
getBackgroundImageForState : function () {},

/**
 * @method initWithCenter
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CCRect}
 */
initWithCenter : function () {},

/**
 * @method setValue
 * @param {double}
 */
setValue : function () {},

/**
 * @method getContinuous
 * @return A value converted from C/C++ "bool"
 */
getContinuous : function () {},

/**
 * @method getAutoRepeat
 * @return A value converted from C/C++ "bool"
 */
getAutoRepeat : function () {},

/**
 * @method setContinuous
 * @param {bool}
 */
setContinuous : function () {},

/**
 * @method getMinValue
 * @return A value converted from C/C++ "double"
 */
getMinValue : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method getIncrementImageForState
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 * @param {CrossApp::CAControlState}
 */
getIncrementImageForState : function () {},

/**
 * @method getDecrementImageForState
 * @return A value converted from C/C++ "CrossApp::CAImage*"
 * @param {CrossApp::CAControlState}
 */
getDecrementImageForState : function () {},

/**
 * @method getMaxValue
 * @return A value converted from C/C++ "double"
 */
getMaxValue : function () {},

/**
 * @method setDecrementImage
 * @param {CrossApp::CAImage*}
 * @param {CrossApp::CAControlState}
 */
setDecrementImage : function () {},

/**
 * @method getWraps
 * @return A value converted from C/C++ "bool"
 */
getWraps : function () {},

/**
 * @method setStepValue
 * @param {double}
 */
setStepValue : function () {},

/**
 * @method initWithFrame
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CCRect}
 */
initWithFrame : function () {},

/**
 * @method setBackgroundImage
 * @param {CrossApp::CAImage*}
 * @param {CrossApp::CAControlState}
 */
setBackgroundImage : function () {},

/**
 * @method setAutoRepeat
 * @param {bool}
 */
setAutoRepeat : function () {},

/**
 * @method setDividerImage
 * @param {CrossApp::CAImage*}
 * @param {CrossApp::CAControlState}
 */
setDividerImage : function () {},

/**
 * @method setTouchEffect
 * @param {bool}
 */
setTouchEffect : function () {},

/**
 * @method setWraps
 * @param {bool}
 */
setWraps : function () {},

/**
 * @method setIncrementImage
 * @param {CrossApp::CAImage*}
 * @param {CrossApp::CAControlState}
 */
setIncrementImage : function () {},

/**
 * @method getStepValue
 * @return A value converted from C/C++ "double"
 */
getStepValue : function () {},

/**
 * @method setMaxValue
 * @param {double}
 */
setMaxValue : function () {},

/**
 * @method getValue
 * @return A value converted from C/C++ "double"
 */
getValue : function () {},

/**
 * @method visit
 */
visit : function () {},

/**
 * @method getTouchEffect
 * @return A value converted from C/C++ "bool"
 */
getTouchEffect : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "CrossApp::CAStepper*"
 */
create : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CrossApp::CAStepper*"
 * @param {CrossApp::CCRect}
 */
createWithFrame : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CrossApp::CAStepper*"
 * @param {CrossApp::CCRect}
 */
createWithCenter : function () {},

/**
 * @method CAStepper
 * @constructor
 */
CAStepper : function () {},

};

/**
 * @class CAScrollView
 */
ca.ScrollView = {

/**
 * @method setBounceVertical
 * @param {bool}
 */
setBounceVertical : function () {},

/**
 * @method removeAllSubviews
 */
removeAllSubviews : function () {},

/**
 * @method isTracking
 * @return A value converted from C/C++ "bool"
 */
isTracking : function () {},

/**
 * @method startDeaccelerateScroll
 */
startDeaccelerateScroll : function () {},

/**
 * @method isBounceVertical
 * @return A value converted from C/C++ "bool"
 */
isBounceVertical : function () {},

/**
 * @method setShowsVerticalScrollIndicator
 * @param {bool}
 */
setShowsVerticalScrollIndicator : function () {},

/**
 * @method setShowsHorizontalScrollIndicator
 * @param {bool}
 */
setShowsHorizontalScrollIndicator : function () {},

/**
 * @method setTouchEnabledAtSubviews
 * @param {bool}
 */
setTouchEnabledAtSubviews : function () {},

/**
 * @method isScrollEnabled
 * @return A value converted from C/C++ "bool"
 */
isScrollEnabled : function () {},

/**
 * @method getZoomScale
 * @return A value converted from C/C++ "float"
 */
getZoomScale : function () {},

/**
 * @method isDecelerating
 * @return A value converted from C/C++ "bool"
 */
isDecelerating : function () {},

/**
 * @method endFooterRefresh
 */
endFooterRefresh : function () {},

/**
 * @method setBackGroundImage
 * @param {CrossApp::CAImage*}
 */
setBackGroundImage : function () {},

/**
 * @method ccTouchEnded
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchEnded : function () {},

/**
 * @method setBackGroundColor
 * @param {CrossApp::CAColor4B}
 */
setBackGroundColor : function () {},

/**
 * @method addSubview
 * @param {CrossApp::CAView*}
 */
addSubview : function () {},

/**
 * @method isBounceHorizontal
 * @return A value converted from C/C++ "bool"
 */
isBounceHorizontal : function () {},

/**
 * @method setContentOffset
 * @param {CrossApp::CCPoint}
 * @param {bool}
 */
setContentOffset : function () {},

/**
 * @method setFooterRefreshView
 * @param {CrossApp::CAPullToRefreshView*}
 */
setFooterRefreshView : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method isShowsVerticalScrollIndicator
 * @return A value converted from C/C++ "bool"
 */
isShowsVerticalScrollIndicator : function () {},

/**
 * @method getSubviews
 * @return A value converted from C/C++ "CrossApp::CCArray*"
 */
getSubviews : function () {},

/**
 * @method ccTouchMoved
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchMoved : function () {},

/**
 * @method setHeaderRefreshView
 * @param {CrossApp::CAPullToRefreshView*}
 */
setHeaderRefreshView : function () {},

/**
 * @method ccTouchBegan
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchBegan : function () {},

/**
 * @method setScrollEnabled
 * @param {bool}
 */
setScrollEnabled : function () {},

/**
 * @method setBounces
 * @param {bool}
 */
setBounces : function () {},

/**
 * @method getContentOffset
 * @return A value converted from C/C++ "CrossApp::CCPoint"
 */
getContentOffset : function () {},

/**
 * @method insertSubview
 * @param {CrossApp::CAView*}
 * @param {int}
 */
insertSubview : function () {},

/**
 * @method removeSubviewByTag
 * @param {int}
 */
removeSubviewByTag : function () {},

/**
 * @method getSubviewByTag
 * @return A value converted from C/C++ "CrossApp::CAView*"
 * @param {int}
 */
getSubviewByTag : function () {},

/**
 * @method isBounces
 * @return A value converted from C/C++ "bool"
 */
isBounces : function () {},

/**
 * @method setViewSize
 * @param {CrossApp::CCSize}
 */
setViewSize : function () {},

/**
 * @method getMaximumZoomScale
 * @return A value converted from C/C++ "float"
 */
getMaximumZoomScale : function () {},

/**
 * @method isSlidingMaxX
 * @return A value converted from C/C++ "bool"
 */
isSlidingMaxX : function () {},

/**
 * @method getViewSize
 * @return A value converted from C/C++ "CrossApp::CCSize"
 */
getViewSize : function () {},

/**
 * @method setMaximumZoomScale
 * @param {float}
 */
setMaximumZoomScale : function () {},

/**
 * @method onExitTransitionDidStart
 */
onExitTransitionDidStart : function () {},

/**
 * @method isZooming
 * @return A value converted from C/C++ "bool"
 */
isZooming : function () {},

/**
 * @method isShowsHorizontalScrollIndicator
 * @return A value converted from C/C++ "bool"
 */
isShowsHorizontalScrollIndicator : function () {},

/**
 * @method getHeaderRefreshView
 * @return A value converted from C/C++ "CrossApp::CAPullToRefreshView*"
 */
getHeaderRefreshView : function () {},

/**
 * @method isSlidingMaxY
 * @return A value converted from C/C++ "bool"
 */
isSlidingMaxY : function () {},

/**
 * @method getFooterRefreshView
 * @return A value converted from C/C++ "CrossApp::CAPullToRefreshView*"
 */
getFooterRefreshView : function () {},

/**
 * @method isSlidingMinY
 * @return A value converted from C/C++ "bool"
 */
isSlidingMinY : function () {},

/**
 * @method stopDeaccelerateScroll
 */
stopDeaccelerateScroll : function () {},

/**
 * @method removeSubview
 * @param {CrossApp::CAView*}
 */
removeSubview : function () {},

/**
 * @method isSlidingMinX
 * @return A value converted from C/C++ "bool"
 */
isSlidingMinX : function () {},

/**
 * @method ccTouchCancelled
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchCancelled : function () {},

/**
 * @method isTouchEnabledAtSubviews
 * @return A value converted from C/C++ "bool"
 */
isTouchEnabledAtSubviews : function () {},

/**
 * @method setMinimumZoomScale
 * @param {float}
 */
setMinimumZoomScale : function () {},

/**
 * @method endHeaderRefresh
 */
endHeaderRefresh : function () {},

/**
 * @method getMinimumZoomScale
 * @return A value converted from C/C++ "float"
 */
getMinimumZoomScale : function () {},

/**
 * @method setBounceHorizontal
 * @param {bool}
 */
setBounceHorizontal : function () {},

/**
 * @method setZoomScale
 * @param {float}
 */
setZoomScale : function () {},

/**
 * @method onEnterTransitionDidFinish
 */
onEnterTransitionDidFinish : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CrossApp::CAScrollView*"
 * @param {CrossApp::CCRect}
 */
createWithFrame : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CrossApp::CAScrollView*"
 * @param {CrossApp::CCRect}
 */
createWithCenter : function () {},

/**
 * @method CAScrollView
 * @constructor
 */
CAScrollView : function () {},

};

/**
 * @class CAPageView
 */
ca.PageView = {

/**
 * @method setPageViewDelegate
 * @param {CAPageViewDelegate*}
 */
setPageViewDelegate : function () {},

/**
 * @method getPageViewDirection
 * @return A value converted from C/C++ "CAPageView::CAPageViewDirection"
 */
getPageViewDirection : function () {},

/**
 * @method setCurrPage
 * @param {int}
 * @param {bool}
 * @param {bool}
 */
setCurrPage : function () {},

/**
 * @method getCurrPage
 * @return A value converted from C/C++ "int"
 */
getCurrPage : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method getPageViewDelegate
 * @return A value converted from C/C++ "CAPageViewDelegate*"
 */
getPageViewDelegate : function () {},

/**
 * @method getPageCount
 * @return A value converted from C/C++ "int"
 */
getPageCount : function () {},

/**
 * @method getSubViewAtIndex
 * @return A value converted from C/C++ "CrossApp::CAView*"
 * @param {int}
 */
getSubViewAtIndex : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CAPageView*"
 * @param {CrossApp::CCRect}
 * @param {CAPageView::CAPageViewDirection}
 */
createWithFrame : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CAPageView*"
 * @param {CrossApp::CCRect}
 * @param {CAPageView::CAPageViewDirection}
 */
createWithCenter : function () {},

};

/**
 * @class CAViewController
 */
ca.ViewController = {

/**
 * @method getView
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
getView : function () {},

/**
 * @method presentModalViewController
 * @param {CrossApp::CAViewController*}
 * @param {bool}
 */
presentModalViewController : function () {},

/**
 * @method nextResponder
 * @return A value converted from C/C++ "CrossApp::CAResponder*"
 */
nextResponder : function () {},

/**
 * @method getTabBarItem
 * @return A value converted from C/C++ "CrossApp::CATabBarItem*"
 */
getTabBarItem : function () {},

/**
 * @method ccTouchCancelled
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchCancelled : function () {},

/**
 * @method ccTouchEnded
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchEnded : function () {},

/**
 * @method setTabBarItem
 * @param {CrossApp::CATabBarItem*}
 */
setTabBarItem : function () {},

/**
 * @method isKeypadEnabled
 * @return A value converted from C/C++ "bool"
 */
isKeypadEnabled : function () {},

/**
 * @method setTitle
 * @param {std::string}
 */
setTitle : function () {},

/**
 * @method isViewRunning
 * @return A value converted from C/C++ "bool"
 */
isViewRunning : function () {},

/**
 * @method getNavigationController
 * @return A value converted from C/C++ "CrossApp::CANavigationController*"
 */
getNavigationController : function () {},

/**
 * @method getTabBarController
 * @return A value converted from C/C++ "CrossApp::CATabBarController*"
 */
getTabBarController : function () {},

/**
 * @method getNibName
 * @return A value converted from C/C++ "const char*"
 */
getNibName : function () {},

/**
 * @method ccTouchMoved
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchMoved : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method getNavigationBarItem
 * @return A value converted from C/C++ "CrossApp::CANavigationBarItem*"
 */
getNavigationBarItem : function () {},

/**
 * @method getTitle
 * @return A value converted from C/C++ "std::string"
 */
getTitle : function () {},

/**
 * @method setNavigationBarItem
 * @param {CrossApp::CANavigationBarItem*}
 */
setNavigationBarItem : function () {},

/**
 * @method ccTouchBegan
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchBegan : function () {},

/**
 * @method setKeypadEnabled
 * @param {bool}
 */
setKeypadEnabled : function () {},

/**
 * @method dismissModalViewController
 * @param {bool}
 */
dismissModalViewController : function () {},

};

/**
 * @class CANavigationController
 */
ca.NavigationController = {

/**
 * @method pushViewController
 * @param {CrossApp::CAViewController*}
 * @param {bool}
 */
pushViewController : function () {},

/**
 * @method updateItem
 * @param {CrossApp::CAViewController*}
 */
updateItem : function () {},

/**
 * @method ccTouchCancelled
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchCancelled : function () {},

/**
 * @method setTouchMoved
 * @param {bool}
 */
setTouchMoved : function () {},

/**
 * @method setNavigationBarHidden
 * @param {bool}
 * @param {bool}
 */
setNavigationBarHidden : function () {},

/**
 * @method getBackViewController
 * @return A value converted from C/C++ "CrossApp::CAViewController*"
 */
getBackViewController : function () {},

/**
 * @method isNavigationBarHidden
 * @return A value converted from C/C++ "bool"
 */
isNavigationBarHidden : function () {},

/**
 * @method getNavigationBar
 * @return A value converted from C/C++ "CrossApp::CANavigationBar*"
 */
getNavigationBar : function () {},

/**
 * @method initWithRootViewController
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CAViewController*}
 * @param {CrossApp::CABarVerticalAlignment}
 */
initWithRootViewController : function () {},

/**
 * @method getViewControllerAtIndex
 * @return A value converted from C/C++ "CrossApp::CAViewController*"
 * @param {int}
 */
getViewControllerAtIndex : function () {},

/**
 * @method ccTouchEnded
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchEnded : function () {},

/**
 * @method popViewControllerAnimated
 * @return A value converted from C/C++ "CrossApp::CAViewController*"
 * @param {bool}
 */
popViewControllerAnimated : function () {},

/**
 * @method isTouchMoved
 * @return A value converted from C/C++ "bool"
 */
isTouchMoved : function () {},

/**
 * @method ccTouchMoved
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchMoved : function () {},

/**
 * @method replaceViewController
 * @param {CrossApp::CAViewController*}
 * @param {bool}
 */
replaceViewController : function () {},

/**
 * @method getNavigationBarVerticalAlignment
 * @return A value converted from C/C++ "CrossApp::CABarVerticalAlignment"
 */
getNavigationBarVerticalAlignment : function () {},

/**
 * @method ccTouchBegan
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchBegan : function () {},

/**
 * @method getViewControllerCount
 * @return A value converted from C/C++ "unsigned long"
 */
getViewControllerCount : function () {},

/**
 * @method CANavigationController
 * @constructor
 */
CANavigationController : function () {},

};

/**
 * @class CATabBarController
 */
ca.TabBarController = {

/**
 * @method setTabBarHidden
 * @param {bool}
 * @param {bool}
 */
setTabBarHidden : function () {},

/**
 * @method updateItem
 * @param {CrossApp::CAViewController*}
 */
updateItem : function () {},

/**
 * @method showSelectedViewController
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CAViewController*}
 */
showSelectedViewController : function () {},

/**
 * @method getSelectedViewController
 * @return A value converted from C/C++ "CrossApp::CAViewController*"
 */
getSelectedViewController : function () {},

/**
 * @method isTabBarHidden
 * @return A value converted from C/C++ "bool"
 */
isTabBarHidden : function () {},

/**
 * @method getViewControllerAtIndex
 * @return A value converted from C/C++ "CrossApp::CAViewController*"
 * @param {unsigned int}
 */
getViewControllerAtIndex : function () {},

/**
 * @method getSelectedViewControllerAtIndex
 * @return A value converted from C/C++ "unsigned int"
 */
getSelectedViewControllerAtIndex : function () {},

/**
 * @method getTabBar
 * @return A value converted from C/C++ "CrossApp::CATabBar*"
 */
getTabBar : function () {},

/**
 * @method isScrollEnabled
 * @return A value converted from C/C++ "bool"
 */
isScrollEnabled : function () {},

/**
 * @method getTabBarVerticalAlignment
 * @return A value converted from C/C++ "CrossApp::CABarVerticalAlignment"
 */
getTabBarVerticalAlignment : function () {},

/**
 * @method setScrollEnabled
 * @param {bool}
 */
setScrollEnabled : function () {},

/**
 * @method showSelectedViewControllerAtIndex
 * @return A value converted from C/C++ "bool"
 * @param {unsigned int}
 */
showSelectedViewControllerAtIndex : function () {},

/**
 * @method CATabBarController
 * @constructor
 */
CATabBarController : function () {},

};

/**
 * @class CADrawerController
 */
ca.DrawerController = {

/**
 * @method hideLeftViewController
 * @param {bool}
 */
hideLeftViewController : function () {},

/**
 * @method showLeftViewController
 * @param {bool}
 */
showLeftViewController : function () {},

/**
 * @method ccTouchEnded
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchEnded : function () {},

/**
 * @method ccTouchBegan
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchBegan : function () {},

/**
 * @method setTouchMoved
 * @param {bool}
 */
setTouchMoved : function () {},

/**
 * @method getRightViewController
 * @return A value converted from C/C++ "CrossApp::CAViewController*"
 */
getRightViewController : function () {},

/**
 * @method ccTouchCancelled
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchCancelled : function () {},

/**
 * @method ccTouchMoved
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchMoved : function () {},

/**
 * @method getBackgroundView
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
getBackgroundView : function () {},

/**
 * @method isShowLeftViewController
 * @return A value converted from C/C++ "bool"
 */
isShowLeftViewController : function () {},

/**
 * @method isTouchMoved
 * @return A value converted from C/C++ "bool"
 */
isTouchMoved : function () {},

/**
 * @method initWithController
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CAViewController*}
 * @param {CrossApp::CAViewController*}
 * @param {float}
 */
initWithController : function () {},

/**
 * @method replaceRightViewController
 * @param {CrossApp::CAViewController*}
 */
replaceRightViewController : function () {},

/**
 * @method getLeftViewController
 * @return A value converted from C/C++ "CrossApp::CAViewController*"
 */
getLeftViewController : function () {},

/**
 * @method setBackgroundView
 * @param {CrossApp::CAView*}
 */
setBackgroundView : function () {},

/**
 * @method CADrawerController
 * @constructor
 */
CADrawerController : function () {},

};

/**
 * @class CAWindow
 */
ca.Window = {

/**
 * @method presentModalViewController
 * @param {CrossApp::CAViewController*}
 * @param {bool}
 */
presentModalViewController : function () {},

/**
 * @method setRootViewController
 * @param {CrossApp::CAViewController*}
 */
setRootViewController : function () {},

/**
 * @method getRootViewController
 * @return A value converted from C/C++ "CrossApp::CAViewController*"
 */
getRootViewController : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method dismissModalViewController
 * @param {bool}
 */
dismissModalViewController : function () {},

/**
 * @method getModalViewController
 * @return A value converted from C/C++ "CrossApp::CAViewController*"
 */
getModalViewController : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "CrossApp::CAWindow*"
 */
create : function () {},

/**
 * @method CAWindow
 * @constructor
 */
CAWindow : function () {},

};

/**
 * @class CAListView
 */
ca.ListView = {

/**
 * @method isAllowsSelection
 * @return A value converted from C/C++ "bool"
 */
isAllowsSelection : function () {},

/**
 * @method ccTouchBegan
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchBegan : function () {},

/**
 * @method setListFooterHeight
 * @param {unsigned int}
 */
setListFooterHeight : function () {},

/**
 * @method ccTouchEnded
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchEnded : function () {},

/**
 * @method setSeparatorColor
 * @param {CrossApp::CAColor4B}
 */
setSeparatorColor : function () {},

/**
 * @method setSelectAtIndex
 * @param {unsigned int}
 */
setSelectAtIndex : function () {},

/**
 * @method setListViewOrientation
 * @param {CrossApp::CAListViewOrientation}
 */
setListViewOrientation : function () {},

/**
 * @method getSeparatorViewHeight
 * @return A value converted from C/C++ "unsigned int"
 */
getSeparatorViewHeight : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method setAllowsHeadAndFootHover
 * @param {bool}
 */
setAllowsHeadAndFootHover : function () {},

/**
 * @method getSeparatorColor
 * @return A value converted from C/C++ "CrossApp::CAColor4B"
 */
getSeparatorColor : function () {},

/**
 * @method ccTouchMoved
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchMoved : function () {},

/**
 * @method setAllowsSelection
 * @param {bool}
 */
setAllowsSelection : function () {},

/**
 * @method reloadData
 */
reloadData : function () {},

/**
 * @method setAllowsMultipleSelection
 * @param {bool}
 */
setAllowsMultipleSelection : function () {},

/**
 * @method isAllowsMultipleSelection
 * @return A value converted from C/C++ "bool"
 */
isAllowsMultipleSelection : function () {},

/**
 * @method setListHeaderView
 * @param {CrossApp::CAView*}
 */
setListHeaderView : function () {},

/**
 * @method getAllowsHeadAndFootHover
 * @return A value converted from C/C++ "bool"
 */
getAllowsHeadAndFootHover : function () {},

/**
 * @method getListFooterHeight
 * @return A value converted from C/C++ "unsigned int"
 */
getListFooterHeight : function () {},

/**
 * @method getListHeaderHeight
 * @return A value converted from C/C++ "unsigned int"
 */
getListHeaderHeight : function () {},

/**
 * @method setListHeaderHeight
 * @param {unsigned int}
 */
setListHeaderHeight : function () {},

/**
 * @method onExitTransitionDidStart
 */
onExitTransitionDidStart : function () {},

/**
 * @method getListHeaderView
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
getListHeaderView : function () {},

/**
 * @method setSeparatorViewHeight
 * @param {unsigned int}
 */
setSeparatorViewHeight : function () {},

/**
 * @method reloadViewSizeData
 */
reloadViewSizeData : function () {},

/**
 * @method getListViewOrientation
 * @return A value converted from C/C++ "CrossApp::CAListViewOrientation"
 */
getListViewOrientation : function () {},

/**
 * @method setListFooterView
 * @param {CrossApp::CAView*}
 */
setListFooterView : function () {},

/**
 * @method getListFooterView
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
getListFooterView : function () {},

/**
 * @method ccTouchCancelled
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchCancelled : function () {},

/**
 * @method setUnSelectAtIndex
 * @param {unsigned int}
 */
setUnSelectAtIndex : function () {},

/**
 * @method dequeueReusableCellWithIdentifier
 * @return A value converted from C/C++ "CrossApp::CAListViewCell*"
 * @param {const char*}
 */
dequeueReusableCellWithIdentifier : function () {},

/**
 * @method onEnterTransitionDidFinish
 */
onEnterTransitionDidFinish : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CrossApp::CAListView*"
 * @param {CrossApp::CCRect}
 */
createWithFrame : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CrossApp::CAListView*"
 * @param {CrossApp::CCRect}
 */
createWithCenter : function () {},

/**
 * @method CAListView
 * @constructor
 */
CAListView : function () {},

};

/**
 * @class CAListViewCell
 */
ca.ListViewCell = {

/**
 * @method getIndex
 * @return A value converted from C/C++ "unsigned int"
 */
getIndex : function () {},

/**
 * @method setReuseIdentifier
 * @param {std::string}
 */
setReuseIdentifier : function () {},

/**
 * @method isAllowsSelected
 * @return A value converted from C/C++ "bool"
 */
isAllowsSelected : function () {},

/**
 * @method getReuseIdentifier
 * @return A value converted from C/C++ "std::string"
 */
getReuseIdentifier : function () {},

/**
 * @method setAllowsSelected
 * @param {bool}
 */
setAllowsSelected : function () {},

/**
 * @method setControlStateEffect
 * @param {bool}
 */
setControlStateEffect : function () {},

/**
 * @method getBackgroundView
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
getBackgroundView : function () {},

/**
 * @method isControlStateEffect
 * @return A value converted from C/C++ "bool"
 */
isControlStateEffect : function () {},

/**
 * @method initWithReuseIdentifier
 * @return A value converted from C/C++ "bool"
 * @param {std::string}
 */
initWithReuseIdentifier : function () {},

/**
 * @method setBackgroundView
 * @param {CrossApp::CAView*}
 */
setBackgroundView : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "CrossApp::CAListViewCell*"
 * @param {std::string}
 */
create : function () {},

/**
 * @method CAListViewCell
 * @constructor
 */
CAListViewCell : function () {},

};

/**
 * @class CATableView
 */
ca.TableView = {

/**
 * @method getTableFooterView
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
getTableFooterView : function () {},

/**
 * @method isAllowsSelection
 * @return A value converted from C/C++ "bool"
 */
isAllowsSelection : function () {},

/**
 * @method ccTouchBegan
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchBegan : function () {},

/**
 * @method ccTouchEnded
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchEnded : function () {},

/**
 * @method setSeparatorColor
 * @param {CrossApp::CAColor4B}
 */
setSeparatorColor : function () {},

/**
 * @method setTableFooterHeight
 * @param {unsigned int}
 */
setTableFooterHeight : function () {},

/**
 * @method setTableHeaderHeight
 * @param {unsigned int}
 */
setTableHeaderHeight : function () {},

/**
 * @method setAlwaysTopSectionHeader
 * @param {bool}
 */
setAlwaysTopSectionHeader : function () {},

/**
 * @method isAlwaysBottomSectionFooter
 * @return A value converted from C/C++ "bool"
 */
isAlwaysBottomSectionFooter : function () {},

/**
 * @method getRowHeightInSectionInRow
 * @return A value converted from C/C++ "float"
 * @param {unsigned int}
 * @param {unsigned int}
 */
getRowHeightInSectionInRow : function () {},

/**
 * @method setTableHeaderView
 * @param {CrossApp::CAView*}
 */
setTableHeaderView : function () {},

/**
 * @method getNumberOfSections
 * @return A value converted from C/C++ "unsigned int"
 */
getNumberOfSections : function () {},

/**
 * @method getSeparatorViewHeight
 * @return A value converted from C/C++ "unsigned int"
 */
getSeparatorViewHeight : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method setSelectRowAtIndexPath
 * @param {unsigned int}
 * @param {unsigned int}
 */
setSelectRowAtIndexPath : function () {},

/**
 * @method getTableViewDataSource
 * @return A value converted from C/C++ "CrossApp::CATableViewDataSource*"
 */
getTableViewDataSource : function () {},

/**
 * @method getSectionHeightInSection
 * @return A value converted from C/C++ "float"
 * @param {unsigned int}
 */
getSectionHeightInSection : function () {},

/**
 * @method isAllowsMultipleSelection
 * @return A value converted from C/C++ "bool"
 */
isAllowsMultipleSelection : function () {},

/**
 * @method getSeparatorColor
 * @return A value converted from C/C++ "CrossApp::CAColor4B"
 */
getSeparatorColor : function () {},

/**
 * @method ccTouchMoved
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchMoved : function () {},

/**
 * @method setAllowsSelection
 * @param {bool}
 */
setAllowsSelection : function () {},

/**
 * @method reloadData
 */
reloadData : function () {},

/**
 * @method setAllowsMultipleSelection
 * @param {bool}
 */
setAllowsMultipleSelection : function () {},

/**
 * @method getTableFooterHeight
 * @return A value converted from C/C++ "unsigned int"
 */
getTableFooterHeight : function () {},

/**
 * @method cellForRowAtIndexPath
 * @return A value converted from C/C++ "CrossApp::CATableViewCell*"
 * @param {unsigned int}
 * @param {unsigned int}
 */
cellForRowAtIndexPath : function () {},

/**
 * @method getTableHeaderView
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
getTableHeaderView : function () {},

/**
 * @method setAlwaysBottomSectionFooter
 * @param {bool}
 */
setAlwaysBottomSectionFooter : function () {},

/**
 * @method onExitTransitionDidStart
 */
onExitTransitionDidStart : function () {},

/**
 * @method isAlwaysTopSectionHeader
 * @return A value converted from C/C++ "bool"
 */
isAlwaysTopSectionHeader : function () {},

/**
 * @method setTableFooterView
 * @param {CrossApp::CAView*}
 */
setTableFooterView : function () {},

/**
 * @method setSeparatorViewHeight
 * @param {unsigned int}
 */
setSeparatorViewHeight : function () {},

/**
 * @method setTableViewDelegate
 * @param {CrossApp::CATableViewDelegate*}
 */
setTableViewDelegate : function () {},

/**
 * @method setTableViewDataSource
 * @param {CrossApp::CATableViewDataSource*}
 */
setTableViewDataSource : function () {},

/**
 * @method ccTouchCancelled
 * @param {CrossApp::CATouch*}
 * @param {CrossApp::CAEvent*}
 */
ccTouchCancelled : function () {},

/**
 * @method getNumberOfRowsInSection
 * @return A value converted from C/C++ "unsigned int"
 * @param {unsigned int}
 */
getNumberOfRowsInSection : function () {},

/**
 * @method setUnSelectRowAtIndexPath
 * @param {unsigned int}
 * @param {unsigned int}
 */
setUnSelectRowAtIndexPath : function () {},

/**
 * @method onEnterTransitionDidFinish
 */
onEnterTransitionDidFinish : function () {},

/**
 * @method getSectionHeaderHeightInSection
 * @return A value converted from C/C++ "float"
 * @param {unsigned int}
 */
getSectionHeaderHeightInSection : function () {},

/**
 * @method getSectionFooterHeightInSection
 * @return A value converted from C/C++ "float"
 * @param {unsigned int}
 */
getSectionFooterHeightInSection : function () {},

/**
 * @method dequeueReusableCellWithIdentifier
 * @return A value converted from C/C++ "CrossApp::CATableViewCell*"
 * @param {const char*}
 */
dequeueReusableCellWithIdentifier : function () {},

/**
 * @method getTableViewDelegate
 * @return A value converted from C/C++ "CrossApp::CATableViewDelegate*"
 */
getTableViewDelegate : function () {},

/**
 * @method getTableHeaderHeight
 * @return A value converted from C/C++ "unsigned int"
 */
getTableHeaderHeight : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CrossApp::CATableView*"
 * @param {CrossApp::CCRect}
 */
createWithFrame : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CrossApp::CATableView*"
 * @param {CrossApp::CCRect}
 */
createWithCenter : function () {},

/**
 * @method CATableView
 * @constructor
 */
CATableView : function () {},

};

/**
 * @class CATableViewCell
 */
ca.TableViewCell = {

/**
 * @method setReuseIdentifier
 * @param {std::string}
 */
setReuseIdentifier : function () {},

/**
 * @method isAllowsSelected
 * @return A value converted from C/C++ "bool"
 */
isAllowsSelected : function () {},

/**
 * @method getReuseIdentifier
 * @return A value converted from C/C++ "std::string"
 */
getReuseIdentifier : function () {},

/**
 * @method setAllowsSelected
 * @param {bool}
 */
setAllowsSelected : function () {},

/**
 * @method setControlStateEffect
 * @param {bool}
 */
setControlStateEffect : function () {},

/**
 * @method getRow
 * @return A value converted from C/C++ "unsigned int"
 */
getRow : function () {},

/**
 * @method getBackgroundView
 * @return A value converted from C/C++ "CrossApp::CAView*"
 */
getBackgroundView : function () {},

/**
 * @method getSection
 * @return A value converted from C/C++ "unsigned int"
 */
getSection : function () {},

/**
 * @method isControlStateEffect
 * @return A value converted from C/C++ "bool"
 */
isControlStateEffect : function () {},

/**
 * @method initWithReuseIdentifier
 * @return A value converted from C/C++ "bool"
 * @param {std::string}
 */
initWithReuseIdentifier : function () {},

/**
 * @method setBackgroundView
 * @param {CrossApp::CAView*}
 */
setBackgroundView : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "CrossApp::CATableViewCell*"
 * @param {std::string}
 */
create : function () {},

/**
 * @method CATableViewCell
 * @constructor
 */
CATableViewCell : function () {},

};

/**
 * @class CAPickerView
 */
ca.PickerView = {

/**
 * @method onEnter
 */
onEnter : function () {},

/**
 * @method setFontSizeNormal
 * @param {float}
 */
setFontSizeNormal : function () {},

/**
 * @method initWithCenter
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CCRect}
 */
initWithCenter : function () {},

/**
 * @method getFontSizeNormal
 * @return A value converted from C/C++ "float"
 */
getFontSizeNormal : function () {},

/**
 * @method reloadAllComponents
 */
reloadAllComponents : function () {},

/**
 * @method getFontSizeSelected
 * @return A value converted from C/C++ "float"
 */
getFontSizeSelected : function () {},

/**
 * @method visit
 */
visit : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method getFontColorSelected
 * @return A value converted from C/C++ "CrossApp::CAColor4B"
 */
getFontColorSelected : function () {},

/**
 * @method initWithFrame
 * @return A value converted from C/C++ "bool"
 * @param {CrossApp::CCRect}
 */
initWithFrame : function () {},

/**
 * @method setFontSizeSelected
 * @param {float}
 */
setFontSizeSelected : function () {},

/**
 * @method getFontColorNormal
 * @return A value converted from C/C++ "CrossApp::CAColor4B"
 */
getFontColorNormal : function () {},

/**
 * @method setFontColorNormal
 * @param {CrossApp::CAColor4B}
 */
setFontColorNormal : function () {},

/**
 * @method numberOfRowsInComponent
 * @return A value converted from C/C++ "int"
 * @param {unsigned int}
 */
numberOfRowsInComponent : function () {},

/**
 * @method getSeparateColor
 * @return A value converted from C/C++ "CrossApp::CAColor4B"
 */
getSeparateColor : function () {},

/**
 * @method selectRow
 * @param {unsigned int}
 * @param {unsigned int}
 * @param {bool}
 */
selectRow : function () {},

/**
 * @method reloadComponent
 * @param {unsigned int}
 * @param {bool}
 */
reloadComponent : function () {},

/**
 * @method numberOfComponents
 * @return A value converted from C/C++ "int"
 */
numberOfComponents : function () {},

/**
 * @method onExit
 */
onExit : function () {},

/**
 * @method setFontColorSelected
 * @param {CrossApp::CAColor4B}
 */
setFontColorSelected : function () {},

/**
 * @method rowSizeForComponent
 * @return A value converted from C/C++ "CrossApp::CCSize"
 * @param {unsigned int}
 */
rowSizeForComponent : function () {},

/**
 * @method selectedRowInComponent
 * @return A value converted from C/C++ "int"
 * @param {unsigned int}
 */
selectedRowInComponent : function () {},

/**
 * @method setSeparateColor
 * @param {CrossApp::CAColor4B}
 */
setSeparateColor : function () {},

/**
 * @method viewForRow
 * @return A value converted from C/C++ "CrossApp::CAView*"
 * @param {unsigned int}
 * @param {unsigned int}
 */
viewForRow : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "CrossApp::CAPickerView*"
 */
create : function () {},

/**
 * @method createWithFrame
 * @return A value converted from C/C++ "CrossApp::CAPickerView*"
 * @param {CrossApp::CCRect}
 */
createWithFrame : function () {},

/**
 * @method createWithCenter
 * @return A value converted from C/C++ "CrossApp::CAPickerView*"
 * @param {CrossApp::CCRect}
 */
createWithCenter : function () {},

/**
 * @method CAPickerView
 * @constructor
 */
CAPickerView : function () {},

};

/**
 * @class JSViewController
 */
ca.JSViewController = {

/**
 * @method viewDidUnload
 */
viewDidUnload : function () {},

/**
 * @method viewDidLoad
 */
viewDidLoad : function () {},

/**
 * @method JSViewController
 * @constructor
 */
JSViewController : function () {},

};

/**
 * @class CCApplication
 */
ca.CCApplication = {

/**
 * @method getTargetPlatform
 * @return A value converted from C/C++ "CrossApp::TargetPlatform"
 */
getTargetPlatform : function () {},

/**
 * @method setAnimationInterval
 * @param {double}
 */
setAnimationInterval : function () {},

/**
 * @method getCurrentLanguage
 * @return A value converted from C/C++ "CrossApp::ccLanguageType"
 */
getCurrentLanguage : function () {},

/**
 * @method sharedApplication
 * @return A value converted from C/C++ "CrossApp::CCApplication*"
 */
sharedApplication : function () {},

};
