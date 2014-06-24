

#include "CAConfiguration.h"
#include "ccMacros.h"
#include "ccConfig.h"
#include <string.h>
#include "cocoa/CCDictionary.h"
#include "cocoa/CCInteger.h"
#include "cocoa/CCBool.h"
#include "CrossApp.h"
#include "platform/CCFileUtils.h"
#include "cocoa/CCDouble.h"
#include "basics/CAApplication.h"

using namespace std;

NS_CC_BEGIN


CAConfiguration* CAConfiguration::s_gSharedConfiguration = NULL;

CAConfiguration::CAConfiguration(void)
: m_nMaxTextureSize(0)
, m_nMaxModelviewStackDepth(0)
, m_bSupportsPVRTC(false)
, m_bSupportsNPOT(false)
, m_bSupportsBGRA8888(false)
, m_bSupportsDiscardFramebuffer(false)
, m_bSupportsShareableVAO(false)
, m_nMaxSamplesAllowed(0)
, m_nMaxTextureUnits(0)
, m_pGlExtensions(NULL)
, m_pValueDict(NULL)
{
}

bool CAConfiguration::init(void)
{
	m_pValueDict = CCDictionary::create();
	m_pValueDict->retain();
    
	m_pValueDict->setObject( CCString::create( CrossAppVersion() ), "CrossApp.version");
    
    
#if CC_ENABLE_PROFILERS
	m_pValueDict->setObject( CCBool::create(true), "CrossApp.compiled_with_profiler");
#else
	m_pValueDict->setObject( CCBool::create(false), "CrossApp.compiled_with_profiler");
#endif
    
#if CC_ENABLE_GL_STATE_CACHE == 0
	m_pValueDict->setObject( CCBool::create(false), "CrossApp.compiled_with_gl_state_cache");
#else
	m_pValueDict->setObject( CCBool::create(true), "CrossApp.compiled_with_gl_state_cache");
#endif
    
	return true;
}

CAConfiguration::~CAConfiguration(void)
{
	m_pValueDict->release();
}

void CAConfiguration::dumpInfo(void) const
{
	// Dump
	CCPrettyPrinter visitor(0);
	m_pValueDict->acceptVisitor(visitor);
    
	CCLOG("%s", visitor.getResult().c_str());
    
    
	// And Dump some warnings as well
#if CC_ENABLE_PROFILERS
    CCLOG("cocos2d: **** WARNING **** CC_ENABLE_PROFILERS is defined. Disable it when you finish profiling (from ccConfig.h)");
    printf("\n");
#endif
    
#if CC_ENABLE_GL_STATE_CACHE == 0
    CCLOG("");
    CCLOG("cocos2d: **** WARNING **** CC_ENABLE_GL_STATE_CACHE is disabled. To improve performance, enable it (from ccConfig.h)");
    printf("\n");
#endif
    
}

void CAConfiguration::gatherGPUInfo()
{
	m_pValueDict->setObject( CCString::create( (const char*)glGetString(GL_VENDOR)), "gl.vendor");
	m_pValueDict->setObject( CCString::create( (const char*)glGetString(GL_RENDERER)), "gl.renderer");
	m_pValueDict->setObject( CCString::create( (const char*)glGetString(GL_VERSION)), "gl.version");
    
    m_pGlExtensions = (char *)glGetString(GL_EXTENSIONS);
    
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_nMaxTextureSize);
	m_pValueDict->setObject( CCInteger::create((int)m_nMaxTextureSize), "gl.max_texture_size");
    
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_nMaxTextureUnits);
	m_pValueDict->setObject( CCInteger::create((int)m_nMaxTextureUnits), "gl.max_texture_units");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    glGetIntegerv(GL_MAX_SAMPLES_APPLE, &m_nMaxSamplesAllowed);
	m_pValueDict->setObject( CCInteger::create((int)m_nMaxSamplesAllowed), "gl.max_samples_allowed");
#endif
    
    m_bSupportsPVRTC = checkForGLExtension("GL_IMG_texture_compression_pvrtc");
	m_pValueDict->setObject( CCBool::create(m_bSupportsPVRTC), "gl.supports_PVRTC");
    
    m_bSupportsNPOT = true;
	m_pValueDict->setObject( CCBool::create(m_bSupportsNPOT), "gl.supports_NPOT");
	
    m_bSupportsBGRA8888 = checkForGLExtension("GL_IMG_texture_format_BGRA888");
	m_pValueDict->setObject( CCBool::create(m_bSupportsBGRA8888), "gl.supports_BGRA8888");
    
    m_bSupportsDiscardFramebuffer = checkForGLExtension("GL_EXT_discard_framebuffer");
	m_pValueDict->setObject( CCBool::create(m_bSupportsDiscardFramebuffer), "gl.supports_discard_framebuffer");
    
    m_bSupportsShareableVAO = checkForGLExtension("vertex_array_object");
	m_pValueDict->setObject( CCBool::create(m_bSupportsShareableVAO), "gl.supports_vertex_array_object");
    
    CHECK_GL_ERROR_DEBUG();
}

CAConfiguration* CAConfiguration::sharedConfiguration(void)
{
    if (! s_gSharedConfiguration)
    {
        s_gSharedConfiguration = new CAConfiguration();
        s_gSharedConfiguration->init();
    }
    
    return s_gSharedConfiguration;
}

