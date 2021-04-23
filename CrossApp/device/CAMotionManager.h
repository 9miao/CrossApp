

#ifndef CAMotionManager_h
#define CAMotionManager_h

#include "basics/CAObject.h"
#include <iostream>
#include <functional>
#include "basics/CAValue.h"
NS_CC_BEGIN



class CC_DLL CAMotionManager : public CAObject
{
public:
    
    struct Data
    {
        double x{0.f};
        double y{0.f};
        double z{0.f};
        float timestamp{0.f};
    };
    
    typedef std::function<void(const CAMotionManager::Data&)> Callback;

public:

    CAMotionManager();
    
    virtual ~CAMotionManager();
    
    static CAMotionManager* getInstance();
    
    void startGyroscope(const CrossApp::CAMotionManager::Callback& callback);
    
    void setGyroInterval(float interval);
    
    void stopGyroscope();

private:
    
    void* m_pMotionManager{nullptr};

    CAMotionManager::Callback m_callback{nullptr};
};

NS_CC_END
#endif /* CAGyroscope_h */
