

#include "CACamera.h"
#include "CCGL.h"
#include "view/CADrawingPrimitives.h"
#include "CAApplication.h"
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN

CACamera::CACamera(void)
{
    init();
}

CACamera::~CACamera(void)
{

}

const char* CACamera::description(void)
{
    return crossapp_format_string("<CACamera | center = (%s, %s, %s)>", m_fCenterX, m_fCenterY, m_fCenterZ).c_str();
}

void CACamera::init(void)
{
    restore();
}

void CACamera::restore(void)
{
    m_fEyeX = m_fEyeY = 0.0f;
    m_fEyeZ = getZEye();

    m_fCenterX = m_fCenterY = m_fCenterZ = 0.0f;

    m_fUpX = 0.0f;
    m_fUpY = 1.0f;
    m_fUpZ = 0.0f;

    kmMat4Identity( &m_lookupMatrix );

    m_bDirty = false;
}

void CACamera::locate(void)
{
    if (m_bDirty)
    {
        kmVec3 eye, center, up;

        kmVec3Fill( &eye, m_fEyeX, m_fEyeY , m_fEyeZ );
        kmVec3Fill( &center, m_fCenterX, m_fCenterY, m_fCenterZ );

        kmVec3Fill( &up, m_fUpX, m_fUpY, m_fUpZ);
        kmMat4LookAt( &m_lookupMatrix, &eye, &center, &up);

        m_bDirty = false;
    }
    kmGLMultMatrix( &m_lookupMatrix );
}

float CACamera::getZEye(void)
{
    return FLT_EPSILON;
}

void CACamera::setEyeXYZ(float fEyeX, float fEyeY, float fEyeZ)
{
    m_fEyeX = fEyeX;
    m_fEyeY = fEyeY;
    m_fEyeZ = fEyeZ;

    m_bDirty = true;
}

void CACamera::setCenterXYZ(float fCenterX, float fCenterY, float fCenterZ)
{
    m_fCenterX = fCenterX;
    m_fCenterY = fCenterY;
    m_fCenterZ = fCenterZ;

    m_bDirty = true;
}

void CACamera::setUpXYZ(float fUpX, float fUpY, float fUpZ)
{
    m_fUpX = fUpX;
    m_fUpY = fUpY;
    m_fUpZ = fUpZ;

    m_bDirty = true;
}

void CACamera::getEyeXYZ(float *pEyeX, float *pEyeY, float *pEyeZ)
{
    *pEyeX = m_fEyeX;
    *pEyeY = m_fEyeY;
    *pEyeZ = m_fEyeZ;
}

void CACamera::getCenterXYZ(float *pCenterX, float *pCenterY, float *pCenterZ)
{
    *pCenterX = m_fCenterX;
    *pCenterY = m_fCenterY;
    *pCenterZ = m_fCenterZ;
}

void CACamera::getUpXYZ(float *pUpX, float *pUpY, float *pUpZ)
{
    *pUpX = m_fUpX;
    *pUpY = m_fUpY;
    *pUpZ = m_fUpZ;
}

NS_CC_END