void CAConfiguration::purgeConfiguration(void)
{
    CC_SAFE_RELEASE_NULL(s_gSharedConfiguration);
}

bool CAConfiguration::checkForGLExtension(const string &searchName) const
{
    bool bRet = false;
    const char *kSearchName = searchName.c_str();
    
    if (m_pGlExtensions &&
        strstr(m_pGlExtensions, kSearchName))
    {
        bRet = true;
    }
    
    return bRet;
}

//
// getters for specific variables.
// Mantained for backward compatiblity reasons only.
//
int CAConfiguration::getMaxTextureSize(void) const
{
	return m_nMaxTextureSize;
}

int CAConfiguration::getMaxModelviewStackDepth(void) const
{
	return m_nMaxModelviewStackDepth;
}

int CAConfiguration::getMaxTextureUnits(void) const
{
	return m_nMaxTextureUnits;
}

bool CAConfiguration::supportsNPOT(void) const
{
	return m_bSupportsNPOT;
}

bool CAConfiguration::supportsPVRTC(void) const
{
	return m_bSupportsPVRTC;
}

bool CAConfiguration::supportsBGRA8888(void) const
{
	return m_bSupportsBGRA8888;
}

bool CAConfiguration::supportsDiscardFramebuffer(void) const
{
	return m_bSupportsDiscardFramebuffer;
}

bool CAConfiguration::supportsShareableVAO(void) const
{
	return m_bSupportsShareableVAO;
}

//
// generic getters for properties
//
const char *CAConfiguration::getCString( const char *key, const char *default_value ) const
{
	CAObject *ret = m_pValueDict->objectForKey(key);
	if( ret ) {
		if( CCString *str=dynamic_cast<CCString*>(ret) )
			return str->getCString();
        
		CCAssert(false, "Key found, but from different type");
	}
    
	// XXX: Should it throw an exception ?
	return default_value;
}

/** returns the value of a given key as a boolean */
bool CAConfiguration::getBool( const char *key, bool default_value ) const
{
	CAObject *ret = m_pValueDict->objectForKey(key);
	if( ret ) {
		if( CCBool *boolobj=dynamic_cast<CCBool*>(ret) )
			return boolobj->getValue();
		if( CCString *strobj=dynamic_cast<CCString*>(ret) )
			return strobj->boolValue();
		CCAssert(false, "Key found, but from different type");
	}
    
	// XXX: Should it throw an exception ?
	return default_value;
}

/** returns the value of a given key as a double */
double CAConfiguration::getNumber( const char *key, double default_value ) const
{
	CAObject *ret = m_pValueDict->objectForKey(key);
	if( ret ) {
		if( CCDouble *obj=dynamic_cast<CCDouble*>(ret) )
			return obj->getValue();
        
		if( CCInteger *obj=dynamic_cast<CCInteger*>(ret) )
			return obj->getValue();
        
		if( CCString *strobj=dynamic_cast<CCString*>(ret) )
			return strobj->doubleValue();
        
		CCAssert(false, "Key found, but from different type");
	}
    
	// XXX: Should it throw an exception ?
	return default_value;
}

CAObject * CAConfiguration::getObject( const char *key ) const
{
	return m_pValueDict->objectForKey(key);
}

void CAConfiguration::setObject( const char *key, CAObject *value )
{
	m_pValueDict->setObject(value, key);
}


//
// load file
//
void CAConfiguration::loadConfigFile( const char *filename )
{
	CCDictionary *dict = CCDictionary::createWithContentsOfFile(filename);
	CCAssert(dict, "cannot create dictionary");
    
	// search for metadata
	bool metadata_ok = false;
	CAObject *metadata = dict->objectForKey("metadata");
	if( metadata && dynamic_cast<CCDictionary*>(metadata) ) {
		CAObject *format_o = static_cast<CCDictionary*>(metadata)->objectForKey("format");
        
		// XXX: cocos2d-x returns CCStrings when importing from .plist. This bug will be addressed in cocos2d-x v3.x
		if( format_o && dynamic_cast<CCString*>(format_o) ) {
			int format = static_cast<CCString*>(format_o)->intValue();
            
			// Support format: 1
			if( format == 1 ) {
				metadata_ok = true;
			}
		}
	}
    
	if( ! metadata_ok ) {
		CCLOG("Invalid config format for file: %s", filename);
		return;
	}
    
	CAObject *data = dict->objectForKey("data");
	if( !data || !dynamic_cast<CCDictionary*>(data) ) {
		CCLOG("Expected 'data' dict, but not found. Config file: %s", filename);
		return;
	}
    
	// Add all keys in the existing dictionary
	CCDictionary *data_dict = static_cast<CCDictionary*>(data);
    CCDictElement* element;
    CCDICT_FOREACH(data_dict, element)
    {
		if( ! m_pValueDict->objectForKey( element->getStrKey() ) )
			m_pValueDict->setObject(element->getObject(), element->getStrKey() );
		else
			CCLOG("Key already present. Ignoring '%s'", element->getStrKey() );
    }
    
    CAApplication::getApplication()->setDefaultValues();
}

NS_CC_END

