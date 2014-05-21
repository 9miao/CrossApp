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

};

/**
 * @class ColliderBody
 */
ccs.ColliderBody = {

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
 * @method changeDisplayWithIndex
 * @param {int}
 * @param {bool}
 */
changeDisplayWithIndex : function () {},

/**
 * @method changeDisplayWithName
 * @param {const char*}
 * @param {bool}
 */
changeDisplayWithName : function () {},

/**
 * @method getCurrentDisplayIndex
 * @return A value converted from C/C++ "int"
 */
getCurrentDisplayIndex : function () {},

/**
 * @method getForceChangeDisplay
 * @return A value converted from C/C++ "bool"
 */
getForceChangeDisplay : function () {},

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
 * @method isVisible
 * @return A value converted from C/C++ "bool"
 */
isVisible : function () {},

/**
 * @method setVisible
 * @param {bool}
 */
setVisible : function () {},

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
 * @method isBlendDirty
 * @return A value converted from C/C++ "bool"
 */
isBlendDirty : function () {},

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
 * @method updateColor
 */
updateColor : function () {},

/**
 * @method setChildArmature
 * @param {cocos2d::extension::CCArmature*}
 */
setChildArmature : function () {},

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
 * @method setParentBone
 * @param {cocos2d::extension::CCBone*}
 */
setParentBone : function () {},

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
 * @method getChildArmature
 * @return A value converted from C/C++ "cocos2d::extension::CCArmature*"
 */
getChildArmature : function () {},

/**
 * @method changeDisplayWithIndex
 * @param {int}
 * @param {bool}
 */
changeDisplayWithIndex : function () {},

/**
 * @method changeDisplayWithName
 * @param {const char*}
 * @param {bool}
 */
changeDisplayWithName : function () {},

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
 * @method setBlendDirty
 * @param {bool}
 */
setBlendDirty : function () {},

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
 * @method setAnimationInternal
 * @param {float}
 */
setAnimationInternal : function () {},

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
 * @method getSpeedScale
 * @return A value converted from C/C++ "float"
 */
getSpeedScale : function () {},

/**
 * @method pause
 */
pause : function () {},

/**
 * @method setSpeedScale
 * @param {float}
 */
setSpeedScale : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::extension::CCArmature*}
 */
init : function () {},

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
 * @method playWithArray
 * @param {cocos2d::CCArray*}
 * @param {int}
 * @param {bool}
 */
playWithArray : function () {},

/**
 * @method playWithIndexArray
 * @param {cocos2d::CCArray*}
 * @param {int}
 * @param {bool}
 */
playWithIndexArray : function () {},

/**
 * @method playWithIndex
 * @param {int}
 * @param {int}
 * @param {int}
 * @param {int}
 * @param {int}
 */
playWithIndex : function () {},

/**
 * @method getCurrentMovementID
 * @return A value converted from C/C++ "std::string"
 */
getCurrentMovementID : function () {},

/**
 * @method setAnimationScale
 * @param {float}
 */
setAnimationScale : function () {},

/**
 * @method setAnimationData
 * @param {cocos2d::extension::CCAnimationData*}
 */
setAnimationData : function () {},

/**
 * @method gotoAndPlay
 * @param {int}
 */
gotoAndPlay : function () {},

/**
 * @method getMovementCount
 * @return A value converted from C/C++ "int"
 */
getMovementCount : function () {},

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
 * @method getParentBone
 * @return A value converted from C/C++ "cocos2d::extension::CCBone*"
 */
getParentBone : function () {},

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
 * @method drawContour
 */
drawContour : function () {},

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
 * @param {float}
 */
getFloat : function () {},

/**
 * @method getBool
 * @return A value converted from C/C++ "bool"
 * @param {const char*}
 * @param {bool}
 */
getBool : function () {},

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
 * @param {const char*}
 */
getCString : function () {},

/**
 * @method serialize
 * @return A value converted from C/C++ "bool"
 * @param {void*}
 */
serialize : function () {},

/**
 * @method setInt
 * @param {const char*}
 * @param {int}
 */
setInt : function () {},

/**
 * @method parse
 * @return A value converted from C/C++ "bool"
 * @param {std::string}
 */
parse : function () {},

/**
 * @method getInt
 * @return A value converted from C/C++ "int"
 * @param {const char*}
 * @param {int}
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

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

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
 * @method serialize
 * @return A value converted from C/C++ "bool"
 * @param {void*}
 */
serialize : function () {},

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

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

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
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

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
 * @method serialize
 * @return A value converted from C/C++ "bool"
 * @param {void*}
 */
serialize : function () {},

/**
 * @method getNode
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getNode : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

};

/**
 * @class LayoutParameter
 */
