#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

#define Screen_Width 1080
#define Screen_Height 1920

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("HelloCpp");
    eglView->setFrameSize(Screen_Width/2, Screen_Height/2);
    return CCApplication::sharedApplication()->run();
}
