

#include "CCFileUtils.h"
#include "basics/CAApplication.h"
#include "cocoa/CCDictionary.h"
#include "cocoa/CCString.h"
#include "CCSAXParser.h"
#include "support/tinyxml2/tinyxml2.h"
#include "support/zip_support/unzip.h"
#include <stack>
#include <algorithm>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#endif

using namespace std;

#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS) && (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)

NS_CC_BEGIN

typedef enum 
{
    SAX_NONE = 0,
    SAX_KEY,
    SAX_DICT,
    SAX_INT,
    SAX_REAL,
    SAX_STRING,
    SAX_ARRAY
}CCSAXState;

typedef enum
{
    SAX_RESULT_NONE = 0,
    SAX_RESULT_DICT,
    SAX_RESULT_ARRAY
}CCSAXResult;

class CCDictMaker : public CCSAXDelegator
{
public:
    CCSAXResult m_eResultType;
    CAVector<CAObject*> m_pRootArray;
    CAMap<CAObject*, CAObject*> m_pRootDict;
    CAMap<CAObject*, CAObject*> m_pCurDict;
    std::stack<CAMap<CAObject*, CAObject*>*> m_tDictStack;
    std::string m_sCurKey;   ///< parsed key
    std::string m_sCurValue; // parsed value
    CCSAXState m_tState;
    CAVector<CAObject*> m_pArray;

    std::stack<CAVector<CAObject*>*> m_tArrayStack;
    std::stack<CCSAXState>  m_tStateStack;

public:
    CCDictMaker()        
        : m_eResultType(SAX_RESULT_NONE),
          m_tState(SAX_NONE)
    {
        m_pRootArray.clear();
        m_pRootDict.clear();
        m_pCurDict.clear();
        m_pArray.clear();
    }

    ~CCDictMaker()
    {
    }

    CAMap<CAObject*, CAObject*> dictionaryWithContentsOfFile(const char *pFileName)
    {
        m_eResultType = SAX_RESULT_DICT;
        CCSAXParser parser;

        if (false == parser.init("UTF-8"))
        {
            CAMap<CAObject*, CAObject*> emptyMap;
            return emptyMap;
        }
        parser.setDelegator(this);

        parser.parse(pFileName);
        return m_pRootDict;
    }

    CAVector<CAObject*> arrayWithContentsOfFile(const char* pFileName)
    {
        m_eResultType = SAX_RESULT_ARRAY;
        CCSAXParser parser;

        if (false == parser.init("UTF-8"))
        {
            CAVector<CAObject*> emptyVec;
            return emptyVec;
        }
        parser.setDelegator(this);

        parser.parse(pFileName);
        return m_pArray;
    }

    void startElement(void *ctx, const char *name, const char **atts)
    {
        CC_UNUSED_PARAM(ctx);
        CC_UNUSED_PARAM(atts);
        std::string sName((char*)name);
        if( sName == "dict" )
        {
            if(m_eResultType == SAX_RESULT_DICT && m_pRootDict.size() == 0)
            {
                // Because it will call m_pCurDict->release() later, so retain here.
                m_pRootDict = m_pCurDict;
                m_pRootDict.retain();
            }
            m_tState = SAX_DICT;

            CCSAXState preState = SAX_NONE;
            if (! m_tStateStack.empty())
            {
                preState = m_tStateStack.top();
            }

            if (SAX_ARRAY == preState)
            {
                // add the dictionary into the array
                m_pArray.pushBack(&m_pCurDict);
            }
            else if (SAX_DICT == preState)
            {
                // add the dictionary into the pre dictionary
                CCAssert(! m_tDictStack.empty(), "The state is wrong!");
                CAMap<CAObject*, CAObject*>* pPreDict = m_tDictStack.top();
                (*pPreDict).assign(&m_pCurDict, CCString::create(m_sCurKey));
            }

            //m_pCurDict.clear();

            // record the dict state
            m_tStateStack.push(m_tState);
            m_tDictStack.push(&m_pCurDict);
        }
        else if(sName == "key")
        {
            m_tState = SAX_KEY;
        }
        else if(sName == "integer")
        {
            m_tState = SAX_INT;
        }
        else if(sName == "real")
        {
            m_tState = SAX_REAL;
        }
        else if(sName == "string")
        {
            m_tState = SAX_STRING;
        }
        else if (sName == "array")
        {
            m_tState = SAX_ARRAY;
            if (m_eResultType == SAX_RESULT_ARRAY && m_pRootArray.size() == 0)
            {
                m_pRootArray = m_pArray;
                m_pRootArray.retain();
            }
            CCSAXState preState = SAX_NONE;
            if (! m_tStateStack.empty())
            {
                preState = m_tStateStack.top();
            }

            if (preState == SAX_DICT)
            {
                m_pCurDict.assign(&m_pArray, CCString::create(m_sCurKey));
            }
            else if (preState == SAX_ARRAY)
            {
                CCAssert(! m_tArrayStack.empty(), "The state is wrong!");
                CAVector<CAObject*>* pPreArray = m_tArrayStack.top();
                (*pPreArray).pushBack(m_pArray);
            }
            //m_pArray.clear();
            // record the array state
            m_tStateStack.push(m_tState);
            m_tArrayStack.push(&m_pArray);
        }
        else
        {
            m_tState = SAX_NONE;
        }
    }

