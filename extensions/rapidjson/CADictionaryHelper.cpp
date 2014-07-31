

#include "CADictionaryHelper.h"

NS_CC_EXT_BEGIN
    
static CADictionaryHelper* sharedHelper = NULL;

CADictionaryHelper::CADictionaryHelper()
{
    
}

CADictionaryHelper::~CADictionaryHelper()
{
    
}

CADictionaryHelper* CADictionaryHelper::shareHelper()
{
    if (!sharedHelper) {
        sharedHelper = new CADictionaryHelper();
    }
    return sharedHelper;
}

void CADictionaryHelper::purgeCADictionaryHelper()
{
	 CC_SAFE_DELETE(sharedHelper);
}

CrossApp::CCDictionary* CADictionaryHelper::getSubDictionary(CrossApp::CCDictionary* root,const char* key)
{
    if (!root) {
        return NULL;
    }
    CrossApp::CAObject* obj = root->objectForKey(key);
    if (!obj) {
        return NULL;
    }
    return (CrossApp::CCDictionary*)(obj);
}

int CADictionaryHelper::getIntValue(CrossApp::CCDictionary* root,const char* key)
{
    if (!root) {
        return 0;
    }
    CrossApp::CAObject* obj = root->objectForKey(key);
    if (!obj) {
        return 0;
    }
    
    CrossApp::CCString* cstr = (CrossApp::CCString*)(obj);
    return cstr->intValue();
}

float CADictionaryHelper::getFloatValue(CrossApp::CCDictionary* root,const char* key)
{
    if (!root) {
        return 0.0;
    }
    CrossApp::CAObject* obj = root->objectForKey(key);
    if (!obj) {
        return 0.0f;
    }
    CrossApp::CCString* cstr = (CrossApp::CCString*)(obj);
    return cstr->floatValue();
}

const char* CADictionaryHelper::getStringValue(CrossApp::CCDictionary* root,const char* key)
{
    if (!root) {
        return NULL;
    }
    CrossApp::CAObject* obj = root->objectForKey(key);
    if (!obj) {
        return NULL;
    }
    CrossApp::CCString* cstr = (CrossApp::CCString*)(obj);
    return cstr->m_sString.c_str();
}

bool CADictionaryHelper::getBooleanValue(CrossApp::CCDictionary* root,const char* key)
{
    return 0 == this->getIntValue(root, key) ? false : true;
}

CrossApp::CCArray* CADictionaryHelper::getArrayValue(CrossApp::CCDictionary *root, const char *key)
{
    if (!root) {
        return NULL;
    }
    CrossApp::CAObject* obj = root->objectForKey(key);
    if (!obj) {
        return NULL;
    }
    CrossApp::CCArray* array = (CrossApp::CCArray*)(obj);
    return array;
}

CrossApp::CAObject* CADictionaryHelper::checkObjectExist(CrossApp::CCDictionary *root, const char *key)
{
    if (!root) {
        return NULL;
    }
    return root->objectForKey(key);
}

int CADictionaryHelper::objectToIntValue(CrossApp::CAObject *obj)
{
    if (!obj)
    {
        return 0;
    }
    CrossApp::CCString* cstr = (CrossApp::CCString*)(obj);
    return cstr->intValue();
}

float CADictionaryHelper::objectToFloatValue(CrossApp::CAObject *obj)
{
    if (!obj)
    {
        return 0.0f;
    }
    CrossApp::CCString* cstr = (CrossApp::CCString*)(obj);
    return cstr->floatValue();
}

const char* CADictionaryHelper::objectToStringValue(CrossApp::CAObject *obj)
{
    if (!obj)
    {
        return NULL;
    }
    CrossApp::CCString* cstr = (CrossApp::CCString*)(obj);
    return cstr->m_sString.c_str();
}

bool CADictionaryHelper::objectToBooleanValue(CrossApp::CAObject *obj)
{
    if (!obj)
    {
        return 0;
    }
    return this->objectToIntValue(obj) != 0? true:false;
}

CrossApp::CCArray* CADictionaryHelper::objectToCCArray(CrossApp::CAObject *obj)
{
    if (!obj)
    {
        return NULL;
    }
    CrossApp::CCArray* array = (CrossApp::CCArray*)(obj);
    return array;
}


const rapidjson::Value& CADictionaryHelper::getSubDictionary_json(const rapidjson::Value &root, const char* key)
{
	return root[key];
}

const rapidjson::Value& CADictionaryHelper::getSubDictionary_json(const rapidjson::Value &root, const char* key, int idx)
{
    return root[key][idx];
}

