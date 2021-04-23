
#include "renderer/CCRenderCommand.h"
#include "basics/CACamera.h"

NS_CC_BEGIN

RenderCommand::RenderCommand()
: _type(RenderCommand::Type::UNKNOWN_COMMAND)
, _zOrder(0)
, _isTransparent(true)
, _skipBatching(false)
, _is3D(false)
, _depth(0)
{
}

RenderCommand::~RenderCommand()
{
}

void RenderCommand::init(float zOrder, const Mat4 &transform, uint32_t flags)
{
    _zOrder = zOrder;
    if (flags & FLAGS_RENDER_AS_3D)//error
    {
        if (CACamera::getVisitingCamera())
            _depth = CACamera::getVisitingCamera()->getDepthInView(transform);
        
        set3D(true);
    }
    else
    {
        set3D(false);
        _depth = 0;
    }
}

void RenderCommand::printID()
{
    printf("Command Depth: %f\n", _zOrder);
}

NS_CC_END
