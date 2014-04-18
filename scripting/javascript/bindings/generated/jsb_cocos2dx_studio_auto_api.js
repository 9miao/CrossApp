/**
 * @module cocos2dx_studio
 */
var ccs = ccs || {};

/**
 * @class CCBaseData
 */
ccs.BaseData = {

/**
 * @method getColor
 * @return A value converted from C/C++ "ccColor4B"
 */
getColor : function () {},

/**
 * @method setColor
 * @param {cocos2d::ccColor4B}
 */
setColor : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::CCBaseData*"
 */
create : function () {},

/**
 * @method CCBaseData
 * @constructor
 */
CCBaseData : function () {},

};

/**
 * @class CCTween
 */
ccs.Tween = {

/**
 * @method play
 * @param {cocos2d::extension::CCMovementBoneData*}
 * @param {int}
 * @param {int}
 * @param {int}
 * @param {int}
 */
play : function () {},

/**
 * @method gotoAndPause
 * @param {int}
 */
gotoAndPause : function () {},

/**
 * @method getAnimation
 * @return A value converted from C/C++ "cocos2d::extension::CCArmatureAnimation*"
 */
getAnimation : function () {},

/**
 * @method gotoAndPlay
 * @param {int}
 */
gotoAndPlay : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::extension::CCBone*}
 */
init : function () {},

/**
 * @method setAnimation
 * @param {cocos2d::extension::CCArmatureAnimation*}
 */
setAnimation : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::CCTween*"
 * @param {cocos2d::extension::CCBone*}
 */
create : function () {},

/**
 * @method CCTween
 * @constructor
 */
CCTween : function () {},

};

/**
 * @class CCColliderFilter
 */
ccs.ColliderFilter = {

/**
 * @method setCollisionType
 * @param {unsigned int}
 */
setCollisionType : function () {},

/**
 * @method setGroup
 * @param {unsigned int}
 */
setGroup : function () {},

/**
 * @method getCollisionType
 * @return A value converted from C/C++ "unsigned int"
 */
getCollisionType : function () {},

/**
 * @method getGroup
 * @return A value converted from C/C++ "unsigned int"
 */
getGroup : function () {},

};

/**
 * @class CCDisplayManager
 */
ccs.DisplayManager = {

/**
 * @method getDisplayRenderNode
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getDisplayRenderNode : function () {},

/**
 * @method getAnchorPointInPoints
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getAnchorPointInPoints : function () {},

/**
 * @method getDisplayRenderNodeType
 * @return A value converted from C/C++ "cocos2d::extension::DisplayType"
 */
getDisplayRenderNodeType : function () {},

/**
 * @method removeDisplay
 * @param {int}
 */
removeDisplay : function () {},

/**
 * @method setForceChangeDisplay
 * @param {bool}
 */
setForceChangeDisplay : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::extension::CCBone*}
 */
init : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method getBoundingBox
 * @return A value converted from C/C++ "cocos2d::CCRect"
 */
getBoundingBox : function () {},

/**
 * @method getCurrentDisplayIndex
 * @return A value converted from C/C++ "int"
 */
getCurrentDisplayIndex : function () {},

/**
 * @method setVisible
 * @param {bool}
 */
setVisible : function () {},

/**
 * @method getAnchorPoint
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getAnchorPoint : function () {},

/**
 * @method getDecorativeDisplayList
 * @return A value converted from C/C++ "cocos2d::CCArray*"
 */
getDecorativeDisplayList : function () {},

/**
 * @method changeDisplayByIndex
 * @param {int}
 * @param {bool}
 */
changeDisplayByIndex : function () {},

/**
 * @method isVisible
 * @return A value converted from C/C++ "bool"
 */
isVisible : function () {},

/**
 * @method getForceChangeDisplay
 * @return A value converted from C/C++ "bool"
 */
getForceChangeDisplay : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::CCDisplayManager*"
 * @param {cocos2d::extension::CCBone*}
 */
create : function () {},

/**
 * @method CCDisplayManager
 * @constructor
 */
CCDisplayManager : function () {},

};

/**
 * @class CCBone
 */
ccs.Bone = {

/**
 * @method nodeToWorldTransform
 * @return A value converted from C/C++ "cocos2d::CCAffineTransform"
 */
nodeToWorldTransform : function () {},

/**
 * @method isTransformDirty
 * @return A value converted from C/C++ "bool"
 */
isTransformDirty : function () {},

/**
 * @method updateZOrder
 */
updateZOrder : function () {},

/**
 * @method setTransformDirty
 * @param {bool}
 */
setTransformDirty : function () {},

/**
 * @method getDisplayRenderNode
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getDisplayRenderNode : function () {},

/**
 * @method addChildBone
 * @param {cocos2d::extension::CCBone*}
 */
addChildBone : function () {},

/**
 * @method getWorldInfo
 * @return A value converted from C/C++ "cocos2d::extension::CCBaseData*"
 */
getWorldInfo : function () {},

/**
 * @method getTween
 * @return A value converted from C/C++ "cocos2d::extension::CCTween*"
 */
getTween : function () {},

/**
 * @method getParentBone
 * @return A value converted from C/C++ "cocos2d::extension::CCBone*"
 */
getParentBone : function () {},

/**
 * @method getBlendType
 * @return A value converted from C/C++ "cocos2d::extension::CCBlendType"
 */
getBlendType : function () {},

/**
 * @method updateColor
 */
updateColor : function () {},

/**
 * @method getName
 * @return A value converted from C/C++ "std::string"
 */
getName : function () {},

/**
 * @method setOpacity
 * @param {unsigned char}
 */
setOpacity : function () {},

/**
 * @method getDisplayRenderNodeType
 * @return A value converted from C/C++ "cocos2d::extension::DisplayType"
 */
getDisplayRenderNodeType : function () {},

/**
 * @method removeDisplay
 * @param {int}
 */
removeDisplay : function () {},

/**
 * @method updateDisplayedOpacity
 * @param {unsigned char}
 */
updateDisplayedOpacity : function () {},

/**
 * @method setParentBone
 * @param {cocos2d::extension::CCBone*}
 */
setParentBone : function () {},

/**
 * @method setZOrder
 * @param {int}
 */
setZOrder : function () {},

/**
 * @method getIgnoreMovementBoneData
 * @return A value converted from C/C++ "bool"
 */
getIgnoreMovementBoneData : function () {},

/**
 * @method getColliderFilter
 * @return A value converted from C/C++ "cocos2d::extension::CCColliderFilter*"
 */
getColliderFilter : function () {},

/**
 * @method setIgnoreMovementBoneData
 * @param {bool}
 */
setIgnoreMovementBoneData : function () {},

/**
 * @method setName
 * @param {std::string}
 */
setName : function () {},

/**
 * @method removeFromParent
 * @param {bool}
 */
removeFromParent : function () {},

/**
 * @method getChildArmature
 * @return A value converted from C/C++ "cocos2d::extension::CCArmature*"
 */
getChildArmature : function () {},

/**
 * @method update
 * @param {float}
 */
update : function () {},

/**
 * @method setDisplayManager
 * @param {cocos2d::extension::CCDisplayManager*}
 */
setDisplayManager : function () {},

/**
 * @method setColliderFilter
 * @param {cocos2d::extension::CCColliderFilter*}
 */
setColliderFilter : function () {},

/**
 * @method getColliderBodyList
 * @return A value converted from C/C++ "cocos2d::CCArray*"
 */
getColliderBodyList : function () {},

/**
 * @method setArmature
 * @param {cocos2d::extension::CCArmature*}
 */
setArmature : function () {},

/**
 * @method setColor
 * @param {cocos2d::ccColor3B}
 */
setColor : function () {},

/**
 * @method removeChildBone
 * @param {cocos2d::extension::CCBone*}
 * @param {bool}
 */
removeChildBone : function () {},

/**
 * @method setChildArmature
 * @param {cocos2d::extension::CCArmature*}
 */
setChildArmature : function () {},

/**
 * @method getDisplayManager
 * @return A value converted from C/C++ "cocos2d::extension::CCDisplayManager*"
 */
getDisplayManager : function () {},

/**
 * @method getArmature
 * @return A value converted from C/C++ "cocos2d::extension::CCArmature*"
 */
getArmature : function () {},

/**
 * @method setBlendType
 * @param {cocos2d::extension::CCBlendType}
 */
setBlendType : function () {},

/**
 * @method changeDisplayByIndex
 * @param {int}
 * @param {bool}
 */
changeDisplayByIndex : function () {},

/**
 * @method nodeToArmatureTransform
 * @return A value converted from C/C++ "cocos2d::CCAffineTransform"
 */
nodeToArmatureTransform : function () {},

/**
 * @method updateDisplayedColor
 * @param {cocos2d::ccColor3B}
 */
updateDisplayedColor : function () {},

/**
 * @method CCBone
 * @constructor
 */
CCBone : function () {},

};

