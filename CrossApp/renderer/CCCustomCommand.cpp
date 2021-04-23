

#include "renderer/CCCustomCommand.h"

NS_CC_BEGIN

CustomCommand::CustomCommand()
: func(nullptr)
{
    _type = RenderCommand::Type::CUSTOM_COMMAND;
}

void CustomCommand::init(float globalZOrder, const Mat4 &modelViewTransform, uint32_t flags)
{
    RenderCommand::init(globalZOrder, modelViewTransform, flags);
}

void CustomCommand::init(float globalOrder)
{
    _zOrder = globalOrder;
}

CustomCommand::~CustomCommand()
{

}

void CustomCommand::execute()
{
    if(func)
    {
        func();
    }
}

NS_CC_END
