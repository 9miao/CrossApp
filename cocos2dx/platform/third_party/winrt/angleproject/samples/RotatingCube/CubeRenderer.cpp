#include "pch.h"
#include "CubeRenderer.h"
#include "shader.h"

struct VertexPositionColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};

void CubeRenderer::CreateResources()
{
    m_windowBounds = Windows::UI::Core::CoreWindow::GetForCurrentThread()->Bounds;
    m_colorProgram = glCreateProgram();
    glProgramBinaryOES(m_colorProgram, GL_PROGRAM_BINARY_ANGLE, gProgram, sizeof(gProgram));
    a_positionColor = glGetAttribLocation(m_colorProgram, "a_position");
    a_colorColor = glGetAttribLocation(m_colorProgram, "a_color");
    u_mvpColor = glGetUniformLocation(m_colorProgram, "u_mvp");

    m_projectionMatrix = XMMatrixPerspectiveFovRH(70.0f * XM_PI / 180.0f, m_windowBounds.Width / m_windowBounds.Height, 0.01f, 100.0f);

    glEnable(GL_DEPTH_TEST);
}

static float ConvertDipsToPixels(float dips)
{
    static const float dipsPerInch = 96.0f;
    return floor(dips * Windows::Graphics::Display::DisplayProperties::LogicalDpi / dipsPerInch + 0.5f); // Round to nearest integer.
}

void CubeRenderer::UpdateForWindowSizeChanged()
{
    Windows::Foundation::Rect windowBounds = Windows::UI::Core::CoreWindow::GetForCurrentThread()->Bounds;
    if (windowBounds.Width  != m_windowBounds.Width ||
        windowBounds.Height != m_windowBounds.Height ||
        m_orientation != Windows::Graphics::Display::DisplayProperties::CurrentOrientation)
    {
        // Store the window bounds so the next time we get a SizeChanged event we can
        // avoid rebuilding everything if the size is identical.
        m_windowBounds = windowBounds;

        // Calculate the necessary swap chain and render target size in pixels.
        float windowWidth = ConvertDipsToPixels(m_windowBounds.Width);
        float windowHeight = ConvertDipsToPixels(m_windowBounds.Height);

        // The width and height of the swap chain must be based on the window's
        // landscape-oriented width and height. If the window is in a portrait
        // orientation, the dimensions must be reversed.
        m_orientation = Windows::Graphics::Display::DisplayProperties::CurrentOrientation;

        glViewport(0, 0, static_cast<UINT>(windowWidth), static_cast<UINT>(windowHeight));
        m_projectionMatrix = XMMatrixPerspectiveFovRH(70.0f * XM_PI / 180.0f, windowWidth / windowHeight, 0.01f, 100.0f);
    }
}

void CubeRenderer::Render()
{
    XMFLOAT4X4 mvp;
    XMStoreFloat4x4(&mvp, (XMMatrixMultiply(XMMatrixMultiply(m_modelMatrix, m_viewMatrix), m_projectionMatrix)));

    glClearColor(0.098f, 0.098f, 0.439f, 1.000f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_colorProgram);
    glUniformMatrix4fv(u_mvpColor, 1, GL_FALSE, &mvp.m[0][0]);

    VertexPositionColor cubeVertices[] = 
	{
		{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
		{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
		{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
		{XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
		{XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
		{XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
		{XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
	};
    unsigned short cubeIndices[] = 
    {

        0,2,1, // -x
        1,2,3,

        4,5,6, // +x
        5,7,6,

        0,1,5, // -y
        0,5,4,

        2,6,7, // +y
        2,7,3,

        0,4,6, // -z
        0,6,2,

        1,3,7, // +z
        1,7,5,
    };

    glEnableVertexAttribArray(a_positionColor);
    glEnableVertexAttribArray(a_colorColor);
    glVertexAttribPointer(a_positionColor, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), cubeVertices);
    glVertexAttribPointer(a_colorColor, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), reinterpret_cast<char*>(cubeVertices) + sizeof(XMFLOAT3));
    glDrawElements(GL_TRIANGLES, ARRAYSIZE(cubeIndices), GL_UNSIGNED_SHORT, cubeIndices);
    glDisableVertexAttribArray(a_positionColor);
    glDisableVertexAttribArray(a_colorColor);
}

void CubeRenderer::Update(float timeTotal, float /*timeDelta*/)
{
    XMVECTOR eye = XMVectorSet(0.0f, 0.7f, 1.5f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
            
	m_viewMatrix = XMMatrixLookAtRH(eye, at, up);
	m_modelMatrix = XMMatrixRotationY(timeTotal * XM_PIDIV4);
}

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_PHONE)
void CubeRenderer::OnOrientationChanged()
{
	m_windowBounds = Windows::UI::Core::CoreWindow::GetForCurrentThread()->Bounds;
	float aspectRatio = m_windowBounds.Width / m_windowBounds.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	XMMATRIX orientationCorrection;
	DXGI_MODE_ROTATION rotation;

	switch(Windows::Graphics::Display::DisplayProperties::CurrentOrientation)
	{
		case Windows::Graphics::Display::DisplayOrientations::Portrait:
			orientationCorrection = XMMatrixIdentity();
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case Windows::Graphics::Display::DisplayOrientations::PortraitFlipped:
			orientationCorrection = XMMatrixRotationZ(XM_PI);
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;

		case Windows::Graphics::Display::DisplayOrientations::Landscape:
			orientationCorrection = XMMatrixRotationZ(-XM_PIDIV2);
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;
			
		case Windows::Graphics::Display::DisplayOrientations::LandscapeFlipped:
			orientationCorrection = XMMatrixRotationZ(XM_PIDIV2);
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;
	}
    m_projectionMatrix = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.01f, 100.0f);
	m_projectionMatrix = XMMatrixMultiply(orientationCorrection, m_projectionMatrix);
}
#endif