    void endElement(void *ctx, const char *name)
    {
        CC_UNUSED_PARAM(ctx);
        CCSAXState curState = m_tStateStack.empty() ? SAX_DICT : m_tStateStack.top();
        std::string sName((char*)name);
        if( sName == "dict" )
        {
            m_tStateStack.pop();
            m_tDictStack.pop();
            if ( !m_tDictStack.empty())
            {
                m_pCurDict = *m_tDictStack.top();
            }
        }
        else if (sName == "array")
        {
            m_tStateStack.pop();
            m_tArrayStack.pop();
            if (! m_tArrayStack.empty())
            {
                m_pArray = *m_tArrayStack.top();
            }
        }
        else if (sName == "true")
        {
            CCString *str = new CCString("1");
            if (SAX_ARRAY == curState)
            {
                m_pArray.pushBack(str);
            }
            else if (SAX_DICT == curState)
            {
                m_pCurDict.assign(str, CCString::create(m_sCurKey));
            }
            str->release();
        }
        else if (sName == "false")
        {
            CCString *str = new CCString("0");
            if (SAX_ARRAY == curState)
            {
                m_pArray.pushBack(str);
            }
            else if (SAX_DICT == curState)
            {
                m_pCurDict.assign(str, CCString::create(m_sCurKey));
            }
            str->release();
        }
        else if (sName == "string" || sName == "integer" || sName == "real")
        {
            CCString* pStrValue = new CCString(m_sCurValue);

            if (SAX_ARRAY == curState)
            {
                m_pArray.pushBack(pStrValue);
            }
            else if (SAX_DICT == curState)
            {
                m_pCurDict.assign(pStrValue, CCString::create(m_sCurKey));
            }

            pStrValue->release();
            m_sCurValue.clear();
        }
        
        m_tState = SAX_NONE;
    }

    void textHandler(void *ctx, const char *ch, int len)
    {
        CC_UNUSED_PARAM(ctx);
        if (m_tState == SAX_NONE)
        {
            return;
        }

        CCSAXState curState = m_tStateStack.empty() ? SAX_DICT : m_tStateStack.top();
        CCString *pText = new CCString(std::string((char*)ch,0,len));

        switch(m_tState)
        {
        case SAX_KEY:
            m_sCurKey = pText->getCString();
            break;
        case SAX_INT:
        case SAX_REAL:
        case SAX_STRING:
            {
                if (curState == SAX_DICT)
                {
                    CCAssert(!m_sCurKey.empty(), "key not found : <integer/real>");
                }
                
                m_sCurValue.append(pText->getCString());
            }
            break;
        default:
            break;
        }
        pText->release();
    }
};

CAMap<CAObject*, CAObject*> CCFileUtils::createCCDictionaryWithContentsOfFile(const std::string& filename)
{
    std::string fullPath = fullPathForFilename(filename.c_str());
    CCDictMaker tMaker;
    return tMaker.dictionaryWithContentsOfFile(fullPath.c_str());
}

CAVector<CAObject*> CCFileUtils::createCCArrayWithContentsOfFile(const std::string& filename)
{
    std::string fullPath = fullPathForFilename(filename.c_str());
    CCDictMaker tMaker;
    return tMaker.arrayWithContentsOfFile(fullPath.c_str());
}

/*
 * forward statement
 */
