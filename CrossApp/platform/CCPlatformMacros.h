
#ifndef __CC_PLATFORM_MACROS_H__
#define __CC_PLATFORM_MACROS_H__

/**
 * define some platform specific macros
 */
#include "ccConfig.h"
#include "CCPlatformConfig.h"
#include "CCPlatformDefine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <functional>
#endif

#define CREATE_FUNC(__TYPE__)           \
static __TYPE__* create()               \
{                                       \
    __TYPE__ *pRet = new __TYPE__();    \
    if (pRet && pRet->init())           \
    {                                   \
        pRet->autorelease();            \
        return pRet;                    \
    }                                   \
    else                                \
    {                                   \
        delete pRet;                    \
        pRet = NULL;                    \
        return NULL;                    \
    }                                   \
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

    #define CC_REBIND_INDICES_BUFFER  1
#else
    #define CC_REBIND_INDICES_BUFFER  0
#endif

// generic macros

// namespace CrossApp {}
#ifdef __cplusplus
	#define NS_CC_BEGIN                     namespace CrossApp {
    #define NS_CC_END                       }
    #define USING_NS_CC                     using namespace CrossApp
#else
    #define NS_CC_BEGIN 
    #define NS_CC_END 
    #define USING_NS_CC 
#endif 

/** CC_PROPERTY_READONLY is used to declare a protected variable.
 We can use getter to read the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 @warning : The getter is a public virtual function, you should rewrite it first.
 The variables and methods declared after CC_PROPERTY_READONLY are all public.
 If you need protected or private, please declare.
 */
#define CC_PROPERTY_READONLY(varType, varName, funName)\
protected: varType varName; \
public: virtual varType get##funName(void);

#define CC_PROPERTY_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; \
public: virtual const varType& get##funName(void);

/** CC_PROPERTY is used to declare a protected variable.
We can use getter to read the variable, and use the setter to change the variable.
@param varType : the type of variable.
@param varName : variable name.
@param funName : "get + funName" is the name of the getter.
"set + funName" is the name of the setter.
@warning : The getter and setter are public virtual functions, you should rewrite them first.
The variables and methods declared after CC_PROPERTY are all public.
If you need protected or private, please declare.
*/
#define CC_PROPERTY_IS(varType, varName, funName)\
protected: varType varName; \
public: virtual varType is##funName(void); \
public: virtual void set##funName(varType var);

#define CC_PROPERTY_IS_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; \
public: virtual const varType& is##funName(void); \
public: virtual void set##funName(const varType& var);

#define CC_PROPERTY(varType, varName, funName)\
protected: varType varName; \
public: virtual varType get##funName(void); \
public: virtual void set##funName(varType var);

#define CC_PROPERTY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; \
public: virtual const varType& get##funName(void); \
public: virtual void set##funName(const varType& var);

#define CC_SYNTHESIZE_IS_READONLY(varType, varName, funName)\
protected: varType varName; \
public: inline virtual varType is##funName(void) const { return varName; }

#define CC_SYNTHESIZE_IS_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; \
public: inline virtual const varType& is##funName(void) const { return varName; }

#define CC_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName; \
public: inline virtual varType get##funName(void) const { return varName; }

#define CC_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; \
public: inline virtual const varType& get##funName(void) const { return varName; }

#define CC_SYNTHESIZE_IS(varType, varName, funName)\
protected: varType varName; \
public: inline virtual varType is##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

#define CC_SYNTHESIZE_IS_PASS_BY_REF(varType, varName, funName)\
protected: varType varName; \
public: inline virtual const varType& is##funName(void) const { return varName; }\
public: virtual void set##funName(const varType& var){ varName = var; }

#define CC_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

#define CC_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline virtual const varType& get##funName(void) const { return varName; }\
public: virtual void set##funName(const varType& var){ varName = var; }

#define CC_SYNTHESIZE_RETAIN(varType, varName, funName)    \
private: varType varName; \
public: inline virtual varType get##funName(void) const { return varName; } \
public: virtual void set##funName(varType var)   \
{ \
    if (varName != var) \
    { \
        CC_SAFE_RETAIN(var); \
        CC_SAFE_RELEASE(varName); \
        varName = var; \
    } \
} 

#define CC_LISTENING_FUNCTION(FUNCTION, VARNAME)\
protected: std::function<FUNCTION> m_ob##VARNAME{nullptr};\
public: void on##VARNAME(const std::function<FUNCTION>& var){ m_ob##VARNAME = var; }

#define CC_LISTENING_FUNCTION_D(FUNCTION, VARNAME)\
protected: std::function<FUNCTION> m_ob##VARNAME{nullptr};\
public: void on##VARNAME(const std::function<FUNCTION>& var);

#define CC_SAFE_DELETE(p)           do { if(p) { delete (p); (p) = 0; } } while(0)
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = 0; } } while(0)
#define CC_SAFE_FREE(p)             do { if(p) { free(p); (p) = 0; } } while(0)
#define CC_SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define CC_SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = 0; } } while(0)
#define CC_SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)
#define CC_RETURN_IF(cond)          if(cond) return
#define CC_BREAK_IF(cond)           if(cond) break
#define CC_CONTINUE_IF(cond)        if(cond) continue

