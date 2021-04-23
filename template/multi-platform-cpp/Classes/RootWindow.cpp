

#include "RootWindow.h"
#include "FirstViewController.h"

static RootWindow* s_pRootWindow = nullptr;

RootWindow* RootWindow::getInstance()
{
    if (s_pRootWindow == nullptr)
    {
        s_pRootWindow = new RootWindow();
        s_pRootWindow->init();
    }
    return s_pRootWindow;
}

void RootWindow::destroyInstance()
{
    CC_SAFE_DELETE(s_pRootWindow);
    s_pRootWindow = nullptr;
}

RootWindow::RootWindow()
{
    
}

RootWindow::~RootWindow()
{
    
}

bool RootWindow::init()
{
    if (!CAWindow::init())
    {
        return false;
    }
    
    FirstViewController* _viewController = new FirstViewController();
    _viewController->init();
    this->setRootViewController(_viewController);
    _viewController->release();
    
    return true;
}
