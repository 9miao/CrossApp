//
//  CACamera.cpp
//  iBook
//
//  Created by lh on 14-5-5.
//
//

#include "../CACamera.h"
#include "CACameraController.h"
static CACamera *pCamera = NULL;
CACamera::CACamera()
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
    
    CACameraController *camera= [[CACameraController alloc] init];
    [camera openCameraView];
}
void CACamera::addAlbumView()
{
    CACameraController *camera= [[CACameraController alloc] init];
    [camera setSender:this];
   [camera openAlbumView];
    
    _camera = camera;
    
}