/**
 * @class CCBatchNode
 */
ccs.BatchNode = {

/**
 * @method getTexureAtlasWithTexture
 * @return A value converted from C/C++ "cocos2d::CCTextureAtlas*"
 * @param {cocos2d::CCTexture2D*}
 */
getTexureAtlasWithTexture : function () {},

/**
 * @method removeChild
 * @param {cocos2d::CCNode*}
 * @param {bool}
 */
removeChild : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::CCBatchNode*"
 */
create : function () {},

};

/**
 * @class CCArmatureAnimation
 */
ccs.ArmatureAnimation = {

/**
 * @method getSpeedScale
 * @return A value converted from C/C++ "float"
 */
getSpeedScale : function () {},

/**
 * @method getAnimationScale
 * @return A value converted from C/C++ "float"
 */
getAnimationScale : function () {},

/**
 * @method play
 * @param {const char*}
 * @param {int}
 * @param {int}
 * @param {int}
 * @param {int}
 */
play : function () {},

/**
 * @method pause
 */
pause : function () {},

/**
 * @method setAnimationScale
 * @param {float}
 */
setAnimationScale : function () {},

/**
 * @method gotoAndPause
 * @param {int}
 */
gotoAndPause : function () {},

/**
 * @method resume
 */
resume : function () {},

/**
 * @method stop
 */
stop : function () {},

/**
 * @method setSpeedScale
 * @param {float}
 */
setSpeedScale : function () {},

/**
 * @method update
 * @param {float}
 */
update : function () {},

/**
 * @method getAnimationData
 * @return A value converted from C/C++ "cocos2d::extension::CCAnimationData*"
 */
getAnimationData : function () {},

/**
 * @method playByIndex
 * @param {int}
 * @param {int}
 * @param {int}
 * @param {int}
 * @param {int}
 */
playByIndex : function () {},

/**
 * @method gotoAndPlay
 * @param {int}
 */
gotoAndPlay : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::extension::CCArmature*}
 */
init : function () {},

/**
 * @method getMovementCount
 * @return A value converted from C/C++ "int"
 */
getMovementCount : function () {},

/**
 * @method getCurrentMovementID
 * @return A value converted from C/C++ "std::string"
 */
getCurrentMovementID : function () {},

/**
 * @method setAnimationData
 * @param {cocos2d::extension::CCAnimationData*}
 */
setAnimationData : function () {},

/**
 * @method setAnimationInternal
 * @param {float}
 */
setAnimationInternal : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::CCArmatureAnimation*"
 * @param {cocos2d::extension::CCArmature*}
 */
create : function () {},

/**
 * @method CCArmatureAnimation
 * @constructor
 */
CCArmatureAnimation : function () {},

};

/**
 * @class CCArmature
 */
ccs.Armature = {

/**
 * @method getBone
 * @return A value converted from C/C++ "cocos2d::extension::CCBone*"
 * @param {const char*}
 */
getBone : function () {},

/**
 * @method changeBoneParent
 * @param {cocos2d::extension::CCBone*}
 * @param {const char*}
 */
changeBoneParent : function () {},

/**
 * @method getTexureAtlasWithTexture
 * @return A value converted from C/C++ "cocos2d::CCTextureAtlas*"
 * @param {cocos2d::CCTexture2D*}
 */
getTexureAtlasWithTexture : function () {},

/**
 * @method setAnimation
 * @param {cocos2d::extension::CCArmatureAnimation*}
 */
setAnimation : function () {},

/**
 * @method getBoneAtPoint
 * @return A value converted from C/C++ "cocos2d::extension::CCBone*"
 * @param {float}
 * @param {float}
 */
getBoneAtPoint : function () {},

/**
 * @method getBatchNode
 * @return A value converted from C/C++ "cocos2d::extension::CCBatchNode*"
 */
getBatchNode : function () {},

/**
 * @method setVersion
 * @param {float}
 */
setVersion : function () {},

/**
 * @method updateOffsetPoint
 */
updateOffsetPoint : function () {},

/**
 * @method setColliderFilter
 * @param {cocos2d::extension::CCColliderFilter*}
 */
setColliderFilter : function () {},

/**
 * @method setName
 * @param {std::string}
 */
setName : function () {},

/**
 * @method removeBone
 * @param {cocos2d::extension::CCBone*}
 * @param {bool}
 */
removeBone : function () {},

/**
 * @method getArmatureTransformDirty
 * @return A value converted from C/C++ "bool"
 */
getArmatureTransformDirty : function () {},

/**
 * @method getName
 * @return A value converted from C/C++ "std::string"
 */
getName : function () {},

/**
 * @method setParentBone
 * @param {cocos2d::extension::CCBone*}
 */
setParentBone : function () {},

/**
 * @method setBatchNode
 * @param {cocos2d::extension::CCBatchNode*}
 */
setBatchNode : function () {},

/**
 * @method draw
 */
draw : function () {},

/**
 * @method nodeToParentTransform
 * @return A value converted from C/C++ "cocos2d::CCAffineTransform"
 */
nodeToParentTransform : function () {},

/**
 * @method setArmatureData
 * @param {cocos2d::extension::CCArmatureData*}
 */
setArmatureData : function () {},

/**
 * @method setTextureAtlas
 * @param {cocos2d::CCTextureAtlas*}
 */
setTextureAtlas : function () {},

/**
 * @method addBone
 * @param {cocos2d::extension::CCBone*}
 * @param {const char*}
 */
addBone : function () {},

/**
 * @method update
 * @param {float}
 */
update : function () {},

/**
 * @method getArmatureData
 * @return A value converted from C/C++ "cocos2d::extension::CCArmatureData*"
 */
getArmatureData : function () {},

/**
 * @method getParentBone
 * @return A value converted from C/C++ "cocos2d::extension::CCBone*"
 */
getParentBone : function () {},

/**
 * @method boundingBox
 * @return A value converted from C/C++ "cocos2d::CCRect"
 */
boundingBox : function () {},

/**
 * @method getVersion
 * @return A value converted from C/C++ "float"
 */
getVersion : function () {},

/**
 * @method getAnimation
 * @return A value converted from C/C++ "cocos2d::extension::CCArmatureAnimation*"
 */
getAnimation : function () {},

/**
 * @method getBoneDic
 * @return A value converted from C/C++ "cocos2d::CCDictionary*"
 */
getBoneDic : function () {},

/**
 * @method getTextureAtlas
 * @return A value converted from C/C++ "cocos2d::CCTextureAtlas*"
 */
getTextureAtlas : function () {},

/**
 * @method CCArmature
 * @constructor
 */
CCArmature : function () {},

};

/**
 * @class CCSkin
 */
ccs.Skin = {

/**
 * @method getBone
 * @return A value converted from C/C++ "cocos2d::extension::CCBone*"
 */
getBone : function () {},

/**
 * @method nodeToWorldTransform
 * @return A value converted from C/C++ "cocos2d::CCAffineTransform"
 */
nodeToWorldTransform : function () {},

/**
 * @method initWithFile
 * @return A value converted from C/C++ "bool"
 * @param {const char*}
 */
initWithFile : function () {},

/**
 * @method nodeToWorldTransformAR
 * @return A value converted from C/C++ "cocos2d::CCAffineTransform"
 */
nodeToWorldTransformAR : function () {},

/**
 * @method updateTransform
 */
updateTransform : function () {},

/**
 * @method getDisplayName
 * @return A value converted from C/C++ "std::string"
 */
getDisplayName : function () {},

/**
 * @method updateArmatureTransform
 */
updateArmatureTransform : function () {},

/**
 * @method initWithSpriteFrameName
 * @return A value converted from C/C++ "bool"
 * @param {const char*}
 */
initWithSpriteFrameName : function () {},

/**
 * @method setBone
 * @param {cocos2d::extension::CCBone*}
 */
setBone : function () {},

/**
 * @method createWithSpriteFrameName
 * @return A value converted from C/C++ "cocos2d::extension::CCSkin*"
 * @param {const char*}
 */
createWithSpriteFrameName : function () {},

/**
 * @method CCSkin
 * @constructor
 */
CCSkin : function () {},

};

/**
 * @class CCArmatureDataManager
 */
