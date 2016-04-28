
#ifndef __Test__ViewController__
#define __Test__ViewController__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class TextFieldTest: public CAViewController, public CATextFieldDelegate
{
    
public:

	TextFieldTest();
    
	virtual ~TextFieldTest();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    virtual bool textFieldShouldBeginEditing(CATextField* sender);
    
    //If the sender doesn't want to detach from the IME, return true;
    virtual bool textFieldShouldEndEditing(CATextField* sender);
    
    //
    virtual void textFieldShouldReturn(CATextField* sender);
    
    virtual void keyBoardHeight(CATextField* sender, int height);
    
    //Warning!!! Warning!!! Warning!!!  This method is not on the OpenGL thread.
    virtual bool textFieldShouldChangeCharacters(CATextField* sender,
                                                 unsigned int location,
                                                 unsigned int lenght,
                                                 const std::string& changedText);
    
};


#endif /* defined(__HelloCpp__ViewController__) */
