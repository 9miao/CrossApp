//
//  CASlider.h
//  project
//
//  Created by lh on 14-4-16.
//

//

#ifndef __project__CASlider__
#define __project__CASlider__

#include <iostream>
#include "sprite_nodes/CCView.h"
#include "sprite_nodes/CCSprite.h"
#include "misc_nodes/CCRenderTexture.h"

NS_CC_BEGIN

typedef enum
{
    CAProgressStyleDefault=0,
    CAProgressStyleBar
}CAProgressStyle;

class CC_DLL CAProgress :public CCNodeRGBA
{
public:
    
    CAProgress();
    
    virtual ~CAProgress();
    
    bool init();
    
    static CAProgress* create();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
public:
    
    void schedule(float dt);
    
    void setProgress(float progress, bool animated = false);
    
    
    CC_SYNTHESIZE(float, m_fProgress, progress);
    
    CC_SYNTHESIZE(CAProgressStyle, m_pProgressStyle, ProgressStyle);
    
    CC_SYNTHESIZE(ccColor3B, m_pProgressTintColor, ProgressTintColor);
    
    CC_SYNTHESIZE(ccColor3B, m_pProgresstrackColor, ProgresstrackColor);
    
    CC_PROPERTY(std::string, m_pProgressTintImage, ProgressTintImage);
    
    CC_PROPERTY(std::string, m_pProgressTrackImage, ProgressTrackImage);
    
   
    
private:
    CCSprite *m_pCopyTarckImage;
    
    CCSprite *m_pCopyProgressImage;
    
    CCRenderTexture *m_pRender;
    
    float m_nCurPercent;
    
    float m_previousPercent;
    
    CCNode *m_pIndicator;
    
    void init9SpriteWithImage(const char *fileName);
    
};

NS_CC_END
#endif /* defined(__project__CASlider__) */
