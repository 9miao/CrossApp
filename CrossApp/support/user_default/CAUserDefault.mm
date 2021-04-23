

#import "CAUserDefault.h"
#import <string>
#import "platform/CAFileUtils.h"
#import "../tinyxml2/tinyxml2.h"
#import "platform/CCPlatformConfig.h"
#import "platform/CCPlatformMacros.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)


#define XML_FILE_NAME "UserDefault.xml"

// root name of xml
#define USERDEFAULT_ROOT_NAME    "userDefaultRoot"

#define KEEP_COMPATABILITY



NS_CC_BEGIN

/**
 * implements of CAUserDefault
 */

CAUserDefault* CAUserDefault::m_spUserDefault = 0;
std::string CAUserDefault::m_sFilePath;
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
            NSLog(@"can not read xml file");
			break;
		}
		xmlDoc->Parse(pXmlBuffer);
        delete[] pXmlBuffer;
		// get root node
		rootNode = xmlDoc->RootElement();
		if (NULL == rootNode)
		{
            NSLog(@"read root node error");
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
 * If the user invoke delete CAUserDefault::sharedUserDefault(), should set m_spUserDefault
 * to null to avoid error when he invoke CAUserDefault::sharedUserDefault() later.
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
    
    bool ret = defaultValue;
    
    NSNumber *value = [[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithUTF8String:pKey]];
    if (value)
    {
        ret = [value boolValue];
    }
    
    return ret;
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
    
    int ret = defaultValue;
    
    NSNumber *value = [[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithUTF8String:pKey]];
    if (value)
    {
        ret = [value intValue];
    }
    
    return ret;
}

float CAUserDefault::getFloatForKey(const char* pKey)
{
    return getFloatForKey(pKey, 0);
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
    
    float ret = defaultValue;
    
    NSNumber *value = [[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithUTF8String:pKey]];
    if (value)
    {
        ret = [value floatValue];
    }
    
    return ret;
}

double  CAUserDefault::getDoubleForKey(const char* pKey)
{
    return getDoubleForKey(pKey, 0);
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
    
	double ret = defaultValue;
    
    NSNumber *value = [[NSUserDefaults standardUserDefaults] objectForKey:[NSString stringWithUTF8String:pKey]];
    if (value)
    {
        ret = [value doubleValue];
    }
    
    return ret;
}

std::string CAUserDefault::getStringForKey(const char* pKey)
{
    return getStringForKey(pKey, "");
}

std::string CAUserDefault::getStringForKey(const char* pKey, const std::string & defaultValue)
{
#ifdef KEEP_COMPATABILITY
    tinyxml2::XMLDocument* doc = NULL;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    if (node)
    {
        if (node->FirstChild())
        {
            std::string ret = (const char*)node->FirstChild()->Value();
            
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
    
    NSString *str = [[NSUserDefaults standardUserDefaults] stringForKey:[NSString stringWithUTF8String:pKey]];
    if (! str || [str isEqualToString:@"null\n"])
    {
        return defaultValue;
    }
    else
    {
        return [str UTF8String];
    }
}

void CAUserDefault::setBoolForKey(const char* pKey, bool value)
{
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif
    
    [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithBool:value] forKey:[NSString stringWithUTF8String:pKey]];
}

void CAUserDefault::setIntegerForKey(const char* pKey, int value)
{
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif
    
    [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithInt:value] forKey:[NSString stringWithUTF8String:pKey]];
}

void CAUserDefault::setFloatForKey(const char* pKey, float value)
{
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif
    
    [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithFloat:value] forKey:[NSString stringWithUTF8String:pKey]];
}

void CAUserDefault::setDoubleForKey(const char* pKey, double value)
{
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif
    
    [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithDouble:value] forKey:[NSString stringWithUTF8String:pKey]];
}

void CAUserDefault::setStringForKey(const char* pKey, const std::string & value)
{
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif
    
    [[NSUserDefaults standardUserDefaults] setObject:[NSString stringWithUTF8String:value.c_str()] forKey:[NSString stringWithUTF8String:pKey]];
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
    FILE *fp = fopen(m_sFilePath.c_str(), "rb");
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
        // xml file is stored in cache directory before 2.1.2
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        m_sFilePath = [documentsDirectory UTF8String];
        m_sFilePath.append("/");
        
        m_sFilePath +=  XML_FILE_NAME;
        m_sbIsFilePathInitialized = true;
    }
#endif
}

// create new xml file
bool CAUserDefault::createXMLFile()
{
    return false;
}

const std::string& CAUserDefault::getXMLFilePath()
{
    return m_sFilePath;
}

void CAUserDefault::flush()
{
    [[NSUserDefaults standardUserDefaults] synchronize];
}


NS_CC_END

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
