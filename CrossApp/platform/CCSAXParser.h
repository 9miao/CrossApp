

#ifndef __CCSAXPARSER_H__
#define __CCSAXPARSER_H__

#include "CCPlatformConfig.h"
#include "CCCommon.h"

NS_CC_BEGIN

/**
 * @addtogroup platform
 * @{
 */

typedef unsigned char CC_XML_CHAR;

/**
 * @js NA
 * @lua NA
 */
class CC_DLL CCSAXDelegator
{
public:
    virtual void startElement(void *ctx, const char *name, const char **atts) = 0;
    virtual void endElement(void *ctx, const char *name) = 0;
    virtual void textHandler(void *ctx, const char *s, int len) = 0;
};

/**
 * @js NA
 * @lua NA
 */
class CC_DLL CCSAXParser
{
    CCSAXDelegator*    m_pDelegator;
public:

    CCSAXParser();
    ~CCSAXParser(void);

    bool init(const char *pszEncoding);
    bool parse(const char* pXMLData, unsigned int uDataLength);
    bool parse(const char *pszFile);
    void setDelegator(CCSAXDelegator* pDelegator);

    static void startElement(void *ctx, const CC_XML_CHAR *name, const CC_XML_CHAR **atts);
    static void endElement(void *ctx, const CC_XML_CHAR *name);
    static void textHandler(void *ctx, const CC_XML_CHAR *name, int len);
};

// end of platform group
/// @}

NS_CC_END

#endif //__CCSAXPARSER_H__