// CrossApp debug
#if !defined(CROSSAPP_DEBUG) || CROSSAPP_DEBUG == 0
#define CCLOG(...)       do {} while (0)

#elif CROSSAPP_DEBUG == 1
#define CCLOG(format, ...)      CrossApp::CCLog(format, ##__VA_ARGS__)

#elif CROSSAPP_DEBUG > 1
#define CCLOG(format, ...)      CrossApp::CCLog(format, ##__VA_ARGS__)
#endif // CROSSAPP_DEBUG


#if defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 4))) \
    || (defined(__clang__) && (__clang_major__ >= 3))
#define CC_DISABLE_COPY(Class) \
private: \
    Class(const Class &) = delete; \
    Class &operator =(const Class &) = delete;
#else
#define CC_DISABLE_COPY(Class) \
private: \
    Class(const Class &); \
    Class &operator =(const Class &);
#endif

/*
 * only certain compilers support __attribute__((deprecated))
 */
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
    #define CC_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
    #define CC_DEPRECATED_ATTRIBUTE __declspec(deprecated) 
#else
    #define CC_DEPRECATED_ATTRIBUTE
#endif 

/** @def CC_DEPRECATED(...)
 * Macro to mark things deprecated as of a particular version
 * can be used with arbitrary parameters which are thrown away.
 * e.g. CC_DEPRECATED(4.0) or CC_DEPRECATED(4.0, "not going to need this anymore") etc.
 */
#define CC_DEPRECATED(...) CC_DEPRECATED_ATTRIBUTE

//javascript版本添加 Lei.zhang
/** @def CC_DISALLOW_COPY_AND_ASSIGN(TypeName)
 * A macro to disallow the copy constructor and operator= functions.
 * This should be used in the private: declarations for a class
 */
#if defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 4))) \
|| (defined(__clang__) && (__clang_major__ >= 3)) || (_MSC_VER >= 1800)
#define CC_DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName &) = delete; \
TypeName &operator =(const TypeName &) = delete;
#else
#define CC_DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName &); \
TypeName &operator =(const TypeName &);
#endif

/*
 * only certain compiler support __attribute__((format))
 * formatPos - 1-based position of format string argument
 * argPos - 1-based position of first format-dependent argument
 */
#if defined(__GNUC__) && (__GNUC__ >= 4)
#define CC_FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))
#elif defined(__has_attribute)
  #if __has_attribute(format)
  #define CC_FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))
  #endif // __has_attribute(format)
#else
#define CC_FORMAT_PRINTF(formatPos, argPos)
#endif

#if defined(_MSC_VER)
#define CC_FORMAT_PRINTF_SIZE_T "%08lX"
#else
#define CC_FORMAT_PRINTF_SIZE_T "%08zX"
#endif

#ifdef __GNUC__
#define CC_UNUSED __attribute__ ((unused))
#else
#define CC_UNUSED
#endif



#define CC_PROPERTY_IS_DEPRECATED_ATTRIBUTE(varType, varName, funName)\
protected: varType varName; \
public: CC_DEPRECATED_ATTRIBUTE varType is##funName(void); \
public: CC_DEPRECATED_ATTRIBUTE void set##funName(varType var);

