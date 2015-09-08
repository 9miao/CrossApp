#include "CAVideoPlayerRender.h"

#include "CAVideoPlayerDecoder.h"
#include "shaders/CAShaderCache.h"
#include "shaders/CAGLProgram.h"
#include "shaders/ccGLStateCache.h"

NS_CC_BEGIN

#pragma mark - shaders


const char* vertexShaderString = 
"													\n\
attribute vec4 a_position;							\n\
attribute vec2 a_texCoord;							\n\
attribute vec4 a_color;								\n\
													\n\
#ifdef GL_ES										\n\
varying lowp vec4 v_fragmentColor;					\n\
varying mediump vec2 v_texCoord;					\n\
#else												\n\
varying vec4 v_fragmentColor;						\n\
varying vec2 v_texCoord;							\n\
#endif												\n\
void main()											\n\
{													\n\
	gl_Position = CC_MVPMatrix * a_position;		\n\
	v_fragmentColor = a_color;						\n\
	v_texCoord = a_texCoord;						\n\
}													\n\
";


const char* rgbFragmentShaderString =
"													\n\
#ifdef GL_ES										\n\
precision lowp float;								\n\
#endif												\n\
varying vec4 v_fragmentColor;						\n\
varying vec2 v_texCoord;							\n\
uniform sampler2D CC_Texture0;						\n\
void main()											\n\
{													\n\
	gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);	\n\
}													\n\
";


const char* yuvFragmentShaderString =
"													\n\
#ifdef GL_ES										\n\
precision lowp float;								\n\
#endif												\n\
varying vec4 v_fragmentColor;						\n\
varying highp vec2 v_texCoord;						\n\
													\n\
uniform sampler2D s_texture_y;						\n\
uniform sampler2D s_texture_u;						\n\
uniform sampler2D s_texture_v;						\n\
													\n\
void main()											\n\
{													\n\
	highp float y = texture2D(s_texture_y, v_texCoord).r;			\n\
	highp float u = texture2D(s_texture_u, v_texCoord).r - 0.5;		\n\
	highp float v = texture2D(s_texture_v, v_texCoord).r - 0.5;		\n\
													\n\
	highp float r = y +             1.402 * v;		\n\
	highp float g = y - 0.344 * u - 0.714 * v;		\n\
	highp float b = y + 1.772 * u;					\n\
													\n\
	gl_FragColor = vec4(r,g,b,1.0);					\n\
}													\n\
";


static bool validateProgram(GLuint prog)
{
    GLint status;
    
    glValidateProgram(prog);
    
#ifdef DEBUG
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        CCLog("Program validate log:\n%s", log);
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        CCLog("Failed to validate program %d", prog);
        return false;
    }

    
    return true;
}

static GLuint compileShader(GLenum type, const char* sources)
{
    GLint status;
    
    GLuint shader = glCreateShader(type);
    if (shader == 0 || shader == GL_INVALID_ENUM) {
        CCLog("Failed to create shader %d", type);
        return 0;
    }
    
    glShaderSource(shader, 1, &sources, NULL);
    glCompileShader(shader);
    
#ifdef DEBUG
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        CCLog("Shader compile log:\n%s", log);
        free(log);
    }
#endif
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glDeleteShader(shader);
        CCLog("Failed to compile shader:\n");
        return 0;
    }
    
    return shader;
}

static void mat4f_LoadOrtho(float left, float right, float bottom, float top, float fnear, float far, float* mout)
{
    float r_l = right - left;
    float t_b = top - bottom;
    float f_n = far - fnear;
    float tx = - (right + left) / (right - left);
    float ty = - (top + bottom) / (top - bottom);
    float tz = - (far + fnear) / (far - fnear);
    
    mout[0] = 2.0f / r_l;
    mout[1] = 0.0f;
    mout[2] = 0.0f;
    mout[3] = 0.0f;
    
    mout[4] = 0.0f;
    mout[5] = 2.0f / t_b;
    mout[6] = 0.0f;
    mout[7] = 0.0f;
    
    mout[8] = 0.0f;
    mout[9] = 0.0f;
    mout[10] = -2.0f / f_n;
    mout[11] = 0.0f;
    
    mout[12] = tx;
    mout[13] = ty;
    mout[14] = tz;
    mout[15] = 1.0f;
}

#pragma mark - Frame Render


VPFrameRenderRGB::VPFrameRenderRGB()
{
    _texture = 0;
    _uniformSampler = 0;
    _key = "CAVideoPlayerRenderRGB";
}

