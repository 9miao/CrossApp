#include "precompiled.h"

#include "winrtangle.h"
#include <windows.h>
#include <crtdbg.h>

using namespace Microsoft::WRL;

#define TRACE OutputDebugString
#define ASSERT _ASSERTE

class WinPhone8XamlD3DWindow : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IWinPhone8XamlD3DWindow>
{
public:
    WinPhone8XamlD3DWindow() 
        : m_device(nullptr)
        , m_context(nullptr)
        , m_renderTarget(nullptr)
    {
        TRACE((LPCWSTR)"WinPhone8XamlD3DWindow()\n");
    }

    ~WinPhone8XamlD3DWindow()
    {
        TRACE((LPCWSTR)"~WinPhone8XamlD3DWindow()\n");
    }

    //
    // IWinPhone8XamlD3DWindow
    //

    virtual ComPtr<IUnknown> __stdcall GetDevice() override
    {
        return m_device;
    }

    virtual ComPtr<IUnknown> __stdcall GetContext() override
    {
        return m_context;
    }

    virtual ComPtr<IUnknown> __stdcall GetRenderTarget() override
    {
        return m_renderTarget;
    }

    virtual void __stdcall Update(ComPtr<IUnknown> device, ComPtr<IUnknown> context, ComPtr<IUnknown> renderTarget) override
    {
        m_device = device;
        m_context = context;
        m_renderTarget = renderTarget;
    }
    
private:
    ComPtr<IUnknown> m_device;
    ComPtr<IUnknown> m_context;
    ComPtr<IUnknown> m_renderTarget;
};

HRESULT __stdcall CreateWinPhone8XamlWindow(IWinPhone8XamlD3DWindow ** result)
{
    ASSERT(result);
    *result = nullptr;

    ComPtr<IWinPhone8XamlD3DWindow> iWindow = Make<WinPhone8XamlD3DWindow>();

    if (!iWindow)
    {
        return E_OUTOFMEMORY;
    }

    *result = iWindow.Detach();
    return S_OK;
}

class WinrtEglWindow : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IWinrtEglWindow>
{
public:
    WinrtEglWindow(ComPtr<IUnknown> windowInterface, ANGLE_D3D_FEATURE_LEVEL featureLevel) 
        : m_windowInterface(windowInterface)
        , m_featureLevel(featureLevel)
		, m_angleD3DDevice(nullptr)
    {
        TRACE((LPCWSTR)"WinrtEglWindow()\n");
    }

    ~WinrtEglWindow()
    {
        TRACE((LPCWSTR)"~WinrtEglWindow()\n");
    }

    //
    // IWinrtEglWindow
    //

    virtual ANGLE_D3D_FEATURE_LEVEL __stdcall GetAngleD3DFeatureLevel() override
    {
        return m_featureLevel;
    }

    virtual ComPtr<IUnknown> __stdcall GetWindowInterface() override
    {
        return m_windowInterface;
    }

    virtual Microsoft::WRL::ComPtr<IUnknown> __stdcall GetAngleD3DDevice() override
    {
		return m_angleD3DDevice;
    }
 
    virtual void __stdcall SetAngleD3DDevice(Microsoft::WRL::ComPtr<IUnknown> device) override
    {
		m_angleD3DDevice = device;
    }

private:
    ANGLE_D3D_FEATURE_LEVEL m_featureLevel;
    ComPtr<IUnknown> m_windowInterface;
    ComPtr<IUnknown> m_angleD3DDevice;
};

HRESULT __stdcall CreateWinrtEglWindow(ComPtr<IUnknown> windowInterface, ANGLE_D3D_FEATURE_LEVEL featureLevel, IWinrtEglWindow ** result)
{
    ASSERT(result);
    *result = nullptr;

    ComPtr<IWinrtEglWindow> iWindow = Make<WinrtEglWindow>(windowInterface, featureLevel);

    if (!iWindow)
    {
        return E_OUTOFMEMORY;
    }

    *result = iWindow.Detach();
    return S_OK;
}


