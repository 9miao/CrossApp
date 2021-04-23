
#ifndef __Test__PickerViewTest__
#define __Test__PickerViewTest__

#include "RootWindow.h"

class PickerViewTest: public CAViewController
{
    
public:

	PickerViewTest();
    
	virtual ~PickerViewTest();
    
    CREATE_FUNC(PickerViewTest);
    
    CC_SYNTHESIZE(ssize_t, m_iIndex, Index);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    virtual void dataPickerViewdidSelectRow(const tm& tm);
    
public:
    
    std::vector<const wchar_t*> m_vCitys;
};


#endif /* defined(__HelloCpp__ViewController__) */