ccs.LayoutParameter = {

/**
 * @method clone
 * @return A value converted from C/C++ "cocos2d::ui::LayoutParameter*"
 */
clone : function () {},

/**
 * @method getLayoutType
 * @return A value converted from C/C++ "cocos2d::ui::LayoutParameterType"
 */
getLayoutType : function () {},

/**
 * @method createCloneInstance
 * @return A value converted from C/C++ "cocos2d::ui::LayoutParameter*"
 */
createCloneInstance : function () {},

/**
 * @method copyProperties
 * @param {cocos2d::ui::LayoutParameter*}
 */
copyProperties : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::LayoutParameter*"
 */
create : function () {},

/**
 * @method LayoutParameter
 * @constructor
 */
LayoutParameter : function () {},

};

/**
 * @class LinearLayoutParameter
 */
ccs.LinearLayoutParameter = {

/**
 * @method setGravity
 * @param {cocos2d::ui::LinearGravity}
 */
setGravity : function () {},

/**
 * @method createCloneInstance
 * @return A value converted from C/C++ "cocos2d::ui::LayoutParameter*"
 */
createCloneInstance : function () {},

/**
 * @method copyProperties
 * @param {cocos2d::ui::LayoutParameter*}
 */
copyProperties : function () {},

/**
 * @method getGravity
 * @return A value converted from C/C++ "cocos2d::ui::LinearGravity"
 */
getGravity : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::LinearLayoutParameter*"
 */
create : function () {},

/**
 * @method LinearLayoutParameter
 * @constructor
 */
LinearLayoutParameter : function () {},

};

/**
 * @class RelativeLayoutParameter
 */
ccs.RelativeLayoutParameter = {

/**
 * @method setAlign
 * @param {cocos2d::ui::RelativeAlign}
 */
setAlign : function () {},

/**
 * @method copyProperties
 * @param {cocos2d::ui::LayoutParameter*}
 */
copyProperties : function () {},

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
 * @return A value converted from C/C++ "cocos2d::ui::RelativeAlign"
 */
getAlign : function () {},

/**
 * @method createCloneInstance
 * @return A value converted from C/C++ "cocos2d::ui::LayoutParameter*"
 */
createCloneInstance : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::RelativeLayoutParameter*"
 */
create : function () {},

/**
 * @method RelativeLayoutParameter
 * @constructor
 */
RelativeLayoutParameter : function () {},

};

/**
 * @class Widget
 */
