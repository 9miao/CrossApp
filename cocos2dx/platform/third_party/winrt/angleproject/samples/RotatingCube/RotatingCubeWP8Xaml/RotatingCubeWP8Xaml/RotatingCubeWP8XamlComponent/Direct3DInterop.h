#pragma once

#include "pch.h"
#include "BasicTimer.h"
#include "CubeRenderer.h"

namespace PhoneDirect3DXamlAppComponent
{

[Windows::Foundation::Metadata::WebHostHidden]
public ref class Direct3DInterop sealed : public Windows::Phone::Input::Interop::IDrawingSurfaceManipulationHandler
{
public:
    Direct3DInterop();

    Windows::Phone::Graphics::Interop::IDrawingSurfaceContentProvider^ CreateContentProvider();

    // IDrawingSurfaceManipulationHandler
    virtual void SetManipulationHost(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ manipulationHost);

    void UpdateForWindowSizeChange(float width, float height);
    void OnFocusChange(bool active);
    void OnResuming();
    Windows::Foundation::IAsyncAction^ OnSuspending();
    bool IsBackKeyHandled();

    property Windows::Graphics::Display::DisplayOrientations WindowOrientation;

    property Windows::Foundation::Size NativeResolution;
    property Windows::Foundation::Size RenderResolution;

protected:
    // Event Handlers
    void OnPointerPressed(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);
    void OnPointerMoved(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);
    void OnPointerReleased(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);

internal:
    void Connect();
    void Disconnect();
    void PrepareResources(LARGE_INTEGER presentTargetTime);
    void Draw(_In_ ID3D11Device1* device, _In_ ID3D11DeviceContext1* context, _In_ ID3D11RenderTargetView* renderTargetView);

private:
    CubeRenderer^ m_renderer;
    BasicTimer^ m_timer;
    Windows::Graphics::Display::DisplayOrientations mCurrentOrientation;
};

}
