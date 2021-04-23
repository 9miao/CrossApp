
#include "CAUserDefault.h"
#include "platform/CCPlatformConfig.h"
#include "platform/CACommon.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/Java_org_CrossApp_lib_CrossAppHelper.h"

// root name of xml
#define USERDEFAULT_ROOT_NAME    "userDefaultRoot"

#define KEEP_COMPATABILITY

#define XML_FILE_NAME "UserDefault.xml"

#ifdef KEEP_COMPATABILITY
#include "platform/CAFileUtils.h"
#include "../tinyxml2/tinyxml2.h"
#endif

using namespace std;

NS_CC_BEGIN

/**
 * implements of CAUserDefault
 */

CAUserDefault* CAUserDefault::m_spUserDefault = 0;
string CAUserDefault::m_sFilePath = string("");
bool CAUserDefault::m_sbIsFilePathInitialized = false;

#ifdef KEEP_COMPATABILITY
static tinyxml2::XMLElement* getXMLNodeForKey(const char* pKey, tinyxml2::XMLDocument **doc)
{
    tinyxml2::XMLElement* curNode = NULL;
    tinyxml2::XMLElement* rootNode = NULL;
    
    if (! CAUserDefault::isXMLFileExist())
    {
        return NULL;
    }
    
    // check the key value
    if (! pKey)
    {
        return NULL;
    }
    
    do
    {
        tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
        *doc = xmlDoc;
        unsigned long nSize;
        const char* pXmlBuffer = (const char*)FileUtils::getInstance()->getFileData(CAUserDefault::getInstance()->getXMLFilePath().c_str(), "rb", &nSize);
        //const char* pXmlBuffer = (const char*)data.getBuffer();
        if(NULL == pXmlBuffer)
        {
            CCLOG("can not read xml file");
            break;
        }
        xmlDoc->Parse(pXmlBuffer);
		delete[] pXmlBuffer;
        // get root node
        rootNode = xmlDoc->RootElement();
        if (NULL == rootNode)
        {
            CCLOG("read root node error");
            break;
        }
        // find the node
        curNode = rootNode->FirstChildElement();
        if (!curNode)
        {
            // There is not xml node, delete xml file.
            remove(CAUserDefault::getInstance()->getXMLFilePath().c_str());
            
            return NULL;
        }
        
        while (NULL != curNode)
        {
            const char* nodeName = curNode->Value();
            if (!strcmp(nodeName, pKey))
            {
                // delete the node
                break;
            }
            
            curNode = curNode->NextSiblingElement();
        }
    } while (0);
    
    return curNode;
}

static void deleteNode(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* node)
{
    if (node)
    {
        doc->DeleteNode(node);
        doc->SaveFile(CAUserDefault::getInstance()->getXMLFilePath().c_str());
        delete doc;
    }
}

static void deleteNodeByKey(const char *pKey)
{
    tinyxml2::XMLDocument* doc = NULL;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    deleteNode(doc, node);
}
#endif

/**
 * If the user invoke delete CAUserDefault::getInstance(), should set m_spUserDefault
 * to null to avoid error when he invoke CAUserDefault::getInstance() later.
 */
CAUserDefault::~CAUserDefault()
{
	CC_SAFE_DELETE(m_spUserDefault);
    m_spUserDefault = NULL;
}

CAUserDefault::CAUserDefault()
{
	m_spUserDefault = NULL;
}

void CAUserDefault::destroyInstance()
{
    m_spUserDefault = NULL;
}

 bool CAUserDefault::getBoolForKey(const char* pKey)
 {
     return getBoolForKey(pKey, false);
 }

bool CAUserDefault::getBoolForKey(const char* pKey, bool defaultValue)
{
#ifdef KEEP_COMPATABILITY
    tinyxml2::XMLDocument* doc = NULL;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    if (node)
    {
        if (node->FirstChild())
        {
            const char* value = (const char*)node->FirstChild()->Value();
            bool ret = (! strcmp(value, "true"));
            
            // set value in NSUserDefaults
            setBoolForKey(pKey, ret);
            flush();
            
            // delete xmle node
            deleteNode(doc, node);
            
            return ret;
        }
        else
        {
            // delete xmle node
            deleteNode(doc, node);
        }
    }
#endif

    return getBoolForKeyJNI(pKey, defaultValue);
}

int CAUserDefault::getIntegerForKey(const char* pKey)
{
    return getIntegerForKey(pKey, 0);
}