ccs.Widget = {

/**
 * @method setSizePercent
 * @param {cocos2d::CCPoint}
 */
setSizePercent : function () {},

/**
 * @method getCustomSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getCustomSize : function () {},

/**
 * @method isFlipX
 * @return A value converted from C/C++ "bool"
 */
isFlipX : function () {},

/**
 * @method setActionTag
 * @param {int}
 */
setActionTag : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "std::string"
 */
getDescription : function () {},

/**
 * @method getLeftInParent
 * @return A value converted from C/C++ "float"
 */
getLeftInParent : function () {},

/**
 * @method isFlipY
 * @return A value converted from C/C++ "bool"
 */
isFlipY : function () {},

/**
 * @method getTouchEndPos
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getTouchEndPos : function () {},

/**
 * @method getChildren
 * @return A value converted from C/C++ "cocos2d::CCArray*"
 */
getChildren : function () {},

/**
 * @method getNodes
 * @return A value converted from C/C++ "cocos2d::CCArray*"
 */
getNodes : function () {},

/**
 * @method getLayoutSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getLayoutSize : function () {},

/**
 * @method getChildByTag
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 * @param {int}
 */
getChildByTag : function () {},

/**
 * @method getName
 * @return A value converted from C/C++ "const char*"
 */
getName : function () {},

/**
 * @method isIgnoreContentAdaptWithSize
 * @return A value converted from C/C++ "bool"
 */
isIgnoreContentAdaptWithSize : function () {},

/**
 * @method getWidgetType
 * @return A value converted from C/C++ "cocos2d::ui::WidgetType"
 */
getWidgetType : function () {},

/**
 * @method setLayoutParameter
 * @param {cocos2d::ui::LayoutParameter*}
 */
setLayoutParameter : function () {},

/**
 * @method getBottomInParent
 * @return A value converted from C/C++ "float"
 */
getBottomInParent : function () {},

/**
 * @method getActionTag
 * @return A value converted from C/C++ "int"
 */
getActionTag : function () {},

/**
 * @method getLayoutParameter
 * @return A value converted from C/C++ "cocos2d::ui::LayoutParameter*"
 * @param {cocos2d::ui::LayoutParameterType}
 */
getLayoutParameter : function () {},

/**
 * @method getPositionType
 * @return A value converted from C/C++ "cocos2d::ui::PositionType"
 */
getPositionType : function () {},

/**
 * @method setName
 * @param {const char*}
 */
setName : function () {},

/**
 * @method getChildByName
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 * @param {const char*}
 */
getChildByName : function () {},

/**
 * @method isEnabled
 * @return A value converted from C/C++ "bool"
 */
isEnabled : function () {},

/**
 * @method removeNodeByTag
 * @param {int}
 */
removeNodeByTag : function () {},

/**
 * @method removeAllChildren
 */
removeAllChildren : function () {},

/**
 * @method isTouchEnabled
 * @return A value converted from C/C++ "bool"
 */
isTouchEnabled : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method getTouchStartPos
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getTouchStartPos : function () {},

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
 * @method setTouchEnabled
 * @param {bool}
 */
setTouchEnabled : function () {},

/**
 * @method clone
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 */
clone : function () {},

/**
 * @method getTouchMovePos
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getTouchMovePos : function () {},

/**
 * @method setEnabled
 * @param {bool}
 */
setEnabled : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method setBrightStyle
 * @param {cocos2d::ui::BrightStyle}
 */
setBrightStyle : function () {},

/**
 * @method removeFromParent
 */
removeFromParent : function () {},

/**
 * @method getColor
 * @return A value converted from C/C++ "cocos2d::ccColor3B"
 */
getColor : function () {},

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
 * @method removeAllChildrenWithCleanup
 * @param {bool}
 */
removeAllChildrenWithCleanup : function () {},

/**
 * @method sortAllChildren
 */
sortAllChildren : function () {},

/**
 * @method ignoreContentAdaptWithSize
 * @param {bool}
 */
ignoreContentAdaptWithSize : function () {},

/**
 * @method isBright
 * @return A value converted from C/C++ "bool"
 */
isBright : function () {},

/**
 * @method clippingParentAreaContainPoint
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::CCPoint}
 */
clippingParentAreaContainPoint : function () {},

/**
 * @method getSizePercent
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getSizePercent : function () {},

/**
 * @method removeFromParentAndCleanup
 * @param {bool}
 */
removeFromParentAndCleanup : function () {},

/**
 * @method setOpacity
 * @param {unsigned char}
 */
setOpacity : function () {},

/**
 * @method getTopInParent
 * @return A value converted from C/C++ "float"
 */
getTopInParent : function () {},

/**
 * @method getOpacity
 * @return A value converted from C/C++ "unsigned char"
 */
getOpacity : function () {},

/**
 * @method setColor
 * @param {cocos2d::ccColor3B}
 */
setColor : function () {},

/**
 * @method getNodeByTag
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 * @param {int}
 */
getNodeByTag : function () {},

/**
 * @method getSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getSize : function () {},

/**
 * @method getRightInParent
 * @return A value converted from C/C++ "float"
 */
getRightInParent : function () {},

/**
 * @method getSizeType
 * @return A value converted from C/C++ "cocos2d::ui::SizeType"
 */
getSizeType : function () {},

/**
 * @method removeNode
 * @param {cocos2d::CCNode*}
 */
removeNode : function () {},

/**
 * @method removeAllNodes
 */
removeAllNodes : function () {},

/**
 * @method getWorldPosition
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getWorldPosition : function () {},

/**
 * @method getPositionPercent
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getPositionPercent : function () {},

/**
 * @method removeChildByTag
 * @param {int}
 * @param {bool}
 */
removeChildByTag : function () {},

/**
 * @method hitTest
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::CCPoint}
 */
hitTest : function () {},

/**
 * @method isFocused
 * @return A value converted from C/C++ "bool"
 */
isFocused : function () {},

/**
 * @method setSizeType
 * @param {cocos2d::ui::SizeType}
 */
setSizeType : function () {},

/**
 * @method checkChildInfo
 * @param {int}
 * @param {cocos2d::ui::Widget*}
 * @param {cocos2d::CCPoint}
 */
checkChildInfo : function () {},

/**
 * @method setSize
 * @param {cocos2d::CCSize}
 */
setSize : function () {},

/**
 * @method setBright
 * @param {bool}
 */
setBright : function () {},

/**
 * @method getChildrenCount
 * @return A value converted from C/C++ "unsigned int"
 */
getChildrenCount : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 */
create : function () {},

/**
 * @method Widget
 * @constructor
 */
Widget : function () {},

};

/**
 * @class Layout
 */
