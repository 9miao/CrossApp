

#ifndef __CC_APPLICATION_MAC_H__
#define __CC_APPLICATION_MAC_H__

#include "platform/CACommon.h"
#include "platform/CCApplicationProtocol.h"
#include <string>

NS_CC_BEGIN

class CC_DLL CCApplication : public CCApplicationProtocol
{
public:
    CCApplication();
    virtual ~CCApplication();
        
    /**
    @brief	Callback by CAApplication for limit FPS.
    @interval       The time, which expressed in second in second, between current frame and next. 
    */
    virtual void setAnimationInterval(double interval);
        
    /**
    @brief	Get status bar rectangle in EGLView window.
    */
        
    /**
    @brief	Run the message loop.
    */
    int run();
        
    /**
    @brief	Get current applicaiton instance.
    @return Current application instance pointer.
    */
    static CCApplication* sharedApplication();

    /**
    @brief Get current language config
    @return Current language config
    */
    virtual LanguageType getCurrentLanguage();
		
    /**
     @brief Get target platform
     */
    virtual TargetPlatform getTargetPlatform();

    /**
     *  Sets the Resource root path.
     *  @deprecated Please use FileUtils::getInstance()->setSearchPaths() instead.
     */
    CC_DEPRECATED_ATTRIBUTE void setResourceRootPath(const std::string& rootResDir);
    
    /** 
     *  Gets the Resource root path.
     *  @deprecated Please use FileUtils::getInstance()->getSearchPaths() instead. 
     */
    CC_DEPRECATED_ATTRIBUTE const std::string& getResourceRootPath(void);
    
    void setStartupScriptFilename(const std::string& startupScriptFile);
    
    const std::string& getStartupScriptFilename(void);
    
protected:
    static CCApplication * sm_pSharedApplication;
    
    std::string m_resourceRootPath;
    std::string m_startupScriptFilename;
};

NS_CC_END

#endif	// end of __CC_APPLICATION_MAC_H__;
