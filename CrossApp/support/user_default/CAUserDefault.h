
#ifndef __SUPPORT_CAUserDefault_H__
#define __SUPPORT_CAUserDefault_H__

#include "platform/CCPlatformMacros.h"
#include <string>

NS_CC_BEGIN

/**
 * @addtogroup data_storage
 * @{
 */

/**
 * CAUserDefault acts as a tiny database. You can save and get base type values by it.
 * For example, setBoolForKey("played", true) will add a bool value true into the database.
 * Its key is "played". You can get the value of the key by getBoolForKey("played").
 * 
 * It supports the following base types:
 * bool, int, float, double, string
 */
class CC_DLL CAUserDefault
{
public:
    ~CAUserDefault();

    // get value methods

    /**
    @brief Get bool value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is false.
    */
    bool    getBoolForKey(const char* pKey);
    bool    getBoolForKey(const char* pKey, bool defaultValue);
    /**
    @brief Get integer value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.
    */
    int     getIntegerForKey(const char* pKey);
    int     getIntegerForKey(const char* pKey, int defaultValue);
    /**
    @brief Get float value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.0f.
    */
    float    getFloatForKey(const char* pKey);
    float    getFloatForKey(const char* pKey, float defaultValue);
    /**
    @brief Get double value by key, if the key doesn't exist, a default value will return.
     You can set the default value, or it is 0.0.
    */
    double  getDoubleForKey(const char* pKey);
    double  getDoubleForKey(const char* pKey, double defaultValue);
    /**
    @brief Get string value by key, if the key doesn't exist, a default value will return.
    You can set the default value, or it is "".
    */
    std::string getStringForKey(const char* pKey);
    std::string getStringForKey(const char* pKey, const std::string & defaultValue);

    // set value methods

    /**
    @brief Set bool value by key.
    */
    void    setBoolForKey(const char* pKey, bool value);
    /**
    @brief Set integer value by key.
    */
    void    setIntegerForKey(const char* pKey, int value);
    /**
    @brief Set float value by key.
    */
    void    setFloatForKey(const char* pKey, float value);
    /**
    @brief Set double value by key.
    */
    void    setDoubleForKey(const char* pKey, double value);
    /**
    @brief Set string value by key.
    */
    void    setStringForKey(const char* pKey, const std::string & value);
    /**
     @brief Save content to xml file
     */
    void    flush();

    static CAUserDefault* getInstance();
    static void destroyInstance();
    const static std::string& getXMLFilePath();
    static bool isXMLFileExist();

private:
    CAUserDefault();
    static bool createXMLFile();
    static void initXMLFilePath();
    
    static CAUserDefault* m_spUserDefault;
    static std::string m_sFilePath;
    static bool m_sbIsFilePathInitialized;
};

// end of data_storage group
/// @}

NS_CC_END

#endif // __SUPPORT_CAUserDefault_H__
