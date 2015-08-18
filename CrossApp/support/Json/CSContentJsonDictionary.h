

#ifndef COCOSTUDIO_CSCONTENTJSONDICTIONARY_H
#define COCOSTUDIO_CSCONTENTJSONDICTIONARY_H

#include "CrossApp.h"
#include "lib_json/json_lib.h"
#include <vector>
#include <string>

typedef enum _DicItemType
{
    EDIC_TYPENULL = 0,
    EDIC_TYPEINT,
    EDIC_TYPEUINT,
    EDIC_TYPEFLOAT,
    EDIC_TYPESTRING,
    EDIC_TYPEBOOLEN,
    EDIC_TYPEARRAY,
    EDIC_TYPEOBJECT
}DicItemType;


class CC_DLL CSJsonDictionary
{
public:
    CSJsonDictionary();
    ~CSJsonDictionary();

public:
    void    initWithDescription(const char *pszDescription);
	void	initWithValue(CSJson::Value& value);

    void    insertItem(const char *pszKey, int nValue);
    void    insertItem(const char *pszKey, double fValue);
    void    insertItem(const char *pszKey, const char * pszValue);
    void    insertItem(const char *pszKey, CSJsonDictionary * subDictionary);
    void    insertItem(const char *pszKey, bool bValue);
    bool    deleteItem(const char *pszKey);
    void    cleanUp();
    bool    isKeyValidate(const char *pszKey);

    int             getItemIntValue(const char *pszKey, int nDefaultValue);
    double          getItemFloatValue(const char *pszKey, double fDefaultValue);
    const char *    getItemStringValue(const char *pszKey);
    bool            getItemBoolvalue(const char *pszKey, bool bDefaultValue);
    CSJsonDictionary *   getSubDictionary(const char *pszKey);

    std::string          getDescription();

    bool    insertItemToArray(const char *pszArrayKey, int nValue);
    bool    insertItemToArray(const char *pszArrayKey, double fValue);
    bool    insertItemToArray(const char *pszArrayKey, const char * pszValue);
    bool    insertItemToArray(const char *pszArrayKey, CSJsonDictionary * subDictionary);

    int getArrayItemCount(const char *pszArrayKey);
    int getIntValueFromArray(const char *pszArrayKey, int nIndex, int nDefaultValue);
    double getFloatValueFromArray(const char *pszArrayKey, int nIndex, double fDefaultValue);
    bool getBoolValueFromArray(const char *pszArrayKey, int nIndex, bool bDefaultValue);
    const char * getStringValueFromArray(const char *pszArrayKey, int nIndex);
    CSJsonDictionary *getSubItemFromArray(const char *pszArrayKey, int nIndex);
    DicItemType getItemTypeFromArray(const char *pszArrayKey, int nIndex);

    int         getItemCount();
    DicItemType getItemType(int nIndex);
    DicItemType getItemType(const char *pszKey);
    std::vector<std::string> getAllMemberNames();

protected:
    CSJson::Value m_cValue;

private:
    inline bool isKeyValidate(const char *pszKey, CSJson::Value& root);
    inline CSJson::Value * validateArrayItem(const char *pszArrayKey, int nIndex);
};

#endif
