//
//  CAImagePickerController.h
//  CrossApp
//
//  Created by 秦乐 on 2017/2/21.
//  Copyright © 2017年 cocos2d-x. All rights reserved.
//

#include "../CAImagePickerController.h"
#include "basics/CAScheduler.h"

NS_CC_BEGIN

std::function<void(CrossApp::CAImage*)> _imagePickerControllerCallBack;

CAImagePickerController::CAImagePickerController(SourceType type)
: m_eSourceType(type)
{
    
}

CAImagePickerController::~CAImagePickerController()
{
    
}

CAImagePickerController* CAImagePickerController::create(SourceType type)
{
    CAImagePickerController* var = new CAImagePickerController(type);
    if (var && var->init())
    {
        var->autorelease();
        return var;
    }
    CC_SAFE_DELETE(var);
    return nullptr;
}

bool CAImagePickerController::init()
{
    return true;
}

void CAImagePickerController::open(const std::function<void(CrossApp::CAImage*)>& callback)
{
    _imagePickerControllerCallBack = callback;
    
    switch (m_eSourceType)
    {
        case CAImagePickerController::SourceType::PhotoLibrary:
        {
        }
            break;
        case CAImagePickerController::SourceType::CameraDeviceRear:
        {
        }
            break;
        case CAImagePickerController::SourceType::CameraDeviceFront:
        {
        }
            break;
        case CAImagePickerController::SourceType::SavedPhotosAlbum:
        {
            CCAssert(false, "");
            return;
        }
            break;
        default:
            break;
    }
}

void CAImagePickerController::writeImageToPhoto(CAImage* image, const std::function<void(bool)>& finishCallback, const std::string &imageName)
{
}

NS_CC_END
