

#include "CAClippingView.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCRenderState.h"
#include "basics/CAApplication.h"
#include "basics/CAPointExtension.h"
#include "view/CADrawingPrimitives.h"

NS_CC_BEGIN


#if !(CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

static void setProgram(CAView *n, GLProgram *p)
{
    n->setShaderProgram(p);
    CC_RETURN_IF(n->getSubviews().empty());
    
    
    auto& subviews = n->getSubviews();
    for(const auto &subview : subviews)
    {
        setProgram(subview, p);
    }
}
#endif


static GLint g_sStencilBits = -1;


CAClippingView::CAClippingView()
: m_pStencil(nullptr)
, m_fAlphaThreshold(1.0f)
, m_bInverted(true)
, m_bClippingEnabled(true)
{}

CAClippingView::~CAClippingView()
{
    CC_SAFE_RELEASE(m_pStencil);
}

CAClippingView* CAClippingView::create()
{
    CAClippingView *pRet = new CAClippingView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

CAClippingView* CAClippingView::create(CAView *pStencil)
{
    CAClippingView *pRet = new CAClippingView();
    if (pRet && pRet->init(pStencil))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool CAClippingView::init()
{
    return init(nullptr);
}

bool CAClippingView::init(CAView *pStencil)
{
    this->setStencil(pStencil);
    
    m_fAlphaThreshold = 1.0f;
    
    static bool once = true;
    if (once)
    {
        glGetIntegerv(GL_STENCIL_BITS, &g_sStencilBits);
        if (g_sStencilBits <= 0)
        {
            //"Stencil buffer is not enabled."
        }
        once = false;
    }
    
    return true;
}

void CAClippingView::onEnter()
{
    CAView::onEnter();
    if (m_pStencil)
    {
        m_pStencil->onEnter();
    }
}

void CAClippingView::onEnterTransitionDidFinish()
{
    CAView::onEnterTransitionDidFinish();
    if (m_pStencil)
    {
        m_pStencil->onEnterTransitionDidFinish();
    }
}

void CAClippingView::onExitTransitionDidStart()
{
    if (m_pStencil)
    {
        m_pStencil->onExitTransitionDidStart();
    }
    CAView::onExitTransitionDidStart();
}

void CAClippingView::onExit()
{
    if (m_pStencil)
    {
        m_pStencil->onExit();
    }
    CAView::onExit();
}


void CAClippingView::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    CC_RETURN_IF(!m_bVisible || m_obSubviews.empty());
    
    if (!m_bClippingEnabled || !m_pStencil || g_sStencilBits < 1)
    {
        // draw everything, as if there where no stencil
        CAView::visit(renderer, parentTransform, parentFlags);
        return;
    }
    
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    
    CAApplication* application = CAApplication::getApplication();
    application->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    application->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, m_tModelViewTransform);
    
    //Add group command
    
    m_obGroupCommand.init(m_nZOrder);
    renderer->addCommand(&m_obGroupCommand);
    
    renderer->pushGroup(m_obGroupCommand.getRenderQueueID());
    
    ///////////////////////////////////
    // INIT
    static GLint layer = -1;
    // increment the current layer
    
    // mask of the current layer (ie: for layer 3: 00000100)
    GLint mask_layer = 0x1 << layer;
    // mask of all layers less than the current (ie: for layer 3: 00000011)
    GLint mask_layer_l = mask_layer - 1;
    // mask of all layers less than or equal to the current (ie: for layer 3: 00000111)
    GLint mask_layer_le = mask_layer | mask_layer_l;
    
    // manually save the stencil state
    GLboolean currentStencilEnabled = GL_FALSE;
    GLuint currentStencilWriteMask = ~0;
    GLenum currentStencilFunc = GL_ALWAYS;
    GLint currentStencilRef = 0;
    GLuint currentStencilValueMask = ~0;
    GLenum currentStencilFail = GL_KEEP;
    GLenum currentStencilPassDepthFail = GL_KEEP;
    GLenum currentStencilPassDepthPass = GL_KEEP;
    GLboolean currentDepthWriteMask = GL_FALSE;
    GLboolean currentAlphaTestEnabled = GL_FALSE;
    GLenum currentAlphaTestFunc = GL_ALWAYS;
    GLclampf currentAlphaTestRef = 1;
    
    
    m_obBeforeVisitCmd.init(m_nZOrder);
    m_obBeforeVisitCmd.func = [=]()
    {
        // increment the current layer
        layer++;
        
        // mask of the current layer (ie: for layer 3: 00000100)
        GLint mask_layer = 0x1 << layer;
        // mask of all layers less than the current (ie: for layer 3: 00000011)
        GLint mask_layer_l = mask_layer - 1;
        // mask of all layers less than or equal to the current (ie: for layer 3: 00000111)
        GLint mask_layer_le = mask_layer | mask_layer_l;
        
        GLint currentStencilRef = 0;
        GLboolean currentDepthWriteMask = GL_FALSE;
        
        // manually save the stencil state
        
        GLboolean currentStencilEnabled = glIsEnabled(GL_STENCIL_TEST);
        glGetIntegerv(GL_STENCIL_WRITEMASK, (GLint *)&currentStencilWriteMask);
        glGetIntegerv(GL_STENCIL_FUNC, (GLint *)&currentStencilFunc);
        glGetIntegerv(GL_STENCIL_REF, &currentStencilRef);
        glGetIntegerv(GL_STENCIL_VALUE_MASK, (GLint *)&currentStencilValueMask);
        glGetIntegerv(GL_STENCIL_FAIL, (GLint *)&currentStencilFail);
        glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, (GLint *)&currentStencilPassDepthFail);
        glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, (GLint *)&currentStencilPassDepthPass);
        
        // enable stencil use
        glEnable(GL_STENCIL_TEST);
        //    RenderState::StateBlock::_defaultState->setStencilTest(true);
        
        // check for OpenGL error while enabling stencil test
        CHECK_GL_ERROR_DEBUG();
        
        // all bits on the stencil buffer are readonly, except the current layer bit,
        // this means that operation like glClear or glStencilOp will be masked with this value
        glStencilMask(mask_layer);
        //    RenderState::StateBlock::_defaultState->setStencilWrite(mask_layer);
        
        // manually save the depth test state
        
        glGetBooleanv(GL_DEPTH_WRITEMASK, &currentDepthWriteMask);
        
        // disable depth test while drawing the stencil
        //glDisable(GL_DEPTH_TEST);
        // disable update to the depth buffer while drawing the stencil,
        // as the stencil is not meant to be rendered in the real scene,
        // it should never prevent something else to be drawn,
        // only disabling depth buffer update should do
        glDepthMask(GL_FALSE);
        RenderState::StateBlock::_defaultState->setDepthWrite(false);
        
        ///////////////////////////////////
        // CLEAR STENCIL BUFFER
        
        // manually clear the stencil buffer by drawing a fullscreen rectangle on it
        // setup the stencil test func like this:
        // for each pixel in the fullscreen rectangle
        //     never draw it into the frame buffer
        //     if not in inverted mode: set the current layer value to 0 in the stencil buffer
        //     if in inverted mode: set the current layer value to 1 in the stencil buffer
        glStencilFunc(GL_NEVER, mask_layer, mask_layer);
        glStencilOp(!m_bInverted ? GL_ZERO : GL_REPLACE, GL_KEEP, GL_KEEP);
        
        // draw a fullscreen solid rectangle to clear the stencil buffer
        //ccDrawSolidRect(Vec2::ZERO, ccpFromSize([[CAApplication sharedCAApplication] winSize]), Color4F(1, 1, 1, 1));
        
        application->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
        application->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
        
        application->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
        application->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
        
        Vec2 vertices[] = {
            Vec2(-1.0f, -1.0f),
            Vec2(1.0f, -1.0f),
            Vec2(1.0f, 1.0f),
            Vec2(-1.0f, 1.0f)
        };
        
        auto glProgram = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR);
        
        int colorLocation = glProgram->getUniformLocation("u_color");
        CHECK_GL_ERROR_DEBUG();
        
        CAColor4F color = CAColor4F(1, 1, 1, 1);
        
        glProgram->use();
        glProgram->setUniformsForBuiltins();
        glProgram->setUniformLocationWith4fv(colorLocation, (GLfloat*) &color.r, 1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        
        INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
        
        application->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
        application->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
        ///////////////////////////////////
        // DRAW CLIPPING STENCIL
        // setup the stencil test func like this:
        // for each pixel in the stencil node
        //     never draw it into the frame buffer
        //     if not in inverted mode: set the current layer value to 1 in the stencil buffer
        //     if in inverted mode: set the current layer value to 0 in the stencil buffer
        glStencilFunc(GL_NEVER, mask_layer, mask_layer);
        //    RenderState::StateBlock::_defaultState->setStencilFunction(RenderState::STENCIL_NEVER, mask_layer, mask_layer);
        
        glStencilOp(!m_bInverted ? GL_REPLACE : GL_ZERO, GL_KEEP, GL_KEEP);
        //    RenderState::StateBlock::_defaultState->setStencilOperation(
        //                                                                !_inverted ? RenderState::STENCIL_OP_REPLACE : RenderState::STENCIL_OP_ZERO,
        //                                                                RenderState::STENCIL_OP_KEEP,
        //                                                                RenderState::STENCIL_OP_KEEP);
        
        
        // enable alpha test only if the alpha threshold < 1,
        // indeed if alpha threshold == 1, every pixel will be drawn anyways
        if (m_fAlphaThreshold < 1)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            // manually save the alpha test state
            GLclampf currentAlphaTestRef = 1;
            GLboolean currentAlphaTestEnabled = glIsEnabled(GL_ALPHA_TEST);
            glGetIntegerv(GL_ALPHA_TEST_FUNC, (GLint *)&currentAlphaTestFunc);
            
            glGetFloatv(GL_ALPHA_TEST_REF, &currentAlphaTestRef);
            // enable alpha testing
            glEnable(GL_ALPHA_TEST);
            // check for OpenGL error while enabling alpha test
            CHECK_GL_ERROR_DEBUG();
            // pixel will be drawn only if greater than an alpha threshold
            glAlphaFunc(GL_GREATER, m_fAlphaThreshold);
#endif
        }
    };
    renderer->addCommand(&m_obBeforeVisitCmd);
    
    auto alphaThreshold = this->getAlphaThreshold();
    if (alphaThreshold < 1)
    {
#if !(CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        // since glAlphaTest do not exists in OES, use a shader that writes
        // pixel only if greater than an alpha threshold
        GLProgram *program = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST_NO_MV);
        GLint alphaValueLocation = glGetUniformLocation(program->getProgram(), GLProgram::UNIFORM_NAME_ALPHA_TEST_VALUE);
        // set our alphaThreshold
        program->use();
        program->setUniformLocationWith1f(alphaValueLocation, alphaThreshold);
        // we need to recursively apply this shader to all the nodes in the stencil node
        // FIXME: we should have a way to apply shader to all nodes without having to do this
        setProgram(m_pStencil, program);
        
