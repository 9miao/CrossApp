
#include "basics/CAConfiguration.h"
#include "platform/CAFileUtils.h"
#include "basics/CAApplication.h"

NS_CC_BEGIN

extern const char* CrossAppVersion();

CAConfiguration* CAConfiguration::s_sharedConfiguration = nullptr;

const char* CAConfiguration::CONFIG_FILE_LOADED = "config_file_loaded";

CAConfiguration::CAConfiguration()
: _maxTextureSize(0) 
, _maxModelviewStackDepth(0)
, _supportsPVRTC(false)
, _supportsETC1(false)
, _supportsS3TC(false)
, _supportsATITC(false)
, _supportsNPOT(false)
, _supportsBGRA8888(false)
, _supportsDiscardFramebuffer(false)
, _supportsShareableVAO(false)
, _supportsOESDepth24(false)
, _supportsOESPackedDepthStencil(false)
, _supportsOESMapBuffer(false)
, _maxSamplesAllowed(0)
, _maxTextureUnits(0)
, _glExtensions(nullptr)
, _maxDirLightInShader(1)
, _maxPointLightInShader(1)
, _maxSpotLightInShader(1)
{
}

bool CAConfiguration::init()
{

	return true;
}

CAConfiguration::~CAConfiguration()
{
    
}

std::string CAConfiguration::getInfo() const
{

    return "";
}

void CAConfiguration::gatherGPUInfo()
{
	_valueDict["gl.vendor"] = CAValue((const char*)glGetString(GL_VENDOR));
	_valueDict["gl.renderer"] = CAValue((const char*)glGetString(GL_RENDERER));
	_valueDict["gl.version"] = CAValue((const char*)glGetString(GL_VERSION));

    _glExtensions = (char *)glGetString(GL_EXTENSIONS);

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_maxTextureSize);
	_valueDict["gl.max_texture_size"] = CAValue((int)_maxTextureSize);

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &_maxTextureUnits);
	_valueDict["gl.max_texture_units"] = CAValue((int)_maxTextureUnits);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    glGetIntegerv(GL_MAX_SAMPLES_APPLE, &_maxSamplesAllowed);
	_valueDict["gl.max_samples_allowed"] = CAValue((int)_maxSamplesAllowed);
#endif
    
    _supportsETC1 = checkForGLExtension("GL_OES_compressed_ETC1_RGB8_texture");
    _valueDict["gl.supports_ETC1"] = CAValue(_supportsETC1);
    
    _supportsS3TC = checkForGLExtension("GL_EXT_texture_compression_s3tc");
    _valueDict["gl.supports_S3TC"] = CAValue(_supportsS3TC);
    
    _supportsATITC = checkForGLExtension("GL_AMD_compressed_ATC_texture");
    _valueDict["gl.supports_ATITC"] = CAValue(_supportsATITC);
    
    _supportsPVRTC = checkForGLExtension("GL_IMG_texture_compression_pvrtc");
	_valueDict["gl.supports_PVRTC"] = CAValue(_supportsPVRTC);

    _supportsNPOT = true;
	_valueDict["gl.supports_NPOT"] = CAValue(_supportsNPOT);
	
    _supportsBGRA8888 = checkForGLExtension("GL_IMG_texture_format_BGRA888");
	_valueDict["gl.supports_BGRA8888"] = CAValue(_supportsBGRA8888);

    _supportsDiscardFramebuffer = checkForGLExtension("GL_EXT_discard_framebuffer");
	_valueDict["gl.supports_discard_framebuffer"] = CAValue(_supportsDiscardFramebuffer);

    _supportsShareableVAO = checkForGLExtension("vertex_array_object");
    _valueDict["gl.supports_vertex_array_object"] = CAValue(_supportsShareableVAO);

    _supportsOESMapBuffer = checkForGLExtension("GL_OES_mapbuffer");
    _valueDict["gl.supports_OES_map_buffer"] = CAValue(_supportsOESMapBuffer);

    _supportsOESDepth24 = checkForGLExtension("GL_OES_depth24");
    _valueDict["gl.supports_OES_depth24"] = CAValue(_supportsOESDepth24);

    
    _supportsOESPackedDepthStencil = checkForGLExtension("GL_OES_packed_depth_stencil");
    _valueDict["gl.supports_OES_packed_depth_stencil"] = CAValue(_supportsOESPackedDepthStencil);


    CHECK_GL_ERROR_DEBUG();
}

CAConfiguration* CAConfiguration::getInstance()
{
    if (! s_sharedConfiguration)
    {
        s_sharedConfiguration = new (std::nothrow) CAConfiguration();
        s_sharedConfiguration->init();
    }
    
    return s_sharedConfiguration;
}

void CAConfiguration::destroyInstance()
{
    CC_SAFE_RELEASE_NULL(s_sharedConfiguration);
}

bool CAConfiguration::checkForGLExtension(const std::string &searchName) const
{
   return  (_glExtensions && strstr(_glExtensions, searchName.c_str() ) ) ? true : false;
}

int CAConfiguration::getMaxTextureSize() const
{
	return _maxTextureSize;
}

int CAConfiguration::getMaxModelviewStackDepth() const
{
	return _maxModelviewStackDepth;
}

