
#ifndef __Java_org_CrossApp_lib_CrossAppHelper_H__
#define __Java_org_CrossApp_lib_CrossAppHelper_H__

#include <string>

typedef void (*EditTextCallback)(const char* pText, void* ctx);

extern const char * getApkPath();
extern void terminateProcessJNI();
extern std::string getCurrentLanguageJNI();
extern std::string getPackageNameJNI();
extern std::string getFileDirectoryJNI();
extern void enableAccelerometerJNI();
extern void disableAccelerometerJNI();
extern void setAccelerometerIntervalJNI(float interval);
// functions for CCUserDefault
extern bool getBoolForKeyJNI(const char* pKey, bool defaultValue);
extern int getIntegerForKeyJNI(const char* pKey, int defaultValue);
extern float getFloatForKeyJNI(const char* pKey, float defaultValue);
extern double getDoubleForKeyJNI(const char* pKey, double defaultValue);
extern std::string getStringForKeyJNI(const char* pKey, const char* defaultValue);
extern void setBoolForKeyJNI(const char* pKey, bool value);
extern void setIntegerForKeyJNI(const char* pKey, int value);
extern void setFloatForKeyJNI(const char* pKey, float value);
extern void setDoubleForKeyJNI(const char* pKey, double value);
extern void setStringForKeyJNI(const char* pKey, const char* value);

#endif /* __Java_org_CrossApp_lib_CrossAppHelper_H__ */