static tinyxml2::XMLElement* generateElementForArray(CrossApp::CCArray *array, tinyxml2::XMLDocument *pDoc);
static tinyxml2::XMLElement* generateElementForDict(CrossApp::CCDictionary *dict, tinyxml2::XMLDocument *pDoc);

/*
 * Use tinyxml2 to write plist files
 */
bool CCFileUtils::writeToFile(CrossApp::CCDictionary *dict, const std::string &fullPath)
{
    //CCLOG("tinyxml2 CCDictionary %d writeToFile %s", dict->m_uID, fullPath.c_str());
    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
    if (NULL == pDoc)
        return false;
    
    tinyxml2::XMLDeclaration *pDeclaration = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    if (NULL == pDeclaration)
    {
        delete pDoc;
        return false;
    }
    
    pDoc->LinkEndChild(pDeclaration);
    tinyxml2::XMLElement *docType = pDoc->NewElement("!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"");
    pDoc->LinkEndChild(docType);
    
    tinyxml2::XMLElement *pRootEle = pDoc->NewElement("plist");
    pRootEle->SetAttribute("version", "1.0");
    if (NULL == pRootEle)
    {
        delete pDoc;
        return false;
    }
    pDoc->LinkEndChild(pRootEle);
    
    tinyxml2::XMLElement *innerDict = generateElementForDict(dict, pDoc);
    if (NULL == innerDict )
    {
        delete pDoc;
        return false;
    }
    pRootEle->LinkEndChild(innerDict);
    
    bool bRet = tinyxml2::XML_SUCCESS == pDoc->SaveFile(fullPath.c_str());
    
    delete pDoc;
    return bRet;
}

/*
 * Generate tinyxml2::XMLElement for CAObject through a tinyxml2::XMLDocument
 */
static tinyxml2::XMLElement* generateElementForObject(CrossApp::CAObject *object, tinyxml2::XMLDocument *pDoc)
{
    // object is CCString
    if (CCString *str = dynamic_cast<CCString *>(object))
    {
        tinyxml2::XMLElement* node = pDoc->NewElement("string");
        tinyxml2::XMLText* content = pDoc->NewText(str->getCString());
        node->LinkEndChild(content);
        return node;
    }
    
    // object is CCArray
    if (CCArray *array = dynamic_cast<CCArray *>(object))
        return generateElementForArray(array, pDoc);
    
    // object is CCDictionary
    if (CCDictionary *innerDict = dynamic_cast<CCDictionary *>(object))
        return generateElementForDict(innerDict, pDoc);
    
    CCLOG("This type cannot appear in property list");
    return NULL;
}

/*
 * Generate tinyxml2::XMLElement for CCDictionary through a tinyxml2::XMLDocument
 */
static tinyxml2::XMLElement* generateElementForDict(CrossApp::CCDictionary *dict, tinyxml2::XMLDocument *pDoc)
{
    tinyxml2::XMLElement* rootNode = pDoc->NewElement("dict");
    
    CCDictElement *dictElement = NULL;
    CCDICT_FOREACH(dict, dictElement)
    {
        tinyxml2::XMLElement* tmpNode = pDoc->NewElement("key");
        rootNode->LinkEndChild(tmpNode);
        tinyxml2::XMLText* content = pDoc->NewText(dictElement->getStrKey());
        tmpNode->LinkEndChild(content);
        
        CAObject *object = dictElement->getObject();
        tinyxml2::XMLElement *element = generateElementForObject(object, pDoc);
        if (element)
            rootNode->LinkEndChild(element);
    }
    return rootNode;
}

/*
 * Generate tinyxml2::XMLElement for CCArray through a tinyxml2::XMLDocument
 */
static tinyxml2::XMLElement* generateElementForArray(CrossApp::CCArray *array, tinyxml2::XMLDocument *pDoc)
{
    tinyxml2::XMLElement* rootNode = pDoc->NewElement("array");
    
    CAObject *object = NULL;
    CCARRAY_FOREACH(array, object)
    {
        tinyxml2::XMLElement *element = generateElementForObject(object, pDoc);
        if (element)
            rootNode->LinkEndChild(element);
    }
    return rootNode;
}


#else
NS_CC_BEGIN

/* The subclass CCFileUtilsIOS and CCFileUtilsMac should override these two method. */
CAMap<CAObject*, CAObject*> CCFileUtils::createCCDictionaryWithContentsOfFile(const std::string& filename) {
    CAMap<CAObject*, CAObject*> FileMap;
    return FileMap;
}
bool CCFileUtils::writeToFile(CrossApp::CCDictionary *dict, const std::string &fullPath) {return NULL;}
CAVector<CAObject*> CCFileUtils::createCCArrayWithContentsOfFile(const std::string& filename) {
    CAVector<CAObject*> fileVec;
    return fileVec;
}

#endif /* (CC_TARGET_PLATFORM != CC_PLATFORM_IOS) && (CC_TARGET_PLATFORM != CC_PLATFORM_MAC) */


CCFileUtils* CCFileUtils::s_sharedFileUtils = NULL;

void CCFileUtils::purgeFileUtils()
{
    CC_SAFE_DELETE(s_sharedFileUtils);
}

CCFileUtils::CCFileUtils()
{
}

CCFileUtils::~CCFileUtils()
{
    m_pFilenameLookupDict.clear();
}

bool CCFileUtils::init()
{
    m_searchPathArray.push_back(m_strDefaultResRootPath);
    m_searchResolutionsOrderArray.push_back("");
    return true;
}

void CCFileUtils::purgeCachedEntries()
{
    m_fullPathCache.clear();
}

unsigned char* CCFileUtils::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
{
    unsigned char * pBuffer = NULL;
    CCAssert(pszFileName != NULL && pSize != NULL && pszMode != NULL, "Invalid parameters.");
    *pSize = 0;
    do
    {
        // read the file from hardware
        std::string fullPath = fullPathForFilename(pszFileName);
        FILE *fp = fopen(fullPath.c_str(), pszMode);
        CC_BREAK_IF(!fp);
        
        fseek(fp,0,SEEK_END);
        *pSize = ftell(fp);
        fseek(fp,0,SEEK_SET);
        pBuffer = new unsigned char[*pSize];
        *pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
        fclose(fp);
    } while (0);
    
    if (! pBuffer)
    {
        std::string msg = "Get data from file(";
        msg.append(pszFileName).append(") failed!");
        
        CCLOG("%s", msg.c_str());
    }
    return pBuffer;
}

unsigned char* CCFileUtils::getFileDataFromZip(const char* pszZipFilePath, const char* pszFileName, unsigned long * pSize)
{
    unsigned char * pBuffer = NULL;
    unzFile pFile = NULL;
    *pSize = 0;

    do 
    {
        CC_BREAK_IF(!pszZipFilePath || !pszFileName);
        CC_BREAK_IF(strlen(pszZipFilePath) == 0);

        pFile = unzOpen(pszZipFilePath);
        CC_BREAK_IF(!pFile);

        int nRet = unzLocateFile(pFile, pszFileName, 1);
        CC_BREAK_IF(UNZ_OK != nRet);

        char szFilePathA[260];
        unz_file_info FileInfo;
        nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
        CC_BREAK_IF(UNZ_OK != nRet);

        nRet = unzOpenCurrentFile(pFile);
        CC_BREAK_IF(UNZ_OK != nRet);

        pBuffer = new unsigned char[FileInfo.uncompressed_size];
        int CC_UNUSED nSize = unzReadCurrentFile(pFile, pBuffer, FileInfo.uncompressed_size);
        CCAssert(nSize == 0 || nSize == (int)FileInfo.uncompressed_size, "the file size is wrong");

        *pSize = FileInfo.uncompressed_size;
        unzCloseCurrentFile(pFile);
    } while (0);

    if (pFile)
    {
        unzClose(pFile);
    }

    return pBuffer;
}

std::string CCFileUtils::getFileString(const char* pszFilePath)
{
    std::string data;
    
    unsigned long pSize = 0;
    unsigned char* pData = this->getFileData(pszFilePath, "rb", &pSize);
    
    data.resize(pSize, sizeof(char));
    for (int i=0; i<data.size(); i++)
    {
        data[i] = pData[i];
    }
    data += '\0';
    
    return data;
}

std::string CCFileUtils::getNewFilename(const std::string& pszFileName)
{
    CCString* pszNewFileName = (CCString*)m_pFilenameLookupDict.getValue(CCString::create(pszFileName));
    m_pFilenameLookupDict.getValue(CCString::create(pszFileName));
    if(NULL == pszNewFileName || pszNewFileName->length() == 0)
    {
        return pszFileName;
    }
    return pszNewFileName->getCString();
}

std::string CCFileUtils::getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath)
{
    std::string file = filename;
    std::string file_path = "";
    size_t pos = filename.find_last_of("/");
    if (pos != std::string::npos)
    {
        file_path = filename.substr(0, pos+1);
        file = filename.substr(pos+1);
    }
    
    // searchPath + file_path + resourceDirectory
    std::string path = searchPath;
    path += file_path;
    path += resolutionDirectory;
    
    path = getFullPathForDirectoryAndFilename(path, file);
    
    return path;
}


