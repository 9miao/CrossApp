
#ifndef __CCARRAY_H__
#define __CCARRAY_H__

#include "support/data_support/ccCArray.h"

/**
 * @addtogroup data_structures
 * @{
 */

/** @def CCARRAY_FOREACH
A convenience macro to iterate over a CCArray using. It is faster than the "fast enumeration" interface.
@since v0.99.4
*/

/*
In CrossApp-iphone 1.0.0, This macro have been update to like this:

#define CCARRAY_FOREACH(__array__, __object__)                                                \
if (__array__ && __array__->data->num > 0)                                                    \
for(id *__arr__ = __array__->data->arr, *end = __array__->data->arr + __array__->data->num-1;    \
__arr__ <= end && ((__object__ = *__arr__) != nil || true);                                        \
__arr__++)

I found that it's not work in C++. So it keep what it's look like in version 1.0.0-rc3. ---By Bin
*/
#define CCARRAY_FOREACH(__array__, __object__)                                                                         \
    if ((__array__) && (__array__)->data->num > 0)                                                                     \
    for(CAObject** __arr__ = (__array__)->data->arr, **__end__ = (__array__)->data->arr + (__array__)->data->num-1;    \
    __arr__ <= __end__ && (((__object__) = *__arr__) != NULL/* || true*/);                                             \
    __arr__++)

#define CCARRAY_FOREACH_REVERSE(__array__, __object__)                                                                  \
    if ((__array__) && (__array__)->data->num > 0)                                                                      \
    for(CAObject** __arr__ = (__array__)->data->arr + (__array__)->data->num-1, **__end__ = (__array__)->data->arr;     \
    __arr__ >= __end__ && (((__object__) = *__arr__) != NULL/* || true*/);                                              \
    __arr__--)

#if defined(CROSSAPP_DEBUG) && (CROSSAPP_DEBUG > 0)
#define CCARRAY_VERIFY_TYPE(__array__, __type__)                                                                 \
    do {                                                                                                         \
        if ((__array__) && (__array__)->data->num > 0)                                                           \
            for(CAObject** __arr__ = (__array__)->data->arr,                                                     \
                **__end__ = (__array__)->data->arr + (__array__)->data->num-1; __arr__ <= __end__; __arr__++)    \
                CCAssert(dynamic_cast<__type__>(*__arr__), "element type is wrong!");                            \
    } while(false)
#else
#define CCARRAY_VERIFY_TYPE(__array__, __type__) void(0)
#endif

#define arrayMakeObjectsPerformSelector(pArray, func, elementType)    \
do {                                                                  \
    if(pArray && pArray->count() > 0)                                 \
    {                                                                 \
        CAObject* child;                                              \
        CCARRAY_FOREACH(pArray, child)                                \
        {                                                             \
            elementType pNode = dynamic_cast<elementType>(child);     \
            if(pNode)                                                 \
            {                                                         \
                pNode->func();                                        \
            }                                                         \
        }                                                             \
    }                                                                 \
}                                                                     \
while(false)

#define arrayMakeObjectsPerformSelectorWithObject(pArray, func, pObject, elementType)   \
do {                                                                  \
    if(pArray && pArray->count() > 0)                                 \
    {                                                                 \
        CAObject* child = NULL;                                       \
        CCARRAY_FOREACH(pArray, child)                                \
        {                                                             \
            elementType pNode = dynamic_cast<elementType>(child);                  \
            if(pNode)                                                 \
            {                                                         \
                pNode->func(pObject);                                 \
            }                                                         \
        }                                                             \
    }                                                                 \
}                                                                     \
while(false)


NS_CC_BEGIN
/**
 * @js NA
 */
class CC_DLL CCArray : public CAObject
{
public:
    /**
     * @lua NA
     */
    ~CCArray();

