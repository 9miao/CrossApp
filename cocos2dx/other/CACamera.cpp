//
//  CACamera.cpp
//  iBook
//
//  Created by lh on 14-5-5.
//
//

#include "CACamera.h"

NS_CC_BEGIN

static CACamera *pCamera = NULL;

CACamera::CACamera()
:m_pCameraDelegate(NULL)
{
    
}

CACamera::~CACamera()
{
    
}

CACamera* CACamera::shareCamera()
{
    if (!pCamera)
    {
        pCamera= new CACamera();
    }
    return  pCamera;
    
}

void CACamera::addCameraView()
{

}

void CACamera::addAlbumView()
{

}

void CACamera::initAndroidImage()
{
    
}

void ShowDlg()
{
    
}

NS_CC_END