ccs.Layout = {

/**
 * @method setBackGroundColorVector
 * @param {cocos2d::CCPoint}
 */
setBackGroundColorVector : function () {},

/**
 * @method setClippingType
 * @param {cocos2d::ui::LayoutClippingType}
 */
setClippingType : function () {},

/**
 * @method setBackGroundColorType
 * @param {cocos2d::ui::LayoutBackGroundColorType}
 */
setBackGroundColorType : function () {},

/**
 * @method setBackGroundImageColor
 * @param {cocos2d::ccColor3B}
 */
setBackGroundImageColor : function () {},

/**
 * @method getBackGroundColorVector
 * @return A value converted from C/C++ "cocos2d::CCPoint"
 */
getBackGroundColorVector : function () {},

/**
 * @method removeAllChildren
 */
removeAllChildren : function () {},

/**
 * @method getClippingType
 * @return A value converted from C/C++ "cocos2d::ui::LayoutClippingType"
 */
getClippingType : function () {},

/**
 * @method removeBackGroundImage
 */
removeBackGroundImage : function () {},

/**
 * @method getBackGroundColorOpacity
 * @return A value converted from C/C++ "unsigned char"
 */
getBackGroundColorOpacity : function () {},

/**
 * @method isClippingEnabled
 * @return A value converted from C/C++ "bool"
 */
isClippingEnabled : function () {},

/**
 * @method setBackGroundImageOpacity
 * @param {unsigned char}
 */
setBackGroundImageOpacity : function () {},

/**
 * @method setBackGroundImage
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
setBackGroundImage : function () {},

/**
 * @method requestDoLayout
 */
requestDoLayout : function () {},

/**
 * @method getBackGroundImageCapInsets
 * @return A value converted from C/C++ "cocos2d::CCRect"
 */
getBackGroundImageCapInsets : function () {},

/**
 * @method getBackGroundColor
 * @return A value converted from C/C++ "cocos2d::ccColor3B"
 */
getBackGroundColor : function () {},

/**
 * @method setClippingEnabled
 * @param {bool}
 */
setClippingEnabled : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "std::string"
 */
getDescription : function () {},

/**
 * @method getBackGroundImageColor
 * @return A value converted from C/C++ "cocos2d::ccColor3B"
 */
getBackGroundImageColor : function () {},

/**
 * @method isBackGroundImageScale9Enabled
 * @return A value converted from C/C++ "bool"
 */
isBackGroundImageScale9Enabled : function () {},

/**
 * @method getBackGroundColorType
 * @return A value converted from C/C++ "cocos2d::ui::LayoutBackGroundColorType"
 */
getBackGroundColorType : function () {},

/**
 * @method getBackGroundEndColor
 * @return A value converted from C/C++ "cocos2d::ccColor3B"
 */
getBackGroundEndColor : function () {},

/**
 * @method setBackGroundColorOpacity
 * @param {unsigned char}
 */
setBackGroundColorOpacity : function () {},

/**
 * @method getBackGroundImageOpacity
 * @return A value converted from C/C++ "unsigned char"
 */
getBackGroundImageOpacity : function () {},

/**
 * @method removeAllChildrenWithCleanup
 * @param {bool}
 */
removeAllChildrenWithCleanup : function () {},

/**
 * @method sortAllChildren
 */
sortAllChildren : function () {},

/**
 * @method setBackGroundImageCapInsets
 * @param {cocos2d::CCRect}
 */
setBackGroundImageCapInsets : function () {},

/**
 * @method getBackGroundImageTextureSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getBackGroundImageTextureSize : function () {},

/**
 * @method getLayoutType
 * @return A value converted from C/C++ "cocos2d::ui::LayoutType"
 */
getLayoutType : function () {},

/**
 * @method getBackGroundStartColor
 * @return A value converted from C/C++ "cocos2d::ccColor3B"
 */
getBackGroundStartColor : function () {},

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
 * @param {cocos2d::ui::LayoutType}
 */
setLayoutType : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::Layout*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method Layout
 * @constructor
 */
Layout : function () {},

};

/**
 * @class Button
 */
ccs.Button = {

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

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
 * @return A value converted from C/C++ "std::string"
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
 * @method getCapInsetPressedRenderer
 * @return A value converted from C/C++ "cocos2d::CCRect"
 */
getCapInsetPressedRenderer : function () {},

/**
 * @method getTitleColor
 * @return A value converted from C/C++ "cocos2d::ccColor3B"
 */
getTitleColor : function () {},

/**
 * @method loadTextureDisabled
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadTextureDisabled : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method setTitleText
 * @param {std::string}
 */
setTitleText : function () {},

/**
 * @method getCapInsetNormalRenderer
 * @return A value converted from C/C++ "cocos2d::CCRect"
 */
getCapInsetNormalRenderer : function () {},

/**
 * @method setCapInsetsNormalRenderer
 * @param {cocos2d::CCRect}
 */
setCapInsetsNormalRenderer : function () {},

/**
 * @method loadTexturePressed
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadTexturePressed : function () {},

/**
 * @method setTitleFontName
 * @param {const char*}
 */
setTitleFontName : function () {},

/**
 * @method loadTextures
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadTextures : function () {},

/**
 * @method isScale9Enabled
 * @return A value converted from C/C++ "bool"
 */
isScale9Enabled : function () {},

/**
 * @method getCapInsetDisabledRenderer
 * @return A value converted from C/C++ "cocos2d::CCRect"
 */
getCapInsetDisabledRenderer : function () {},

/**
 * @method loadTextureNormal
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadTextureNormal : function () {},

/**
 * @method setCapInsetsPressedRenderer
 * @param {cocos2d::CCRect}
 */
setCapInsetsPressedRenderer : function () {},

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
 * @method setCapInsets
 * @param {cocos2d::CCRect}
 */
setCapInsets : function () {},

/**
 * @method setPressedActionEnabled
 * @param {bool}
 */
setPressedActionEnabled : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::Button*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method Button
 * @constructor
 */
Button : function () {},

};

