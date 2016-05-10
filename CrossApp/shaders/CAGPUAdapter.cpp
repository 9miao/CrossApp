
#include "CAGPUAdapter.h"


NS_CC_BEGIN

CAGPUAdapter::CAGPUAdapter()
	:mGLVersion(GL_INVALID_VER)
	,mGLSLVersion(GLSL_INVALID_VER)
	,mStandard(None)
	,mGLSLGeneration(k110)
{
}

CAGPUAdapter* CAGPUAdapter::create()
{
	CAGPUAdapter* adapter = new CAGPUAdapter();

	if (adapter && adapter->init())
	{
		adapter->autorelease();
		return adapter;
	}

	CC_SAFE_DELETE(adapter);
	return NULL;
}

bool CAGPUAdapter::init()
{
	const char* verStr = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	mGPUGLVersion = verStr;
	mGLVersion = GetGLVersionFromString(mGPUGLVersion.c_str());
	mStandard = GetGLStandardInUseFromString(mGPUGLVersion.c_str());
	if (GetGLSLGeneration(mStandard))
		return true;
	return false;
}

std::string CAGPUAdapter::getGLSLGenerationString()
{
	switch (GetGLSLGeneration()) {
	case k110:
		if (GLES == GetGLStandardInUse()) {
			// ES2s shader language is based on version 1.20 but is version
			// 1.00 of the ES language.
			return "#version 100\n";
		}
		else {
			CCAssert(GL == GetGLStandardInUse(), "");
			return "#version 110\n";
		}
	case k120:
		CCAssert(GL == GetGLStandardInUse(), "");
		return "#version 120\n";
	case k130:
		CCAssert(GL == GetGLStandardInUse(), "");
		return "#version 130\n";
	case k140:
		CCAssert(GL == GetGLStandardInUse(), "");
		return "#version 140\n";
	case k150:
		CCAssert(GL == GetGLStandardInUse(), "");
		return "#version 150\n";
	case k330:
		if (GLES == GetGLStandardInUse()) {
			return "#version 300 es\n";
		}
		else {
			CCAssert(GL == GetGLStandardInUse(), "");
			return "#version 330\n";
		}
	case k310es:
		CCAssert(GLES == GetGLStandardInUse(), "");
		return "#version 310 es\n";
	default:
		CCLOG("CrossApp: ERROR: Unknown GL version.");
		return ""; // suppress warning
	}
}

GLStandard CAGPUAdapter::GetGLStandardInUseFromString(const char* versionString)
{
	if (NULL == versionString) {
		CCLOG("NULL GL version string.");
		return None;
	}

	int major, minor;

	// check for desktop
	int n = sscanf(versionString, "%d.%d", &major, &minor);
	if (2 == n) {
		return GL;
	}

	// check for ES 1
	char profile[2];
	n = sscanf(versionString, "OpenGL ES-%c%c %d.%d", profile, profile+1, &major, &minor);
	if (4 == n) {
		// we no longer support ES1.
		return None;
	}

	// check for ES2
	n = sscanf(versionString, "OpenGL ES %d.%d", &major, &minor);
	if (2 == n) {
		return GLES;
	}
	return None;
}

GLVersion CAGPUAdapter::GetGLVersionFromString(const char* versionString)
{
    if (NULL == versionString) {
        CCLOG("NULL GL version string.");
        return GL_INVALID_VER;
    }

    int major, minor;

    // check for mesa
    int mesaMajor, mesaMinor;
    int n = sscanf(versionString, "%d.%d Mesa %d.%d", &major, &minor, &mesaMajor, &mesaMinor);
    if (4 == n) {
        return GL_VER(major, minor);
    }

    n = sscanf(versionString, "%d.%d", &major, &minor);
    if (2 == n) {
        return GL_VER(major, minor);
    }

    char profile[2];
    n = sscanf(versionString, "OpenGL ES-%c%c %d.%d", profile, profile+1,
               &major, &minor);
    if (4 == n) {
        return GL_VER(major, minor);
    }

    n = sscanf(versionString, "OpenGL ES %d.%d", &major, &minor);
    if (2 == n) {
        return GL_VER(major, minor);
    }

    return GL_INVALID_VER;
}


GLSLVersion CAGPUAdapter::GetGLSLVersionFromString(const char* versionString)
{
    if (NULL == versionString) 
	{
        CCLOG("NULL GLSL version string.");
        return GLSL_INVALID_VER;
    }

    int major, minor;

    int n = sscanf(versionString, "%d.%d", &major, &minor);
    if (2 == n) {
        return GLSL_VER(major, minor);
    }

    n = sscanf(versionString, "OpenGL ES GLSL ES %d.%d", &major, &minor);
    if (2 == n) {
        return GLSL_VER(major, minor);
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
    // android hack until the gpu vender updates their drivers
    n = sscanf(versionString, "OpenGL ES GLSL %d.%d", &major, &minor);
    if (2 == n) {
        return GLSL_VER(major, minor);
    }
#endif

    return GLSL_INVALID_VER;
}

bool CAGPUAdapter::GetGLSLGeneration(const GLStandard standard)
{
	mGPUGLSLVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    GLSLVersion ver = GetGLSLVersionFromString(mGPUGLSLVersion.c_str());
    if (GLSL_INVALID_VER == ver) {
        return false;
    }
    switch (standard) 
	{
	case GL:
		assert(ver >= GLSL_VER(1, 10));
		if (ver >= GLSL_VER(3, 30)) {
			mGLSLGeneration = k330;
		}
		else if (ver >= GLSL_VER(1, 50)) {
			mGLSLGeneration = k150;
		}
		else if (ver >= GLSL_VER(1, 40)) {
			mGLSLGeneration = k140;
		}
		else if (ver >= GLSL_VER(1, 30)) {
			mGLSLGeneration = k130;
		}
		else if (ver >= GLSL_VER(1, 20)) {
			mGLSLGeneration = k120;
		}
		else {
			mGLSLGeneration = k110;
		}
		return true;
	case GLES:
		assert(ver >= GL_VER(1, 00));
		if (ver >= GLSL_VER(3, 1)) {
			mGLSLGeneration = k310es;
		}
		else if (ver >= GLSL_VER(3, 0)) {
			mGLSLGeneration = k330;
		}
		else {
			mGLSLGeneration = k110;
		}
		return true;
	default:
		CCLOG("CrossApp: ERROR: Unknown GL Standard");
		return false;
    }
}

NS_CC_END