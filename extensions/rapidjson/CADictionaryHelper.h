

#ifndef __CADictionaryHelper_H__
#define __CADictionaryHelper_H__

#include "CrossApp.h"
#include "ExtensionMacros.h"
#include "rapidjson/document.h"

#define DICTOOL CADictionaryHelper::shareHelper()

NS_CC_EXT_BEGIN
/**
*   @js NA
*   @lua NA
*/
class CADictionaryHelper
{
public:
    CADictionaryHelper();
    ~CADictionaryHelper();
    static CADictionaryHelper* shareHelper();
	static void purgeCADictionaryHelper();
    CrossApp::CCDictionary* getSubDictionary(CrossApp::CCDictionary* root,const char* key);
    int   getIntValue(CrossApp::CCDictionary* root,const char* key);
    float getFloatValue(CrossApp::CCDictionary* root,const char* key);
    const char* getStringValue(CrossApp::CCDictionary* root,const char* key);
    bool  getBooleanValue(CrossApp::CCDictionary* root,const char* key);
    CrossApp::CCArray* getArrayValue(CrossApp::CCDictionary* root,const char* key);
    CrossApp::CAObject* checkObjectExist(CrossApp::CCDictionary* root,const char* key);
    int   objectToIntValue(CrossApp::CAObject* obj);
    float objectToFloatValue(CrossApp::CAObject* obj);
    const char* objectToStringValue(CrossApp::CAObject* obj);
    bool  objectToBooleanValue(CrossApp::CAObject* obj);
    CrossApp::CCArray* objectToCCArray(CrossApp::CAObject* obj);
    
	const rapidjson::Value& getSubDictionary_json(const rapidjson::Value &root, const char* key);
    const rapidjson::Value& getSubDictionary_json(const rapidjson::Value &root, const char* key, int idx);
    const rapidjson::Value& getSubDictionary_json(const rapidjson::Value &root, int idx);
    
	int   getIntValue_json(const rapidjson::Value& root, const char* key, int def = 0);
	float getFloatValue_json(const rapidjson::Value& root,const char* key, float def = 0.0f);
    bool  getBooleanValue_json(const rapidjson::Value& root,const char* key, bool def = false);
    const char* getStringValue_json(const rapidjson::Value& root,const char* key, const char *def = NULL);
    int   getArrayCount_json(const rapidjson::Value& root,const char* key, int def = 0);
	
    int   getIntValueFromArray_json(const rapidjson::Value& root,const char* arrayKey,int idx, int def = 0);
	float getFloatValueFromArray_json(const rapidjson::Value& root,const char* arrayKey,int idx, float def = 0.0f);
	bool  getBoolValueFromArray_json(const rapidjson::Value& root,const char* arrayKey,int idx, bool def = false);
	const char* getStringValueFromArray_json(const rapidjson::Value& root,const char* arrayKey,int idx, const char *def = NULL);
	const rapidjson::Value &getDictionaryFromArray_json(const rapidjson::Value &root, const char* key,int idx);
	bool checkObjectExist_json(const rapidjson::Value &root);
    bool checkObjectExist_json(const rapidjson::Value &root, const char* key);
    bool checkObjectExist_json(const rapidjson::Value &root, int index);
};

NS_CC_EXT_END

#endif /* defined(__CocoGUI__CADictionaryHelper__) */