    /** Create an array */
    static CCArray* create();
    /** Create an array with some objects 
     *  @lua NA
     */
    static CCArray* create(CAObject* pObject, ...);
    /** Create an array with one object */
    static CCArray* createWithObject(CAObject* pObject);
    /** Create an array with capacity */
    static CCArray* createWithCapacity(unsigned int capacity);
    /** Create an array with an existing array */
    static CCArray* createWithArray(CCArray* otherArray);
    /**
     @brief   Generate a CCArray pointer by file
     @param   pFileName  The file name of *.plist file
     @return  The CCArray pointer generated from the file
     */
    static CCArray* createWithContentsOfFile(const char* pFileName);
    
    /*
     @brief The same meaning as arrayWithContentsOfFile(), but it doesn't call autorelease, so the
     invoker should call release().
     @lua NA
     */
    static CCArray* createWithContentsOfFileThreadSafe(const char* pFileName);

    /** Initializes an array 
     *  @lua NA
     */
    bool init();
    /** Initializes an array with one object
     *  @lua NA
     */
    bool initWithObject(CAObject* pObject);
    /** Initializes an array with some objects 
     *  @lua NA
     */
    bool initWithObjects(CAObject* pObject, ...);
    /** Initializes an array with capacity 
     *  @lua NA
     */
    bool initWithCapacity(unsigned int capacity);
    /** Initializes an array with an existing array 
     *  @lua NA
     */
    bool initWithArray(CCArray* otherArray);

    // Querying an Array

    /** Returns element count of the array */
    unsigned int count() const;
    /** Returns capacity of the array */
    unsigned int capacity() const;
    /** Returns index of a certain object, return UINT_MAX if doesn't contain the object */
    unsigned int indexOfObject(CAObject* object) const;
    /** Returns an element with a certain index */
    CAObject* objectAtIndex(unsigned int index);
    /** Returns last element */
    CAObject* lastObject();
    /** Returns a random element */
    CAObject* randomObject();
    /** Returns a Boolean value that indicates whether object is present in array. */
    bool containsObject(CAObject* object) const;
    /** @since 1.1 */
    bool isEqualToArray(CCArray* pOtherArray);
    // Adding Objects

    /** Add a certain object */
    void addObject(CAObject* object);
    /** Add all elements of an existing array */
    void addObjectsFromArray(CCArray* otherArray);
    /** Insert a certain object at a certain index */
    void insertObject(CAObject* object, unsigned int index);

    // Removing Objects

    /** Remove last object */
    void removeLastObject(bool bReleaseObj = true);
    /** Remove a certain object */
    void removeObject(CAObject* object, bool bReleaseObj = true);
    /** Remove an element with a certain index */
    void removeObjectAtIndex(unsigned int index, bool bReleaseObj = true);
    /** Remove all elements */
    void removeObjectsInArray(CCArray* otherArray);
    /** Remove all objects */
    void removeAllObjects();
    /** Fast way to remove a certain object */
    void fastRemoveObject(CAObject* object);
    /** Fast way to remove an element with a certain index */
    void fastRemoveObjectAtIndex(unsigned int index);

    // Rearranging Content

    /** Swap two elements */
    void exchangeObject(CAObject* object1, CAObject* object2);
    /** Swap two elements with certain indexes */
    void exchangeObjectAtIndex(unsigned int index1, unsigned int index2);

    /** Replace object at index with another object. */
    void replaceObjectAtIndex(unsigned int uIndex, CAObject* pObject, bool bReleaseObject = true);

    /** Revers the array */
    void reverseObjects();
    /* Shrinks the array so the memory footprint corresponds with the number of items */
    void reduceMemoryFootprint();
  
    /** override functions 
     *  @js NA
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone* pZone);

public:
    ccArray* data;
    /**
     *  @lua NA
     */
    CCArray();
    /**
     *  @lua NA
     */
    CCArray(unsigned int capacity);
};

// end of data_structure group
/// @}

NS_CC_END

#endif // __CCARRAY_H__