ccs.ArmatureDataManager = {

/**
 * @method removeArmatureFileInfo
 * @param {const char*}
 */
removeArmatureFileInfo : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method isAutoLoadSpriteFile
 * @return A value converted from C/C++ "bool"
 */
isAutoLoadSpriteFile : function () {},

/**
 * @method addSpriteFrameFromFile
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 */
addSpriteFrameFromFile : function () {},

/**
 * @method purge
 */
purge : function () {},

/**
 * @method sharedArmatureDataManager
 * @return A value converted from C/C++ "cocos2d::extension::CCArmatureDataManager*"
 */
sharedArmatureDataManager : function () {},

};

/**
 * @class CCComAttribute
 */
ccs.ComAttribute = {

/**
 * @method getFloat
 * @return A value converted from C/C++ "float"
 * @param {const char*}
 */
getFloat : function () {},

/**
 * @method setFloat
 * @param {const char*}
 * @param {float}
 */
setFloat : function () {},

/**
 * @method setCString
 * @param {const char*}
 * @param {const char*}
 */
setCString : function () {},

/**
 * @method getCString
 * @return A value converted from C/C++ "const char*"
 * @param {const char*}
 */
getCString : function () {},

/**
 * @method getBool
 * @return A value converted from C/C++ "bool"
 * @param {const char*}
 */
getBool : function () {},

/**
 * @method setInt
 * @param {const char*}
 * @param {int}
 */
setInt : function () {},

/**
 * @method getInt
 * @return A value converted from C/C++ "int"
 * @param {const char*}
 */
getInt : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method setBool
 * @param {const char*}
 * @param {bool}
 */
setBool : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::CCComAttribute*"
 */
create : function () {},

};

/**
 * @class CCComAudio
 */
ccs.ComAudio = {

/**
 * @method stopAllEffects
 */
stopAllEffects : function () {},

/**
 * @method getEffectsVolume
 * @return A value converted from C/C++ "float"
 */
getEffectsVolume : function () {},

/**
 * @method stopEffect
 * @param {unsigned int}
 */
stopEffect : function () {},

/**
 * @method getBackgroundMusicVolume
 * @return A value converted from C/C++ "float"
 */
getBackgroundMusicVolume : function () {},

/**
 * @method willPlayBackgroundMusic
 * @return A value converted from C/C++ "bool"
 */
willPlayBackgroundMusic : function () {},

/**
 * @method setBackgroundMusicVolume
 * @param {float}
 */
setBackgroundMusicVolume : function () {},

/**
 * @method end
 */
end : function () {},

/**
 * @method isEnabled
 * @return A value converted from C/C++ "bool"
 */
isEnabled : function () {},

/**
 * @method pauseBackgroundMusic
 */
pauseBackgroundMusic : function () {},

/**
 * @method isBackgroundMusicPlaying
 * @return A value converted from C/C++ "bool"
 */
isBackgroundMusicPlaying : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method isLoop
 * @return A value converted from C/C++ "bool"
 */
isLoop : function () {},

/**
 * @method pauseAllEffects
 */
pauseAllEffects : function () {},

/**
 * @method setEnabled
 * @param {bool}
 */
setEnabled : function () {},

/**
 * @method preloadBackgroundMusic
 * @param {const char*}
 */
preloadBackgroundMusic : function () {},

/**
 * @method resumeAllEffects
 */
resumeAllEffects : function () {},

/**
 * @method setLoop
 * @param {bool}
 */
setLoop : function () {},

/**
 * @method unloadEffect
 * @param {const char*}
 */
unloadEffect : function () {},

/**
 * @method rewindBackgroundMusic
 */
rewindBackgroundMusic : function () {},

/**
 * @method preloadEffect
 * @param {const char*}
 */
preloadEffect : function () {},

/**
 * @method pauseEffect
 * @param {unsigned int}
 */
pauseEffect : function () {},

/**
 * @method resumeBackgroundMusic
 */
resumeBackgroundMusic : function () {},

/**
 * @method setFile
 * @param {const char*}
 */
setFile : function () {},

/**
 * @method setEffectsVolume
 * @param {float}
 */
setEffectsVolume : function () {},

/**
 * @method getFile
 * @return A value converted from C/C++ "const char*"
 */
getFile : function () {},

/**
 * @method resumeEffect
 * @param {unsigned int}
 */
resumeEffect : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::CCComAudio*"
 */
create : function () {},

};

/**
 * @class CCInputDelegate
 */
ccs.InputDelegate = {

/**
 * @method isAccelerometerEnabled
 * @return A value converted from C/C++ "bool"
 */
isAccelerometerEnabled : function () {},

/**
 * @method setKeypadEnabled
 * @param {bool}
 */
setKeypadEnabled : function () {},

/**
 * @method getTouchMode
 * @return A value converted from C/C++ "int"
 */
getTouchMode : function () {},

/**
 * @method setAccelerometerEnabled
 * @param {bool}
 */
setAccelerometerEnabled : function () {},

/**
 * @method isKeypadEnabled
 * @return A value converted from C/C++ "bool"
 */
isKeypadEnabled : function () {},

/**
 * @method isTouchEnabled
 * @return A value converted from C/C++ "bool"
 */
isTouchEnabled : function () {},

/**
 * @method setTouchPriority
 * @param {int}
 */
setTouchPriority : function () {},

/**
 * @method getTouchPriority
 * @return A value converted from C/C++ "int"
 */
getTouchPriority : function () {},

/**
 * @method setTouchEnabled
 * @param {bool}
 */
setTouchEnabled : function () {},

/**
 * @method setTouchMode
 * @param {cocos2d::ccTouchesMode}
 */
setTouchMode : function () {},

};

/**
 * @class CCComController
 */
ccs.ComController = {

/**
 * @method setEnabled
 * @param {bool}
 */
setEnabled : function () {},

/**
 * @method isEnabled
 * @return A value converted from C/C++ "bool"
 */
isEnabled : function () {},

/**
 * @method update
 * @param {float}
 */
update : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::CCComController*"
 */
create : function () {},

/**
 * @method CCComController
 * @constructor
 */
CCComController : function () {},

};

/**
 * @class CCComRender
 */
ccs.ComRender = {

/**
 * @method setNode
 * @param {cocos2d::CCNode*}
 */
setNode : function () {},

/**
 * @method getNode
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getNode : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::CCComRender*"
 * @param {cocos2d::CCNode*}
 * @param {const char*}
 */
create : function () {},

};

/**
 * @class UILayoutParameter
 */
ccs.UILayoutParameter = {

/**
 * @method getLayoutType
 * @return A value converted from C/C++ "cocos2d::extension::LayoutParameterType"
 */
getLayoutType : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UILayoutParameter*"
 */
create : function () {},

/**
 * @method UILayoutParameter
 * @constructor
 */
UILayoutParameter : function () {},

};

/**
 * @class UILinearLayoutParameter
 */
ccs.UILinearLayoutParameter = {

/**
 * @method setGravity
 * @param {cocos2d::extension::UILinearGravity}
 */
setGravity : function () {},

/**
 * @method getGravity
 * @return A value converted from C/C++ "cocos2d::extension::UILinearGravity"
 */
getGravity : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UILinearLayoutParameter*"
 */
create : function () {},

/**
 * @method UILinearLayoutParameter
 * @constructor
 */
UILinearLayoutParameter : function () {},

};

/**
 * @class UIRelativeLayoutParameter
 */
ccs.UIRelativeLayoutParameter = {

/**
 * @method setAlign
 * @param {cocos2d::extension::UIRelativeAlign}
 */
setAlign : function () {},

/**
 * @method setRelativeToWidgetName
 * @param {const char*}
 */
setRelativeToWidgetName : function () {},

/**
 * @method getRelativeName
 * @return A value converted from C/C++ "const char*"
 */
getRelativeName : function () {},

/**
 * @method getRelativeToWidgetName
 * @return A value converted from C/C++ "const char*"
 */
getRelativeToWidgetName : function () {},

/**
 * @method setRelativeName
 * @param {const char*}
 */
setRelativeName : function () {},

/**
 * @method getAlign
 * @return A value converted from C/C++ "cocos2d::extension::UIRelativeAlign"
 */
getAlign : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UIRelativeLayoutParameter*"
 */
create : function () {},

/**
 * @method UIRelativeLayoutParameter
 * @constructor
 */
UIRelativeLayoutParameter : function () {},

};

/**
 * @class UIWidget
 */
