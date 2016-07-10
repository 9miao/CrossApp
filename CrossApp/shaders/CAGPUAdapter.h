#ifndef __GPU_ADAPTER_H__
#define __GPU_ADAPTER_H__

#include "ccMacros.h"
#include "basics/CAObject.h"
#include <string>
#include "CCGL.h"

NS_CC_BEGIN

enum GLStandard 
{
    None,
    GL,
    GLES
};

// Limited set of GLSL versions we build shaders for. Caller should round
// down the GLSL version to one of these enums.
enum GLSLGeneration {
    /**
     * Desktop GLSL 1.10 and ES2 shading language (based on desktop GLSL 1.20)
     */
    k110,
    /**
     * Desktop GLSL 1.20
     */
	k120,
    /**
     * Desktop GLSL 1.30
     */
    k130,
    /**
     * Desktop GLSL 1.40
     */
    k140,
    /**
     * Desktop GLSL 1.50
     */
    k150,
    /**
     * Desktop GLSL 3.30, and ES GLSL 3.00
     */
    k330,
    /**
     * ES GLSL 3.10 only TODO Make GLSLCap objects to make this more granular
     */
    k310es,
};

typedef unsigned int GLVersion;
typedef unsigned int GLSLVersion;

#define GL_VER(major, minor) ((static_cast<int>(major) << 16) | static_cast<int>(minor))
#define GLSL_VER(major, minor) ((static_cast<int>(major) << 16) | static_cast<int>(minor))

#define GL_INVALID_VER GL_VER(0, 0)
#define GLSL_INVALID_VER GL_VER(0, 0)

class CAGPUAdapter : public CAObject
{
public:
	CAGPUAdapter();
	virtual ~CAGPUAdapter(){}

	static CAGPUAdapter* create();

	bool init();

	std::string getGLSLGenerationString();

	GLVersion GetGLVersion(){return mGLVersion ;}
	GLSLVersion GetGLSLVersion(){return mGLSLVersion;}
	GLStandard GetGLStandardInUse(){return mStandard;}
	GLSLGeneration GetGLSLGeneration(){return mGLSLGeneration;}

protected:
	GLStandard GetGLStandardInUseFromString(const char* versionString);
	GLVersion GetGLVersionFromString(const char* versionString) ;
	GLSLVersion GetGLSLVersionFromString(const char* versionString);
	bool GetGLSLGeneration(const GLStandard standard);

protected:
	GLVersion mGLVersion;
	GLSLVersion mGLSLVersion;
	GLStandard mStandard;
	GLSLGeneration mGLSLGeneration;

	std::string mGPUGLVersion;
	std::string mGPUGLSLVersion;
};

NS_CC_END

#endif
