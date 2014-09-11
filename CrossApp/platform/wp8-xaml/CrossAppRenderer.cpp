#include "CrossAppRenderer.h"
#include "CrossApp.h"
#include "CCApplication.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include <ppltasks.h>

using namespace Concurrency;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace PhoneDirect3DXamlAppComponent;

USING_NS_CC;

CrossAppRenderer::CrossAppRenderer(): mInitialized(false), m_loadingComplete(false), m_delegate(nullptr), m_messageBoxDelegate(nullptr)
{
    mApp = new AppDelegate();
}

// Creates and restores Cocos2d-x after DirectX and Angle contexts are created or updated
void CrossAppRenderer::CreateGLResources()
{
    if(!mInitialized)
    {
        mInitialized = true;
        CCEGLView* pEGLView = new CCEGLView();
	    pEGLView->Create(m_eglDisplay, m_eglContext, m_eglSurface, m_renderTargetSize.Width, m_renderTargetSize.Height);
        pEGLView->setViewName("CrossApp");
        CCApplication::sharedApplication()->run();
        pEGLView->SetXamlEventDelegate(m_delegate);
        pEGLView->SetXamlMessageBoxDelegate(m_messageBoxDelegate);
        pEGLView->SetXamlEditBoxDelegate(m_editBoxDelegate);
   }
    else
    {
        ccGLInvalidateStateCache();
        CAShaderCache::sharedShaderCache()->reloadDefaultShaders();
        ccDrawInit();
        CAImageCache::sharedImageCache()->reloadAllImages();
        CANotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_FOREGROUND, NULL);
		CAApplication::getApplication()->setGLDefaultValues(); 
        CAApplication::getApplication()->resume(); 
   }

    m_loadingComplete = true;
}

void CrossAppRenderer::Connect()
{
}

// purge Cocos2d-x gl GL resourses since the DirectX/Angle Context has been lost 
void CrossAppRenderer::Disconnect()
{
    CAApplication::getApplication()->pause(); 
	CAApplication::getApplication()->purgeCachedData(); 
    CloseAngle();
    m_loadingComplete = false;
}

// save your game state here
IAsyncAction^ CrossAppRenderer::OnSuspending()
{
    return create_async([]() { 
        // save your game state here
    });
}


// user pressed the Back Key on the phone
void CrossAppRenderer::OnBackKeyPress()
{
    // handle the backkey in your app here.
    // call Cocos2dEvent::TerminateApp if it is time to exit your app.
    // ie. the user is on your first page and wishes to exit your app.
    m_delegate->Invoke(Cocos2dEvent::TerminateApp);
}

void CrossAppRenderer::OnUpdateDevice()
{
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pEGLView->UpdateDevice(m_eglDisplay, m_eglContext, m_eglSurface);
}

void CrossAppRenderer::OnOrientationChanged(Windows::Graphics::Display::DisplayOrientations orientation)
{
	DirectXBase::OnOrientationChanged(orientation);
    CCEGLView::sharedOpenGLView()->UpdateOrientation(orientation);
}

// return true if eglSwapBuffers was called by OnRender()
bool CrossAppRenderer::OnRender()
{
    if(m_loadingComplete)
    {
        CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
        pEGLView->Render();
        return true; // eglSwapBuffers was called by pEGLView->Render();
    }
    return false;
}

void CrossAppRenderer::OnPointerPressed(PointerEventArgs^ args)
{
    CCEGLView::sharedOpenGLView()->OnPointerPressed(args);
}

void CrossAppRenderer::OnPointerMoved(PointerEventArgs^ args)
{
    CCEGLView::sharedOpenGLView()->OnPointerMoved(args);
}

void CrossAppRenderer::OnPointerReleased(PointerEventArgs^ args)
{
    CCEGLView::sharedOpenGLView()->OnPointerReleased(args);
}

void CrossAppRenderer::OnKeyPressed(Platform::String^ text)
{
    char szUtf8[8] = {0};
    int nLen = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)text->Data(), 1, szUtf8, sizeof(szUtf8), NULL, NULL);
    CAIMEDispatcher::sharedDispatcher()->dispatchInsertText(szUtf8, nLen);
}


void CrossAppRenderer::OnCocos2dKeyEvent(Cocos2dKeyEvent event)
{
    switch(event)
    {
    case Cocos2dKeyEvent::Escape:
        CAApplication::getApplication()->getKeypadDispatcher()->dispatchKeypadMSG(kTypeBackClicked);
        break;
	case Cocos2dKeyEvent::Back:
        CAIMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();
        break;
    case Cocos2dKeyEvent::Enter:
		//SetFocus(false);
        CAIMEDispatcher::sharedDispatcher()->dispatchInsertText("\n", 1);
        break;
    default:
        break;
    }

}

void CrossAppRenderer::SetXamlEventDelegate(PhoneDirect3DXamlAppComponent::Cocos2dEventDelegate^ delegate)
{
    m_delegate = delegate;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    if(eglView)
    {
        eglView->SetXamlEventDelegate(delegate);
    }
}

void CrossAppRenderer::SetXamlMessageBoxDelegate(PhoneDirect3DXamlAppComponent::Cocos2dMessageBoxDelegate^ delegate)
{
    m_messageBoxDelegate = delegate;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    if(eglView)
    {
        eglView->SetXamlMessageBoxDelegate(delegate);
    }
}

void CrossAppRenderer::SetXamlEditBoxDelegate(PhoneDirect3DXamlAppComponent::Cocos2dEditBoxDelegate^ delegate)
{
    m_editBoxDelegate = delegate;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    if(eglView)
    {
        eglView->SetXamlEditBoxDelegate(delegate);
    }
}