VPFrameRenderRGB::~VPFrameRenderRGB()
{
    if (_texture) {
        glDeleteTextures(1, &_texture);
        _texture = 0;
    }
}

bool VPFrameRenderRGB::isValid()
{
    return (_texture != 0);
}

const char* VPFrameRenderRGB::fragmentShader()
{
    return rgbFragmentShaderString;
}

void VPFrameRenderRGB::resolveUniforms(GLuint program)
{
    _uniformSampler = glGetUniformLocation(program, "s_texture");
}

void VPFrameRenderRGB::setFrame(VPVideoFrame* frame)
{
    VPVideoFrameRGB *rgbFrame = (VPVideoFrameRGB *)frame;
    
    CCAssert((rgbFrame->getDataLength() == rgbFrame->getWidth() * rgbFrame->getHeight() * 3), "");
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    if (0 == _texture)
        glGenTextures(1, &_texture);
    
    glBindTexture(GL_TEXTURE_2D, _texture);
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 frame->getWidth(),
                 frame->getHeight(),
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 rgbFrame->getData());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

bool VPFrameRenderRGB::prepareRender()
{
    if (_texture == 0) {
        return false;
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glUniform1i(_uniformSampler, 0);
    
    return true;
}

const char* VPFrameRenderRGB::key()
{
    return _key.c_str();
}

VPFrameRenderYUV::VPFrameRenderYUV()
{
    for (int i=0; i<3; i++) {
        _textures[i] = 0;
        _uniformSamplers[i] = 0;
    }
    
    _key = "CAVideoPlayerRenderYUV";
}

VPFrameRenderYUV::~VPFrameRenderYUV()
{
    if (_textures[0])
        glDeleteTextures(3, _textures);
}

bool VPFrameRenderYUV::isValid()
{
    return (_textures[0] != 0);
}

const char* VPFrameRenderYUV::fragmentShader()
{
    return yuvFragmentShaderString;
}

void VPFrameRenderYUV::resolveUniforms(GLuint program)
{
    _uniformSamplers[0] = glGetUniformLocation(program, "s_texture_y");
    _uniformSamplers[1] = glGetUniformLocation(program, "s_texture_u");
    _uniformSamplers[2] = glGetUniformLocation(program, "s_texture_v");
}

void VPFrameRenderYUV::setFrame(VPVideoFrame* frame)
{
    VPVideoFrameYUV *yuvFrame = (VPVideoFrameYUV *)frame;
    
    CCAssert((yuvFrame->getLumaLength() == yuvFrame->getWidth() * yuvFrame->getHeight()), "");
    CCAssert((yuvFrame->getChromaBLength() == (yuvFrame->getWidth() * yuvFrame->getHeight())/4), "");
    CCAssert((yuvFrame->getChromaRLength() == (yuvFrame->getWidth() * yuvFrame->getHeight())/4), "");
    
    const unsigned int frameWidth = frame->getWidth();
    const unsigned int frameHeight = frame->getHeight();
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    if (0 == _textures[0])
        glGenTextures(3, _textures);
    
    const char *pixels[3] = { yuvFrame->getLuma(), yuvFrame->getChromaB(), yuvFrame->getChromaR() };
    const unsigned int widths[3]  = { frameWidth, frameWidth / 2, frameWidth / 2 };
    const unsigned int heights[3] = { frameHeight, frameHeight / 2, frameHeight / 2 };
    
    for (int i = 0; i < 3; ++i) {
        
        glBindTexture(GL_TEXTURE_2D, _textures[i]);
        
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_LUMINANCE,
                     widths[i],
                     heights[i],
                     0,
                     GL_LUMINANCE,
                     GL_UNSIGNED_BYTE,
                     pixels[i]);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

bool VPFrameRenderYUV::prepareRender()
{
    if (_textures[0] == 0)
        return false;
    
    for (int i = 0; i < 3; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, _textures[i]);
        glUniform1i(_uniformSamplers[i], i);
    }
    
    return true;
}

const char* VPFrameRenderYUV::key()
{
    return _key.c_str();
}

VPFrameRender::VPFrameRender()
{
    _program = 0;
    _renderBufer = 0;
    _uniformMatrix = 0;
    
    _vertices[0] = -1.0f;  // x0
    _vertices[1] = -1.0f;  // y0
    _vertices[2] =  1.0f;  // ..
    _vertices[3] = -1.0f;
    _vertices[4] = -1.0f;
    _vertices[5] =  1.0f;
    _vertices[6] =  1.0f;  // x3
    _vertices[7] =  1.0f;  // y3
}

VPFrameRender::~VPFrameRender()
{
    if (_program) {
        glDeleteProgram(_program);
        _program = 0;
    }
    
    if (_renderBufer) {
        glDeleteRenderbuffers(1, &_renderBufer);
        _renderBufer = 0;
    }
}

bool VPFrameRender::loadShaders()
{
	CAGLProgram* pProgram = CAShaderCache::sharedShaderCache()->programForKey("CAVideoPlayerRenderYUV");
    if (!pProgram) {
        pProgram = new CAGLProgram();
        pProgram->initWithVertexShaderByteArray(vertexShaderString, yuvFragmentShaderString);
        CHECK_GL_ERROR_DEBUG();
        pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        pProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        CHECK_GL_ERROR_DEBUG();
        pProgram->link();
        CHECK_GL_ERROR_DEBUG();
        pProgram->updateUniforms();
        CAShaderCache::sharedShaderCache()->addProgram(pProgram, "CAVideoPlayerRenderYUV");
        pProgram->release();
    }
    
	pProgram = CAShaderCache::sharedShaderCache()->programForKey("CAVideoPlayerRenderRGB");
    if (!pProgram) {
        pProgram = new CAGLProgram();
        pProgram->initWithVertexShaderByteArray(vertexShaderString, rgbFragmentShaderString);
        CHECK_GL_ERROR_DEBUG();
        pProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        pProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        pProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        CHECK_GL_ERROR_DEBUG();
        pProgram->link();
        CHECK_GL_ERROR_DEBUG();
        pProgram->updateUniforms();
        CAShaderCache::sharedShaderCache()->addProgram(pProgram, "CAVideoPlayerRenderRGB");
        pProgram->release();
    }
    
	pProgram = CAShaderCache::sharedShaderCache()->programForKey(key());
    if (pProgram) {
        resolveUniforms(pProgram->getProgram());
    }
    return true;
}

#include "kazmath/GL/matrix.h"

void VPFrameRender::draw(VPVideoFrame *frame, long offset)
{
//    static const GLfloat texCoords[] = {
//        0.0f, 1.0f,
//        1.0f, 1.0f,
//        0.0f, 0.0f,
//        1.0f, 0.0f,
//    };

//    static const GLfloat texCoords[] = {
//        1.0f, 1.0f,
//        1.0f, 0.0f,
//        0.0f, 1.0f,
//        0.0f, 0.0f,
//    };

    CAGLProgram *pProgram = CAShaderCache::sharedShaderCache()->programForKey(key());
    pProgram->use();
    pProgram->setUniformsForBuiltins();
    
//    ccGLUseProgram(_program);
//    glUseProgram(_program);
    
    if (frame) {
        setFrame(frame);
    } else {
        return;
    }
    
    if (prepareRender())
    {

    #define kQuadSize sizeof(ccV3F_C4B_T2F)

        ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );

        // vertex
        int diff = offsetof( ccV3F_C4B_T2F, vertices);
        glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));

        // texCoods
        diff = offsetof( ccV3F_C4B_T2F, texCoords);
        glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

        // color
        diff = offsetof( ccV3F_C4B_T2F, colors);
        glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));

//        GLfloat modelviewProj[16];
//        mat4f_LoadOrtho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f, modelviewProj);
//        glUniformMatrix4fv(_uniformMatrix, 1, GL_FALSE, modelviewProj);
//        
//        glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, 0, 0, _vertices);
//        glEnableVertexAttribArray(ATTRIBUTE_VERTEX);
//        glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, 0, 0, texCoords);
//        glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

CCRect VPFrameRender::updateVertices(float width, float height, float screen_w, float screen_h)
{
    const bool fit      = true;
    const float dH      = (float)screen_h / height;
    const float dW      = (float)screen_w / width;
    const float dd      = fit ? MIN(dH, dW) : MAX(dH, dW);
    const float h       = (height * dd / (float)screen_h);
    float w       = (width  * dd / (float)screen_w );

    _vertices[0] = - w;
    _vertices[1] = - h;
    _vertices[2] =   w;
    _vertices[3] = - h;
    _vertices[4] = - w;
    _vertices[5] =   h;
    _vertices[6] =   w;
    _vertices[7] =   h;
    
    return CCRect((screen_w - w * screen_w)/2, (screen_h - screen_h * h)/2, w * screen_w, screen_h * h);
}

NS_CC_END