
#include "CCFileUtilsMac.h"
#import <Foundation/Foundation.h>
#include <string>
#include <stack>
#include "cocoa/CCString.h"
#include "CCFileUtils.h"
#include "basics/CAApplication.h"
#include "CCSAXParser.h"
#include "support/zip_support/unzip.h"


NS_CC_BEGIN

static void addValueToCCDict(id key, id value, CAMap<CAObject*, CAObject*>& pDict);
static void addCAObjectToNSDict(const char*key, CAObject* object, NSMutableDictionary *dict);

static void addItemToCCVector(id item, CAVector<CAObject*>& pArray)
{
    // add string value into array
    if ([item isKindOfClass:[NSString class]]) {
        CCString* pValue = new CCString([item UTF8String]);
        
        pArray.pushBack(pValue);
        pValue->release();
        return;
    }
    
    // add number value into array(such as int, float, bool and so on)
    if ([item isKindOfClass:[NSNumber class]]) {
        NSString* pStr = [item stringValue];
        CCString* pValue = new CCString([pStr UTF8String]);
        
        pArray.pushBack(pValue);
        pValue->release();
        return;
    }
    
    // add dictionary value into array
    if ([item isKindOfClass:[NSDictionary class]]) {
        CAMap<CAObject*, CAObject*> pDictItem;;
        for (id subKey in [item allKeys]) {
            id subValue = [item objectForKey:subKey];
            addValueToCCDict(subKey, subValue, pDictItem);
        }
        pArray.pushBack(&pDictItem);
        pDictItem.clear();
        return;
    }
    
    // add array value into array
    if ([item isKindOfClass:[NSArray class]]) {
        CAVector<CAObject*> pArrayItem;
        for (id subItem in item) {
            addItemToCCVector(subItem, pArrayItem);
        }
        pArray.pushBack(pArrayItem);
        pArrayItem.clear();
        return;
    }
}

static void addCAObjectToNSArray(CAObject *object, NSMutableArray *array)
{
    
    // add string into array
    if (CCString *ccString = dynamic_cast<CCString *>(object)) {
        NSString *strElement = [NSString stringWithCString:ccString->getCString() encoding:NSUTF8StringEncoding];
        [array addObject:strElement];
        return;
    }
    
    // add array into array
    if (CAVector<CAObject*> *ccVertor = dynamic_cast< CAVector<CAObject*> *>(object)) {
        NSMutableArray *arrElement = [NSMutableArray array];
        CAObject *element = NULL;
        for(int i=0; i < ccVertor->size(); i++)
        {
            element = ccVertor->at(i);
            addCAObjectToNSArray(element, arrElement);
        }
        [array addObject:arrElement];
        return;
    }
    
    // add dictionary value into array
    if (CAMap<CAObject*, CAObject*> *ccDict = dynamic_cast<CAMap<CAObject*, CAObject*> *>(object)) {
        NSMutableDictionary *dictElement = [NSMutableDictionary dictionary];
        
        CAMap<CAObject* , CAObject*>::iterator itr = ccDict->begin();
        for(; itr != ccDict->end(); itr++)
        {
            addCAObjectToNSDict( ((CCString*)(itr->first))->getCString(), (CAObject*)((itr)->second), dictElement);
        }
        [array addObject:dictElement];
    }
}

static void addValueToCCDict(id key, id value, CAMap<CAObject*, CAObject*>& pDict)
{
    // the key must be a string
    CCAssert([key isKindOfClass:[NSString class]], "The key should be a string!");
    std::string pKey = [key UTF8String];
    CCString * tmpKey = CCString::create(pKey);
    // the value is a new dictionary
    if ([value isKindOfClass:[NSDictionary class]]) {
        CAMap<CAObject*, CAObject*> pSubDict;
        for (id subKey in [value allKeys]) {
            id subValue = [value objectForKey:subKey];
            addValueToCCDict(subKey, subValue, pSubDict);
        }
        pDict.assign(&pSubDict, tmpKey);
        pSubDict.clear();
        return;
    }
    
    // the value is a string
    if ([value isKindOfClass:[NSString class]]) {
        CCString* pValue = new CCString([value UTF8String]);
        
        pDict.assign(pValue, tmpKey);
        pValue->release();
        return;
    }
    
    // the value is a number
    if ([value isKindOfClass:[NSNumber class]]) {
        NSString* pStr = [value stringValue];
        CCString* pValue = new CCString([pStr UTF8String]);
        
        pDict.assign(pValue, tmpKey);
        pValue->release();
        return;
    }
    
    // the value is a array
    if ([value isKindOfClass:[NSArray class]]) {
        CAVector<CAObject*> pArray;
        for (id item in value) {
            addItemToCCVector(item, pArray);
        }
        pDict.assign(&pArray, tmpKey);
        pArray.clear();
        return;
    }
}

