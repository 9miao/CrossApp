#ifndef __CAValue__
#define __CAValue__

#include "platform/CCPlatformMacros.h"
#include "ccMacros.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "basics/CAObject.h"
/**
 * @addtogroup base
 * @{
 */

NS_CC_BEGIN

class CAValue;

typedef std::vector<CAValue> CAValueVector;
typedef std::map<std::string, CAValue> CAValueMap;
typedef std::map<int, CAValue> CAValueMapIntKey;

CC_DLL extern const CAValueVector CAValueVectorNull;
CC_DLL extern const CAValueMap CAValueMapNull;
CC_DLL extern const CAValueMapIntKey CAValueMapIntKeyNull;

/*
 * This class is provide as a wrapper of basic types, such as int and bool.
 */
class CC_DLL CAValue: public CAObject
{
public:

    static const CAValue Null;

    CAValue();

    explicit CAValue(int v);
    
    explicit CAValue(float v);
    
    explicit CAValue(double v);
    
    explicit CAValue(bool v);
    
    explicit CAValue(const char* v);
    
    explicit CAValue(const std::string& v);
    
    explicit CAValue(const CAValueVector& v);
    
    explicit CAValue(const CAValueMap& v);
    
    explicit CAValue(const CAValueMapIntKey& v);

    CAValue(const CAValue& other);

    ~CAValue();

    CAValue& operator= (const CAValue& other);

    CAValue& operator= (CAValue&& other);

    CAValue& operator= (int v);

    CAValue& operator= (float v);

    CAValue& operator= (double v);

    CAValue& operator= (bool v);

    CAValue& operator= (const char* v);

    CAValue& operator= (const std::string& v);

    CAValue& operator= (const CAValueVector& v);

    CAValue& operator= (const CAValueMap& v);
    
    CAValue& operator= (const CAValueMapIntKey& v);

    bool operator!= (const CAValue& v);

    bool operator!= (const CAValue& v) const;

    bool operator== (const CAValue& v);

    bool operator== (const CAValue& v) const;

    int asInt() const;

    float asFloat() const;

    double asDouble() const;

    bool asBool() const;

    std::string asString() const;

    CAValueVector& asValueVector();

    const CAValueVector& asValueVector() const;

    CAValueMap& asValueMap();

    const CAValueMap& asValueMap() const;

    CAValueMapIntKey& asIntKeyMap();

    const CAValueMapIntKey& asIntKeyMap() const;

    inline bool isNull() const { return _type == Type::NONE; }

    enum class Type
    {
        /// no CAValue is wrapped, an empty CAValue
        NONE = 0,
        /// wrap integer
        INTEGER,
        /// wrap float
        FLOAT,
        /// wrap double
        DOUBLE,
        /// wrap bool
        BOOLEAN,
        /// wrap string
        STRING,
        /// wrap vector
        VECTOR,
        /// wrap CAValueMap
        MAP,
        /// wrap CAValueMapIntKey
        INT_KEY_MAP
    };

    inline Type getType() const { return _type; }

    std::string getDescription() const;

private:
    void clear();
    void reset(Type type);

    union
    {
        int intVal;
        float floatVal;
        double doubleVal;
        bool boolVal;

        std::string* strVal;
        CAValueVector* vectorVal;
        CAValueMap* mapVal;
        CAValueMapIntKey* intKeyMapVal;
    }_field;

    Type _type;
};

/** @} */

NS_CC_END


#endif /* defined(__CossApp_libs__CCCAValue__) */
