
#include "CAValue.h"
#include <sstream>
#include <iomanip>
#include "support/ccUtils.h"

NS_CC_BEGIN

CC_DLL const CAValueVector CAValueVectorNull;
CC_DLL const CAValueMap CAValueMapNull;
CC_DLL const CAValueMapIntKey CAValueMapIntKeyNull;

const CAValue CAValue::Null;

CAValue::CAValue()
: _type(Type::NONE)
{
    memset(&_field, 0, sizeof(_field));
}

CAValue::CAValue(int v)
: _type(Type::INTEGER)
{
    _field.intVal = v;
}

CAValue::CAValue(float v)
: _type(Type::FLOAT)
{
    _field.floatVal = v;
}

CAValue::CAValue(double v)
: _type(Type::DOUBLE)
{
    _field.doubleVal = v;
}

CAValue::CAValue(bool v)
: _type(Type::BOOLEAN)
{
    _field.boolVal = v;
}

CAValue::CAValue(const char* v)
: _type(Type::STRING)
{
    _field.strVal = new (std::nothrow) std::string();
    if (v)
    {
        *_field.strVal = v;
    }
}

CAValue::CAValue(const std::string& v)
: _type(Type::STRING)
{
    _field.strVal = new (std::nothrow) std::string();
    *_field.strVal = v;
}

CAValue::CAValue(const CAValueVector& v)
: _type(Type::VECTOR)
{
    _field.vectorVal = new (std::nothrow) CAValueVector();
    *_field.vectorVal = v;
}

CAValue::CAValue(const CAValueMap& v)
: _type(Type::MAP)
{
    _field.mapVal = new (std::nothrow) CAValueMap();
    *_field.mapVal = v;
}

CAValue::CAValue(const CAValueMapIntKey& v)
: _type(Type::INT_KEY_MAP)
{
    _field.intKeyMapVal = new (std::nothrow) CAValueMapIntKey();
    *_field.intKeyMapVal = v;
}

CAValue::CAValue(const CAValue& other)
: _type(Type::NONE)
{
    *this = other;
}

CAValue::~CAValue()
{
    clear();
}

CAValue& CAValue::operator= (const CAValue& other)
{
    if (this != &other) {
        reset(other._type);

        switch (other._type) {
            case Type::INTEGER:
                _field.intVal = other._field.intVal;
                break;
            case Type::FLOAT:
                _field.floatVal = other._field.floatVal;
                break;
            case Type::DOUBLE:
                _field.doubleVal = other._field.doubleVal;
                break;
            case Type::BOOLEAN:
                _field.boolVal = other._field.boolVal;
                break;
            case Type::STRING:
                if (_field.strVal == nullptr)
                {
                    _field.strVal = new std::string();
                }
                *_field.strVal = *other._field.strVal;
                break;
            case Type::VECTOR:
                if (_field.vectorVal == nullptr)
                {
                    _field.vectorVal = new (std::nothrow) CAValueVector();
                }
                *_field.vectorVal = *other._field.vectorVal;
                break;
            case Type::MAP:
                if (_field.mapVal == nullptr)
                {
                    _field.mapVal = new (std::nothrow) CAValueMap();
                }
                *_field.mapVal = *other._field.mapVal;
                break;
            case Type::INT_KEY_MAP:
                if (_field.intKeyMapVal == nullptr)
                {
                    _field.intKeyMapVal = new (std::nothrow) CAValueMapIntKey();
                }
                *_field.intKeyMapVal = *other._field.intKeyMapVal;
                break;
            default:
                break;
        }
    }
    return *this;
}

CAValue& CAValue::operator= (CAValue&& other)
{
    if (this != &other)
    {
        clear();
        switch (other._type)
        {
            case Type::INTEGER:
                _field.intVal = other._field.intVal;
                break;
            case Type::FLOAT:
                _field.floatVal = other._field.floatVal;
                break;
            case Type::DOUBLE:
                _field.doubleVal = other._field.doubleVal;
                break;
            case Type::BOOLEAN:
                _field.boolVal = other._field.boolVal;
                break;
            case Type::STRING:
                _field.strVal = other._field.strVal;
                break;
            case Type::VECTOR:
                _field.vectorVal = other._field.vectorVal;
                break;
            case Type::MAP:
                _field.mapVal = other._field.mapVal;
                break;
            case Type::INT_KEY_MAP:
                _field.intKeyMapVal = other._field.intKeyMapVal;
                break;
            default:
                break;
        }
        _type = other._type;

        memset(&other._field, 0, sizeof(other._field));
        other._type = Type::NONE;
    }

    return *this;
}

