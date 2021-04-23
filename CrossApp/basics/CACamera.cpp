
#include "basics/CACamera.h"
#include "basics/CACameraBackgroundBrush.h"
#include "basics/CAApplication.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCQuadCommand.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCFrameBuffer.h"
#include "renderer/CCRenderState.h"
#include "view/CAWindow.h"
NS_CC_BEGIN

CACamera* CACamera::_visitingCamera = nullptr;
experimental::Viewport CACamera::_defaultViewport;

// start static methods

CACamera* CACamera::create()
{
    CACamera* camera = new (std::nothrow) CACamera();
    camera->initDefault();
    camera->autorelease();
    camera->setDepth(0.f);
    
    return camera;
}

CACamera* CACamera::createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
    auto ret = new (std::nothrow) CACamera();
    if (ret)
    {
        ret->initPerspective(fieldOfView, aspectRatio, nearPlane, farPlane);
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CACamera* CACamera::createOrthographic(float zoomX, float zoomY, float nearPlane, float farPlane)
{
    auto ret = new (std::nothrow) CACamera();
    if (ret)
    {
        ret->initOrthographic(zoomX, zoomY, nearPlane, farPlane);
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CACamera* CACamera::getDefaultCamera()
{
    auto window = CAApplication::getApplication()->getRootWindow();
    if(window)
    {
        return window->getDefaultCamera();
    }
    
    return nullptr;
}

const experimental::Viewport& CACamera::getDefaultViewport()
{
    return _defaultViewport;
}
void CACamera::setDefaultViewport(const experimental::Viewport& vp)
{
    _defaultViewport = vp;
}

const CACamera* CACamera::getVisitingCamera()
{
    return _visitingCamera;
}

// end static methods

CACamera::CACamera()
: _window(nullptr)
, _viewProjectionDirty(true)
, _cameraFlag(1)
, _frustumDirty(true)
, _depth(-1)
, _fbo(nullptr)
{
    _clearBrush = CACameraBackgroundBrush::createDepthBrush(1.f);
    _clearBrush->retain();
}

CACamera::~CACamera()
{
    CC_SAFE_RELEASE_NULL(_fbo);
    CC_SAFE_RELEASE(_clearBrush);
}

const Mat4& CACamera::getProjectionMatrix() const
{
    return _projection;
}
const Mat4& CACamera::getViewMatrix() const
{
    Mat4 viewInv(getViewToWorldTransform());
    static int count = sizeof(float) * 16;
    if (memcmp(viewInv.m, _viewInv.m, count) != 0)
    {
        _viewProjectionDirty = true;
        _frustumDirty = true;
        _viewInv = viewInv;
        _view = viewInv.getInversed();
    }
    return _view;
}
void CACamera::lookAt(const Vec3& lookAtPos, const Vec3& up)
{
    Vec3 upv = up;
    upv.normalize();
    Vec3 zaxis;
    Vec3::subtract(DPoint3D(-m_obPoint.x, -m_obPoint.y, -m_fPointZ), lookAtPos, &zaxis);
    zaxis.normalize();
    
    Vec3 xaxis;
    Vec3::cross(upv, zaxis, &xaxis);
    xaxis.normalize();
    
    Vec3 yaxis;
    Vec3::cross(zaxis, xaxis, &yaxis);
    yaxis.normalize();
    Mat4  rotation;
    rotation.m[0] = xaxis.x;
    rotation.m[1] = xaxis.y;
    rotation.m[2] = xaxis.z;
    rotation.m[3] = 0;
    
    rotation.m[4] = yaxis.x;
    rotation.m[5] = yaxis.y;
    rotation.m[6] = yaxis.z;
    rotation.m[7] = 0;
    rotation.m[8] = zaxis.x;
    rotation.m[9] = zaxis.y;
    rotation.m[10] = zaxis.z;
    rotation.m[11] = 0;
    
    Quaternion  quaternion;
    Quaternion::createFromRotationMatrix(rotation,&quaternion);
    quaternion.normalize();
//    setRotationQuat(quaternion);
}

const Mat4& CACamera::getViewProjectionMatrix() const
{
    getViewMatrix();
    
    if (_viewProjectionDirty)
    {
        _viewProjectionDirty = false;
        Mat4::multiply(_projection, _view, &_viewProjection);
    }
    
    return _viewProjection;
}

void CACamera::setAdditionalProjection(const Mat4& mat)
{
    _projection = mat * _projection;
    getViewProjectionMatrix();
}

bool CACamera::initDefault()
{
    auto size = CAApplication::getApplication()->getWinSize();
    //create default CACamera
    auto projection = CAApplication::getApplication()->getProjection();
    switch (projection)
    {
        case CAApplication::Projection::P2D:
        {
            initOrthographic(size.width, size.height, -1024, 1024);
            this->setPointZ(0);
            this->setPoint(DPoint(0, size.height));
            setRotationX(0);
            setRotationY(0);
            setRotation(0);
            break;
        }
        case CAApplication::Projection::P3D:
        {
            float zeye = CAApplication::getApplication()->getZEye();
            initPerspective(60, (GLfloat)size.width / size.height, 10, zeye + size.height / 2.0f);
            Vec3 eye(size.width/2, size.height/2.0f, zeye), center(size.width/2, size.height/2, 0.0f), up(0.0f, 1.0f, 0.0f);
            this->setPointZ(eye.z);
            this->setPoint(DPoint(eye.x, eye.y));
            lookAt(center, up);
            break;
        }
        default:
            CCLOG("unrecognized projection");
            break;
    }
    return true;
}

bool CACamera::initPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
    _fieldOfView = fieldOfView;
    _aspectRatio = aspectRatio;
    _nearPlane = nearPlane;
    _farPlane = farPlane;
    Mat4::createPerspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane, &_projection);
    _viewProjectionDirty = true;
    _frustumDirty = true;
    
    return true;
}

bool CACamera::initOrthographic(float zoomX, float zoomY, float nearPlane, float farPlane)
{
    _zoom[0] = zoomX;
    _zoom[1] = zoomY;
    _nearPlane = nearPlane;
    _farPlane = farPlane;
    Mat4::createOrthographicOffCenter(0, _zoom[0], 0, _zoom[1], _nearPlane, _farPlane, &_projection);
    _viewProjectionDirty = true;
    _frustumDirty = true;
    
    return true;
}

Vec2 CACamera::project(const Vec3& src) const
{
    Vec2 screenPos;
    
    auto viewport = CAApplication::getApplication()->getWinSize();
    Vec4 clipPos;
    getViewProjectionMatrix().transformVector(Vec4(src.x, src.y, src.z, 1.0f), &clipPos);
    
    CCASSERT(clipPos.w != 0.0f, "clipPos.w can't be 0.0f!");
    float ndcX = clipPos.x / clipPos.w;
    float ndcY = clipPos.y / clipPos.w;
    
    screenPos.x = (ndcX + 1.0f) * 0.5f * viewport.width;
    screenPos.y = (1.0f - (ndcY + 1.0f) * 0.5f) * viewport.height;
    return screenPos;
}

Vec2 CACamera::projectGL(const Vec3& src) const
{
    Vec2 screenPos;
    
    auto viewport = CAApplication::getApplication()->getWinSize();
    Vec4 clipPos;
    getViewProjectionMatrix().transformVector(Vec4(src.x, src.y, src.z, 1.0f), &clipPos);
    
    float ndcX = (clipPos.w == 0) ? clipPos.x : (clipPos.x / clipPos.w);
    float ndcY = (clipPos.w == 0) ? clipPos.y : (clipPos.y / clipPos.w);
    
    screenPos.x = (ndcX + 1.0f) * 0.5f * viewport.width;
    screenPos.y = (ndcY + 1.0f) * 0.5f * viewport.height;
    return screenPos;
}

Vec3 CACamera::unproject(const Vec3& src) const
{
    Vec3 dst;
    unproject(CAApplication::getApplication()->getWinSize(), &src, &dst);
    return dst;
}

Vec3 CACamera::unprojectGL(const Vec3& src) const
{
    Vec3 dst;
    unprojectGL(CAApplication::getApplication()->getWinSize(), &src, &dst);
    return dst;
}

void CACamera::unproject(const DSize& viewport, const Vec3* src, Vec3* dst) const
{
    CCASSERT(src && dst, "vec3 can not be null");
    
    Vec4 screen(src->x / viewport.width, ((viewport.height - src->y)) / viewport.height, src->z, 1.0f);
    screen.x = screen.x * 2.0f - 1.0f;
    screen.y = screen.y * 2.0f - 1.0f;
    screen.z = screen.z * 2.0f - 1.0f;
    
    getViewProjectionMatrix().getInversed().transformVector(screen, &screen);
    if (screen.w != 0.0f)
    {
        screen.x /= screen.w;
        screen.y /= screen.w;
        screen.z /= screen.w;
    }
    
    dst->setPoint(screen.x, screen.y, screen.z);
}

void CACamera::unprojectGL(const DSize& viewport, const Vec3* src, Vec3* dst) const
{
    CCASSERT(src && dst, "vec3 can not be null");
    
    Vec4 screen(src->x / viewport.width, src->y / viewport.height, src->z, 1.0f);
    screen.x = screen.x * 2.0f - 1.0f;
    screen.y = screen.y * 2.0f - 1.0f;
    screen.z = screen.z * 2.0f - 1.0f;
    
    getViewProjectionMatrix().getInversed().transformVector(screen, &screen);
    if (screen.w != 0.0f)
    {
        screen.x /= screen.w;
        screen.y /= screen.w;
        screen.z /= screen.w;
    }
    
    dst->setPoint(screen.x, screen.y, screen.z);
}

float CACamera::getDepthInView(const Mat4& transform) const
{
    Mat4 camWorldMat = getViewToWorldTransform();
    const Mat4 &viewMat = camWorldMat.getInversed();
    float depth = -(viewMat.m[2] * transform.m[12] + viewMat.m[6] * transform.m[13] + viewMat.m[10] * transform.m[14] + viewMat.m[14]);
    return depth;
}

void CACamera::setDepth(int8_t depth)
{
    if (_depth != depth)
    {
        _depth = depth;
        if (_window)
        {
            //notify scene that the CACamera order is dirty
            _window->setCameraOrderDirty();
        }
    }
}

void CACamera::onEnter()
{
    if (_window == nullptr)
    {
        auto node = m_pSuperview;
        while (node->m_pSuperview)
        {
            node = node->m_pSuperview;
        }

        if (auto window = dynamic_cast<CAWindow*>(node))
        {
            this->setWindow(window);
        }
    }
    CAView::onEnter();
}

void CACamera::onExit()
{
    this->setWindow(nullptr);
    CAView::onExit();
}

void CACamera::setWindow(CAWindow* window)
{
	_window = window;
	if (window)
	{
		auto& cameras = _window->m_vCameras;
		auto it = std::find(cameras.begin(), cameras.end(), this);
		if (it == cameras.end())
		{
			_window->m_vCameras.push_back(this);
			//notify scene that the CACamera order is dirty
			_window->setCameraOrderDirty();
		}
	}
}

void CACamera::clearBackground()
{
    if (_clearBrush)
    {
        _clearBrush->drawBackground(this);
    }
}

void CACamera::setFrameBufferObject(experimental::FrameBuffer *fbo)
{
    CC_SAFE_RETAIN(fbo);
    CC_SAFE_RELEASE_NULL(_fbo);
    _fbo = fbo;
    if(_window)
    {
        _window->setCameraOrderDirty();
    }
}

void CACamera::apply()
{
    applyFrameBufferObject();
    applyViewport();
}

void CACamera::applyFrameBufferObject()
{
    if(nullptr == _fbo)
    {
        // inherit from context if it doesn't have a FBO
        // don't call apply the default one
        //        experimental::FrameBuffer::applyDefaultFBO();
    }
    else
    {
        _fbo->applyFBO();
    }
}

void CACamera::applyViewport()
{
    glGetIntegerv(GL_VIEWPORT, _oldViewport);
    
    if(nullptr == _fbo)
    {
        glViewport(getDefaultViewport()._left, getDefaultViewport()._bottom, getDefaultViewport()._width, getDefaultViewport()._height);
    }
    else
    {
        glViewport(_viewport._left * _fbo->getWidth(), _viewport._bottom * _fbo->getHeight(),
                   _viewport._width * _fbo->getWidth(), _viewport._height * _fbo->getHeight());
    }
}

void CACamera::setViewport(const experimental::Viewport& vp)
{
    _viewport = vp;
}

void CACamera::restore()
{
    restoreFrameBufferObject();
    restoreViewport();
}

void CACamera::restoreFrameBufferObject()
{
    if(nullptr == _fbo)
    {
        // it was inherited from context if it doesn't have a FBO
        // don't call restore the default one... just keep using the previous one
        //        experimental::FrameBuffer::applyDefaultFBO();
    }
    else
    {
        _fbo->restoreFBO();
    }
}

void CACamera::restoreViewport()
{
    glViewport(_oldViewport[0], _oldViewport[1], _oldViewport[2], _oldViewport[3]);
}

int CACamera::getRenderOrder() const
{
    int result(0);
    if(_fbo)
    {
        result = _fbo->getFID()<<8;
    }
    else
    {
        result = 127 <<8;
    }
    result += _depth;
    return result;
}

void CACamera::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    _viewProjectionUpdated = m_bTransformUpdated;
    return CAView::visit(renderer, parentTransform, parentFlags);
}

void CACamera::setBackgroundBrush(CACameraBackgroundBrush* clearBrush)
{
    CC_SAFE_RETAIN(clearBrush);
    CC_SAFE_RELEASE(_clearBrush);
    _clearBrush = clearBrush;
}

bool CACamera::isBrushValid()
{
    return _clearBrush != nullptr && _clearBrush->isValid();
}

NS_CC_END
