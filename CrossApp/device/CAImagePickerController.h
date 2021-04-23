
#ifndef CAImagePickerController_h
#define CAImagePickerController_h

#include "basics/CAObject.h"
#include "images/CAImage.h"
#include <iostream>
#include <functional>
NS_CC_BEGIN

class CC_DLL CAImagePickerController : public CAObject
{
public:
    
    enum class SourceType : int
    {
        PhotoLibrary = 0,
        CameraDeviceRear,
        CameraDeviceFront,
        SavedPhotosAlbum,
    };
    
public:
    
    CAImagePickerController(SourceType type);
    
    virtual ~CAImagePickerController();
    
    static CAImagePickerController* create(SourceType type);
    
    bool init();
    
    void open(const std::function<void(CAImage*)>& callback);

    void writeImageToPhoto(CAImage* image, const std::function<void(bool)>& finishCallback, const std::string &imageName = "");
    
private:

    CAImage* m_pSavedImage;

    SourceType m_eSourceType;
    
    void* m_pOriginal;
};

NS_CC_END

#endif /* CACamera_h */