static void addCAObjectToNSDict(const char * key, CAObject* object, NSMutableDictionary *dict)
{
    NSString *NSkey = [NSString stringWithCString:key encoding:NSUTF8StringEncoding];
    
    // the object is a CAMap
    if (CAMap<CAObject*, CAObject*> *ccDict = dynamic_cast<CAMap<CAObject*, CAObject*> *>(object)) {
        NSMutableDictionary *dictElement = [NSMutableDictionary dictionary];
        CAMap<CAObject* , CAObject*>::iterator itr = ccDict->begin();
        for(; itr != ccDict->end(); itr++)
        {
            addCAObjectToNSDict( ((CCString*)(itr->first))->getCString(), (CAObject*)((itr)->second), dictElement);
        }
        
        [dict setObject:dictElement forKey:NSkey];
        return;
    }
    
    // the object is a CCString
    if (CCString *element = dynamic_cast<CCString *>(object)) {
        NSString *strElement = [NSString stringWithCString:element->getCString() encoding:NSUTF8StringEncoding];
        [dict setObject:strElement forKey:NSkey];
        return;
    }
    
    // the object is a CCVector
    if (CAVector<CAObject*> *ccVertor = dynamic_cast< CAVector<CAObject*> *>(object)) {
        NSMutableArray *arrElement = [NSMutableArray array];
        CAObject *element = NULL;
        for(int i=0; i < ccVertor->size(); i++)
        {
            element = ccVertor->at(i);
            addCAObjectToNSArray(element, arrElement);
        }
        [dict setObject:arrElement forKey:NSkey];
        return;
    }
}

CCFileUtils* CCFileUtils::sharedFileUtils()
{
    if (s_sharedFileUtils == NULL)
    {
        s_sharedFileUtils = new CCFileUtilsMac();
        s_sharedFileUtils->init();
    }
    return s_sharedFileUtils;
}


static NSFileManager* s_fileManager = [NSFileManager defaultManager];

std::string CCFileUtilsMac::getWritablePath()
{
    // save to document folder
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
    strRet.append("/CrossApp/");
    this->createDirectory(strRet.c_str());
    return strRet;
}

bool CCFileUtilsMac::isFileExist(const std::string& strFilePath)
{
    if (0 == strFilePath.length())
    {
        return false;
    }
    
    bool bRet = false;
    
    if (strFilePath[0] != '/')
    {
        std::string path = strFilePath;
        std::string file;
        size_t pos = path.find_last_of("/");
        if (pos != std::string::npos)
        {
            file = path.substr(pos+1);
            path = path.substr(0, pos+1);
            NSString* fullpath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:file.c_str()]
                                                                 ofType:nil
                                                            inDirectory:[NSString stringWithUTF8String:path.c_str()]];
            if (fullpath != nil) {
                bRet = true;
            }
        }
    }
    else
    {
        // Search path is an absolute path.
        if ([s_fileManager fileExistsAtPath:[NSString stringWithUTF8String:strFilePath.c_str()]]) {
            bRet = true;
        }
    }
    
    return bRet;
}

std::string CCFileUtilsMac::getFullPathForDirectoryAndFilename(const std::string& strDirectory, const std::string& strFilename)
{
    if (strDirectory[0] != '/')
    {
        NSString* fullpath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:strFilename.c_str()]
                                                             ofType:nil
                                                        inDirectory:[NSString stringWithUTF8String:strDirectory.c_str()]];
        if (fullpath != nil) {
            return [fullpath UTF8String];
        }
    }
    else
    {
        std::string fullPath = strDirectory+strFilename;
        // Search path is an absolute path.
        if ([s_fileManager fileExistsAtPath:[NSString stringWithUTF8String:fullPath.c_str()]]) {
            return fullPath;
        }
    }
    return "";
}

bool CCFileUtilsMac::isAbsolutePath(const std::string& strPath)
{
    NSString* path = [NSString stringWithUTF8String:strPath.c_str()];
    return [path isAbsolutePath] ? true : false;
}

CAMap<CAObject*, CAObject*> CCFileUtilsMac::createCCDictionaryWithContentsOfFile(const std::string& filename)
{
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename.c_str());
    NSString* pPath = [NSString stringWithUTF8String:fullPath.c_str()];
    NSDictionary* pDict = [NSDictionary dictionaryWithContentsOfFile:pPath];
    
    CAMap<CAObject*, CAObject*> pRet;
    for (id key in [pDict allKeys]) {
        id value = [pDict objectForKey:key];
        addValueToCCDict(key, value, pRet);
    }
    
    return pRet;
}

bool CCFileUtilsMac::writeToFile(CAMap<std::string, CAObject*> *dict, const std::string &fullPath)
{
    CCLOG("iOS||Mac CCDictionary %d write to file %s", dict->m_u__ID, fullPath.c_str());
    NSMutableDictionary *nsDict = [NSMutableDictionary dictionary];
    CAMap<std::string, CAObject*>::iterator itr = dict->begin();
    for(; itr != dict->end(); itr++)
    {
        addCAObjectToNSDict( (itr->first).c_str(), (CAObject*)((itr)->second), nsDict);
    }
    
    NSString *file = [NSString stringWithUTF8String:fullPath.c_str()];
    // do it atomically
    return [nsDict writeToFile:file atomically:YES];
}

CAVector<CAObject*> CCFileUtilsMac::createCCVectorWithContentsOfFile(const std::string& filename)
{
    //    NSString* pPath = [NSString stringWithUTF8String:pFileName];
    //    NSString* pathExtension= [pPath pathExtension];
    //    pPath = [pPath stringByDeletingPathExtension];
    //    pPath = [[NSBundle mainBundle] pathForResource:pPath ofType:pathExtension];
    //    fixing cannot read data using CCArray::createWithContentsOfFile
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename.c_str());
    NSString* pPath = [NSString stringWithUTF8String:fullPath.c_str()];
    NSArray* pArray = [NSArray arrayWithContentsOfFile:pPath];
    
    CAVector<CAObject*> pRet;
    for (id value in pArray) {
        addItemToCCVector(value, pRet);
    }
    
    return pRet;
}


NS_CC_END

