#include "pch.h"
#include "RotatingCube.h"
#include "BasicTimer.h"

#define STRINGIFY(x) #x

const char g_colorVertexShader[] = STRINGIFY(
precision mediump float;
attribute vec3 a_position;
attribute vec3 a_color;
varying vec4 v_color;
uniform mat4 u_mvp;
void main(void)
{
    gl_Position = u_mvp * vec4(a_position, 1);
    v_color = vec4(a_color, 1);
}
);

const char g_colorFragmentShader[] = STRINGIFY(
precision mediump float;
varying vec4 v_color;
void main(void)
{
    gl_FragColor = v_color;
}
);

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace concurrency;

struct VertexPositionColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};

RotatingCube::RotatingCube() :
	m_windowClosed(false),
	m_windowVisible(true)
{
}

void RotatingCube::Initialize(CoreApplicationView^ applicationView)
{
	applicationView->Activated +=
        ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &RotatingCube::OnActivated);

	CoreApplication::Suspending +=
        ref new EventHandler<SuspendingEventArgs^>(this, &RotatingCube::OnSuspending);

	CoreApplication::Resuming +=
        ref new EventHandler<Platform::Object^>(this, &RotatingCube::OnResuming);
}

void RotatingCube::SetWindow(CoreWindow^ window)
{
	window->SizeChanged += 
        ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &RotatingCube::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &RotatingCube::OnVisibilityChanged);

	window->Closed += 
        ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &RotatingCube::OnWindowClosed);

#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_PHONE)
	window->PointerCursor = ref new CoreCursor(CoreCursorType::Arrow, 0);
#endif

	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &RotatingCube::OnPointerPressed);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &RotatingCube::OnPointerMoved);

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_PHONE)
	DisplayProperties::AutoRotationPreferences = 
		DisplayOrientations::Landscape | 
		DisplayOrientations::LandscapeFlipped | 
		DisplayOrientations::Portrait | 
		DisplayOrientations::PortraitFlipped;
	DisplayProperties::OrientationChanged +=
		ref new DisplayPropertiesEventHandler(this, &RotatingCube::OnOrientationChanged);
#endif

	m_orientation = DisplayProperties::CurrentOrientation;
    m_windowBounds = window->Bounds;

    esInitContext ( &m_esContext );
    m_esContext.hWnd = WINRT_EGL_WINDOW(CoreWindow::GetForCurrentThread());

    //title, width, and height are unused, but included for backwards compatibility
    esCreateWindow ( &m_esContext, nullptr, 0, 0, ES_WINDOW_RGB | ES_WINDOW_DEPTH );

    m_cubeRenderer.CreateResources();
}

void RotatingCube::Load(Platform::String^ entryPoint)
{
}

void RotatingCube::Run()
{
	BasicTimer^ timer = ref new BasicTimer();

	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			timer->Update();
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
            m_cubeRenderer.Update(timer->Total, timer->Delta);
            m_cubeRenderer.Render();
            eglSwapBuffers(m_esContext.eglDisplay, m_esContext.eglSurface);
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void RotatingCube::Uninitialize()
{
}

void RotatingCube::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
    m_cubeRenderer.UpdateForWindowSizeChanged();
}

void RotatingCube::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void RotatingCube::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

void RotatingCube::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	// Insert your code here.
}

void RotatingCube::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	// Insert your code here.
}

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_PHONE)
void RotatingCube::OnOrientationChanged(Platform::Object^ sender)
{
	m_cubeRenderer.OnOrientationChanged();
}
#endif

void RotatingCube::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	CoreWindow::GetForCurrentThread()->Activate();
}

void RotatingCube::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		// Insert your code here.

		deferral->Complete();
	});
}
 
void RotatingCube::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
    return ref new RotatingCube();
}

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource);
	return 0;
}
