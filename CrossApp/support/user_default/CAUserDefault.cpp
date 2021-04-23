
#include "CAUserDefault.h"
#include "platform/CACommon.h"
#include "platform/CAFileUtils.h"
#include "../tinyxml2/tinyxml2.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)

// root name of xml
#define USERDEFAULT_ROOT_NAME    "userDefaultRoot"

#define XML_FILE_NAME "UserDefault.xml"

using namespace std;

NS_CC_BEGIN

/**
 * define the functions here because we don't want to
 * export xmlNodePtr and other types in "CAUserDefault.h"
 */

static tinyxml2::XMLElement* getXMLNodeForKey(const char* pKey, tinyxml2::XMLElement** rootNode, tinyxml2::XMLDocument **doc)
{
    tinyxml2::XMLElement* curNode = NULL;

    // check the key value
    if (! pKey)
    {
        return NULL;
    }

    do 
    {
 		tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
		*doc = xmlDoc;
		//CCFileData data(CAUserDefault::getInstance()->getXMLFilePath().c_str(),"rt");
		unsigned long nSize;
		const char* pXmlBuffer = (const char*)FileUtils::getInstance()->getFileData(CAUserDefault::getInstance()->getXMLFilePath().c_str(), "rb", &nSize);
		//const char* pXmlBuffer = (const char*)data.getBuffer();
		if(NULL == pXmlBuffer)
		{
			CCLOG("can not read xml file");
			break;
		}
		xmlDoc->Parse(pXmlBuffer, nSize);
        delete[] pXmlBuffer;
		// get root node
		*rootNode = xmlDoc->RootElement();
		if (NULL == *rootNode)
		{
			CCLOG("read root node error");
			break;
		}
		// find the node
		curNode = (*rootNode)->FirstChildElement();
		while (NULL != curNode)
		{
			const char* nodeName = curNode->Value();
			if (!strcmp(nodeName, pKey))
			{
				break;
			}

			curNode = curNode->NextSiblingElement();
		}
	} while (0);

	return curNode;
}

static void setValueForKey(const char* pKey, const char* pValue)
{
 	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	// check the params
	if (! pKey || ! pValue)
	{
		return;
	}
	// find the node
	node = getXMLNodeForKey(pKey, &rootNode, &doc);
	// if node exist, change the content
	if (node)
	{
        if (node->FirstChild())
        {
            node->FirstChild()->SetValue(pValue);
        }
        else
        {
            tinyxml2::XMLText* content = doc->NewText(pValue);
            node->LinkEndChild(content);
        }
	}
	else
	{
		if (rootNode)
		{
			tinyxml2::XMLElement* tmpNode = doc->NewElement(pKey);//new tinyxml2::XMLElement(pKey);
			rootNode->LinkEndChild(tmpNode);
			tinyxml2::XMLText* content = doc->NewText(pValue);//new tinyxml2::XMLText(pValue);
			tmpNode->LinkEndChild(content);
		}	
	}

    // save file and free doc
	if (doc)
	{
		doc->SaveFile(CAUserDefault::getInstance()->getXMLFilePath().c_str());
		delete doc;
	}
}

/**
 * implements of CAUserDefault
 */

CAUserDefault* CAUserDefault::m_spUserDefault = 0;
string CAUserDefault::m_sFilePath = string("");
bool CAUserDefault::m_sbIsFilePathInitialized = false;

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
    const char* value = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}

	bool ret = defaultValue;

	if (value)
	{
		ret = (! strcmp(value, "true"));
	}

    if (doc) delete doc;

	return ret;
}

int CAUserDefault::getIntegerForKey(const char* pKey)
{
    return getIntegerForKey(pKey, 0);
}

int CAUserDefault::getIntegerForKey(const char* pKey, int defaultValue)
{
	const char* value = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}

	int ret = defaultValue;

	if (value)
	{
		ret = atoi(value);
	}

	if(doc)
	{
		delete doc;
	}


	return ret;
}

float CAUserDefault::getFloatForKey(const char* pKey)
{
    return getFloatForKey(pKey, 0.0f);
}