CAValue& CAValue::operator= (int v)
{
    reset(Type::INTEGER);
    _field.intVal = v;
    return *this;
}

CAValue& CAValue::operator= (float v)
{
    reset(Type::FLOAT);
    _field.floatVal = v;
    return *this;
}

CAValue& CAValue::operator= (double v)
{
    reset(Type::DOUBLE);
    _field.doubleVal = v;
    return *this;
}

CAValue& CAValue::operator= (bool v)
{
    reset(Type::BOOLEAN);
    _field.boolVal = v;
    return *this;
}

CAValue& CAValue::operator= (const char* v)
{
    reset(Type::STRING);
    *_field.strVal = v ? v : "";
    return *this;
}

CAValue& CAValue::operator= (const std::string& v)
{
    reset(Type::STRING);
    *_field.strVal = v;
    return *this;
}

CAValue& CAValue::operator= (const CAValueVector& v)
{
    reset(Type::VECTOR);
    *_field.vectorVal = v;
    return *this;
}

CAValue& CAValue::operator= (const CAValueMap& v)
{
    reset(Type::MAP);
    *_field.mapVal = v;
    return *this;
}

CAValue& CAValue::operator= (const CAValueMapIntKey& v)
{
    reset(Type::INT_KEY_MAP);
    *_field.intKeyMapVal = v;
    return *this;
}

bool CAValue::operator!= (const CAValue& v)
{
    return !(*this == v);
}
bool CAValue::operator!= (const CAValue& v) const
{
    return !(*this == v);
}

bool CAValue::operator== (const CAValue& v)
{
    const auto &t = *this;
    return t == v;
}
bool CAValue::operator== (const CAValue& v) const
{
    if (this == &v) return true;
    if (v._type != this->_type) return false;
    if (this->isNull()) return true;
    switch (_type)
    {
    case Type::INTEGER: return v._field.intVal    == this->_field.intVal;
    case Type::BOOLEAN: return v._field.boolVal   == this->_field.boolVal;
    case Type::STRING:  return *v._field.strVal   == *this->_field.strVal;
    case Type::FLOAT:   return fabs(v._field.floatVal  - this->_field.floatVal)  <= FLT_EPSILON;
    case Type::DOUBLE:  return fabs(v._field.doubleVal - this->_field.doubleVal) <= FLT_EPSILON;
    case Type::VECTOR:
    {
        const auto &v1 = *(this->_field.vectorVal);
        const auto &v2 = *(v._field.vectorVal);
        const auto size = v1.size();
        if (size == v2.size())
        {
            for (size_t i = 0; i < size; i++)
            {
                if (v1[i] != v2[i]) return false;
            }
        }
        return true;
    }
    case Type::MAP:
    {
        const auto &map1 = *(this->_field.mapVal);
        const auto &map2 = *(v._field.mapVal);
        for (const auto &kvp : map1)
        {
            auto it = map2.find(kvp.first);
            if (it == map2.end() || it->second != kvp.second)
            {
                return false;
            }
        }
        return true;
    }
    case Type::INT_KEY_MAP:
    {
        const auto &map1 = *(this->_field.intKeyMapVal);
        const auto &map2 = *(v._field.intKeyMapVal);
        for (const auto &kvp : map1)
        {
            auto it = map2.find(kvp.first);
            if (it == map2.end() || it->second != kvp.second)
            {
                return false;
            }
        }
        return true;
    }
    default:
        break;
    };

    return false;
}

/// Convert CAValue to a specified type