std::string CCFileUtils::fullPathForFilename(const std::string& pszFileName)
{
    std::string strFileName = std::string(pszFileName);
    if (isAbsolutePath(pszFileName))
    {
        //CCLOG("Return absolute path( %s ) directly.", pszFileName);
        return pszFileName;
    }
    // Already Cached ?
    std::map<std::string, std::string>::iterator cacheIter = m_fullPathCache.find(pszFileName);
    if (cacheIter != m_fullPathCache.end())
    {
        //CCLOG("Return full path from cache: %s", cacheIter->second.c_str());
        return cacheIter->second;
    }
    // Get the new file name.
    std::string newFilename = getNewFilename(pszFileName);
    
    string fullpath = "";
    
    std::vector<std::string>::iterator searchPathsIter;
    for (searchPathsIter = m_searchPathArray.begin();
         searchPathsIter != m_searchPathArray.end();
         ++searchPathsIter)
    {
        std::vector<std::string>::iterator resOrderIter;
        for (resOrderIter = m_searchResolutionsOrderArray.begin();
             resOrderIter != m_searchResolutionsOrderArray.end();
             ++resOrderIter)
        {
            fullpath = this->getPathForFilename(newFilename, *resOrderIter, *searchPathsIter);
            
            if (fullpath.length() > 0)
            {
                // Using the filename passed in as key.
                m_fullPathCache.insert(std::pair<std::string, std::string>(pszFileName, fullpath));
                return fullpath;
            }
        }
    }
    
    return pszFileName;
}

