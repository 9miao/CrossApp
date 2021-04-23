#ifndef __CCSAXPARSER_H__
#define __CCSAXPARSER_H__
/// @cond DO_NOT_SHOW

#include "platform/CCPlatformConfig.h"
#include "platform/CACommon.h"
#include <string>

NS_CC_BEGIN

/**
 * @addtogroup platform
 * @{
 */

typedef unsigned char CC_XML_CHAR;

class CC_DLL SAXDelegator
{
public:
    virtual ~SAXDelegator() {}

    /**
     * @js NA
     * @lua NA
     */
    virtual void startElement(void *ctx, const char *name, const char **atts) = 0;
    /**
     * @js NA
     * @lua NA
     */
    virtual void endElement(void *ctx, const char *name) = 0;
    /**
     * @js NA
     * @lua NA
     */
    virtual void textHandler(void *ctx, const char *s, int len) = 0;
};

class CC_DLL SAXParser
{
    SAXDelegator*    _delegator;
public:
    /**
     * @js NA
     * @lua NA
     */
    SAXParser();
    /**
     * @js NA
     * @lua NA
     */
    ~SAXParser(void);
    /**
     * @js NA
     * @lua NA
     */
    bool init(const char *encoding);
    /**
     * @js NA
     * @lua NA
     */
    bool parse(const char* xmlData, size_t dataLength);
    /**
     * @js NA
     * @lua NA
     */
    bool parse(const std::string& filename);
    /**
     * @js NA
     * @lua NA
     */
    void setDelegator(SAXDelegator* delegator);
    /**
     * @js NA
     * @lua NA
     */
    static void startElement(void *ctx, const CC_XML_CHAR *name, const CC_XML_CHAR **atts);
    /**
     * @js NA
     * @lua NA
     */
    static void endElement(void *ctx, const CC_XML_CHAR *name);
    /**
     * @js NA
     * @lua NA
     */
    static void textHandler(void *ctx, const CC_XML_CHAR *name, int len);
};

// end of platform group
/// @}

NS_CC_END

/// @endcond
#endif //__CCSAXPARSER_H__
