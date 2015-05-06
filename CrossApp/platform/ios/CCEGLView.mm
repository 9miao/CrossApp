
#include "EAGLView.h"
#include "CAApplicationCaller.h"
#include "CCEGLView.h"
#include "CCSet.h"
#include "CATouch.h"
#include "dispatcher/CATouchDispatcher.h"

NS_CC_BEGIN

CCEGLView::CCEGLView()
{
    m_obScreenSize.width = m_obDesignResolutionSize.width = [[EAGLView sharedEGLView] getWidth];
    m_obScreenSize.height = m_obDesignResolutionSize.height = [[EAGLView sharedEGLView] getHeight];
}

CCEGLView::~CCEGLView()
{

}

bool CCEGLView::isOpenGLReady()
{
    return [EAGLView sharedEGLView] != NULL;
}

void CCEGLView::end()
{
    [CAApplicationCaller destroy];
    
    // destroy EAGLView
    [[EAGLView sharedEGLView] removeFromSuperview];
}


void CCEGLView::swapBuffers()
{
    [[EAGLView sharedEGLView] swapBuffers];
}

void CCEGLView::checkContext()
{
    [[EAGLView sharedEGLView] checkContext];
}

void CCEGLView::setIMEKeyboardState(bool bOpen)
{
    if (bOpen)
    {
        [[EAGLView sharedEGLView] becomeFirstResponder];
    }
    else
    {
        [[EAGLView sharedEGLView] resignFirstResponder];
    }
}
void CCEGLView::setIMEKeyboardNumber()
{
    EAGLView * view = [EAGLView sharedEGLView];
    view.keyboardType =UIKeyboardTypeDecimalPad;
}
void CCEGLView::setIMEKeyboardDefault()
{
    EAGLView * view = [EAGLView sharedEGLView];
    view.keyboardType =UIKeyboardTypeDefault;
}
void CCEGLView::setIMEKeyboardAlphabet()
{
    EAGLView * view = [EAGLView sharedEGLView];
    view.keyboardType =UIKeyboardTypeASCIICapable;
}
CCEGLView* CCEGLView::sharedOpenGLView()
{
    static CCEGLView instance;
    return &instance;
}
void CCEGLView::setIMEKeyboardReturnSend()
{
    EAGLView * view = [EAGLView sharedEGLView];
    view.returnKeyType =UIReturnKeySend;
}

void CCEGLView::setIMEKeyboardReturnSearch()
{
    EAGLView * view = [EAGLView sharedEGLView];
    view.returnKeyType =UIReturnKeySearch;
}

void CCEGLView::setIMEKeyboardReturnDone()
{
    EAGLView * view = [EAGLView sharedEGLView];
    view.returnKeyType =UIReturnKeyDone;
}

void CCEGLView::setIMEKeyboardReturnEnter()
{
    EAGLView * view = [EAGLView sharedEGLView];
    view.keyboardType =UIKeyboardTypeDefault;
    view.returnKeyType =UIReturnKeyDefault;
}
NS_CC_END

