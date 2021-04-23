

#include "renderer/CCTechnique.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/CCMaterial.h"
#include "renderer/CCPass.h"

NS_CC_BEGIN

Technique* Technique::createWithGLProgramState(Material* parent, GLProgramState* state)
{
    auto technique = new (std::nothrow) Technique();
    if (technique && technique->init(parent))
    {
        auto pass = Pass::createWithGLProgramState(technique, state);
        technique->addPass(pass);

        technique->autorelease();
        return technique;
    }
    return  nullptr;
}

Technique* Technique::create(Material* material)
{
    auto technique = new (std::nothrow) Technique();
    if (technique && technique->init(material))
    {
        technique->autorelease();
        return technique;
    }
    return nullptr;
}

Technique::Technique()
: _name("")
{
}

Technique::~Technique()
{
}

bool Technique::init(Material* parent)
{
    _parent = parent;
    return true;
}

Technique* Technique::clone() const
{
    auto technique = new (std::nothrow) Technique();

    if (technique)
    {
        technique->_name = _name;
        RenderState::cloneInto(technique);

        for (const auto pass: _passes)
        {
            auto p = pass->clone();
            p->_parent = technique;
            technique->_passes.pushBack(p);
        }

        technique->autorelease();
    }
    return technique;
}

void Technique::addPass(Pass *pass)
{
    _passes.pushBack(pass);
}

std::string Technique::getName() const
{
    return _name;
}

void Technique::setName(const std::string &name)
{
    _name = name;
}

Pass* Technique::getPassByIndex(ssize_t index) const
{
    CC_ASSERT(index>=0 && index<_passes.size() && "Invalid index");
    return _passes.at(index);
}

ssize_t Technique::getPassCount() const
{
    return _passes.size();
}

const CAVector<Pass*>& Technique::getPasses() const
{
    return _passes;
}

NS_CC_END
