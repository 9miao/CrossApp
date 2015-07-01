
#ifndef __HelloCpp__ViewController__
#define __HelloCpp__ViewController__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class FirstViewController: public CAViewController
{
    
public:

	FirstViewController();
    
	virtual ~FirstViewController();
	CAImage* getBlackFilterEffect(CAImage *sender);

	void onJietuBtnclick(CAControl *control, CCPoint point);
	void onLvjingBtnclick(CAControl *control, CCPoint point);

	
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
  
private:
	CAImageView* clipNode;
};


#endif /* defined(__HelloCpp__ViewController__) */