ccs.UIWidget = {

/**
 * @method addChild
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::extension::UIWidget*}
 */
addChild : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method setActionTag
 * @param {int}
 */
setActionTag : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method setCascadeOpacityEnabled
 * @param {bool}
 */
setCascadeOpacityEnabled : function () {},

/**
 * @method getChildren
 * @return A value converted from C/C++ "cocos2d::CCArray*"
 */
getChildren : function () {},

/**
 * @method getRelativeLeftPos
 * @return A value converted from C/C++ "float"
 */
getRelativeLeftPos : function () {},

/**
 * @method getPositionType
 * @return A value converted from C/C++ "cocos2d::extension::PositionType"
 */
getPositionType : function () {},

/**
 * @method getChildByName
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 * @param {const char*}
 */
getChildByName : function () {},

/**
 * @method isEnabled
 * @return A value converted from C/C++ "bool"
 */
isEnabled : function () {},

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
 * @method didNotSelectSelf
 */
didNotSelectSelf : function () {},

/**
 * @method setFocused
 * @param {bool}
 */
setFocused : function () {},

/**
 * @method setZOrder
 * @param {int}
 */
setZOrder : function () {},

/**
 * @method getWidgetParent
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 */
getWidgetParent : function () {},

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
 * @method getLayoutParameter
 * @return A value converted from C/C++ "cocos2d::extension::UILayoutParameter*"
 * @param {cocos2d::extension::LayoutParameterType}
 */
getLayoutParameter : function () {},

/**
 * @method getContainerNode
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getContainerNode : function () {},

/**
 * @method getColor
 * @return A value converted from C/C++ "cocos2d::ccColor3B"
 */
getColor : function () {},

/**
 * @method getWidgetZOrder
 * @return A value converted from C/C++ "int"
 */
getWidgetZOrder : function () {},

/**
 * @method isBright
 * @return A value converted from C/C++ "bool"
 */
isBright : function () {},

/**
 * @method getRelativeRightPos
 * @return A value converted from C/C++ "float"
 */
getRelativeRightPos : function () {},

/**
 * @method getTag
 * @return A value converted from C/C++ "int"
 */
getTag : function () {},

/**
 * @method addRenderer
 * @param {cocos2d::CCNode*}
 * @param {int}
 */
addRenderer : function () {},

/**
 * @method getSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getSize : function () {},

/**
 * @method getSizeType
 * @return A value converted from C/C++ "cocos2d::extension::SizeType"
 */
getSizeType : function () {},

/**
 * @method getPositionPercent
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getPositionPercent : function () {},

/**
 * @method getValidNode
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getValidNode : function () {},

/**
 * @method convertToWorldSpace
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 * @param {cocos2d::CCPoint}
 */
convertToWorldSpace : function () {},

/**
 * @method setSize
 * @param {cocos2d::CCSize}
 */
setSize : function () {},

/**
 * @method setWidgetParent
 * @param {cocos2d::extension::UIWidget*}
 */
setWidgetParent : function () {},

/**
 * @method removeAllChildren
 */
removeAllChildren : function () {},

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
 * @method getSizePercent
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getSizePercent : function () {},

/**
 * @method setPositionType
 * @param {cocos2d::extension::PositionType}
 */
setPositionType : function () {},

/**
 * @method isCascadeOpacityEnabled
 * @return A value converted from C/C++ "bool"
 */
isCascadeOpacityEnabled : function () {},

/**
 * @method setParent
 * @param {cocos2d::extension::UIWidget*}
 */
setParent : function () {},

/**
 * @method getName
 * @return A value converted from C/C++ "const char*"
 */
getName : function () {},

/**
 * @method getWidgetTag
 * @return A value converted from C/C++ "int"
 */
getWidgetTag : function () {},

/**
 * @method isTouchEnabled
 * @return A value converted from C/C++ "bool"
 */
isTouchEnabled : function () {},

/**
 * @method setTouchEnabled
 * @param {bool}
 */
setTouchEnabled : function () {},

/**
 * @method setLayoutParameter
 * @param {cocos2d::extension::UILayoutParameter*}
 */
setLayoutParameter : function () {},

/**
 * @method addCCNode
 * @param {cocos2d::CCNode*}
 */
addCCNode : function () {},

/**
 * @method clippingParentAreaContainPoint
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::CCPoint}
 */
clippingParentAreaContainPoint : function () {},

/**
 * @method removeFromParentAndCleanup
 * @param {bool}
 */
removeFromParentAndCleanup : function () {},

/**
 * @method active
 */
active : function () {},

/**
 * @method setPosition
 * @param {cocos2d::CCPoint}
 */
setPosition : function () {},

/**
 * @method removeRenderer
 * @param {cocos2d::CCNode*}
 * @param {bool}
 */
removeRenderer : function () {},

/**
 * @method stopActionByTag
 * @param {int}
 */
stopActionByTag : function () {},

/**
 * @method reorderChild
 * @param {cocos2d::extension::UIWidget*}
 */
reorderChild : function () {},

/**
 * @method isActive
 * @return A value converted from C/C++ "bool"
 */
isActive : function () {},

/**
 * @method getScale
 * @return A value converted from C/C++ "float"
 */
getScale : function () {},

/**
 * @method setColor
 * @param {cocos2d::ccColor3B}
 */
setColor : function () {},

/**
 * @method getAnchorPoint
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getAnchorPoint : function () {},

/**
 * @method setWidgetTag
 * @param {int}
 */
setWidgetTag : function () {},

/**
 * @method isFocused
 * @return A value converted from C/C++ "bool"
 */
isFocused : function () {},

/**
 * @method isVisible
 * @return A value converted from C/C++ "bool"
 */
isVisible : function () {},

/**
 * @method setTouchEnable
 * @param {bool}
 * @param {bool}
 */
setTouchEnable : function () {},

/**
 * @method setAnchorPoint
 * @param {cocos2d::CCPoint}
 */
setAnchorPoint : function () {},

/**
 * @method getLeftInParent
 * @return A value converted from C/C++ "float"
 */
getLeftInParent : function () {},

/**
 * @method setPositionPercent
 * @param {cocos2d::CCPoint}
 */
setPositionPercent : function () {},

/**
 * @method getRotation
 * @return A value converted from C/C++ "float"
 */
getRotation : function () {},

/**
 * @method getBottomInParent
 * @return A value converted from C/C++ "float"
 */
getBottomInParent : function () {},

/**
 * @method ignoreContentAdaptWithSize
 * @param {bool}
 */
ignoreContentAdaptWithSize : function () {},

/**
 * @method getZOrder
 * @return A value converted from C/C++ "int"
 */
getZOrder : function () {},

/**
 * @method runAction
 * @return A value converted from C/C++ "cocos2d::CCAction*"
 * @param {cocos2d::CCAction*}
 */
runAction : function () {},

/**
 * @method stopAllActions
 */
stopAllActions : function () {},

/**
 * @method getActionTag
 * @return A value converted from C/C++ "int"
 */
getActionTag : function () {},

/**
 * @method getActionByTag
 * @return A value converted from C/C++ "cocos2d::CCAction*"
 * @param {int}
 */
getActionByTag : function () {},

/**
 * @method setRotationX
 * @param {float}
 */
setRotationX : function () {},

/**
 * @method setRotationY
 * @param {float}
 */
setRotationY : function () {},

/**
 * @method setBrightStyle
 * @param {cocos2d::extension::BrightStyle}
 */
setBrightStyle : function () {},

/**
 * @method setName
 * @param {const char*}
 */
setName : function () {},

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
 * @method setUpdateEnabled
 * @param {bool}
 */
setUpdateEnabled : function () {},

/**
 * @method setRotation
 * @param {float}
 */
setRotation : function () {},

/**
 * @method setActionManager
 * @param {cocos2d::CCActionManager*}
 */
setActionManager : function () {},

/**
 * @method getPosition
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getPosition : function () {},

/**
 * @method getParent
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 */
getParent : function () {},

/**
 * @method hitTest
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::CCPoint}
 */
hitTest : function () {},

/**
 * @method setVisible
 * @param {bool}
 */
setVisible : function () {},

/**
 * @method setWidgetZOrder
 * @param {int}
 */
setWidgetZOrder : function () {},

/**
 * @method setSizePercent
 * @param {cocos2d::CCPoint}
 */
setSizePercent : function () {},

/**
 * @method setEnabled
 * @param {bool}
 */
setEnabled : function () {},

/**
 * @method setScale
 * @param {float}
 */
setScale : function () {},

/**
 * @method getTouchEndPos
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getTouchEndPos : function () {},

/**
 * @method getChildByTag
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 * @param {int}
 */
getChildByTag : function () {},

/**
 * @method isIgnoreContentAdaptWithSize
 * @return A value converted from C/C++ "bool"
 */
isIgnoreContentAdaptWithSize : function () {},

/**
 * @method isUpdateEnabled
 * @return A value converted from C/C++ "bool"
 */
isUpdateEnabled : function () {},

/**
 * @method updateSizeAndPosition
 */
updateSizeAndPosition : function () {},

/**
 * @method getRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getRenderer : function () {},

/**
 * @method getWidgetType
 * @return A value converted from C/C++ "cocos2d::extension::WidgetType"
 */
getWidgetType : function () {},

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
 * @method getRect
 * @return A value converted from C/C++ "cocos2d::CCRect"
 */
getRect : function () {},

/**
 * @method setCascadeColorEnabled
 * @param {bool}
 */
setCascadeColorEnabled : function () {},

/**
 * @method setOpacity
 * @param {int}
 */
setOpacity : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method getRelativeBottomPos
 * @return A value converted from C/C++ "float"
 */
getRelativeBottomPos : function () {},

/**
 * @method getOpacity
 * @return A value converted from C/C++ "int"
 */
getOpacity : function () {},

/**
 * @method removeAllChildrenAndCleanUp
 * @param {bool}
 */
removeAllChildrenAndCleanUp : function () {},

/**
 * @method getTouchMovePos
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getTouchMovePos : function () {},

/**
 * @method removeFromParent
 */
removeFromParent : function () {},

/**
 * @method getWorldPosition
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getWorldPosition : function () {},

/**
 * @method clone
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 */
clone : function () {},

/**
 * @method isCascadeColorEnabled
 * @return A value converted from C/C++ "bool"
 */
isCascadeColorEnabled : function () {},

/**
 * @method disable
 */
disable : function () {},

/**
 * @method checkChildInfo
 * @param {int}
 * @param {cocos2d::extension::UIWidget*}
 * @param {cocos2d::CCPoint}
 */
checkChildInfo : function () {},

/**
 * @method getTopInParent
 * @return A value converted from C/C++ "float"
 */
getTopInParent : function () {},

/**
 * @method getRightInParent
 * @return A value converted from C/C++ "float"
 */
getRightInParent : function () {},

/**
 * @method getRelativeTopPos
 * @return A value converted from C/C++ "float"
 */
getRelativeTopPos : function () {},

/**
 * @method setTag
 * @param {int}
 */
setTag : function () {},

/**
 * @method getTouchStartPos
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getTouchStartPos : function () {},

/**
 * @method setSizeType
 * @param {cocos2d::extension::SizeType}
 */
setSizeType : function () {},

/**
 * @method stopAction
 * @param {cocos2d::CCAction*}
 */
stopAction : function () {},

/**
 * @method getActionManager
 * @return A value converted from C/C++ "cocos2d::CCActionManager*"
 */
getActionManager : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 */
create : function () {},

/**
 * @method UIWidget
 * @constructor
 */
UIWidget : function () {},

};

