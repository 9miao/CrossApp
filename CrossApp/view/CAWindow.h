

#ifndef __CAWINDOW_H__
#define __CAWINDOW_H__

#include "CAView.h"
#include "controller/CAViewController.h"
NS_CC_BEGIN

enum
{
    CAWindowZOrderBottom = 0,
    CAWindowZOrderCenter = 1,
    CAWindowZOrderTop = 0xffff
};

class CACamera;
class CC_DLL CAWindow : public CAView
{

public:

    static CAWindow *create(void);
    
    CAWindow();

    virtual ~CAWindow();
    
    bool init() override;
    
    virtual void onEnterTransitionDidFinish() override;
        
    virtual void onExitTransitionDidStart() override;
    
    void presentModalViewController(CAViewController* controller, bool animated);
    
    void dismissModalViewController(bool animated);

    virtual void render(Renderer* renderer, const Mat4& eyeTransform, const Mat4* eyeProjection = nullptr);
    
    virtual void removeAllSubviews() override;
    
    const std::vector<CACamera*>& getCameras();
    
    CACamera* getDefaultCamera() const { return m_pDefaultCamera; }
    
    void setCameraOrderDirty() { m_bCameraOrderDirty = true; }

protected:
    
    CC_PROPERTY(CAViewController*, m_pRootViewController, RootViewController);
    
    CC_SYNTHESIZE_READONLY(CAViewController*, m_pModalViewController, ModalViewController);
    
    CAMap<std::string, CAViewController*> m_obViewControllerMap;
    
    std::vector<CACamera*> m_vCameras;

    CACamera*              m_pDefaultCamera;
    
    bool                   m_bCameraOrderDirty;
};

NS_CC_END

#endif // __CAWINDOW_H__
