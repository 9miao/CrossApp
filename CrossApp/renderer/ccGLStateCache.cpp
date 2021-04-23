
#include "renderer/ccGLStateCache.h"

#include "renderer/CCGLProgram.h"
#include "renderer/CCRenderState.h"
#include "basics/CAApplication.h"
#include "ccConfig.h"
#include "basics/CAConfiguration.h"

NS_CC_BEGIN

static const int MAX_ATTRIBUTES = 16;
static const int MAX_ACTIVE_TEXTURE = 16;

namespace
{
    static GLuint s_currentProjectionMatrix = -1;
    static uint32_t s_attributeFlags = 0;  // 32 attributes max

#if CC_ENABLE_GL_STATE_CACHE

    static GLuint    s_currentShaderProgram = -1;
    static GLuint    s_currentBoundTexture[MAX_ACTIVE_TEXTURE] =  {(GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, };
    static GLenum    s_blendingSource = -1;
    static GLenum    s_blendingDest = -1;
    static int       s_GLServerState = 0;
    static GLuint    s_VAO = 0;
    static GLenum    s_activeTexture = -1;

#endif // CC_ENABLE_GL_STATE_CACHE
}

// GL State Cache functions

namespace GL {

void invalidateStateCache( void )
{
    CAApplication::getApplication()->resetMatrixStack();
    s_currentProjectionMatrix = -1;
    s_attributeFlags = 0;

#if CC_ENABLE_GL_STATE_CACHE
    s_currentShaderProgram = -1;
    for( int i=0; i < MAX_ACTIVE_TEXTURE; i++ )
    {
        s_currentBoundTexture[i] = -1;
    }

    s_blendingSource = -1;
    s_blendingDest = -1;
    s_GLServerState = 0;
    s_VAO = 0;
    
#endif // CC_ENABLE_GL_STATE_CACHE
}

void deleteProgram( GLuint program )
{
#if CC_ENABLE_GL_STATE_CACHE
    if(program == s_currentShaderProgram)
    {
        s_currentShaderProgram = -1;
    }
#endif // CC_ENABLE_GL_STATE_CACHE

    glDeleteProgram( program );
}

void useProgram( GLuint program )
{
#if CC_ENABLE_GL_STATE_CACHE
    if( program != s_currentShaderProgram ) {
        s_currentShaderProgram = program;
        glUseProgram(program);
    }
#else
    glUseProgram(program);
#endif // CC_ENABLE_GL_STATE_CACHE
}

static void SetBlending(GLenum sfactor, GLenum dfactor)
{
	if (sfactor == GL_ONE && dfactor == GL_ZERO)
    {
		glDisable(GL_BLEND);
        RenderState::StateBlock::_defaultState->setBlend(false);
	}
    else
    {
		glEnable(GL_BLEND);
		glBlendFunc(sfactor, dfactor);

        RenderState::StateBlock::_defaultState->setBlend(true);
        RenderState::StateBlock::_defaultState->setBlendSrc((RenderState::Blend)sfactor);
        RenderState::StateBlock::_defaultState->setBlendDst((RenderState::Blend)dfactor);
    }
}

void blendFunc(GLenum sfactor, GLenum dfactor)
{
#if CC_ENABLE_GL_STATE_CACHE
    if (sfactor != s_blendingSource || dfactor != s_blendingDest)
    {
        s_blendingSource = sfactor;
        s_blendingDest = dfactor;
        SetBlending(sfactor, dfactor);
    }
#else
    SetBlending( sfactor, dfactor );
#endif // CC_ENABLE_GL_STATE_CACHE
}

void blendResetToCache(void)
{
	glBlendEquation(GL_FUNC_ADD);
#if CC_ENABLE_GL_STATE_CACHE
	SetBlending(s_blendingSource, s_blendingDest);
#else
	SetBlending(CC_BLEND_SRC, CC_BLEND_DST);
#endif // CC_ENABLE_GL_STATE_CACHE
}

void bindTexture2D(GLuint textureId)
{
    GL::bindTexture2DN(0, textureId);
}

void bindTexture2DN(GLuint textureUnit, GLuint textureId)
{
#if CC_ENABLE_GL_STATE_CACHE
	CCASSERT(textureUnit < MAX_ACTIVE_TEXTURE, "textureUnit is too big");
	if (s_currentBoundTexture[textureUnit] != textureId)
	{
		s_currentBoundTexture[textureUnit] = textureId;
		activeTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
#else
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureId);
#endif
}

void bindTextureN(GLuint textureUnit, GLuint textureId, GLuint textureType/* = GL_TEXTURE_2D*/)
{
#if CC_ENABLE_GL_STATE_CACHE
    CCASSERT(textureUnit < MAX_ACTIVE_TEXTURE, "textureUnit is too big");
    if (s_currentBoundTexture[textureUnit] != textureId)
    {
        s_currentBoundTexture[textureUnit] = textureId;
        activeTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(textureType, textureId);
    }
#else
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(textureType, textureId);
#endif
}


void deleteTexture(GLuint textureId)
{
#if CC_ENABLE_GL_STATE_CACHE
    for (size_t i = 0; i < MAX_ACTIVE_TEXTURE; ++i)
    {
        if (s_currentBoundTexture[i] == textureId)
        {
            s_currentBoundTexture[i] = -1;
        }
    }
#endif // CC_ENABLE_GL_STATE_CACHE
    
	glDeleteTextures(1, &textureId);
}

void activeTexture(GLenum texture)
{
#if CC_ENABLE_GL_STATE_CACHE
    if(s_activeTexture != texture) {
        s_activeTexture = texture;
        glActiveTexture(s_activeTexture);
    }
#else
    glActiveTexture(texture);
#endif
}

void bindVAO(GLuint vaoId)
{
#if CC_TEXTURE_ATLAS_USE_VAO

#if CC_ENABLE_GL_STATE_CACHE
    if (s_VAO != vaoId)
    {
        s_VAO = vaoId;
        glBindVertexArray(vaoId);
    }
#else
    glBindVertexArray(vaoId);
#endif // CC_ENABLE_GL_STATE_CACHE
    
#endif
}

// GL Vertex Attrib functions

void enableVertexAttribs(uint32_t flags)
{
    bindVAO(0);

    // hardcoded!
    for(int i=0; i < MAX_ATTRIBUTES; i++) {
        unsigned int bit = 1 << i;
        //FIXME:Cache is disabled, try to enable cache as before
        bool enabled = (flags & bit) != 0;
        bool enabledBefore = (s_attributeFlags & bit) != 0;
        if(enabled != enabledBefore) 
        {
            if( enabled )
                glEnableVertexAttribArray(i);
            else
                glDisableVertexAttribArray(i);
        }
    }
    s_attributeFlags = flags;
}

// GL Uniforms functions

void setProjectionMatrixDirty( void )
{
    s_currentProjectionMatrix = -1;
}

} // Namespace GL

NS_CC_END