int CAUserDefault::getIntegerForKey(const char* pKey, int defaultValue)
{
#ifdef KEEP_COMPATABILITY
    tinyxml2::XMLDocument* doc = NULL;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    if (node)
    {
        if (node->FirstChild())
        {
            int ret = atoi((const char*)node->FirstChild()->Value());
            
            // set value in NSUserDefaults
            setIntegerForKey(pKey, ret);
            flush();
            
            // delete xmle node
            deleteNode(doc, node);
            
            return ret;
        }
        else
        {
            // delete xmle node
            deleteNode(doc, node);
        }
    }
#endif
    
	return getIntegerForKeyJNI(pKey, defaultValue);
}

float CAUserDefault::getFloatForKey(const char* pKey)
{
    return getFloatForKey(pKey, 0.0f);
}

float CAUserDefault::getFloatForKey(const char* pKey, float defaultValue)
{
#ifdef KEEP_COMPATABILITY
    tinyxml2::XMLDocument* doc = NULL;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    if (node)
    {
        if (node->FirstChild())
        {
            float ret = atof((const char*)node->FirstChild()->Value());
            
            // set value in NSUserDefaults
            setFloatForKey(pKey, ret);
            flush();
            
            // delete xmle node
            deleteNode(doc, node);
            
            return ret;
        }
        else
        {
            // delete xmle node
            deleteNode(doc, node);
        }
    }
#endif

    return getFloatForKeyJNI(pKey, defaultValue);
}

double  CAUserDefault::getDoubleForKey(const char* pKey)
{
    return getDoubleForKey(pKey, 0.0);
}

double CAUserDefault::getDoubleForKey(const char* pKey, double defaultValue)
{
#ifdef KEEP_COMPATABILITY
    tinyxml2::XMLDocument* doc = NULL;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    if (node)
    {
        if (node->FirstChild())
        {
            double ret = atof((const char*)node->FirstChild()->Value());
            
            // set value in NSUserDefaults
            setDoubleForKey(pKey, ret);
            flush();
            
            // delete xmle node
            deleteNode(doc, node);
            
            return ret;
        }
        else
        {
            // delete xmle node
            deleteNode(doc, node);
        }
    }
#endif

	return getDoubleForKeyJNI(pKey, defaultValue);
}

std::string CAUserDefault::getStringForKey(const char* pKey)
{
    return getStringForKey(pKey, "");
}

string CAUserDefault::getStringForKey(const char* pKey, const std::string & defaultValue)
{
#ifdef KEEP_COMPATABILITY
    tinyxml2::XMLDocument* doc = NULL;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    if (node)
    {
        if (node->FirstChild())
        {
            string ret = (const char*)node->FirstChild()->Value();
            
            // set value in NSUserDefaults
            setStringForKey(pKey, ret);
            flush();
            
            // delete xmle node
            deleteNode(doc, node);
            
            return ret;
        }
        else
        {
            // delete xmle node
            deleteNode(doc, node);
        }
    }
#endif

    return getStringForKeyJNI(pKey, defaultValue.c_str());
}

void CAUserDefault::setBoolForKey(const char* pKey, bool value)
{
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif

    return setBoolForKeyJNI(pKey, value);
}

void CAUserDefault::setIntegerForKey(const char* pKey, int value)
{
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif

    return setIntegerForKeyJNI(pKey, value);
}

void CAUserDefault::setFloatForKey(const char* pKey, float value)
{
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif

    return setFloatForKeyJNI(pKey, value);
}

void CAUserDefault::setDoubleForKey(const char* pKey, double value)
{
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif

    return setDoubleForKeyJNI(pKey, value);
}

void CAUserDefault::setStringForKey(const char* pKey, const std::string & value)
{
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif

    return setStringForKeyJNI(pKey, value.c_str());
}

CAUserDefault* CAUserDefault::getInstance()
{
#ifdef KEEP_COMPATABILITY
    initXMLFilePath();
#endif
    
    if (! m_spUserDefault)
    {
        m_spUserDefault = new CAUserDefault();
    }

    return m_spUserDefault;
}

bool CAUserDefault::isXMLFileExist()
{
    FILE *fp = fopen(m_sFilePath.c_str(), "r");
    bool bRet = false;
    
    if (fp)
    {
        bRet = true;
        fclose(fp);
    }
    
    return bRet;
}

void CAUserDefault::initXMLFilePath()
{
#ifdef KEEP_COMPATABILITY
    if (! m_sbIsFilePathInitialized)
    {
        // UserDefault.xml is stored in /data/data/<package-path>/ before v2.1.2
        m_sFilePath += "/data/data/" + getPackageNameJNI() + "/" + XML_FILE_NAME;
        m_sbIsFilePathInitialized = true;
    }
#endif
}

// create new xml file
bool CAUserDefault::createXMLFile()
{
    return false;
}

const string& CAUserDefault::getXMLFilePath()
{
    return m_sFilePath;
}

void CAUserDefault::flush()
{
}

NS_CC_END

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
