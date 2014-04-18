//
// Copyright (c) 2002-2013 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// Display.h: Defines the egl::Display class, representing the abstract
// display on which graphics are drawn. Implements EGLDisplay.
// [EGL 1.4] section 2.1.2 page 3.

#ifndef LIBEGL_DISPLAY_H_
#define LIBEGL_DISPLAY_H_

#include "common/system.h"

#include <set>
#include <vector>

#include "libEGL/Config.h"

namespace gl
{
class Context;
}

namespace egl
{
class Surface;

class Display
{
  public:
    ~Display();

    bool initialize();
    void terminate();

    static egl::Display *getDisplay(EGLNativeDisplayType displayId);

    bool getConfigs(EGLConfig *configs, const EGLint *attribList, EGLint configSize, EGLint *numConfig);
    bool getConfigAttrib(EGLConfig config, EGLint attribute, EGLint *value);

    EGLSurface createWindowSurface(EGLNativeWindowType window, EGLConfig config, const EGLint *attribList);
    EGLSurface createOffscreenSurface(EGLConfig config, HANDLE shareHandle, const EGLint *attribList);
    EGLContext createContext(EGLConfig configHandle, const gl::Context *shareContext, bool notifyResets, bool robustAccess);

    void destroySurface(egl::Surface *surface);
    void destroyContext(gl::Context *context);

    bool isInitialized() const;
    bool isValidConfig(EGLConfig config);
    bool isValidContext(gl::Context *context);
    bool isValidSurface(egl::Surface *surface);
    bool hasExistingWindowSurface(EGLNativeWindowType window);

    rx::Renderer *getRenderer() { return mRenderer; };

    // exported methods must be virtual
    virtual void notifyDeviceLost();
    virtual void recreateSwapChains();

    const char *getExtensionString() const;
    const char *getVendorString() const;

  private:
    DISALLOW_COPY_AND_ASSIGN(Display);

    Display(EGLNativeDisplayType displayId, HDC deviceContext);

    bool restoreLostDevice();

#if defined(ANGLE_PLATFORM_WINRT)
#if 0
    void onWindowSizeChanged(ABI::Windows::UI::Core::CoreWindow* sender, ABI::Windows::UI::Core::WindowSizeChangedEventArgs^ args, Surface *surface);

    class DisplayRT
    {
      internal:
        DisplayRT(Display *display, Surface *surface);
        void onWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
        
        Display *mDisplay;
        Surface *mSurface;
    };
    
    DisplayRT ^mDisplayRT;
	ABI::Windows::Graphics::Display::DisplayOrientations m_orientation;

	ABI::Windows::Foundation::Rect m_windowBounds;
#endif
#endif // #if defined(ANGLE_PLATFORM_WINRT)

    EGLNativeDisplayType mDisplayId;
    const HDC mDc;

    bool mSoftwareDevice;
    
    typedef std::set<Surface*> SurfaceSet;
    SurfaceSet mSurfaceSet;

    ConfigSet mConfigSet;

    typedef std::set<gl::Context*> ContextSet;
    ContextSet mContextSet;

    rx::Renderer *mRenderer;

    void initExtensionString();
    void initVendorString();
    std::string mExtensionString;
    std::string mVendorString;
};
}

#endif   // LIBEGL_DISPLAY_H_