/**
 * @class CheckBox
 */
ccs.CheckBox = {

/**
 * @method getSelectedState
 * @return A value converted from C/C++ "bool"
 */
getSelectedState : function () {},

/**
 * @method loadTextureBackGroundSelected
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadTextureBackGroundSelected : function () {},

/**
 * @method loadTextureBackGroundDisabled
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadTextureBackGroundDisabled : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "std::string"
 */
getDescription : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method loadTextureFrontCross
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadTextureFrontCross : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method loadTextures
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadTextures : function () {},

/**
 * @method loadTextureBackGround
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadTextureBackGround : function () {},

/**
 * @method setSelectedState
 * @param {bool}
 */
setSelectedState : function () {},

/**
 * @method loadTextureFrontCrossDisabled
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadTextureFrontCrossDisabled : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::CheckBox*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method CheckBox
 * @constructor
 */
CheckBox : function () {},

};

/**
 * @class ImageView
 */
ccs.ImageView = {

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method ignoreContentAdaptWithSize
 * @param {bool}
 */
ignoreContentAdaptWithSize : function () {},

/**
 * @method loadTexture
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadTexture : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "std::string"
 */
getDescription : function () {},

/**
 * @method setScale9Enabled
 * @param {bool}
 */
setScale9Enabled : function () {},

/**
 * @method setTextureRect
 * @param {cocos2d::CCRect}
 */
setTextureRect : function () {},

/**
 * @method setCapInsets
 * @param {cocos2d::CCRect}
 */
setCapInsets : function () {},

/**
 * @method getCapInsets
 * @return A value converted from C/C++ "cocos2d::CCRect"
 */
getCapInsets : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method isScale9Enabled
 * @return A value converted from C/C++ "bool"
 */
isScale9Enabled : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::ImageView*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method ImageView
 * @constructor
 */
ImageView : function () {},

};

/**
 * @class Label
 */
ccs.Label = {

/**
 * @method getStringLength
 * @return A value converted from C/C++ "int"
 */
getStringLength : function () {},

/**
 * @method setFontName
 * @param {std::string}
 */
setFontName : function () {},

/**
 * @method setTouchScaleChangeEnabled
 * @param {bool}
 */
setTouchScaleChangeEnabled : function () {},

/**
 * @method getFontSize
 * @return A value converted from C/C++ "int"
 */
getFontSize : function () {},

/**
 * @method getTextVerticalAlignment
 * @return A value converted from C/C++ "cocos2d::CCVerticalTextAlignment"
 */
getTextVerticalAlignment : function () {},

/**
 * @method getStringValue
 * @return A value converted from C/C++ "const char*"
 */
getStringValue : function () {},

/**
 * @method setText
 * @param {std::string}
 */
setText : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "std::string"
 */
getDescription : function () {},

/**
 * @method getVirtualRenderer
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 */
getVirtualRenderer : function () {},

/**
 * @method getTextHorizontalAlignment
 * @return A value converted from C/C++ "cocos2d::CCTextAlignment"
 */
getTextHorizontalAlignment : function () {},

/**
 * @method getTextAreaSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getTextAreaSize : function () {},

/**
 * @method setTextVerticalAlignment
 * @param {cocos2d::CCVerticalTextAlignment}
 */
setTextVerticalAlignment : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method setFontSize
 * @param {int}
 */
setFontSize : function () {},

/**
 * @method isTouchScaleChangeEnabled
 * @return A value converted from C/C++ "bool"
 */
isTouchScaleChangeEnabled : function () {},

/**
 * @method setTextHorizontalAlignment
 * @param {cocos2d::CCTextAlignment}
 */
setTextHorizontalAlignment : function () {},

/**
 * @method getFontName
 * @return A value converted from C/C++ "const char*"
 */
getFontName : function () {},

/**
 * @method setTextAreaSize
 * @param {cocos2d::CCSize}
 */
setTextAreaSize : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::Label*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method Label
 * @constructor
 */
Label : function () {},

};

/**
 * @class LabelAtlas
 */
ccs.LabelAtlas = {

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
 * @return A value converted from C/C++ "std::string"
 */
getDescription : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method setProperty
 * @param {std::string}
 * @param {std::string}
 * @param {int}
 * @param {int}
 * @param {std::string}
 */
setProperty : function () {},

/**
 * @method setStringValue
 * @param {std::string}
 */
setStringValue : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::LabelAtlas*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method LabelAtlas
 * @constructor
 */
LabelAtlas : function () {},

};

/**
 * @class LoadingBar
 */
