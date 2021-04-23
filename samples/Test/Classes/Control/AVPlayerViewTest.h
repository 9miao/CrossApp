
#ifndef __Test__AVPlayerViewTest__
#define __Test__AVPlayerViewTest__

#include <iostream>
#include "RootWindow.h"

class AVPlayerViewTest: public CAViewController
{
    
public:

	AVPlayerViewTest();
    
	virtual ~AVPlayerViewTest();
    
    CREATE_FUNC(AVPlayerViewTest);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void playVideo();
    
    void setTextTagAndBackgroundImage(CAButton* btn, const std::string& textTag, const std::string& filePath);
    
private:
    
    CAButton* m_pPreviewBtn;
    
    CAAVPlayer* m_pAvplayer;
};


#endif /* defined(__Test__AVPlayerViewTest__) */