int CAValue::asInt() const
{
    CCASSERT(_type != Type::VECTOR && _type != Type::MAP && _type != Type::INT_KEY_MAP, "Only base type (bool, string, float, double, int) could be converted");
    if (_type == Type::INTEGER)
    {
        return _field.intVal;
    }

    if (_type == Type::STRING)
    {
        return atoi(_field.strVal->c_str());
    }

    if (_type == Type::FLOAT)
    {
        return static_cast<int>(_field.floatVal);
    }

    if (_type == Type::DOUBLE)
    {
        return static_cast<int>(_field.doubleVal);
    }

    if (_type == Type::BOOLEAN)
    {
        return _field.boolVal ? 1 : 0;
    }

    return 0;
}

float CAValue::asFloat() const
{
    CCASSERT(_type != Type::VECTOR && _type != Type::MAP && _type != Type::INT_KEY_MAP, "Only base type (bool, string, float, double, int) could be converted");
    if (_type == Type::FLOAT)
    {
        return _field.floatVal;
    }

    if (_type == Type::STRING)
    {
        return atof(_field.strVal->c_str());
    }

    if (_type == Type::INTEGER)
    {
        return static_cast<float>(_field.intVal);
    }

    if (_type == Type::DOUBLE)
    {
        return static_cast<float>(_field.doubleVal);
    }

    if (_type == Type::BOOLEAN)
    {
        return _field.boolVal ? 1.0f : 0.0f;
    }

    return 0.0f;
}

double CAValue::asDouble() const
{
    CCASSERT(_type != Type::VECTOR && _type != Type::MAP && _type != Type::INT_KEY_MAP, "Only base type (bool, string, float, double, int) could be converted");
    if (_type == Type::DOUBLE)
    {
        return _field.doubleVal;
    }

    if (_type == Type::STRING)
    {
        return static_cast<double>(atof(_field.strVal->c_str()));
    }

    if (_type == Type::INTEGER)
    {
        return static_cast<double>(_field.intVal);
    }

    if (_type == Type::FLOAT)
    {
        return static_cast<double>(_field.floatVal);
    }

    if (_type == Type::BOOLEAN)
    {
        return _field.boolVal ? 1.0 : 0.0;
    }

    return 0.0;
}

bool CAValue::asBool() const
{
    CCASSERT(_type != Type::VECTOR && _type != Type::MAP && _type != Type::INT_KEY_MAP, "Only base type (bool, string, float, double, int) could be converted");
    if (_type == Type::BOOLEAN)
    {
        return _field.boolVal;
    }

    if (_type == Type::STRING)
    {
        return (*_field.strVal == "0" || *_field.strVal == "false") ? false : true;
    }

    if (_type == Type::INTEGER)
    {
        return _field.intVal == 0 ? false : true;
    }

    if (_type == Type::FLOAT)
    {
        return _field.floatVal == 0.0f ? false : true;
    }

    if (_type == Type::DOUBLE)
    {
        return _field.doubleVal == 0.0 ? false : true;
    }

    return false;
}

std::string CAValue::asString() const
{
    CCASSERT(_type != Type::VECTOR && _type != Type::MAP && _type != Type::INT_KEY_MAP, "Only base type (bool, string, float, double, int) could be converted");

    if (_type == Type::STRING)
    {
        return *_field.strVal;
    }

    std::stringstream ret;

    switch (_type)
    {
        case Type::INTEGER:
            ret << _field.intVal;
            break;
        case Type::FLOAT:
            ret << std::fixed << std::setprecision( 7 )<< _field.floatVal;
            break;
        case Type::DOUBLE:
            ret << std::fixed << std::setprecision( 16 ) << _field.doubleVal;
            break;
        case Type::BOOLEAN:
            ret << (_field.boolVal ? "true" : "false");
            break;
        default:
            break;
    }
    return ret.str();
}

CAValueVector& CAValue::asValueVector()
{
    CCASSERT(_type == Type::VECTOR, "The CAValue type isn't Type::VECTOR");
    return *_field.vectorVal;
}

const CAValueVector& CAValue::asValueVector() const
{
    CCASSERT(_type == Type::VECTOR, "The CAValue type isn't Type::VECTOR");
    return *_field.vectorVal;
}

CAValueMap& CAValue::asValueMap()
{
    CCASSERT(_type == Type::MAP, "The CAValue type isn't Type::MAP");
    return *_field.mapVal;
}

const CAValueMap& CAValue::asValueMap() const
{
    CCASSERT(_type == Type::MAP, "The CAValue type isn't Type::MAP");
    return *_field.mapVal;
}