ccs.LoadingBar = {

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
 * @method ignoreContentAdaptWithSize
 * @param {bool}
 */
ignoreContentAdaptWithSize : function () {},

/**
 * @method loadTexture
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadTexture : function () {},

/**
 * @method setDirection
 * @param {cocos2d::ui::LoadingBarType}
 */
setDirection : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "std::string"
 */
getDescription : function () {},

/**
 * @method setScale9Enabled
 * @param {bool}
 */
setScale9Enabled : function () {},

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
 * @method getCapInsets
 * @return A value converted from C/C++ "cocos2d::CCRect"
 */
getCapInsets : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method isScale9Enabled
 * @return A value converted from C/C++ "bool"
 */
isScale9Enabled : function () {},

/**
 * @method getPercent
 * @return A value converted from C/C++ "int"
 */
getPercent : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::LoadingBar*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method LoadingBar
 * @constructor
 */
LoadingBar : function () {},

};

/**
 * @class ScrollView
 */
ccs.ScrollView = {

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
 * @return A value converted from C/C++ "std::string"
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
 * @return A value converted from C/C++ "cocos2d::ui::SCROLLVIEW_DIR"
 */
getDirection : function () {},

/**
 * @method getChildByTag
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 * @param {int}
 */
getChildByTag : function () {},

/**
 * @method scrollToBottomLeft
 * @param {float}
 * @param {bool}
 */
scrollToBottomLeft : function () {},

/**
 * @method getInnerContainer
 * @return A value converted from C/C++ "cocos2d::ui::Layout*"
 */
getInnerContainer : function () {},

/**
 * @method jumpToBottom
 */
jumpToBottom : function () {},

/**
 * @method getChildByName
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 * @param {const char*}
 */
getChildByName : function () {},

/**
 * @method setDirection
 * @param {cocos2d::ui::SCROLLVIEW_DIR}
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
 * @method removeNodeByTag
 * @param {int}
 */
removeNodeByTag : function () {},

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
 * @return A value converted from C/C++ "cocos2d::ui::LayoutType"
 */
getLayoutType : function () {},

/**
 * @method removeAllChildrenWithCleanup
 * @param {bool}
 */
removeAllChildrenWithCleanup : function () {},

/**
 * @method getNodeByTag
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 * @param {int}
 */
getNodeByTag : function () {},

/**
 * @method isInertiaScrollEnabled
 * @return A value converted from C/C++ "bool"
 */
isInertiaScrollEnabled : function () {},

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
 * @method removeNode
 * @param {cocos2d::CCNode*}
 */
removeNode : function () {},

/**
 * @method removeAllNodes
 */
removeAllNodes : function () {},

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
 * @method getNodes
 * @return A value converted from C/C++ "cocos2d::CCArray*"
 */
getNodes : function () {},

/**
 * @method setLayoutType
 * @param {cocos2d::ui::LayoutType}
 */
setLayoutType : function () {},

/**
 * @method jumpToRight
 */
jumpToRight : function () {},

/**
 * @method getChildrenCount
 * @return A value converted from C/C++ "unsigned int"
 */
getChildrenCount : function () {},

/**
 * @method scrollToTopRight
 * @param {float}
 * @param {bool}
 */
scrollToTopRight : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::ScrollView*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method ScrollView
 * @constructor
 */
ScrollView : function () {},

};

/**
 * @class Slider
 */
ccs.Slider = {

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
 * @param {cocos2d::ui::TextureResType}
 */
loadSlidBallTextureNormal : function () {},

/**
 * @method loadProgressBarTexture
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadProgressBarTexture : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "std::string"
 */
getDescription : function () {},

/**
 * @method setScale9Enabled
 * @param {bool}
 */
setScale9Enabled : function () {},

/**
 * @method getCapInsetBarRenderer
 * @return A value converted from C/C++ "cocos2d::CCRect"
 */
getCapInsetBarRenderer : function () {},

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
 * @param {cocos2d::ui::TextureResType}
 */
loadSlidBallTextures : function () {},

/**
 * @method getContentSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getContentSize : function () {},

/**
 * @method loadBarTexture
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadBarTexture : function () {},

/**
 * @method loadSlidBallTexturePressed
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadSlidBallTexturePressed : function () {},

/**
 * @method isScale9Enabled
 * @return A value converted from C/C++ "bool"
 */
isScale9Enabled : function () {},

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
 * @method setCapInsets
 * @param {cocos2d::CCRect}
 */
setCapInsets : function () {},

/**
 * @method loadSlidBallTextureDisabled
 * @param {const char*}
 * @param {cocos2d::ui::TextureResType}
 */
loadSlidBallTextureDisabled : function () {},

/**
 * @method getPercent
 * @return A value converted from C/C++ "int"
 */
getPercent : function () {},

/**
 * @method getCapInsetProgressBarRebderer
 * @return A value converted from C/C++ "cocos2d::CCRect"
 */
getCapInsetProgressBarRebderer : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::Slider*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method Slider
 * @constructor
 */
Slider : function () {},

};