const char* CCFileUtils::fullPathFromRelativeFile(const char *pszFilename, const char *pszRelativeFile)
{
    std::string relativeFile = pszRelativeFile;
    CCString *pRet = CCString::create("");
    pRet->m_sString = relativeFile.substr(0, relativeFile.rfind('/')+1);
    pRet->m_sString += getNewFilename(pszFilename);
    return pRet->getCString();
}

void CCFileUtils::setSearchResolutionsOrder(const std::vector<std::string>& searchResolutionsOrder)
{
    bool bExistDefault = false;
    m_fullPathCache.clear();
    m_searchResolutionsOrderArray.clear();
    for (std::vector<std::string>::const_iterator iter = searchResolutionsOrder.begin(); iter != searchResolutionsOrder.end(); ++iter)
    {
        std::string resolutionDirectory = *iter;
        if (!bExistDefault && resolutionDirectory == "")
        {
            bExistDefault = true;
        }
        
        if (resolutionDirectory.length() > 0 && resolutionDirectory[resolutionDirectory.length()-1] != '/')
        {
            resolutionDirectory += "/";
        }
        
        m_searchResolutionsOrderArray.push_back(resolutionDirectory);
    }
    if (!bExistDefault)
    {
        m_searchResolutionsOrderArray.push_back("");
    }
}

void CCFileUtils::addSearchResolutionsOrder(const char* order)
{
    m_searchResolutionsOrderArray.push_back(order);
}

const std::vector<std::string>& CCFileUtils::getSearchResolutionsOrder()
{
    return m_searchResolutionsOrderArray;
}

const std::vector<std::string>& CCFileUtils::getSearchPaths()
{
    return m_searchPathArray;
}

