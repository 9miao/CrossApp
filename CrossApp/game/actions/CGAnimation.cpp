
#include "game/actions/CGAnimation.h"
#include "images/CAImageCache.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN

AnimationFrame* AnimationFrame::create(CGSpriteFrame* spriteFrame, float delayUnits, const CAValueMap& userInfo)
{
    auto ret = new (std::nothrow) AnimationFrame();
    if (ret && ret->initWithSpriteFrame(spriteFrame, delayUnits, userInfo))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

AnimationFrame::AnimationFrame()
: _spriteFrame(nullptr)
, _delayUnits(0.0f)
{

}

bool AnimationFrame::initWithSpriteFrame(CGSpriteFrame* spriteFrame, float delayUnits, const CAValueMap& userInfo)
{
    setSpriteFrame(spriteFrame);
    setDelayUnits(delayUnits);
    setUserInfo(userInfo);

    return true;
}

AnimationFrame::~AnimationFrame()
{    
    CCLOG( "deallocing AnimationFrame: %p", this);

    CC_SAFE_RELEASE(_spriteFrame);
}

AnimationFrame* AnimationFrame::clone() const
{
    // no copy constructor
    auto frame = new (std::nothrow) AnimationFrame();
    frame->initWithSpriteFrame(_spriteFrame->clone(),
                               _delayUnits,
                               _userInfo);

    frame->autorelease();
    return frame;
}

// implementation of Animation

Animation* Animation::create()
{
    Animation *animation = new (std::nothrow) Animation();
    animation->init();
    animation->autorelease();

    return animation;
} 

Animation* Animation::createWithSpriteFrames(const CAVector<CGSpriteFrame*>& frames, float delay/* = 0.0f*/, unsigned int loops/* = 1*/)
{
    Animation *animation = new (std::nothrow) Animation();
    animation->initWithSpriteFrames(frames, delay, loops);
    animation->autorelease();

    return animation;
}

Animation* Animation::create(const CAVector<AnimationFrame*>& arrayOfAnimationFrameNames, float delayPerUnit, unsigned int loops /* = 1 */)
{
    Animation *animation = new (std::nothrow) Animation();
    animation->initWithAnimationFrames(arrayOfAnimationFrameNames, delayPerUnit, loops);
    animation->autorelease();
    return animation;
}

bool Animation::init()
{
    _loops = 1;
    _delayPerUnit = 0.0f;
    
    return true;
}

bool Animation::initWithSpriteFrames(const CAVector<CGSpriteFrame*>& frames, float delay/* = 0.0f*/, unsigned int loops/* = 1*/)
{
    _delayPerUnit = delay;
    _loops = loops;

    for (auto& spriteFrame : frames)
    {
        auto animFrame = AnimationFrame::create(spriteFrame, 1, CAValueMap());
        _frames.pushBack(animFrame);
        _totalDelayUnits++;
    }

    return true;
}

bool Animation::initWithAnimationFrames(const CAVector<AnimationFrame*>& arrayOfAnimationFrames, float delayPerUnit, unsigned int loops)
{
    _delayPerUnit = delayPerUnit;
    _loops = loops;

    setFrames(arrayOfAnimationFrames);

    for (auto& animFrame : _frames)
    {
        _totalDelayUnits += animFrame->getDelayUnits();
    }
    return true;
}

Animation::Animation()
: _totalDelayUnits(0.0f)
, _delayPerUnit(0.0f)
, _duration(0.0f)
, _restoreOriginalFrame(false)
, _loops(0)
{

}

Animation::~Animation(void)
{
    CCLOG("deallocing Animation: %p", this);
}

void Animation::addSpriteFrame(CGSpriteFrame* spriteFrame)
{
    AnimationFrame *animFrame = AnimationFrame::create(spriteFrame, 1.0f, CAValueMap());
    _frames.pushBack(animFrame);

    // update duration
    _totalDelayUnits++;
}

void Animation::addSpriteFrameWithFile(const std::string& filename)
{
    CAImage *image = CAImage::create(filename);
    DRect rect = DRectZero;
    rect.size = image->getContentSize();
    CGSpriteFrame *frame = CGSpriteFrame::createWithImage(image, rect);
    addSpriteFrame(frame);
}

void Animation::addSpriteFrameWithImage(CAImage *image, const DRect& rect)
{
    CGSpriteFrame *frame = CGSpriteFrame::createWithImage(image, rect);
    addSpriteFrame(frame);
}

float Animation::getDuration(void) const
{
    return _totalDelayUnits * _delayPerUnit;
}

Animation* Animation::clone() const
{
    // no copy constructor    
    auto a = new (std::nothrow) Animation();
    a->initWithAnimationFrames(_frames, _delayPerUnit, _loops);
    a->setRestoreOriginalFrame(_restoreOriginalFrame);
    a->autorelease();
    return a;
}

NS_CC_END