/**
 * @class UILayout
 */
ccs.UILayout = {

/**
 * @method setBackGroundColorVector
 * @param {cocos2d::CCPoint}
 */
setBackGroundColorVector : function () {},

/**
 * @method addChild
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::extension::UIWidget*}
 */
addChild : function () {},

/**
 * @method setBackGroundColorType
 * @param {cocos2d::extension::LayoutBackGroundColorType}
 */
setBackGroundColorType : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method removeBackGroundImage
 */
removeBackGroundImage : function () {},

/**
 * @method setBackGroundImage
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setBackGroundImage : function () {},

/**
 * @method setOpacity
 * @param {int}
 */
setOpacity : function () {},

/**
 * @method isClippingEnabled
 * @return A value converted from C/C++ "bool"
 */
isClippingEnabled : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method doLayout
 */
doLayout : function () {},

/**
 * @method setClippingEnabled
 * @param {bool}
 */
setClippingEnabled : function () {},

/**
 * @method setClippingEnable
 * @param {bool}
 */
setClippingEnable : function () {},

/**
 * @method setBackGroundColorOpacity
 * @param {int}
 */
setBackGroundColorOpacity : function () {},

/**
 * @method getLayoutType
 * @return A value converted from C/C++ "cocos2d::extension::LayoutType"
 */
getLayoutType : function () {},

/**
 * @method setBackGroundImageCapInsets
 * @param {cocos2d::CCRect}
 */
setBackGroundImageCapInsets : function () {},

/**
 * @method setBackGroundImageScale9Enable
 * @param {bool}
 */
setBackGroundImageScale9Enable : function () {},

/**
 * @method getBackGroundImageTextureSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getBackGroundImageTextureSize : function () {},

/**
 * @method setColor
 * @param {cocos2d::ccColor3B}
 */
setColor : function () {},

/**
 * @method hitTest
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::CCPoint}
 */
hitTest : function () {},

/**
 * @method setBackGroundImageScale9Enabled
 * @param {bool}
 */
setBackGroundImageScale9Enabled : function () {},

/**
 * @method setLayoutType
 * @param {cocos2d::extension::LayoutType}
 */
setLayoutType : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UILayout*"
 */
create : function () {},

/**
 * @method UILayout
 * @constructor
 */
UILayout : function () {},

};

/**
 * @class UIRootWidget
 */
ccs.UIRootWidget = {

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UIRootWidget*"
 */
create : function () {},

/**
 * @method UIRootWidget
 * @constructor
 */
UIRootWidget : function () {},

};

/**
 * @class UIButton
 */
ccs.UIButton = {

/**
 * @method setAnchorPoint
 * @param {cocos2d::CCPoint}
 */
setAnchorPoint : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method setNormalTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setNormalTexture : function () {},

/**
 * @method getTitleText
 * @return A value converted from C/C++ "const char*"
 */
getTitleText : function () {},

/**
 * @method setTitleFontSize
 * @param {float}
 */
setTitleFontSize : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method setScale9Enabled
 * @param {bool}
 */
setScale9Enabled : function () {},

/**
 * @method setTitleColor
 * @param {cocos2d::ccColor3B}
 */
setTitleColor : function () {},

/**
 * @method setFontName
 * @param {const char*}
 */
setFontName : function () {},

/**
 * @method setTextures
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setTextures : function () {},

/**
 * @method ignoreContentAdaptWithSize
 * @param {bool}
 */
ignoreContentAdaptWithSize : function () {},

/**
 * @method setCapInsetsDisabledRenderer
 * @param {cocos2d::CCRect}
 */
setCapInsetsDisabledRenderer : function () {},

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
 * @method loadTextureDisabled
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadTextureDisabled : function () {},

/**
 * @method setTextColor
 * @param {int}
 * @param {int}
 * @param {int}
 */
setTextColor : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method setTitleText
 * @param {const char*}
 */
setTitleText : function () {},

/**
 * @method setCapInsetsNormalRenderer
 * @param {cocos2d::CCRect}
 */
setCapInsetsNormalRenderer : function () {},

/**
 * @method loadTexturePressed
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadTexturePressed : function () {},

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
 * @method setTitleFontName
 * @param {const char*}
 */
setTitleFontName : function () {},

/**
 * @method setFontSize
 * @param {int}
 */
setFontSize : function () {},

/**
 * @method loadTextures
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadTextures : function () {},

/**
 * @method setScale9Size
 * @param {cocos2d::CCSize}
 */
setScale9Size : function () {},

/**
 * @method getTitleColor
 * @return A value converted from C/C++ "cocos2d::ccColor3B"
 */
getTitleColor : function () {},

/**
 * @method loadTextureNormal
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadTextureNormal : function () {},

/**
 * @method setPressedTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setPressedTexture : function () {},

/**
 * @method setColor
 * @param {cocos2d::ccColor3B}
 */
setColor : function () {},

/**
 * @method setCapInsetsPressedRenderer
 * @param {cocos2d::CCRect}
 */
setCapInsetsPressedRenderer : function () {},

/**
 * @method setText
 * @param {const char*}
 */
setText : function () {},

/**
 * @method getTitleFontSize
 * @return A value converted from C/C++ "float"
 */
getTitleFontSize : function () {},

/**
 * @method getTitleFontName
 * @return A value converted from C/C++ "const char*"
 */
getTitleFontName : function () {},

/**
 * @method setScale9Enable
 * @param {bool}
 */
setScale9Enable : function () {},

/**
 * @method setCapInsets
 * @param {cocos2d::CCRect}
 */
setCapInsets : function () {},

/**
 * @method setDisabledTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setDisabledTexture : function () {},

/**
 * @method setPressedActionEnabled
 * @param {bool}
 */
setPressedActionEnabled : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UIButton*"
 */
create : function () {},

/**
 * @method UIButton
 * @constructor
 */
UIButton : function () {},

};

/**
 * @class UICheckBox
 */