const rapidjson::Value& CADictionaryHelper::getSubDictionary_json(const rapidjson::Value &root, int idx)
{
    return root[idx];
}

int CADictionaryHelper::getIntValue_json(const rapidjson::Value& root, const char* key, int def)
{
    int nRet = def;
    do {
        CC_BREAK_IF(root.IsNull());
        CC_BREAK_IF(root[key].IsNull());
        nRet = root[key].GetInt();
    } while (0);
    
    return nRet;
}


float CADictionaryHelper::getFloatValue_json(const rapidjson::Value& root,const char* key, float def)
{
	float fRet = def;
    do {
        CC_BREAK_IF(root.IsNull());
        CC_BREAK_IF(root[key].IsNull());
        fRet = (float)root[key].GetDouble();
    } while (0);
    
    return fRet;
}

bool CADictionaryHelper::getBooleanValue_json(const rapidjson::Value& root,const char* key, bool def)
{
    bool bRet = def;
    do {
        CC_BREAK_IF(root.IsNull());
        CC_BREAK_IF(root[key].IsNull());
        bRet = root[key].GetBool();
    } while (0);
    
    return bRet;
}

const char* CADictionaryHelper::getStringValue_json(const rapidjson::Value& root,const char* key, const char *def)
{
    const char* sRet = def;
    do {
        CC_BREAK_IF(root.IsNull());
        CC_BREAK_IF(root[key].IsNull());
        sRet = root[key].GetString();
    } while (0);
    
    return sRet;
}



int CADictionaryHelper::getArrayCount_json(const rapidjson::Value& root, const char* key, int def)
{
    int nRet = def;
    do {
        CC_BREAK_IF(root.IsNull());
        CC_BREAK_IF(root[key].IsNull());
        nRet = (int)(root[key].Size());
    } while (0);
    
    return nRet;
}


int CADictionaryHelper::getIntValueFromArray_json(const rapidjson::Value& root,const char* arrayKey,int idx, int def)
{
    int nRet = def;
    do {
        CC_BREAK_IF(root.IsNull());
        CC_BREAK_IF(root[arrayKey].IsNull());
        CC_BREAK_IF(root[arrayKey][idx].IsNull());
        nRet = root[arrayKey][idx].GetInt();
    } while (0);
    
    return nRet;
}


float CADictionaryHelper::getFloatValueFromArray_json(const rapidjson::Value& root,const char* arrayKey,int idx, float def)
{
    float fRet = def;
    do {
        CC_BREAK_IF(root.IsNull());
        CC_BREAK_IF(root[arrayKey].IsNull());
        CC_BREAK_IF(root[arrayKey][idx].IsNull());
        fRet = (float)root[arrayKey][idx].GetDouble();
    } while (0);
    
    return fRet;
}

bool CADictionaryHelper::getBoolValueFromArray_json(const rapidjson::Value& root,const char* arrayKey,int idx, bool def)
{
	bool bRet = def;
    do {
        CC_BREAK_IF(root.IsNull());
        CC_BREAK_IF(root[arrayKey].IsNull());
        CC_BREAK_IF(root[arrayKey][idx].IsNull());
        bRet = root[arrayKey][idx].GetBool();
    } while (0);
    
    return bRet;
}

const char* CADictionaryHelper::getStringValueFromArray_json(const rapidjson::Value& root,const char* arrayKey,int idx, const char *def)
{
    const char *sRet = def;
    do {
        CC_BREAK_IF(root.IsNull());
        CC_BREAK_IF(root[arrayKey].IsNull());
        CC_BREAK_IF(root[arrayKey][idx].IsNull());
        sRet = root[arrayKey][idx].GetString();
    } while (0);
    
    return sRet;
}

const rapidjson::Value &CADictionaryHelper::getDictionaryFromArray_json(const rapidjson::Value &root, const char* key,int idx)
{
	return root[key][idx];
}

bool CADictionaryHelper::checkObjectExist_json(const rapidjson::Value &root)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(root.IsNull());
        bRet = true;
    } while (0);
    
    return bRet;
}

bool CADictionaryHelper::checkObjectExist_json(const rapidjson::Value &root,const char* key)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(root.IsNull());
        bRet = root.HasMember(key);
    } while (0);
    
    return bRet;
}

bool CADictionaryHelper::checkObjectExist_json(const rapidjson::Value &root, int index)
{
    bool bRet = false;
    do {   
        CC_BREAK_IF(root.IsNull());
        CC_BREAK_IF(!root.IsArray());
        CC_BREAK_IF(index < 0 || root.Size() <= (unsigned int )index);
        bRet = true;
    } while (0);

    return bRet;
}

NS_CC_EXT_END