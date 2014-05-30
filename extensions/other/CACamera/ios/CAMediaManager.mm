//
//  CAMediaManager.cpp
//  LocationTest
//
//  Created by lh on 14-5-29.
//
//

#include "../CAMediaManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "CACameraController.h"
#include "CAAlbumController.h"
#endif
static CAMediaManager *pCamera = NULL;
CAMediaManager::CAMediaManager()
{
    
}
CAMediaManager::~CAMediaManager()
{
    
}
CAMediaManager* CAMediaManager::shareCAMediaManager()
{
    if (!pCamera)
    {
        pCamera= new CAMediaManager();
    }
    return  pCamera;
    
}
void CAMediaManager::addCameraView()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CACameraController *camera= [[CACameraController alloc] init];
    [camera setSender:this];
    [camera openCameraView];
#endif
}
void CAMediaManager::addAlbumView()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CAAlbumController *album= [[CAAlbumController alloc] init];
    [album setSender:this];
#endif
}