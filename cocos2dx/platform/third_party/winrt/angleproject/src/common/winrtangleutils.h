//
// Copyright (c) 2002-2013 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// winrtangleutils.h: utility functions for handling Angle WinRT types

#ifndef WINRT_ANGLE_UTILS_H_
#define WINRT_ANGLE_UTILS_H_

#include "common/winrtangle.h"

#include <unknwn.h>
#include <wrl.h>
#include <D3D11_1.h>

namespace winrtangleutils
{

HRESULT getIWinRTWindow(Microsoft::WRL::ComPtr<IUnknown> iWindow, Microsoft::WRL::ComPtr<IWinrtEglWindow> *iWinRTWindow);
HRESULT getIPhoneXamlWindow(Microsoft::WRL::ComPtr<IUnknown> iWindow, Microsoft::WRL::ComPtr<IWinPhone8XamlD3DWindow> *iPhoneXamlWindow);
HRESULT getWindowSize(Microsoft::WRL::ComPtr<IUnknown> iWindow, int& width, int& height);

HRESULT getID3D11Device(Microsoft::WRL::ComPtr<IWinPhone8XamlD3DWindow> iWindow, Microsoft::WRL::ComPtr<ID3D11Device>* idevice);
HRESULT getID3D11DeviceContext(Microsoft::WRL::ComPtr<IWinPhone8XamlD3DWindow> iWindow, Microsoft::WRL::ComPtr<ID3D11DeviceContext>* iContext);
HRESULT getID3D11RenderTargetView(Microsoft::WRL::ComPtr<IWinPhone8XamlD3DWindow> iWindow, Microsoft::WRL::ComPtr<ID3D11RenderTargetView>* iRtv);

HRESULT getBackBuffer(Microsoft::WRL::ComPtr<IWinPhone8XamlD3DWindow> iWindow, Microsoft::WRL::ComPtr<ID3D11Texture2D>* backBuffer);

D3D_FEATURE_LEVEL getD3DFeatureLevel(Microsoft::WRL::ComPtr<IWinrtEglWindow> iWindow);

bool hasIPhoneXamlWindow(Microsoft::WRL::ComPtr<IWinrtEglWindow> iWindow);

} //namespace winrtangleutils 


#endif   // WINRT_ANGLE_UTILS_H_