#endif
        
    }
    m_pStencil->visit(renderer, m_tModelViewTransform, flags);
    
    m_obAfterDrawStencilCmd.init(m_nZOrder);
    m_obAfterDrawStencilCmd.func = [=]()
    {
        if (m_fAlphaThreshold < 1)
        {
#if !(CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            // FIXME: we need to find a way to restore the shaders of the stencil node and its children
#else
            // manually restore the alpha test state
            glAlphaFunc(currentAlphaTestFunc, currentAlphaTestRef);
            if (!currentAlphaTestEnabled)
            {
                glDisable(GL_ALPHA_TEST);
            }
#endif
        }
        
        // restore the depth test state
        glDepthMask(currentDepthWriteMask);
        RenderState::StateBlock::_defaultState->setDepthWrite(currentDepthWriteMask != 0);
        
        //if (currentDepthTestEnabled) {
        //    glEnable(GL_DEPTH_TEST);
        //}
        
        ///////////////////////////////////
        // DRAW CONTENT
        
        // setup the stencil test function like this:
        // for each pixel of this node and its children
        //     if all layers less than or equals to the current are set to 1 in the stencil buffer
        //         draw the pixel and keep the current layer in the stencil buffer
        //     else
        //         do not draw the pixel but keep the current layer in the stencil buffer
        glStencilFunc(GL_EQUAL, mask_layer_le, mask_layer_le);
        //    RenderState::StateBlock::_defaultState->setStencilFunction(RenderState::STENCIL_EQUAL, _mask_layer_le, _mask_layer_le);
        
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    };
    renderer->addCommand(&m_obAfterDrawStencilCmd);
    
    int i = 0;
    bool visibleByCamera = isVisitableByVisitingCamera();
    
    if(!m_obSubviews.empty())
    {
        sortAllSubviews();
        // draw children zOrder < 0
        for( ; i < m_obSubviews.size(); i++ )
        {
            auto view = m_obSubviews.at(i);
            
            if ( view && view->getZOrder() < 0 )
            view->visit(renderer, m_tModelViewTransform, flags);
            else
            break;
        }
        // self draw
        if (visibleByCamera)
        this->draw(renderer, m_tModelViewTransform, flags);
        
        for(auto it=m_obSubviews.begin()+i; it != m_obSubviews.end(); ++it)
        (*it)->visit(renderer, m_tModelViewTransform, flags);
    }
    else if (visibleByCamera)
    {
        this->draw(renderer, m_tModelViewTransform, flags);
    }
    
    m_obAfterVisitCmd.init(m_nZOrder);
    m_obAfterVisitCmd.func = [=]()
    {
        // manually restore the stencil state
        glStencilFunc(currentStencilFunc, currentStencilRef, currentStencilValueMask);
        //    RenderState::StateBlock::_defaultState->setStencilFunction((RenderState::StencilFunction)_currentStencilFunc, _currentStencilRef, _currentStencilValueMask);
        
        glStencilOp(currentStencilFail, currentStencilPassDepthFail, currentStencilPassDepthPass);
        //    RenderState::StateBlock::_defaultState->setStencilOperation((RenderState::StencilOperation)_currentStencilFail,
        //                                                                (RenderState::StencilOperation)_currentStencilPassDepthFail,
        //                                                                (RenderState::StencilOperation)_currentStencilPassDepthPass);
        
        glStencilMask(currentStencilWriteMask);
        if (!currentStencilEnabled)
        {
            glDisable(GL_STENCIL_TEST);
            //        RenderState::StateBlock::_defaultState->setStencilTest(false);
        }
        
        // we are done using this layer, decrement
        layer--;
    };
    renderer->addCommand(&m_obAfterVisitCmd);
    
    renderer->popGroup();
    
    application->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