#define CC_PROPERTY_IS_PASS_BY_REF_DEPRECATED_ATTRIBUTE(varType, varName, funName)\
protected: varType varName; \
public: CC_DEPRECATED_ATTRIBUTE const varType& is##funName(void); \
public: CC_DEPRECATED_ATTRIBUTE void set##funName(const varType& var);

#define CC_PROPERTY_DEPRECATED_ATTRIBUTE(varType, varName, funName)\
protected: varType varName; \
public: CC_DEPRECATED_ATTRIBUTE varType get##funName(void); \
public: CC_DEPRECATED_ATTRIBUTE void set##funName(varType var);

#define CC_PROPERTY_PASS_BY_REF_DEPRECATED_ATTRIBUTE(varType, varName, funName)\
protected: varType varName; \
public: CC_DEPRECATED_ATTRIBUTE const varType& get##funName(void); \
public: CC_DEPRECATED_ATTRIBUTE void set##funName(const varType& var);

#define CC_SYNTHESIZE_IS_READONLY_DEPRECATED_ATTRIBUTE(varType, varName, funName)\
protected: CC_DEPRECATED_ATTRIBUTE varName; \
public: CC_DEPRECATED_ATTRIBUTE varType is##funName(void) const { return varName; }

#define CC_SYNTHESIZE_IS_READONLY_PASS_BY_REF_DEPRECATED_ATTRIBUTE(varType, varName, funName)\
protected: varType varName; \
public: CC_DEPRECATED_ATTRIBUTE const varType& is##funName(void) const { return varName; }

#define CC_SYNTHESIZE_READONLY_DEPRECATED_ATTRIBUTE(varType, varName, funName)\
protected: varType varName; \
public: CC_DEPRECATED_ATTRIBUTE varType get##funName(void) const { return varName; }

#define CC_SYNTHESIZE_READONLY_PASS_BY_REF_DEPRECATED_ATTRIBUTE(varType, varName, funName)\
protected: varType varName; \
public: CC_DEPRECATED_ATTRIBUTE const varType& get##funName(void) const { return varName; }

#define CC_SYNTHESIZE_IS_DEPRECATED_ATTRIBUTE(varType, varName, funName)\
protected: varType varName; \
public: CC_DEPRECATED_ATTRIBUTE varType is##funName(void) const { return varName; }\
public: CC_DEPRECATED_ATTRIBUTE void set##funName(varType var){ varName = var; }

#define CC_SYNTHESIZE_IS_PASS_BY_REF_DEPRECATED_ATTRIBUTE(varType, varName, funName)\
protected: varType varName; \
public: CC_DEPRECATED_ATTRIBUTE const varType& is##funName(void) const { return varName; }\
public: CC_DEPRECATED_ATTRIBUTE void set##funName(const varType& var){ varName = var; }

#define CC_SYNTHESIZE_DEPRECATED_ATTRIBUTE(varType, varName, funName)\
protected: varType varName;\
public: CC_DEPRECATED_ATTRIBUTE varType get##funName(void) const { return varName; }\
public: CC_DEPRECATED_ATTRIBUTE void set##funName(varType var){ varName = var; }

#define CC_SYNTHESIZE_PASS_BY_REF_DEPRECATED_ATTRIBUTE(varType, varName, funName)\
protected: varType varName;\
public: CC_DEPRECATED_ATTRIBUTE const varType& get##funName(void) const { return varName; }\
public: CC_DEPRECATED_ATTRIBUTE void set##funName(const varType& var){ varName = var; }

#define CC_SYNTHESIZE_RETAIN_DEPRECATED_ATTRIBUTE(varType, varName, funName)    \
private: varType varName; \
public: CC_DEPRECATED_ATTRIBUTE varType get##funName(void) const { return varName; } \
public: CC_DEPRECATED_ATTRIBUTE void set##funName(varType var)   \
{ \
if (varName != var) \
{ \
CC_SAFE_RETAIN(var); \
CC_SAFE_RELEASE(varName); \
varName = var; \
} \
}

#endif // __CC_PLATFORM_MACROS_H__
