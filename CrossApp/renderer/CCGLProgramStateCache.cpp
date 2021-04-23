

#include "renderer/CCGLProgramStateCache.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCGLProgram.h"


NS_CC_BEGIN

GLProgramStateCache* GLProgramStateCache::s_instance = nullptr;

GLProgramStateCache::GLProgramStateCache()
{
}

GLProgramStateCache::~GLProgramStateCache()
{
    _glProgramStates.clear();
}

GLProgramStateCache* GLProgramStateCache::getInstance()
{
    if (s_instance == nullptr)
        s_instance = new (std::nothrow) GLProgramStateCache();
    
    return s_instance;
}

void GLProgramStateCache::destroyInstance()
{
    CC_SAFE_DELETE(s_instance);
}

GLProgramState* GLProgramStateCache::getGLProgramState(GLProgram* glprogram)
{
    if (_glProgramStates.contains(glprogram))
    {
        return _glProgramStates.at(glprogram);
    }
    
    auto ret = new (std::nothrow) GLProgramState;
    if(ret && ret->init(glprogram)) {
        _glProgramStates.insert(glprogram, ret);
        ret->release();
        return ret;
    }
    
    CC_SAFE_RELEASE(ret);
    return ret;
}

void GLProgramStateCache::removeUnusedGLProgramState()
{
    for( auto it=_glProgramStates.begin(); it!=_glProgramStates.end(); /* nothing */) {
        auto value = it->second;
        if( value->retainCount() == 1 )
        {
            it = _glProgramStates.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void GLProgramStateCache::removeAllGLProgramState()
{
    _glProgramStates.clear();
}

NS_CC_END