CAValueMapIntKey& CAValue::asIntKeyMap()
{
    CCASSERT(_type == Type::INT_KEY_MAP, "The CAValue type isn't Type::INT_KEY_MAP");
    return *_field.intKeyMapVal;
}

const CAValueMapIntKey& CAValue::asIntKeyMap() const
{
    CCASSERT(_type == Type::INT_KEY_MAP, "The CAValue type isn't Type::INT_KEY_MAP");
    return *_field.intKeyMapVal;
}

static std::string getTabs(int depth)
{
    std::string tabWidth;

    for (int i = 0; i < depth; ++i)
    {
        tabWidth += "\t";
    }

    return tabWidth;
}

static std::string visit(const CAValue& v, int depth);

static std::string visitVector(const CAValueVector& v, int depth)
{
    std::stringstream ret;

    if (depth > 0)
        ret << "\n";

    ret << getTabs(depth) << "[\n";

    int i = 0;
    for (const auto& child : v)
    {
        ret << getTabs(depth+1) << i << ": " << visit(child, depth + 1);
        ++i;
    }

    ret << getTabs(depth) << "]\n";

    return ret.str();
}

template <class T>
static std::string visitMap(const T& v, int depth)
{
    std::stringstream ret;

    if (depth > 0)
        ret << "\n";

    ret << getTabs(depth) << "{\n";

    for (auto iter = v.begin(); iter != v.end(); ++iter)
    {
        ret << getTabs(depth + 1) << iter->first << ": ";
        ret << visit(iter->second, depth + 1);
    }

    ret << getTabs(depth) << "}\n";

    return ret.str();
}

static std::string visit(const CAValue& v, int depth)
{
    std::stringstream ret;

    switch (v.getType())
    {
        case CAValue::Type::NONE:
        case CAValue::Type::INTEGER:
        case CAValue::Type::FLOAT:
        case CAValue::Type::DOUBLE:
        case CAValue::Type::BOOLEAN:
        case CAValue::Type::STRING:
            ret << v.asString() << "\n";
            break;
        case CAValue::Type::VECTOR:
            ret << visitVector(v.asValueVector(), depth);
            break;
        case CAValue::Type::MAP:
            ret << visitMap(v.asValueMap(), depth);
            break;
        case CAValue::Type::INT_KEY_MAP:
            ret << visitMap(v.asIntKeyMap(), depth);
            break;
        default:
            CCASSERT(false, "Invalid type!");
            break;
    }

    return ret.str();
}

std::string CAValue::getDescription() const
{
    std::string ret("\n");
    ret += visit(*this, 0);
    return ret;
}

void CAValue::clear()
{
    // Free memory the old CAValue allocated
    switch (_type)
    {
        case Type::INTEGER:
            _field.intVal = 0;
            break;
        case Type::FLOAT:
            _field.floatVal = 0.0f;
            break;
        case Type::DOUBLE:
            _field.doubleVal = 0.0;
            break;
        case Type::BOOLEAN:
            _field.boolVal = false;
            break;
        case Type::STRING:
            CC_SAFE_DELETE(_field.strVal);
            break;
        case Type::VECTOR:
            CC_SAFE_DELETE(_field.vectorVal);
            break;
        case Type::MAP:
            CC_SAFE_DELETE(_field.mapVal);
            break;
        case Type::INT_KEY_MAP:
            CC_SAFE_DELETE(_field.intKeyMapVal);
            break;
        default:
            break;
    }
    
    _type = Type::NONE;
}

void CAValue::reset(Type type)
{
    if (_type == type)
        return;

    clear();

    // Allocate memory for the new CAValue
    switch (type)
    {
        case Type::STRING:
            _field.strVal = new (std::nothrow) std::string();
            break;
        case Type::VECTOR:
            _field.vectorVal = new (std::nothrow) CAValueVector();
            break;
        case Type::MAP:
            _field.mapVal = new (std::nothrow) CAValueMap();
            break;
        case Type::INT_KEY_MAP:
            _field.intKeyMapVal = new (std::nothrow) CAValueMapIntKey();
            break;
        default:
            break;
    }

    _type = type;
}

NS_CC_END
