

#ifndef __CACONFIGURATION_H__
#define __CACONFIGURATION_H__

#include <string>

#include "basics/CAObject.h"
#include "basics/CAValue.h"
#include "CCGL.h"

NS_CC_BEGIN

class CC_DLL CAConfiguration : public CAObject
{
public:
    
    static CAConfiguration *getInstance();

    static void destroyInstance();

public:

	virtual ~CAConfiguration();

	int getMaxTextureSize() const;

	int getMaxModelviewStackDepth() const;

	int getMaxTextureUnits() const;

	bool supportsNPOT() const;

	bool supportsPVRTC() const;

    bool supportsETC() const;

    bool supportsS3TC() const;

    bool supportsATITC() const;

	bool supportsBGRA8888() const;

	bool supportsDiscardFramebuffer() const;

	bool supportsShareableVAO() const;

    bool supportsOESDepth24() const;

    bool supportsOESPackedDepthStencil() const;

    bool supportsMapBuffer() const;

    int getMaxSupportDirLightInShader() const;

    int getMaxSupportPointLightInShader() const;

    int getMaxSupportSpotLightInShader() const;

    bool checkForGLExtension(const std::string &searchName) const;

    bool init();

	const CAValue& getValue(const std::string& key, const CAValue& defaultValue = CAValue::Null) const;

	void setValue(const std::string& key, const CAValue& value);

    std::string getInfo() const;

	void gatherGPUInfo();

	void loadConfigFile(const std::string& filename);
    
    static const char* CONFIG_FILE_LOADED;

private:
    CAConfiguration(void);
    static CAConfiguration    *s_sharedConfiguration;
	static std::string		s_configfile;
    
protected:
    GLint           _maxTextureSize;
    GLint           _maxModelviewStackDepth;
    bool            _supportsPVRTC;
    bool            _supportsETC1;
    bool            _supportsS3TC;
    bool            _supportsATITC;
    bool            _supportsNPOT;
    bool            _supportsBGRA8888;
    bool            _supportsDiscardFramebuffer;
    bool            _supportsShareableVAO;
    bool            _supportsOESMapBuffer;
    bool            _supportsOESDepth24;
    bool            _supportsOESPackedDepthStencil;
    
    GLint           _maxSamplesAllowed;
    GLint           _maxTextureUnits;
    char *          _glExtensions;
    int             _maxDirLightInShader; //max support directional light in shader
    int             _maxPointLightInShader; // max support point light in shader
    int             _maxSpotLightInShader; // max support spot light in shader
	
	CAValueMap        _valueDict;
};

NS_CC_END

#endif