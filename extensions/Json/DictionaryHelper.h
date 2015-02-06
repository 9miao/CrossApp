

#ifndef __DICTIONARYHELPER_H__
#define __DICTIONARYHELPER_H__

#include "CSContentJsonDictionary.h"

#define DICTOOL DictionaryHelper::shareHelper()

NS_CC_EXT_BEGIN
/**
*   @js NA
*   @lua NA
*/
class DictionaryHelper
{
public:
    DictionaryHelper();
    ~DictionaryHelper();
    static DictionaryHelper* shareHelper();
	static void purgeDictionaryHelper();
    CrossApp::CCDictionary* getSubDictionary(CrossApp::CCDictionary* root,const char* key);
    int getIntValue(CrossApp::CCDictionary* root,const char* key);
    float getFloatValue(CrossApp::CCDictionary* root,const char* key);
    const char* getStringValue(CrossApp::CCDictionary* root,const char* key);
    bool getBooleanValue(CrossApp::CCDictionary* root,const char* key);
    CrossApp::CCArray* getArrayValue(CrossApp::CCDictionary* root,const char* key);
    CrossApp::CAObject* checkObjectExist(CrossApp::CCDictionary* root,const char* key);
    int objectToIntValue(CrossApp::CAObject* obj);
    float objectToFloatValue(CrossApp::CAObject* obj);
    const char* objectToStringValue(CrossApp::CAObject* obj);
    bool objectToBooleanValue(CrossApp::CAObject* obj);
    CrossApp::CCArray* objectToCCArray(CrossApp::CAObject* obj);
    
    CSJsonDictionary* getSubDictionary_json(CSJsonDictionary* root,const char* key);
    int getIntValue_json(CSJsonDictionary* root,const char* key);
    float getFloatValue_json(CSJsonDictionary* root,const char* key);
    const char* getStringValue_json(CSJsonDictionary* root,const char* key);
    bool getBooleanValue_json(CSJsonDictionary* root,const char* key);
    int getArrayCount_json(CSJsonDictionary* root,const char* key);
    int getIntValueFromArray_json(CSJsonDictionary* root,const char* arrayKey,int idx);
    float getFloatValueFromArray_json(CSJsonDictionary* root,const char* arrayKey,int idx);
    bool getBoolValueFromArray_json(CSJsonDictionary* root,const char* arrayKey,int idx);
    const char* getStringValueFromArray_json(CSJsonDictionary* root,const char* arrayKey,int idx);
    CSJsonDictionary* getDictionaryFromArray_json(CSJsonDictionary* root,const char* arrayKey,int idx);
    bool checkObjectExist_json(CSJsonDictionary* root,const char* key);
};

NS_CC_EXT_END

#endif /* defined(__CocoGUI__DictionaryHelper__) */
