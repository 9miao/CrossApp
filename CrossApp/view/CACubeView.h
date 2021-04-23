
#ifndef __CLUBVIEW_H__
#define __CLUBVIEW_H__

#include "ccTypes.h"
#include "platform/CCPlatformMacros.h"
#include "renderer/CCCustomCommand.h"
#include "view/CAView.h"

NS_CC_BEGIN

class CAImageCube;

class CC_DLL CACubeView : public CAView
{
public:
    
    static CACubeView* create(const std::string& positive_x, const std::string& negative_x,
                               const std::string& positive_y, const std::string& negative_y,
                               const std::string& positive_z, const std::string& negative_z);

    void setImage(CAImageCube* image);

    virtual void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) override;

    void reload();

    CACubeView();

    virtual ~CACubeView();

    virtual bool init() override;

    bool init(const std::string& positive_x, const std::string& negative_x,
              const std::string& positive_y, const std::string& negative_y,
              const std::string& positive_z, const std::string& negative_z);

protected:

    void initBuffers();

    void onDraw(const Mat4& transform, uint32_t flags);

    GLuint      _vao;
    GLuint      _vertexBuffer;
    GLuint      _indexBuffer;

    CustomCommand _customCommand;

    CAImageCube*  m_pImage;
};

NS_CC_END

#endif // __CLUBVIEW_H__