ccs.UICheckBox = {

/**
 * @method setAnchorPoint
 * @param {cocos2d::CCPoint}
 */
setAnchorPoint : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method setBackGroundDisabledTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setBackGroundDisabledTexture : function () {},

/**
 * @method setTextures
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setTextures : function () {},

/**
 * @method loadTextureBackGroundDisabled
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadTextureBackGroundDisabled : function () {},

/**
 * @method setBackGroundSelectedTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setBackGroundSelectedTexture : function () {},

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
 * @method loadTextureFrontCross
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadTextureFrontCross : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method loadTextureBackGround
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadTextureBackGround : function () {},

/**
 * @method setSelectedState
 * @param {bool}
 */
setSelectedState : function () {},

/**
 * @method setFrontCrossDisabledTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setFrontCrossDisabledTexture : function () {},

/**
 * @method getSelectedState
 * @return A value converted from C/C++ "bool"
 */
getSelectedState : function () {},

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
 * @method loadTextures
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadTextures : function () {},

/**
 * @method loadTextureFrontCrossDisabled
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadTextureFrontCrossDisabled : function () {},

/**
 * @method loadTextureBackGroundSelected
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadTextureBackGroundSelected : function () {},

/**
 * @method setFrontCrossTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setFrontCrossTexture : function () {},

/**
 * @method setBackGroundTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setBackGroundTexture : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UICheckBox*"
 */
create : function () {},

/**
 * @method UICheckBox
 * @constructor
 */
UICheckBox : function () {},

};

/**
 * @class UIImageView
 */
ccs.UIImageView = {

/**
 * @method setAnchorPoint
 * @param {cocos2d::CCPoint}
 */
setAnchorPoint : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method setTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setTexture : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method setScale9Enabled
 * @param {bool}
 */
setScale9Enabled : function () {},

/**
 * @method checkDoubleClick
 * @param {float}
 */
checkDoubleClick : function () {},

/**
 * @method ignoreContentAdaptWithSize
 * @param {bool}
 */
ignoreContentAdaptWithSize : function () {},

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
 * @method setTextureRect
 * @param {cocos2d::CCRect}
 */
setTextureRect : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method loadTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadTexture : function () {},

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
 * @method setScale9Size
 * @param {cocos2d::CCSize}
 */
setScale9Size : function () {},

/**
 * @method setDoubleClickEnabled
 * @param {bool}
 */
setDoubleClickEnabled : function () {},

/**
 * @method setScale9Enable
 * @param {bool}
 */
setScale9Enable : function () {},

/**
 * @method setCapInsets
 * @param {cocos2d::CCRect}
 */
setCapInsets : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UIImageView*"
 */
create : function () {},

/**
 * @method UIImageView
 * @constructor
 */
UIImageView : function () {},

};

/**
 * @class UILabel
 */
ccs.UILabel = {

/**
 * @method setAnchorPoint
 * @param {cocos2d::CCPoint}
 */
setAnchorPoint : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method getStringValue
 * @return A value converted from C/C++ "const char*"
 */
getStringValue : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method setScale
 * @param {float}
 */
setScale : function () {},

/**
 * @method setTextVerticalAlignment
 * @param {cocos2d::CCVerticalTextAlignment}
 */
setTextVerticalAlignment : function () {},

/**
 * @method setFontName
 * @param {const char*}
 */
setFontName : function () {},

/**
 * @method setTouchScaleChangeEnabled
 * @param {bool}
 */
setTouchScaleChangeEnabled : function () {},

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
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method isTouchScaleChangeEnabled
 * @return A value converted from C/C++ "bool"
 */
isTouchScaleChangeEnabled : function () {},

/**
 * @method setTextAreaSize
 * @param {cocos2d::CCSize}
 */
setTextAreaSize : function () {},

/**
 * @method getStringLength
 * @return A value converted from C/C++ "int"
 */
getStringLength : function () {},

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
 * @method setTouchScaleChangeAble
 * @param {bool}
 */
setTouchScaleChangeAble : function () {},

/**
 * @method setFontSize
 * @param {int}
 */
setFontSize : function () {},

/**
 * @method setText
 * @param {const char*}
 */
setText : function () {},

/**
 * @method getTouchScaleChangeAble
 * @return A value converted from C/C++ "bool"
 */
getTouchScaleChangeAble : function () {},

/**
 * @method setTextHorizontalAlignment
 * @param {cocos2d::CCTextAlignment}
 */
setTextHorizontalAlignment : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UILabel*"
 */
create : function () {},

/**
 * @method UILabel
 * @constructor
 */
UILabel : function () {},

};

/**
 * @class UILabelAtlas
 */
ccs.UILabelAtlas = {

/**
 * @method setAnchorPoint
 * @param {cocos2d::CCPoint}
 */
setAnchorPoint : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method getStringValue
 * @return A value converted from C/C++ "const char*"
 */
getStringValue : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method setProperty
 * @param {const char*}
 * @param {const char*}
 * @param {int}
 * @param {int}
 * @param {const char*}
 */
setProperty : function () {},

/**
 * @method setStringValue
 * @param {const char*}
 */
setStringValue : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UILabelAtlas*"
 */
create : function () {},

/**
 * @method UILabelAtlas
 * @constructor
 */
UILabelAtlas : function () {},

};

/**
 * @class UILoadingBar
 */
ccs.UILoadingBar = {

/**
 * @method setPercent
 * @param {int}
 */
setPercent : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method setTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setTexture : function () {},

/**
 * @method ignoreContentAdaptWithSize
 * @param {bool}
 */
ignoreContentAdaptWithSize : function () {},

/**
 * @method loadTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadTexture : function () {},

/**
 * @method setDirection
 * @param {cocos2d::extension::LoadingBarType}
 */
setDirection : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method setScale9Enabled
 * @param {bool}
 */
setScale9Enabled : function () {},

/**
 * @method setScale9Size
 * @param {cocos2d::CCSize}
 */
setScale9Size : function () {},

/**
 * @method setScale9Enable
 * @param {bool}
 */
setScale9Enable : function () {},

/**
 * @method setCapInsets
 * @param {cocos2d::CCRect}
 */
setCapInsets : function () {},

/**
 * @method getDirection
 * @return A value converted from C/C++ "int"
 */
getDirection : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method getPercent
 * @return A value converted from C/C++ "int"
 */
getPercent : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UILoadingBar*"
 */
create : function () {},

/**
 * @method UILoadingBar
 * @constructor
 */
UILoadingBar : function () {},

};

/**
 * @class UIScrollView
 */
