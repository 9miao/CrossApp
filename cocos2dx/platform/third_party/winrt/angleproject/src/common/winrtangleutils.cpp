#include "winrtangleutils.h"
#include "winrtutils.h"

using namespace Microsoft::WRL;

namespace winrtangleutils
{

bool hasIPhoneXamlWindow(ComPtr<IWinrtEglWindow> iWindow)
{
	ComPtr<IWinPhone8XamlD3DWindow> iPhoneXamlWindow;
	ComPtr<IUnknown> iWindowInterface = iWindow.Get()->GetWindowInterface();
	return iWindowInterface == nullptr ? false : iWindowInterface.As(&iPhoneXamlWindow) == S_OK;
}

HRESULT getIWinRTWindow(ComPtr<IUnknown> iWindow, ComPtr<IWinrtEglWindow> *iWinRTWindow)
{
	return iWindow.As(iWinRTWindow);
}

HRESULT getIPhoneXamlWindow(ComPtr<IUnknown> iWindow, ComPtr<IWinPhone8XamlD3DWindow> *iPhoneXamlWindow)
{
	ComPtr<IWinrtEglWindow> iWinRTWindow;
	HRESULT result = getIWinRTWindow(iWindow, &iWinRTWindow);
    if (SUCCEEDED(result))
	{
		ComPtr<IUnknown> iWindowInterface = iWinRTWindow.Get()->GetWindowInterface();
		result = iWindowInterface.Get() == nullptr? S_FALSE : iWindowInterface.As(iPhoneXamlWindow);
	}
	return result;
}

HRESULT getID3D11Device(ComPtr<IWinPhone8XamlD3DWindow> iWindow, ComPtr<ID3D11Device>* idevice)
{
	return iWindow.Get()->GetDevice().As(idevice);
}

HRESULT getID3D11DeviceContext(ComPtr<IWinPhone8XamlD3DWindow> iWindow, ComPtr<ID3D11DeviceContext>* iContext)
{
	return iWindow.Get()->GetContext().As(iContext);
}

HRESULT getID3D11RenderTargetView(ComPtr<IWinPhone8XamlD3DWindow> iWindow, ComPtr<ID3D11RenderTargetView>* iRtv)
{
	return iWindow.Get()->GetRenderTarget().As(iRtv);
}


HRESULT getBackBuffer(ComPtr<IWinPhone8XamlD3DWindow> iWindow, ComPtr<ID3D11Texture2D>* backBuffer)
{
	ComPtr<ID3D11RenderTargetView> rtv;
	HRESULT result = getID3D11RenderTargetView(iWindow, &rtv) ;
	if (SUCCEEDED(result))
	{
		ComPtr<ID3D11Resource> renderTargetViewResource;
		rtv->GetResource(&renderTargetViewResource);
		result = renderTargetViewResource.As(backBuffer);
	}

	return result;
}




HRESULT getWindowSize(ComPtr<IUnknown> iWindow, int& width, int& height)
{
	ComPtr<IWinPhone8XamlD3DWindow> iPhoneWindow;
	HRESULT result = getIPhoneXamlWindow(iWindow, &iPhoneWindow);
  
    if (SUCCEEDED(result))
    {
		ComPtr<ID3D11Texture2D> backBuffer;
		result = getBackBuffer(iPhoneWindow, &backBuffer);

		if(SUCCEEDED(result))
		{
			D3D11_TEXTURE2D_DESC backBufferDesc;
			backBuffer->GetDesc(&backBufferDesc);
			width = backBufferDesc.Width;
			height = backBufferDesc.Height;
		}
    }
	else
	{
		 winrt::getCurrentWindowDimensions(width, height);
	}

	return result;
}

D3D_FEATURE_LEVEL getD3DFeatureLevel(ComPtr<IWinrtEglWindow> iWindow)
{
	D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;

	switch(iWindow->GetAngleD3DFeatureLevel())
	{
		case ANGLE_D3D_FEATURE_LEVEL_ANY:
		case ANGLE_D3D_FEATURE_LEVEL_11_0:
			level = D3D_FEATURE_LEVEL_11_0;
			break;
		case ANGLE_D3D_FEATURE_LEVEL_10_1:
			level = D3D_FEATURE_LEVEL_10_1;
			break;
		case ANGLE_D3D_FEATURE_LEVEL_10_0:
			level = D3D_FEATURE_LEVEL_10_0;
			break;
		case ANGLE_D3D_FEATURE_LEVEL_9_3:
			level = D3D_FEATURE_LEVEL_9_3;
			break;
		case ANGLE_D3D_FEATURE_LEVEL_9_2:
			level = D3D_FEATURE_LEVEL_9_2;
			break;		
		case ANGLE_D3D_FEATURE_LEVEL_9_1:
			level = D3D_FEATURE_LEVEL_9_1;
			break;
	}

	return level;
}

} //namespace winrtangleutils 