CAView* CAClippingView::getStencil() const
{
    return m_pStencil;
}

void CAClippingView::setStencil(CAView *pStencil)
{
    if (m_pStencil)
    {
        m_pStencil->setSuperview(nullptr);
        if (this->isRunning())
        {
            m_pStencil->onExitTransitionDidStart();
            m_pStencil->onExit();
        }
    }
    CC_SAFE_RELEASE(m_pStencil);
    m_pStencil = pStencil;
    CC_SAFE_RETAIN(m_pStencil);
    
    if (m_pStencil)
    {
        m_pStencil->setSuperview(this);
        if (this->isRunning())
        {
            m_pStencil->onEnter();
            m_pStencil->onEnterTransitionDidFinish();
        }
    }
    
}

GLfloat CAClippingView::getAlphaThreshold() const
{
    return m_fAlphaThreshold;
}

void CAClippingView::setAlphaThreshold(GLfloat fAlphaThreshold)
{
    m_fAlphaThreshold = fAlphaThreshold;
}

bool CAClippingView::isInverted() const
{
    return m_bInverted;
}

void CAClippingView::setInverted(bool bInverted)
{
    m_bInverted = bInverted;
}

bool CAClippingView::isClippingEnabled() const
{
    return m_bClippingEnabled;
}
void CAClippingView::setClippingEnabled(bool bClippingEnabled)
{
    m_bClippingEnabled = bClippingEnabled;
}

NS_CC_END
