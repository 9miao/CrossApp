

#include "platform/CASAXParser.h"
#include "platform/CAFileUtils.h"
#include "support/tinyxml2/tinyxml2.h"
#include <vector> // because its based on windows 8 build :P


NS_CC_BEGIN

class XmlSaxHander : public tinyxml2::XMLVisitor
{
public:
    XmlSaxHander():_ccsaxParserImp(0){};
    
    virtual bool VisitEnter( const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute );
    virtual bool VisitExit( const tinyxml2::XMLElement& element );
    virtual bool Visit( const tinyxml2::XMLText& text );
    virtual bool Visit( const tinyxml2::XMLUnknown&){ return true; }

    void setSAXParserImp(SAXParser* parser)
    {
        _ccsaxParserImp = parser;
    }

private:
    SAXParser *_ccsaxParserImp;
};


bool XmlSaxHander::VisitEnter( const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute )
{
    //log(" VisitEnter %s",element.Value());

    std::vector<const char*> attsVector;
    for( const tinyxml2::XMLAttribute* attrib = firstAttribute; attrib; attrib = attrib->Next() )
    {
        //log("%s", attrib->Name());
        attsVector.push_back(attrib->Name());
        //log("%s",attrib->Value());
        attsVector.push_back(attrib->Value());
    }
    
    // nullptr is used in c++11
    //attsVector.push_back(nullptr);
    attsVector.push_back(nullptr);

    SAXParser::startElement(_ccsaxParserImp, (const CC_XML_CHAR *)element.Value(), (const CC_XML_CHAR **)(&attsVector[0]));
    return true;
}
bool XmlSaxHander::VisitExit( const tinyxml2::XMLElement& element )
{
    //log("VisitExit %s",element.Value());

    SAXParser::endElement(_ccsaxParserImp, (const CC_XML_CHAR *)element.Value());
    return true;
}

bool XmlSaxHander::Visit( const tinyxml2::XMLText& text )
{
    //log("Visit %s",text.Value());
    SAXParser::textHandler(_ccsaxParserImp, (const CC_XML_CHAR *)text.Value(), static_cast<int>(strlen(text.Value())));
    return true;
}

SAXParser::SAXParser()
{
    _delegator = nullptr;
}

SAXParser::~SAXParser(void)
{
}

bool SAXParser::init(const char *encoding)
{
    CC_UNUSED_PARAM(encoding);
    // nothing to do
    return true;
}

bool SAXParser::parse(const char* xmlData, size_t dataLength)
{
    tinyxml2::XMLDocument tinyDoc;
    tinyDoc.Parse(xmlData, dataLength);
    XmlSaxHander printer;
    printer.setSAXParserImp(this);
    
    return tinyDoc.Accept( &printer );  
}

bool SAXParser::parse(const std::string& filename)
{
    bool ret = false;
    std::string data = FileUtils::getInstance()->getFileString(filename);
    if (!data.empty())
    {
        ret = parse(data.c_str(), data.length());
    }

    return ret;
}

void SAXParser::startElement(void *ctx, const CC_XML_CHAR *name, const CC_XML_CHAR **atts)
{
    ((SAXParser*)(ctx))->_delegator->startElement(ctx, (char*)name, (const char**)atts);
}

void SAXParser::endElement(void *ctx, const CC_XML_CHAR *name)
{
    ((SAXParser*)(ctx))->_delegator->endElement(ctx, (char*)name);
}
void SAXParser::textHandler(void *ctx, const CC_XML_CHAR *name, int len)
{
    ((SAXParser*)(ctx))->_delegator->textHandler(ctx, (char*)name, len);
}
void SAXParser::setDelegator(SAXDelegator* delegator)
{
    _delegator = delegator;
}

NS_CC_END