void CCFileUtils::setSearchPaths(const std::vector<std::string>& searchPaths)
{
    bool bExistDefaultRootPath = false;

    m_fullPathCache.clear();
    m_searchPathArray.clear();
    for (std::vector<std::string>::const_iterator iter = searchPaths.begin(); iter != searchPaths.end(); ++iter)
    {
        std::string strPrefix;
        std::string path;
        if (!isAbsolutePath(*iter))
        { // Not an absolute path
            strPrefix = m_strDefaultResRootPath;
        }
        path = strPrefix+(*iter);
        if (path.length() > 0 && path[path.length()-1] != '/')
        {
            path += "/";
        }
        if (!bExistDefaultRootPath && path == m_strDefaultResRootPath)
        {
            bExistDefaultRootPath = true;
        }
        m_searchPathArray.push_back(path);
    }
    
    if (!bExistDefaultRootPath)
    {
        //CCLOG("Default root path doesn't exist, adding it.");
        m_searchPathArray.push_back(m_strDefaultResRootPath);
    }
}

void CCFileUtils::addSearchPath(const char* path_)
{
    std::string strPrefix;
    std::string path(path_);
    if (!isAbsolutePath(path))
    { // Not an absolute path
        strPrefix = m_strDefaultResRootPath;
    }
    path = strPrefix + path;
    if (path.length() > 0 && path[path.length()-1] != '/')
    {
        path += "/";
    }
    m_searchPathArray.push_back(path);
}

void CCFileUtils::removeSearchPath(const char *path_)
{
	std::string strPrefix;
	std::string path(path_);
	if (!isAbsolutePath(path))
	{ // Not an absolute path
		strPrefix = m_strDefaultResRootPath;
	}
	path = strPrefix + path;
	if (path.length() > 0 && path[path.length()-1] != '/')
	{
		path += "/";
	}
	std::vector<std::string>::iterator iter = std::find(m_searchPathArray.begin(), m_searchPathArray.end(), path);
	m_searchPathArray.erase(iter);
}

void CCFileUtils::removeAllPaths()
{
	m_searchPathArray.clear();
}
void CCFileUtils::setFilenameLookupDictionary(CAMap<CAObject*, CAObject*> &pFilenameLookupDict)
{
    m_fullPathCache.clear();
    m_pFilenameLookupDict.clear();
    m_pFilenameLookupDict = pFilenameLookupDict;
}

void CCFileUtils::loadFilenameLookupDictionaryFromFile(const char* filename)
{
    std::string fullPath = this->fullPathForFilename(filename);
    if (fullPath.length() > 0)
    {
        CAMap<CAObject*, CAObject*> pDict = CCFileUtils::sharedFileUtils()->createCCDictionaryWithContentsOfFile(fullPath.c_str());
        if (!pDict.empty())
        {
            CAObject* mapObj = pDict.getValue(CCString::create("metadata"));
            CAMap<CAObject*, CAObject*>* pMetadata = (CAMap<CAObject*, CAObject*>*) (mapObj);
            int version = ((CCString*)pMetadata->getValue(CCString::create("version")))->intValue();
            if (version != 1)
            {
                CCLOG("CrossApp: ERROR: Invalid filenameLookup dictionary version: %ld. Filename: %s", (long)version, filename);
                return;
            }
            CAMap<CAObject*, CAObject*>* mapTmp = (CAMap<CAObject*, CAObject*>*) (pDict.getValue(CCString::create("filenames")));
            setFilenameLookupDictionary(*mapTmp);
        }
    }
}

std::string CCFileUtils::getFullPathForDirectoryAndFilename(const std::string& strDirectory, const std::string& strFilename)
{
    std::string ret = strDirectory;
    if (strDirectory.size() && strDirectory[strDirectory.size()-1] != '/')
    {
        ret += '/';
    }
    ret += strFilename;
    
    if (!isFileExist(ret)) {
        ret = "";
    }
    return ret;
}

bool CCFileUtils::isAbsolutePath(const std::string& strPath)
{
    return strPath[0] == '/' ? true : false;
}

//////////////////////////////////////////////////////////////////////////
// Notification support when getFileData from invalid file path.
//////////////////////////////////////////////////////////////////////////
static bool s_bPopupNotify = true;

void CCFileUtils::setPopupNotify(bool bNotify)
{
    s_bPopupNotify = bNotify;
}

bool CCFileUtils::isPopupNotify()
{
    return s_bPopupNotify;
}

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#endif
/*
 * Create a direcotry is platform depended.
 */
bool CCFileUtils::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    DIR *dir=NULL;
    dir = opendir(path);
    if (dir)
    {
        closedir(dir);
        return false;
    }
    
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    return true;
#else
    BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
    return true;
#endif
}


NS_CC_END