/**
 * @class TextField
 */
ccs.TextField = {

/**
 * @method setAttachWithIME
 * @param {bool}
 */
setAttachWithIME : function () {},

/**
 * @method getFontSize
 * @return A value converted from C/C++ "int"
 */
getFontSize : function () {},

/**
 * @method getStringValue
 * @return A value converted from C/C++ "const char*"
 */
getStringValue : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "std::string"
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
 * @method getPlaceHolder
 * @return A value converted from C/C++ "const char*"
 */
getPlaceHolder : function () {},

/**
 * @method getAttachWithIME
 * @return A value converted from C/C++ "bool"
 */
getAttachWithIME : function () {},

/**
 * @method setFontName
 * @param {std::string}
 */
setFontName : function () {},

/**
 * @method getInsertText
 * @return A value converted from C/C++ "bool"
 */
getInsertText : function () {},

/**
 * @method setInsertText
 * @param {bool}
 */
setInsertText : function () {},

/**
 * @method getDetachWithIME
 * @return A value converted from C/C++ "bool"
 */
getDetachWithIME : function () {},

/**
 * @method setTextVerticalAlignment
 * @param {cocos2d::CCVerticalTextAlignment}
 */
setTextVerticalAlignment : function () {},

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
 * @method getFontName
 * @return A value converted from C/C++ "const char*"
 */
getFontName : function () {},

/**
 * @method setTextAreaSize
 * @param {cocos2d::CCSize}
 */
setTextAreaSize : function () {},

/**
 * @method attachWithIME
 */
attachWithIME : function () {},

/**
 * @method getPasswordStyleText
 * @return A value converted from C/C++ "const char*"
 */
getPasswordStyleText : function () {},

/**
 * @method setPasswordEnabled
 * @param {bool}
 */
setPasswordEnabled : function () {},

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
 * @method isPasswordEnabled
 * @return A value converted from C/C++ "bool"
 */
isPasswordEnabled : function () {},

/**
 * @method setDeleteBackward
 * @param {bool}
 */
setDeleteBackward : function () {},

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
 * @method setPasswordStyleText
 * @param {const char*}
 */
setPasswordStyleText : function () {},

/**
 * @method setTextHorizontalAlignment
 * @param {cocos2d::CCTextAlignment}
 */
setTextHorizontalAlignment : function () {},

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
 * @param {std::string}
 */
setText : function () {},

/**
 * @method setTouchAreaEnabled
 * @param {bool}
 */
setTouchAreaEnabled : function () {},

/**
 * @method hitTest
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::CCPoint}
 */
hitTest : function () {},

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
 * @method getTouchSize
 * @return A value converted from C/C++ "cocos2d::CCSize"
 */
getTouchSize : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::TextField*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method TextField
 * @constructor
 */
TextField : function () {},

};

/**
 * @class ListView
 */
ccs.ListView = {

/**
 * @method getIndex
 * @return A value converted from C/C++ "unsigned int"
 * @param {cocos2d::ui::Widget*}
 */
getIndex : function () {},

/**
 * @method removeAllItems
 */
removeAllItems : function () {},

/**
 * @method setGravity
 * @param {cocos2d::ui::ListViewGravity}
 */
setGravity : function () {},

/**
 * @method pushBackCustomItem
 * @param {cocos2d::ui::Widget*}
 */
pushBackCustomItem : function () {},

/**
 * @method setDirection
 * @param {cocos2d::ui::SCROLLVIEW_DIR}
 */
setDirection : function () {},

/**
 * @method getItems
 * @return A value converted from C/C++ "cocos2d::CCArray*"
 */
getItems : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "std::string"
 */
getDescription : function () {},

/**
 * @method removeItem
 * @param {int}
 */
removeItem : function () {},

/**
 * @method getCurSelectedIndex
 * @return A value converted from C/C++ "int"
 */
getCurSelectedIndex : function () {},

/**
 * @method insertDefaultItem
 * @param {int}
 */
insertDefaultItem : function () {},

/**
 * @method sortAllChildren
 */
sortAllChildren : function () {},

/**
 * @method setItemsMargin
 * @param {float}
 */
setItemsMargin : function () {},

/**
 * @method refreshView
 */
refreshView : function () {},

/**
 * @method removeLastItem
 */
removeLastItem : function () {},

/**
 * @method getItemsMargin
 * @return A value converted from C/C++ "float"
 */
getItemsMargin : function () {},

/**
 * @method getItem
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 * @param {unsigned int}
 */
getItem : function () {},

/**
 * @method setItemModel
 * @param {cocos2d::ui::Widget*}
 */
setItemModel : function () {},

/**
 * @method requestRefreshView
 */
requestRefreshView : function () {},

/**
 * @method pushBackDefaultItem
 */
pushBackDefaultItem : function () {},

/**
 * @method insertCustomItem
 * @param {cocos2d::ui::Widget*}
 * @param {int}
 */
insertCustomItem : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::ListView*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method ListView
 * @constructor
 */
ListView : function () {},

};