int CAConfiguration::getMaxTextureUnits() const
{
	return _maxTextureUnits;
}

bool CAConfiguration::supportsNPOT() const
{
	return _supportsNPOT;
}

bool CAConfiguration::supportsPVRTC() const
{
	return _supportsPVRTC;
}

bool CAConfiguration::supportsETC() const
{
    //GL_ETC1_RGB8_OES is not defined in old opengl version
#ifdef GL_ETC1_RGB8_OES
    return _supportsETC1;
#else
    return false;
#endif
}

bool CAConfiguration::supportsS3TC() const
{
#ifdef GL_EXT_texture_compression_s3tc
    return _supportsS3TC;
#else
    return false;
#endif
}

bool CAConfiguration::supportsATITC() const
{
    return _supportsATITC;
}

bool CAConfiguration::supportsBGRA8888() const
{
	return _supportsBGRA8888;
}

bool CAConfiguration::supportsDiscardFramebuffer() const
{
	return _supportsDiscardFramebuffer;
}

bool CAConfiguration::supportsShareableVAO() const
{
#if CC_TEXTURE_ATLAS_USE_VAO
    return _supportsShareableVAO;
#else
    return false;
#endif
}

bool CAConfiguration::supportsMapBuffer() const
{
    // Fixes Github issue #16123
    //
    // XXX: Fixme. Should check GL ES and not iOS or Android
    // For example, linux could be compiled with GL ES. Or perhaps in the future Android will
    // support OpenGL. This is because glMapBufferOES() is an extension of OpenGL ES. And glMapBuffer()
    // is always implemented in OpenGL.

    // XXX: Warning. On iOS this is always `true`. Avoiding the comparison.
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return _supportsOESMapBuffer;
#else
    return true;
#endif
}

bool CAConfiguration::supportsOESDepth24() const
{
    return _supportsOESDepth24;
    
}
bool CAConfiguration::supportsOESPackedDepthStencil() const
{
    return _supportsOESPackedDepthStencil;
}



int CAConfiguration::getMaxSupportDirLightInShader() const
{
    return _maxDirLightInShader;
}

int CAConfiguration::getMaxSupportPointLightInShader() const
{
    return _maxPointLightInShader;
}

int CAConfiguration::getMaxSupportSpotLightInShader() const
{
    return _maxSpotLightInShader;
}

const CAValue& CAConfiguration::getValue(const std::string& key, const CAValue& defaultValue) const
{
    auto iter = _valueDict.find(key);
    if (iter != _valueDict.cend())
        return _valueDict.at(key);
	return defaultValue;
}

void CAConfiguration::setValue(const std::string& key, const CAValue& value)
{
	_valueDict[key] = value;
}


//
// load file
//
void CAConfiguration::loadConfigFile(const std::string& filename)
{
	CAValueMap dict = FileUtils::getInstance()->getValueMapFromFile(filename);
	CCASSERT(!dict.empty(), "cannot create dictionary");

	// search for metadata
	bool validMetadata = false;
	auto metadataIter = dict.find("metadata");
	if (metadataIter != dict.cend() && metadataIter->second.getType() == CAValue::Type::MAP)
    {
        
		const auto& metadata = metadataIter->second.asValueMap();
        auto formatIter = metadata.find("format");
        
		if (formatIter != metadata.cend())
        {
			int format = formatIter->second.asInt();

			// Support format: 1
			if (format == 1)
            {
				validMetadata = true;
			}
		}
	}

	if (! validMetadata)
    {
		CCLOG("Invalid config format for file: %s", filename.c_str());
		return;
	}

	auto dataIter = dict.find("data");
	if (dataIter == dict.cend() || dataIter->second.getType() != CAValue::Type::MAP)
    {
		CCLOG("Expected 'data' dict, but not found. Config file: %s", filename.c_str());
		return;
	}

	// Add all keys in the existing dictionary
    
	const auto& dataMap = dataIter->second.asValueMap();
    for (auto dataMapIter = dataMap.cbegin(); dataMapIter != dataMap.cend(); ++dataMapIter)
    {
        if (_valueDict.find(dataMapIter->first) == _valueDict.cend())
            _valueDict[dataMapIter->first] = dataMapIter->second;
        else
            CCLOG("Key already present. Ignoring '%s'",dataMapIter->first.c_str());
    }
    
    //light info
    std::string name = "cross.3d.max_dir_light_in_shader";
	if (_valueDict.find(name) != _valueDict.end())
        _maxDirLightInShader = _valueDict[name].asInt();
    else
        _valueDict[name] = CAValue(_maxDirLightInShader);
    
    name = "cross.3d.max_point_light_in_shader";
	if (_valueDict.find(name) != _valueDict.end())
        _maxPointLightInShader = _valueDict[name].asInt();
    else
        _valueDict[name] = CAValue(_maxPointLightInShader);
    
    name = "cross.3d.max_spot_light_in_shader";
	if (_valueDict.find(name) != _valueDict.end())
        _maxSpotLightInShader = _valueDict[name].asInt();
    else
        _valueDict[name] = CAValue(_maxSpotLightInShader);
}

NS_CC_END
