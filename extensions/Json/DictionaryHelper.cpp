/****************************************************************************
 Copyright (c) 2013 9miao.com
 
 http://www.9miao.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "DictionaryHelper.h"

NS_CC_EXT_BEGIN
    
static DictionaryHelper* sharedHelper = NULL;

DictionaryHelper::DictionaryHelper()
{
    
}

DictionaryHelper::~DictionaryHelper()
{
    
}

DictionaryHelper* DictionaryHelper::shareHelper()
{
    if (!sharedHelper) {
        sharedHelper = new DictionaryHelper();
    }
    return sharedHelper;
}

void DictionaryHelper::purgeDictionaryHelper()
{
	 CC_SAFE_DELETE(sharedHelper);
}

CrossApp::CCDictionary* DictionaryHelper::getSubDictionary(CrossApp::CCDictionary* root,const char* key)
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

int DictionaryHelper::getIntValue(CrossApp::CCDictionary* root,const char* key)
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

float DictionaryHelper::getFloatValue(CrossApp::CCDictionary* root,const char* key)
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

const char* DictionaryHelper::getStringValue(CrossApp::CCDictionary* root,const char* key)
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

bool DictionaryHelper::getBooleanValue(CrossApp::CCDictionary* root,const char* key)
{
    return this->getIntValue(root, key);
}

CrossApp::CCArray* DictionaryHelper::getArrayValue(CrossApp::CCDictionary *root, const char *key)
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

CrossApp::CAObject* DictionaryHelper::checkObjectExist(CrossApp::CCDictionary *root, const char *key)
{
    if (!root) {
        return NULL;
    }
    return root->objectForKey(key);
}

int DictionaryHelper::objectToIntValue(CrossApp::CAObject *obj)
{
    if (!obj)
    {
        return 0;
    }
    CrossApp::CCString* cstr = (CrossApp::CCString*)(obj);
    return cstr->intValue();
}

float DictionaryHelper::objectToFloatValue(CrossApp::CAObject *obj)
{
    if (!obj)
    {
        return 0.0f;
    }
    CrossApp::CCString* cstr = (CrossApp::CCString*)(obj);
    return cstr->floatValue();
}

const char* DictionaryHelper::objectToStringValue(CrossApp::CAObject *obj)
{
    if (!obj)
    {
        return NULL;
    }
    CrossApp::CCString* cstr = (CrossApp::CCString*)(obj);
    return cstr->m_sString.c_str();
}

bool DictionaryHelper::objectToBooleanValue(CrossApp::CAObject *obj)
{
    if (!obj)
    {
        return 0;
    }
    return this->objectToIntValue(obj);
}

CrossApp::CCArray* DictionaryHelper::objectToCCArray(CrossApp::CAObject *obj)
{
    if (!obj)
    {
        return NULL;
    }
    CrossApp::CCArray* array = (CrossApp::CCArray*)(obj);
    return array;
}

cs::CSJsonDictionary* DictionaryHelper::getSubDictionary_json(cs::CSJsonDictionary* root,const char* key)
{
    if (!root)
    {
        return NULL;
    }
    return root->getSubDictionary(key);
}

int DictionaryHelper::getIntValue_json(cs::CSJsonDictionary* root,const char* key)
{
    if (!root)
    {
        return 0;
    }
    return root->getItemIntValue(key, 0);
}

float DictionaryHelper::getFloatValue_json(cs::CSJsonDictionary* root,const char* key)
{
    if (!root)
    {
        return 0.0f;
    }
    return root->getItemFloatValue(key, 0.0);
}

const char* DictionaryHelper::getStringValue_json(cs::CSJsonDictionary* root,const char* key)
{
    if (!root)
    {
        return NULL;
    }
    return root->getItemStringValue(key);
}

bool DictionaryHelper::getBooleanValue_json(cs::CSJsonDictionary* root,const char* key)
{
    if (!root)
    {
        return 0;
    }
    return root->getItemBoolvalue(key, false);
}

int DictionaryHelper::getArrayCount_json(cs::CSJsonDictionary* root,const char* key)
{
    if (!root)
    {
        return 0;
    }
    return root->getArrayItemCount(key);
}

int DictionaryHelper::getIntValueFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx)
{
    if (!root)
    {
        return 0;
    }
    return root->getIntValueFromArray(arrayKey, idx, 0);
}

float DictionaryHelper::getFloatValueFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx)
{
    if (!root)
    {
        return 0.0f;
    }
    return root->getFloatValueFromArray(arrayKey, idx, 0.0);
}

bool DictionaryHelper::getBoolValueFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx)
{
    if (!root)
    {
        return false;
    }
    return root->getBoolValueFromArray(arrayKey, idx, false);
}

const char* DictionaryHelper::getStringValueFromArray_json(cs::CSJsonDictionary *root, const char *arrayKey, int idx)
{
    if (!root)
    {
        return NULL;
    }
    return root->getStringValueFromArray(arrayKey, idx);
}

cs::CSJsonDictionary* DictionaryHelper::getDictionaryFromArray_json(cs::CSJsonDictionary* root,const char* arrayKey,int idx)
{
    if (!root)
    {
        return NULL;
    }
    return root->getSubItemFromArray(arrayKey, idx);
}

bool DictionaryHelper::checkObjectExist_json(cs::CSJsonDictionary *root, const char *key)
{
    if (!root)
    {
        return false;
    }
    return root->isKeyValidate(key);
}

NS_CC_EXT_END