float CAUserDefault::getFloatForKey(const char* pKey, float defaultValue)
{
    float ret = (float)getDoubleForKey(pKey, (double)defaultValue);
 
    return ret;
}

double  CAUserDefault::getDoubleForKey(const char* pKey)
{
    return getDoubleForKey(pKey, 0.0);
}

double CAUserDefault::getDoubleForKey(const char* pKey, double defaultValue)
{
	const char* value = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}

	double ret = defaultValue;

	if (value)
	{
		ret = atof(value);
	}

    if (doc) delete doc;

	return ret;
}

std::string CAUserDefault::getStringForKey(const char* pKey)
{
    return getStringForKey(pKey, "");
}

string CAUserDefault::getStringForKey(const char* pKey, const std::string & defaultValue)
{
    const char* value = NULL;
	tinyxml2::XMLElement* rootNode;
	tinyxml2::XMLDocument* doc;
	tinyxml2::XMLElement* node;
	node =  getXMLNodeForKey(pKey, &rootNode, &doc);
	// find the node
	if (node && node->FirstChild())
	{
        value = (const char*)(node->FirstChild()->Value());
	}

	string ret = defaultValue;

	if (value)
	{
		ret = string(value);
	}

    if (doc) delete doc;

	return ret;
}

void CAUserDefault::setBoolForKey(const char* pKey, bool value)
{
    // save bool value as string

    if (true == value)
    {
        setStringForKey(pKey, "true");
    }
    else
    {
        setStringForKey(pKey, "false");
    }
}

void CAUserDefault::setIntegerForKey(const char* pKey, int value)
{
    // check key
    if (! pKey)
    {
        return;
    }

    // format the value
    char tmp[50];
    memset(tmp, 0, 50);
    sprintf(tmp, "%d", value);

    setValueForKey(pKey, tmp);
}

void CAUserDefault::setFloatForKey(const char* pKey, float value)
{
    setDoubleForKey(pKey, value);
}

void CAUserDefault::setDoubleForKey(const char* pKey, double value)
{
    // check key
    if (! pKey)
    {
        return;
    }

    // format the value
    char tmp[50];
    memset(tmp, 0, 50);
    sprintf(tmp, "%f", value);

    setValueForKey(pKey, tmp);
}

void CAUserDefault::setStringForKey(const char* pKey, const std::string & value)
{
    // check key
    if (! pKey)
    {
        return;
    }

    setValueForKey(pKey, value.c_str());
}

CAUserDefault* CAUserDefault::getInstance()
{
    initXMLFilePath();

    // only create xml file one time
    // the file exists after the program exit
    if ((! isXMLFileExist()) && (! createXMLFile()))
    {
        return NULL;
    }

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
    if (! m_sbIsFilePathInitialized)
    {
        m_sFilePath += FileUtils::getInstance()->getWritablePath() + XML_FILE_NAME;
        m_sbIsFilePathInitialized = true;
    }    
}

// create new xml file
bool CAUserDefault::createXMLFile()
{
	bool bRet = false;  
    tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument(); 
    if (NULL==pDoc)  
    {  
        return false;  
    }  
	tinyxml2::XMLDeclaration *pDeclaration = pDoc->NewDeclaration(NULL);  
	if (NULL==pDeclaration)  
	{  
		return false;  
	}  
	pDoc->LinkEndChild(pDeclaration); 
	tinyxml2::XMLElement *pRootEle = pDoc->NewElement(USERDEFAULT_ROOT_NAME);  
	if (NULL==pRootEle)  
	{  
		return false;  
	}  
	pDoc->LinkEndChild(pRootEle);  
	bRet = tinyxml2::XML_SUCCESS == pDoc->SaveFile(m_sFilePath.c_str());

	if(pDoc)
	{
		delete pDoc;
	}

	return bRet;
}

const string& CAUserDefault::getXMLFilePath()
{
    return m_sFilePath;
}

void CAUserDefault::flush()
{
}

NS_CC_END

#endif // (CC_TARGET_PLATFORM != CC_PLATFORM_IOS && CC_PLATFORM != CC_PLATFORM_ANDROID)
