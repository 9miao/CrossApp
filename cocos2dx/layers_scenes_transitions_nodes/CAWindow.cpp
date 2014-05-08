/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CAWindow.h"
#include "support/CCPointExtension.h"
#include "CCDirector.h"

NS_CC_BEGIN

CAWindow::CAWindow()
:m_pRootViewController(NULL)
{
    m_bIgnoreAnchorPointForPosition = true;
    setAnchorPoint(ccp(0.5f, 0.5f));
}

CAWindow::~CAWindow()
{
    CC_SAFE_RELEASE_NULL(m_pRootViewController);
}

bool CAWindow::init()
{
    bool bRet = false;
     do 
     {
         CCDirector * pDirector;
         CC_BREAK_IF( ! (pDirector = CCDirector::sharedDirector()) );
         this->setContentSize(pDirector->getWinSize());
         // success
         bRet = true;
     } while (0);
     return bRet;
}

CAWindow *CAWindow::create()
{
    CAWindow *pRet = new CAWindow();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

void CAWindow::setRootViewController(cocos2d::CAViewController *var)
{
    if (m_pRootViewController)
    {
        m_pRootViewController->removeViewAtSuperNode();
        m_pRootViewController->release();
        m_pRootViewController = NULL;
    }
    
    if (var)
    {
        var->retain();
        m_pRootViewController = var;
        m_pRootViewController->setViewSuperNode(this);
    }
}

CAViewController* CAWindow::getRootViewController()
{
    return m_pRootViewController;
}

NS_CC_END