ccs.UIScrollView = {

/**
 * @method addChild
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::extension::UIWidget*}
 */
addChild : function () {},

/**
 * @method scrollToTop
 * @param {float}
 * @param {bool}
 */
scrollToTop : function () {},

/**
 * @method scrollToPercentHorizontal
 * @param {float}
 * @param {float}
 * @param {bool}
 */
scrollToPercentHorizontal : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method removeAllChildren
 */
removeAllChildren : function () {},

/**
 * @method scrollToPercentBothDirection
 * @param {cocos2d::CCPoint}
 * @param {float}
 * @param {bool}
 */
scrollToPercentBothDirection : function () {},

/**
 * @method getChildren
 * @return A value converted from C/C++ "cocos2d::CCArray*"
 */
getChildren : function () {},

/**
 * @method getDirection
 * @return A value converted from C/C++ "cocos2d::extension::SCROLLVIEW_DIR"
 */
getDirection : function () {},

/**
 * @method scrollToBottomLeft
 * @param {float}
 * @param {bool}
 */
scrollToBottomLeft : function () {},

/**
 * @method getInnerContainer
 * @return A value converted from C/C++ "cocos2d::extension::UILayout*"
 */
getInnerContainer : function () {},

/**
 * @method jumpToBottom
 */
jumpToBottom : function () {},

/**
 * @method setDirection
 * @param {cocos2d::extension::SCROLLVIEW_DIR}
 */
setDirection : function () {},

/**
 * @method scrollToTopLeft
 * @param {float}
 * @param {bool}
 */
scrollToTopLeft : function () {},

/**
 * @method jumpToTopRight
 */
jumpToTopRight : function () {},

/**
 * @method jumpToBottomLeft
 */
jumpToBottomLeft : function () {},

/**
 * @method setInnerContainerSize
 * @param {cocos2d::CCSize}
 */
setInnerContainerSize : function () {},

/**
 * @method isInertiaScrollEnabled
 * @return A value converted from C/C++ "bool"
 */
isInertiaScrollEnabled : function () {},

/**
 * @method getInnerContainerSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getInnerContainerSize : function () {},

/**
 * @method isBounceEnabled
 * @return A value converted from C/C++ "bool"
 */
isBounceEnabled : function () {},

/**
 * @method jumpToPercentVertical
 * @param {float}
 */
jumpToPercentVertical : function () {},

/**
 * @method doLayout
 */
doLayout : function () {},

/**
 * @method setInertiaScrollEnabled
 * @param {bool}
 */
setInertiaScrollEnabled : function () {},

/**
 * @method jumpToTopLeft
 */
jumpToTopLeft : function () {},

/**
 * @method jumpToPercentHorizontal
 * @param {float}
 */
jumpToPercentHorizontal : function () {},

/**
 * @method jumpToBottomRight
 */
jumpToBottomRight : function () {},

/**
 * @method update
 * @param {float}
 */
update : function () {},

/**
 * @method getLayoutType
 * @return A value converted from C/C++ "cocos2d::extension::LayoutType"
 */
getLayoutType : function () {},

/**
 * @method setBounceEnabled
 * @param {bool}
 */
setBounceEnabled : function () {},

/**
 * @method jumpToTop
 */
jumpToTop : function () {},

/**
 * @method scrollToLeft
 * @param {float}
 * @param {bool}
 */
scrollToLeft : function () {},

/**
 * @method jumpToPercentBothDirection
 * @param {cocos2d::CCPoint}
 */
jumpToPercentBothDirection : function () {},

/**
 * @method scrollToPercentVertical
 * @param {float}
 * @param {float}
 * @param {bool}
 */
scrollToPercentVertical : function () {},

/**
 * @method scrollToBottom
 * @param {float}
 * @param {bool}
 */
scrollToBottom : function () {},

/**
 * @method scrollToBottomRight
 * @param {float}
 * @param {bool}
 */
scrollToBottomRight : function () {},

/**
 * @method jumpToLeft
 */
jumpToLeft : function () {},

/**
 * @method scrollToRight
 * @param {float}
 * @param {bool}
 */
scrollToRight : function () {},

/**
 * @method removeChild
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::extension::UIWidget*}
 */
removeChild : function () {},

/**
 * @method setLayoutType
 * @param {cocos2d::extension::LayoutType}
 */
setLayoutType : function () {},

/**
 * @method jumpToRight
 */
jumpToRight : function () {},

/**
 * @method scrollToTopRight
 * @param {float}
 * @param {bool}
 */
scrollToTopRight : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UIScrollView*"
 */
create : function () {},

/**
 * @method UIScrollView
 * @constructor
 */
UIScrollView : function () {},

};

/**
 * @class UISlider
 */
ccs.UISlider = {

/**
 * @method setPercent
 * @param {int}
 */
setPercent : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method loadSlidBallTextureNormal
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadSlidBallTextureNormal : function () {},

/**
 * @method loadProgressBarTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadProgressBarTexture : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method setScale9Enabled
 * @param {bool}
 */
setScale9Enabled : function () {},

/**
 * @method setBarTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setBarTexture : function () {},

/**
 * @method ignoreContentAdaptWithSize
 * @param {bool}
 */
ignoreContentAdaptWithSize : function () {},

/**
 * @method loadSlidBallTextures
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadSlidBallTextures : function () {},

/**
 * @method setSlidBallDisabledTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setSlidBallDisabledTexture : function () {},

/**
 * @method setSlidBallTextures
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setSlidBallTextures : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method setSlidBallNormalTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setSlidBallNormalTexture : function () {},

/**
 * @method loadBarTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadBarTexture : function () {},

/**
 * @method setSlidBallPercent
 * @param {int}
 */
setSlidBallPercent : function () {},

/**
 * @method loadSlidBallTexturePressed
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadSlidBallTexturePressed : function () {},

/**
 * @method setSlidBallPressedTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setSlidBallPressedTexture : function () {},

/**
 * @method setScale9Size
 * @param {cocos2d::CCSize}
 */
setScale9Size : function () {},

/**
 * @method setCapInsetProgressBarRebderer
 * @param {cocos2d::CCRect}
 */
setCapInsetProgressBarRebderer : function () {},

/**
 * @method setCapInsetsBarRenderer
 * @param {cocos2d::CCRect}
 */
setCapInsetsBarRenderer : function () {},

/**
 * @method setScale9Enable
 * @param {bool}
 */
setScale9Enable : function () {},

/**
 * @method setCapInsets
 * @param {cocos2d::CCRect}
 */
setCapInsets : function () {},

/**
 * @method setProgressBarTexture
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
setProgressBarTexture : function () {},

/**
 * @method loadSlidBallTextureDisabled
 * @param {const char*}
 * @param {cocos2d::extension::TextureResType}
 */
loadSlidBallTextureDisabled : function () {},

/**
 * @method getPercent
 * @return A value converted from C/C++ "int"
 */
getPercent : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UISlider*"
 */
create : function () {},

/**
 * @method UISlider
 * @constructor
 */
UISlider : function () {},

};

/**
 * @class UITextField
 */
ccs.UITextField = {

/**
 * @method setAnchorPoint
 * @param {cocos2d::CCPoint}
 */
setAnchorPoint : function () {},

/**
 * @method setAttachWithIME
 * @param {bool}
 */
setAttachWithIME : function () {},

/**
 * @method getStringValue
 * @return A value converted from C/C++ "const char*"
 */
getStringValue : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method getDeleteBackward
 * @return A value converted from C/C++ "bool"
 */
getDeleteBackward : function () {},

/**
 * @method getAttachWithIME
 * @return A value converted from C/C++ "bool"
 */
getAttachWithIME : function () {},

/**
 * @method setFontName
 * @param {const char*}
 */
setFontName : function () {},

/**
 * @method getInsertText
 * @return A value converted from C/C++ "bool"
 */
getInsertText : function () {},

/**
 * @method initRenderer
 */
initRenderer : function () {},

/**
 * @method getDetachWithIME
 * @return A value converted from C/C++ "bool"
 */
getDetachWithIME : function () {},

/**
 * @method setOpacity
 * @param {int}
 */
setOpacity : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method didNotSelectSelf
 */
didNotSelectSelf : function () {},

/**
 * @method isPasswordEnabled
 * @return A value converted from C/C++ "bool"
 */
isPasswordEnabled : function () {},

/**
 * @method attachWithIME
 */
attachWithIME : function () {},

/**
 * @method setMaxLengthEnable
 * @param {bool}
 */
setMaxLengthEnable : function () {},

/**
 * @method setPasswordEnabled
 * @param {bool}
 */
setPasswordEnabled : function () {},

/**
 * @method setPasswordEnable
 * @param {bool}
 */
setPasswordEnable : function () {},

/**
 * @method update
 * @param {float}
 */
update : function () {},

/**
 * @method setMaxLengthEnabled
 * @param {bool}
 */
setMaxLengthEnabled : function () {},

/**
 * @method setPasswordStyleText
 * @param {const char*}
 */
setPasswordStyleText : function () {},

/**
 * @method setFontSize
 * @param {int}
 */
setFontSize : function () {},

/**
 * @method setPlaceHolder
 * @param {const char*}
 */
setPlaceHolder : function () {},

/**
 * @method setColor
 * @param {cocos2d::ccColor3B}
 */
setColor : function () {},

/**
 * @method getMaxLength
 * @return A value converted from C/C++ "int"
 */
getMaxLength : function () {},

/**
 * @method isMaxLengthEnabled
 * @return A value converted from C/C++ "bool"
 */
isMaxLengthEnabled : function () {},

/**
 * @method setDetachWithIME
 * @param {bool}
 */
setDetachWithIME : function () {},

/**
 * @method setText
 * @param {const char*}
 */
setText : function () {},

/**
 * @method setInsertText
 * @param {bool}
 */
setInsertText : function () {},

/**
 * @method setMaxLength
 * @param {int}
 */
setMaxLength : function () {},

/**
 * @method setTouchSize
 * @param {cocos2d::CCSize}
 */
setTouchSize : function () {},

/**
 * @method setDeleteBackward
 * @param {bool}
 */
setDeleteBackward : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UITextField*"
 */
create : function () {},

/**
 * @method UITextField
 * @constructor
 */
UITextField : function () {},

};

/**
 * @class UIListView
 */
