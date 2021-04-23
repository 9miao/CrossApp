#ifndef _CCCAMERA_H__
#define _CCCAMERA_H__

#include "renderer/CCQuadCommand.h"
#include "renderer/CCCustomCommand.h"
#include "renderer/CCFrameBuffer.h"
#include "view/CAWindow.h"
NS_CC_BEGIN

class CACameraBackgroundBrush;

enum class CACameraFlag
{
    DEFAULT = 1,
    USER1 = 1 << 1,
    USER2 = 1 << 2,
    USER3 = 1 << 3,
    USER5 = 1 << 5,
    USER4 = 1 << 4,
    USER6 = 1 << 6,
    USER7 = 1 << 7,
    USER8 = 1 << 8,
};

class CC_DLL CACamera : public CAView
{
    friend class CAApplication;
    friend class CAWindow;
public:

    enum class Type
    {
        PERSPECTIVE = 1,
        ORTHOGRAPHIC = 2
    };
public:

    static CACamera* createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

    static CACamera* createOrthographic(float zoomX, float zoomY, float nearPlane, float farPlane);

    static CACamera* create();

    static const CACamera* getVisitingCamera();
    
    static const experimental::Viewport& getDefaultViewport();
    static void setDefaultViewport(const experimental::Viewport& vp);

    static CACamera* getDefaultCamera();

    CACamera::Type getType() const { return _type; }

    CACameraFlag getCameraFlag() const { return (CACameraFlag)_cameraFlag; }
    void setCameraFlag(CACameraFlag flag) { _cameraFlag = (unsigned short)flag; }

    virtual void lookAt(const Vec3& target, const Vec3& up = DPoint3DUNIT_Y);

    const Mat4& getProjectionMatrix() const;

    const Mat4& getViewMatrix() const;

    const Mat4& getViewProjectionMatrix() const;

    Vec2 project(const Vec3& src) const;

    Vec2 projectGL(const Vec3& src) const;

    Vec3 unproject(const Vec3& src) const;

    Vec3 unprojectGL(const Vec3& src) const;

    void unproject(const DSize& size, const Vec3* src, Vec3* dst) const;

    void unprojectGL(const DSize& size, const Vec3* src, Vec3* dst) const;

    float getDepthInView(const Mat4& transform) const;

    void setDepth(int8_t depth);

    int8_t getDepth() const { return _depth; }

    int getRenderOrder() const;

    float getFarPlane() const { return _farPlane; }

    float getNearPlane() const { return _nearPlane; }

    virtual void onEnter() override;
    virtual void onExit() override;

    void clearBackground();

    void apply();

    void restore();

    void setFrameBufferObject(experimental::FrameBuffer* fbo);

    void setViewport(const experimental::Viewport& vp);

    bool isViewProjectionUpdated() const {return _viewProjectionUpdated;}

    void setBackgroundBrush(CACameraBackgroundBrush* clearBrush);

    CACameraBackgroundBrush* getBackgroundBrush() const { return _clearBrush; }
    
    virtual void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags) override {};
    
    bool isBrushValid();
    
    CACamera();
    ~CACamera();

protected:
    
    void setWindow(CAWindow* window);
    void setAdditionalProjection(const Mat4& mat);

    bool initDefault();
    bool initPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
    bool initOrthographic(float zoomX, float zoomY, float nearPlane, float farPlane);
    void applyFrameBufferObject();
    void applyViewport();
    void restoreFrameBufferObject();
    void restoreViewport();
    
protected:
    static CACamera* _visitingCamera;
    static experimental::Viewport _defaultViewport;
    
    CAWindow* _window;
    Mat4 _projection;
    mutable Mat4 _view;
    mutable Mat4 _viewInv;
    mutable Mat4 _viewProjection;
    
    Vec3 _up;
    CACamera::Type _type;
    float _fieldOfView;
    float _zoom[2];
    float _aspectRatio;
    float _nearPlane;
    float _farPlane;
    mutable bool  _viewProjectionDirty;
    bool _viewProjectionUpdated; //Whether or not the viewprojection matrix was updated since the last frame.
    unsigned short _cameraFlag; // CACamera flag
    //mutable Frustum _frustum;   // CACamera frustum
    mutable bool _frustumDirty;
    int8_t  _depth;                 //camera depth, the depth of CACamera with CACameraFlag::DEFAULT flag is 0 by default, a CACamera with larger depth is drawn on top of CACamera with smaller depth
    
    CACameraBackgroundBrush* _clearBrush; //brush used to clear the back ground
    
    experimental::Viewport _viewport;
    experimental::FrameBuffer* _fbo;
    GLint _oldViewport[4];
};

NS_CC_END

#endif// __CCCAMERA_H_
