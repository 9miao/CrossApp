//
//  CAMediaManager.cpp
//  LocationTest
//
//  Created by lh on 14-5-29.
//
//

#include "../CAMediaManager.h"

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

}
void CAMediaManager::addAlbumView()
{

}