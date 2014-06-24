
#ifndef __CCCONFIGURATION_H__
#define __CCCONFIGURATION_H__

#include "basics/CAObject.h"
#include "CCGL.h"
#include "cocoa/CCString.h"
#include <string>



NS_CC_BEGIN

typedef enum _ConfigurationType {
    CAConfigurationError,
    CAConfigurationString,
    CAConfigurationInt,
    CAConfigurationDouble,
    CAConfigurationBoolean
} CAConfigurationType;


/**
 * @addtogroup global
 * @{
 */
/**
 @brief CAConfiguration contains some openGL variables
 @since v0.99.0
 */
class CC_DLL CAConfiguration : public CAObject
{
public:
    /** returns a shared instance of CAConfiguration */
    static CAConfiguration *sharedConfiguration(void);

    /** purge the shared instance of CAConfiguration */
    static void purgeConfiguration(void);

public:
    /**
     *  @js NA
     *  @lua NA
     */
	virtual ~CAConfiguration(void);

    /** OpenGL Max Image size. */
	int getMaxTextureSize(void) const;

    /** OpenGL Max Modelview Stack Depth. */
	int getMaxModelviewStackDepth(void) const;

    /** returns the maximum Image units
     @since v2.0.0
     */
	int getMaxTextureUnits(void) const;

    /** Whether or not the GPU supports NPOT (Non Power Of Two) textures.
     OpenGL ES 2.0 already supports NPOT (iOS).
     
     @since v0.99.2
     */
	bool supportsNPOT(void) const;

    /** Whether or not PVR Image Compressed is supported */
	bool supportsPVRTC(void) const;

    /** Whether or not BGRA8888 textures are supported.
     @since v0.99.2
     */
	bool supportsBGRA8888(void) const;

    /** Whether or not glDiscardFramebufferEXT is supported
     @since v0.99.2
     */
	bool supportsDiscardFramebuffer(void) const;

    /** Whether or not shareable VAOs are supported.
     @since v2.0.0
     */
	bool supportsShareableVAO(void) const;

    /** returns whether or not an OpenGL is supported */
    bool checkForGLExtension(const std::string &searchName) const;

    bool init(void);

	/** returns the value of a given key as a string.
	 If the key is not found, it will return the default value */
	const char* getCString( const char *key, const char *default_value=NULL ) const;

	/** returns the value of a given key as a boolean.
	 If the key is not found, it will return the default value */
	bool getBool( const char *key, bool default_value=false ) const;

	/** returns the value of a given key as a double.
	 If the key is not found, it will return the default value */
	double getNumber( const char *key, double default_value=0.0 ) const;

	/** returns the value of a given key as a double */
	CAObject * getObject( const char *key ) const;

	/** sets a new key/value pair  in the configuration dictionary */
	void setObject( const char *key, CAObject *value );

	/** dumps the current configuration on the console */
	void dumpInfo(void) const;

	/** gathers OpenGL / GPU information */
	void gatherGPUInfo( void );

	/** Loads a config file. If the keys are already present, then they are going to be replaced. Otherwise the new keys are added. */
	void loadConfigFile( const char *filename );

private:
    CAConfiguration(void);
    static CAConfiguration *s_gSharedConfiguration;
	static std::string		s_sConfigfile;
    
protected:
    GLint           m_nMaxTextureSize;
    GLint           m_nMaxModelviewStackDepth;
    bool            m_bSupportsPVRTC;
    bool            m_bSupportsNPOT;
    bool            m_bSupportsBGRA8888;
    bool            m_bSupportsDiscardFramebuffer;
    bool            m_bSupportsShareableVAO;
    GLint           m_nMaxSamplesAllowed;
    GLint           m_nMaxTextureUnits;
    char *          m_pGlExtensions;
	
	CCDictionary	*m_pValueDict;
};

// end of global group
/// @}

NS_CC_END

#endif // __CCCONFIGURATION_H__
