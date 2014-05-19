//
// Copyright (c) 2002-2013 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// winrtangle.h: Defines the interface for the EGLNativeWindowType

#ifndef WINRT_ANGLE_H_
#define WINRT_ANGLE_H_

#include <unknwn.h>
#include <wrl.h>

typedef enum {
    ANGLE_D3D_FEATURE_LEVEL_ANY = 0,
    ANGLE_D3D_FEATURE_LEVEL_9_1,
    ANGLE_D3D_FEATURE_LEVEL_9_2,
    ANGLE_D3D_FEATURE_LEVEL_9_3,
    ANGLE_D3D_FEATURE_LEVEL_10_0,
    ANGLE_D3D_FEATURE_LEVEL_10_1,
    ANGLE_D3D_FEATURE_LEVEL_11_0
} ANGLE_D3D_FEATURE_LEVEL;

struct __declspec(uuid("736E7573-FD24-433F-811E-893B36B7A078")) IWinrtEglWindow : IUnknown
{
    virtual ANGLE_D3D_FEATURE_LEVEL __stdcall GetAngleD3DFeatureLevel() = 0;
    virtual Microsoft::WRL::ComPtr<IUnknown> __stdcall GetAngleD3DDevice() = 0;
    virtual void __stdcall SetAngleD3DDevice(Microsoft::WRL::ComPtr<IUnknown>) = 0;
    virtual Microsoft::WRL::ComPtr<IUnknown> __stdcall GetWindowInterface() = 0;
};

struct __declspec(uuid("6B70903A-0D55-45BA-A10E-CA2D428E4867")) IWinPhone8XamlD3DWindow : IUnknown
{
    virtual Microsoft::WRL::ComPtr<IUnknown> __stdcall GetDevice() = 0;
    virtual Microsoft::WRL::ComPtr<IUnknown> __stdcall GetContext() = 0;
    virtual Microsoft::WRL::ComPtr<IUnknown> __stdcall GetRenderTarget() = 0;
    virtual void __stdcall Update(Microsoft::WRL::ComPtr<IUnknown> device,
                        Microsoft::WRL::ComPtr<IUnknown> context,
                        Microsoft::WRL::ComPtr<IUnknown> renderTarget) = 0;
};


HRESULT __stdcall CreateWinrtEglWindow(Microsoft::WRL::ComPtr<IUnknown> windowInterface, ANGLE_D3D_FEATURE_LEVEL featureLevel, IWinrtEglWindow ** result);
HRESULT __stdcall CreateWinPhone8XamlWindow(IWinPhone8XamlD3DWindow ** result);

#endif   // WINRT_ANGLE_H_