ccs.UIListView = {

/**
 * @method addChild
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::extension::UIWidget*}
 */
addChild : function () {},

/**
 * @method initChildWithDataLength
 * @param {int}
 */
initChildWithDataLength : function () {},

/**
 * @method getDataLength
 * @return A value converted from C/C++ "int"
 */
getDataLength : function () {},

/**
 * @method getUpdateChild
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 */
getUpdateChild : function () {},

/**
 * @method update
 * @param {float}
 */
update : function () {},

/**
 * @method setDirection
 * @param {cocos2d::extension::ListViewDirection}
 */
setDirection : function () {},

/**
 * @method setUpdateSuccess
 * @param {bool}
 */
setUpdateSuccess : function () {},

/**
 * @method removeAllChildren
 */
removeAllChildren : function () {},

/**
 * @method removeChild
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::extension::UIWidget*}
 */
removeChild : function () {},

/**
 * @method getDirection
 * @return A value converted from C/C++ "cocos2d::extension::ListViewDirection"
 */
getDirection : function () {},

/**
 * @method getUpdateSuccess
 * @return A value converted from C/C++ "bool"
 */
getUpdateSuccess : function () {},

/**
 * @method doLayout
 */
doLayout : function () {},

/**
 * @method getUpdateDataIndex
 * @return A value converted from C/C++ "int"
 */
getUpdateDataIndex : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UIListView*"
 */
create : function () {},

/**
 * @method UIListView
 * @constructor
 */
UIListView : function () {},

};

/**
 * @class UILabelBMFont
 */
ccs.UILabelBMFont = {

/**
 * @method setAnchorPoint
 * @param {cocos2d::CCPoint}
 */
setAnchorPoint : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method getStringValue
 * @return A value converted from C/C++ "const char*"
 */
getStringValue : function () {},

/**
 * @method setText
 * @param {const char*}
 */
setText : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method setFntFile
 * @param {const char*}
 */
setFntFile : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UILabelBMFont*"
 */
create : function () {},

/**
 * @method UILabelBMFont
 * @constructor
 */
UILabelBMFont : function () {},

};

/**
 * @class UIPageView
 */
ccs.UIPageView = {

/**
 * @method getLayoutType
 * @return A value converted from C/C++ "cocos2d::extension::LayoutType"
 */
getLayoutType : function () {},

/**
 * @method getCurPageIndex
 * @return A value converted from C/C++ "int"
 */
getCurPageIndex : function () {},

/**
 * @method addWidgetToPage
 * @param {cocos2d::extension::UIWidget*}
 * @param {int}
 * @param {bool}
 */
addWidgetToPage : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "const char*"
 */
getDescription : function () {},

/**
 * @method insertPage
 * @param {cocos2d::extension::UILayout*}
 * @param {int}
 */
insertPage : function () {},

/**
 * @method setLayoutType
 * @param {cocos2d::extension::LayoutType}
 */
setLayoutType : function () {},

/**
 * @method scrollToPage
 * @param {int}
 */
scrollToPage : function () {},

/**
 * @method update
 * @param {float}
 */
update : function () {},

/**
 * @method doLayout
 */
doLayout : function () {},

/**
 * @method getPages
 * @return A value converted from C/C++ "cocos2d::CCArray*"
 */
getPages : function () {},

/**
 * @method removeAllPages
 */
removeAllPages : function () {},

/**
 * @method addPage
 * @param {cocos2d::extension::UILayout*}
 */
addPage : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UIPageView*"
 */
create : function () {},

/**
 * @method UIPageView
 * @constructor
 */
UIPageView : function () {},

};

/**
 * @class UIHelper
 */
ccs.UIHelper = {

/**
 * @method seekActionWidgetByActionTag
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 * @param {cocos2d::extension::UIWidget*}
 * @param {int}
 */
seekActionWidgetByActionTag : function () {},

/**
 * @method seekWidgetByTag
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 * @param {cocos2d::extension::UIWidget*}
 * @param {int}
 */
seekWidgetByTag : function () {},

/**
 * @method seekWidgetByRelativeName
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 * @param {cocos2d::extension::UIWidget*}
 * @param {const char*}
 */
seekWidgetByRelativeName : function () {},

/**
 * @method seekWidgetByName
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 * @param {cocos2d::extension::UIWidget*}
 * @param {const char*}
 */
seekWidgetByName : function () {},

};

/**
 * @class GUIReader
 */
ccs.GUIReader = {

/**
 * @method widgetFromJsonFile
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 * @param {const char*}
 */
widgetFromJsonFile : function () {},

/**
 * @method purgeGUIReader
 */
purgeGUIReader : function () {},

/**
 * @method shareReader
 * @return A value converted from C/C++ "cocos2d::extension::GUIReader*"
 */
shareReader : function () {},

};

/**
 * @class UILayer
 */
ccs.UILayer = {

/**
 * @method removeWidget
 * @param {cocos2d::extension::UIWidget*}
 */
removeWidget : function () {},

/**
 * @method clear
 */
clear : function () {},

/**
 * @method setVisible
 * @param {bool}
 */
setVisible : function () {},

/**
 * @method getWidgetByTag
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 * @param {int}
 */
getWidgetByTag : function () {},

/**
 * @method dispose
 */
dispose : function () {},

/**
 * @method getRootWidget
 * @return A value converted from C/C++ "cocos2d::extension::UIRootWidget*"
 */
getRootWidget : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method removeWidgetAndCleanUp
 * @param {cocos2d::extension::UIWidget*}
 * @param {bool}
 */
removeWidgetAndCleanUp : function () {},

/**
 * @method getWidgetByName
 * @return A value converted from C/C++ "cocos2d::extension::UIWidget*"
 * @param {const char*}
 */
getWidgetByName : function () {},

/**
 * @method addWidget
 * @param {cocos2d::extension::UIWidget*}
 */
addWidget : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::extension::UILayer*"
 */
create : function () {},

/**
 * @method UILayer
 * @constructor
 */
UILayer : function () {},

};

/**
 * @class SceneReader
 */
ccs.SceneReader = {

/**
 * @method purgeSceneReader
 */
purgeSceneReader : function () {},

/**
 * @method createNodeWithSceneFile
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 * @param {const char*}
 */
createNodeWithSceneFile : function () {},

/**
 * @method sceneReaderVersion
 * @return A value converted from C/C++ "const char*"
 */
sceneReaderVersion : function () {},

/**
 * @method sharedSceneReader
 * @return A value converted from C/C++ "cocos2d::extension::SceneReader*"
 */
sharedSceneReader : function () {},

};

/**
 * @class ActionObject
 */
ccs.ActionObject = {

/**
 * @method setCurrentTime
 * @param {float}
 */
setCurrentTime : function () {},

/**
 * @method pause
 */
pause : function () {},

/**
 * @method setName
 * @param {const char*}
 */
setName : function () {},

/**
 * @method setUnitTime
 * @param {float}
 */
setUnitTime : function () {},

/**
 * @method getName
 * @return A value converted from C/C++ "const char*"
 */
getName : function () {},

/**
 * @method stop
 */
stop : function () {},

/**
 * @method play
 */
play : function () {},

/**
 * @method getCurrentTime
 * @return A value converted from C/C++ "float"
 */
getCurrentTime : function () {},

/**
 * @method removeActionNode
 * @param {cocos2d::extension::ActionNode*}
 */
removeActionNode : function () {},

/**
 * @method getLoop
 * @return A value converted from C/C++ "bool"
 */
getLoop : function () {},

/**
 * @method initWithDictionary
 * @param {cs::CSJsonDictionary*}
 * @param {cocos2d::CCObject*}
 */
initWithDictionary : function () {},

/**
 * @method addActionNode
 * @param {cocos2d::extension::ActionNode*}
 */
addActionNode : function () {},

/**
 * @method getUnitTime
 * @return A value converted from C/C++ "float"
 */
getUnitTime : function () {},

/**
 * @method isPlaying
 * @return A value converted from C/C++ "bool"
 */
isPlaying : function () {},

/**
 * @method updateToFrameByTime
 * @param {float}
 */
updateToFrameByTime : function () {},

/**
 * @method setLoop
 * @param {bool}
 */
setLoop : function () {},

/**
 * @method simulationActionUpdate
 * @param {float}
 */
simulationActionUpdate : function () {},

/**
 * @method ActionObject
 * @constructor
 */
ActionObject : function () {},

};

/**
 * @class ActionManager
 */
ccs.ActionManager = {

/**
 * @method playActionByName
 * @return A value converted from C/C++ "cocos2d::extension::ActionObject*"
 * @param {const char*}
 * @param {const char*}
 */
playActionByName : function () {},

/**
 * @method getActionByName
 * @return A value converted from C/C++ "cocos2d::extension::ActionObject*"
 * @param {const char*}
 * @param {const char*}
 */
getActionByName : function () {},

/**
 * @method releaseActions
 */
releaseActions : function () {},

/**
 * @method purgeActionManager
 */
purgeActionManager : function () {},

/**
 * @method shareManager
 * @return A value converted from C/C++ "cocos2d::extension::ActionManager*"
 */
shareManager : function () {},

};
