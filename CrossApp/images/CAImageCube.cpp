
#include "images/CAImageCube.h"
#include "platform/CAFileUtils.h"
#include "renderer/ccGLStateCache.h"

NS_CC_BEGIN

CAImageCube::CAImageCube()
{
    _imgPath.resize(6);
}

CAImageCube::~CAImageCube()
{
}

CAImageCube* CAImageCube::create(const std::string& positive_x, const std::string& negative_x,
                                 const std::string& positive_y, const std::string& negative_y,
                                 const std::string& positive_z, const std::string& negative_z)
{
    CAImageCube* ret = new (std::nothrow) CAImageCube();
    if (ret && ret->init(positive_x, negative_x, positive_y, negative_y, positive_z, negative_z))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CAImageCube::init(const std::string& positive_x, const std::string& negative_x,
                       const std::string& positive_y, const std::string& negative_y,
                       const std::string& positive_z, const std::string& negative_z)
{
    _imgPath[0] = positive_x;
    _imgPath[1] = negative_x;
    _imgPath[2] = positive_y;
    _imgPath[3] = negative_y;
    _imgPath[4] = positive_z;
    _imgPath[5] = negative_z;

    std::vector<CAImage*> images(6);

    images[0] = CAImage::create(positive_x);
    images[1] = CAImage::create(negative_x);
    images[2] = CAImage::create(positive_y);
    images[3] = CAImage::create(negative_y);
    images[4] = CAImage::create(positive_z);
    images[5] = CAImage::create(negative_z);

    GLuint handle;
    glGenTextures(1, &handle);

    GL::bindTextureN(0, handle, GL_TEXTURE_CUBE_MAP);

    for (int i = 0; i < 6; i++)
    {
        CAImage* img = images[i];

        CAImage::PixelFormat    ePixelFmt   = img->getPixelFormat();
        unsigned char*          pData       = img->getData()->getBytes();
        if (ePixelFmt == CAImage::PixelFormat::RGBA8888 || ePixelFmt == CAImage::PixelFormat::DEFAULT)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,                  // level
                         GL_RGBA,            // internal format
                         img->getPixelsWide(),    // width
                         img->getPixelsHigh(),   // height
                         0,                  // border
                         GL_RGBA,            // format
                         GL_UNSIGNED_BYTE,   // type
                         pData);             // pixel data
        }
        else if (ePixelFmt == CAImage::PixelFormat::RGB888)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,                  // level
                         GL_RGB,             // internal format
                         img->getPixelsWide(),    // width
                         img->getPixelsHigh(),   // height
                         0,                  // border
                         GL_RGB,             // format
                         GL_UNSIGNED_BYTE,   // type
                         pData);             // pixel data
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_uName = handle;

    GL::bindTextureN(0, 0, GL_TEXTURE_CUBE_MAP);

    return true;
}

void CAImageCube::setTexParameters(const TexParams& texParams)
{
    CCASSERT(m_uName != 0, __FUNCTION__);

    GL::bindTextureN(0, m_uName, GL_TEXTURE_CUBE_MAP);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, texParams.minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, texParams.magFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, texParams.wrapS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, texParams.wrapT);

    GL::bindTextureN(0, 0, GL_TEXTURE_CUBE_MAP);
}

bool CAImageCube::reloadImage()
{
    return init(_imgPath[0], _imgPath[1], _imgPath[2], _imgPath[3], _imgPath[4], _imgPath[5]);
}

NS_CC_END
