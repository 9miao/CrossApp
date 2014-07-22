
#pragma once

#include "InputEvent.h"
#include "DirectXBase.h"

class AppDelegate;
 
ref class CrossAppRenderer sealed : public DirectXBase
{
public:
	CrossAppRenderer();

	// Direct3DBase methods.
	virtual void OnOrientationChanged(Windows::Graphics::Display::DisplayOrientations orientation) override;
	virtual bool OnRender() override;
    virtual void CreateGLResources() override;

    void OnBackButton();
    void OnKeyPressed(Platform::String^ text);
    void OnCocos2dKeyEvent(PhoneDirect3DXamlAppComponent::Cocos2dKeyEvent event);
    void SetXamlEventDelegate(PhoneDirect3DXamlAppComponent::Cocos2dEventDelegate^ delegate);
    void SetXamlMessageBoxDelegate(PhoneDirect3DXamlAppComponent::Cocos2dMessageBoxDelegate^ delegate);
    void SetXamlEditBoxDelegate(PhoneDirect3DXamlAppComponent::Cocos2dEditBoxDelegate^ delegate);

	void OnPointerPressed(Windows::UI::Core::PointerEventArgs^ args);
	void OnPointerMoved(Windows::UI::Core::PointerEventArgs^ args);
	void OnPointerReleased(Windows::UI::Core::PointerEventArgs^ args);
    Windows::Foundation::IAsyncAction^ OnSuspending();
    void OnBackKeyPress();
    void Connect();
    void Disconnect();

protected:
    virtual void OnUpdateDevice() override;

private:

    bool m_loadingComplete;
    bool mInitialized;

    PhoneDirect3DXamlAppComponent::Cocos2dEventDelegate^ m_delegate;
    PhoneDirect3DXamlAppComponent::Cocos2dMessageBoxDelegate^ m_messageBoxDelegate;
    PhoneDirect3DXamlAppComponent::Cocos2dEditBoxDelegate^ m_editBoxDelegate;

	// The AppDelegate for the Cocos2D app
	AppDelegate* mApp;
};
