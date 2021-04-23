

#include "CAImagePickerController.h"
#include "basics/CAApplication.h"
#include "CATouchDispatcher.h"
#import <EAGLView.h>


NS_CC_BEGIN


CAImagePickerController::CAImagePickerController(SourceType type)
: m_eSourceType(type)
, m_pSavedImage(nullptr)
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
    
}

void CAImagePickerController::writeImageToPhoto(CAImage* image, const std::function<void(bool)>& finishCallback, const std::string &imageName)
{

}

NS_CC_END









