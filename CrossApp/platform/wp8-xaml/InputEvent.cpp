#include "InputEvent.h"
#include "Cocos2dRenderer.h"

namespace PhoneDirect3DXamlAppComponent
{

PointerEvent::PointerEvent(PointerEventType type, Windows::UI::Core::PointerEventArgs^ args)
    : m_type(type), m_args(args)
{

}

void PointerEvent::execute(Cocos2dRenderer ^ renderer)
{
    switch(m_type)
    {
    case PointerEventType::PointerPressed:
        renderer->OnPointerPressed(m_args.Get());
        break;
    case PointerEventType::PointerMoved:
        renderer->OnPointerMoved(m_args.Get());
        break;           
    case PointerEventType::PointerReleased:
        renderer->OnPointerReleased(m_args.Get());
        break;
    }
}

KeyboardEvent::KeyboardEvent(Cocos2dKeyEvent type)
    : m_type(type), m_text(nullptr)
{

}

KeyboardEvent::KeyboardEvent(Cocos2dKeyEvent type, Platform::String^ text)
    : m_type(type), m_text(text)
{

}

void KeyboardEvent::execute(Cocos2dRenderer ^ renderer)
{
    switch(m_type)
    {
    case Cocos2dKeyEvent::Text:
        renderer->OnKeyPressed(m_text.Get());
        break;
    default:
        renderer->OnCocos2dKeyEvent(m_type);
        break;      
    }
}


BackButtonEvent::BackButtonEvent()
{

}

void BackButtonEvent::execute(Cocos2dRenderer ^ renderer)
{
    renderer->OnBackKeyPress();
}


}