/**
 * @class LabelBMFont
 */
ccs.LabelBMFont = {

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
 * @return A value converted from C/C++ "std::string"
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
 * @return A value converted from C/C++ "cocos2d::ui::LabelBMFont*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method LabelBMFont
 * @constructor
 */
LabelBMFont : function () {},

};

/**
 * @class PageView
 */
ccs.PageView = {

/**
 * @method getLayoutType
 * @return A value converted from C/C++ "cocos2d::ui::LayoutType"
 */
getLayoutType : function () {},

/**
 * @method getCurPageIndex
 * @return A value converted from C/C++ "int"
 */
getCurPageIndex : function () {},

/**
 * @method addWidgetToPage
 * @param {cocos2d::ui::Widget*}
 * @param {int}
 * @param {bool}
 */
addWidgetToPage : function () {},

/**
 * @method getPage
 * @return A value converted from C/C++ "cocos2d::ui::Layout*"
 * @param {int}
 */
getPage : function () {},

/**
 * @method removePage
 * @param {cocos2d::ui::Layout*}
 */
removePage : function () {},

/**
 * @method getDescription
 * @return A value converted from C/C++ "std::string"
 */
getDescription : function () {},

/**
 * @method insertPage
 * @param {cocos2d::ui::Layout*}
 * @param {int}
 */
insertPage : function () {},

/**
 * @method setLayoutType
 * @param {cocos2d::ui::LayoutType}
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
 * @method removePageAtIndex
 * @param {int}
 */
removePageAtIndex : function () {},

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
 * @param {cocos2d::ui::Layout*}
 */
addPage : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::PageView*"
 */
create : function () {},

/**
 * @method createInstance
 * @return A value converted from C/C++ "cocos2d::CCObject*"
 */
createInstance : function () {},

/**
 * @method PageView
 * @constructor
 */
PageView : function () {},

};

/**
 * @class UIHelper
 */
ccs.UIHelper = {

/**
 * @method seekActionWidgetByActionTag
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 * @param {cocos2d::ui::Widget*}
 * @param {int}
 */
seekActionWidgetByActionTag : function () {},

/**
 * @method seekWidgetByTag
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 * @param {cocos2d::ui::Widget*}
 * @param {int}
 */
seekWidgetByTag : function () {},

/**
 * @method seekWidgetByRelativeName
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 * @param {cocos2d::ui::Widget*}
 * @param {const char*}
 */
seekWidgetByRelativeName : function () {},

/**
 * @method seekWidgetByName
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 * @param {cocos2d::ui::Widget*}
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
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 * @param {const char*}
 */
widgetFromJsonFile : function () {},

/**
 * @method getFilePath
 * @return A value converted from C/C++ "std::string"
 */
getFilePath : function () {},

/**
 * @method purge
 */
purge : function () {},

/**
 * @method shareReader
 * @return A value converted from C/C++ "cocos2d::extension::GUIReader*"
 */
shareReader : function () {},

};

/**
 * @class TouchGroup
 */
ccs.UILayer = {

/**
 * @method removeWidget
 * @param {cocos2d::ui::Widget*}
 */
removeWidget : function () {},

/**
 * @method getWidgetByTag
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 * @param {int}
 */
getWidgetByTag : function () {},

/**
 * @method getRootWidget
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 */
getRootWidget : function () {},

/**
 * @method init
 * @return A value converted from C/C++ "bool"
 */
init : function () {},

/**
 * @method getWidgetByName
 * @return A value converted from C/C++ "cocos2d::ui::Widget*"
 * @param {const char*}
 */
getWidgetByName : function () {},

/**
 * @method addWidget
 * @param {cocos2d::ui::Widget*}
 */
addWidget : function () {},

/**
 * @method clear
 */
clear : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "cocos2d::ui::TouchGroup*"
 */
create : function () {},

/**
 * @method TouchGroup
 * @constructor
 */
TouchGroup : function () {},

};

/**
 * @class SceneReader
 */
ccs.SceneReader = {

/**
 * @method createNodeWithSceneFile
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 * @param {const char*}
 */
createNodeWithSceneFile : function () {},

/**
 * @method getNodeByTag
 * @return A value converted from C/C++ "cocos2d::CCNode*"
 * @param {int}
 */
getNodeByTag : function () {},

/**
 * @method purge
 */
purge : function () {},

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
 * @method getTotalTime
 * @return A value converted from C/C++ "float"
 */
getTotalTime : function () {},

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
 * @method purge
 */
purge : function () {},

/**
 * @method shareManager
 * @return A value converted from C/C++ "cocos2d::extension::ActionManager*"
 */
shareManager : function () {},

};
