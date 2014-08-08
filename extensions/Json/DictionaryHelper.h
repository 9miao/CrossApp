

#ifndef __DICTIONARYHELPER_H__
#define __DICTIONARYHELPER_H__

#include "CrossAppExt.h"
#include "ExtensionMacros.h"

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
    
    cs::CSJsonDictionary* getSubDictionary_json(cs::CSJsonDictionary* root,const char* key);
    int getIntValue_json(cs::CSJsonDictionary* root,const char* key);
    float getFloatValue_json(cs::CSJsonDictionary* root,const char* key);
    const char* getStringValue_json(cs::CSJsonDictionary* root,const char* key);
    bool getBooleanValue_json(cs::CSJsonDictionary* root,const char* key);
    int getArrayCount_json(cs::CSJsonDictionary* root,const char* key);
    int getIntValueFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx);
    float getFloatValueFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx);
    bool getBoolValueFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx);
    const char* getStringValueFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx);
    cs::CSJsonDictionary* getDictionaryFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx);
    bool checkObjectExist_json(cs::CSJsonDictionary* root,const char* key);
};

NS_CC_EXT_END

#endif /* defined(__CocoGUI__DictionaryHelper__) */
