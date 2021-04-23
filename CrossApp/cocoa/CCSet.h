
#ifndef __CC_SET_H__
#define __CC_SET_H__

#include <set>
#include "basics/CAObject.h"

NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

typedef std::set<CAObject *>::iterator CCSetIterator;

class CC_DLL CCSet : public CAObject
{
public:
    /**
     * @js  ctor
     * @lua NA
     */
    CCSet(void);
    /**
     * @lua NA
     */
    CCSet(const CCSet &rSetObject);
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCSet(void);

    /**
    * @brief Create and return a new empty set.
    * @lua NA
    */
    static CCSet * create();

    /**
    *@brief Return a copy of the CCSet, it will copy all the elements.
    */
    CCSet* copy();
    /**
    *@brief It is the same as copy().
    *@lua NA
    */
    CCSet* mutableCopy();
    /**
    *@brief Return the number of elements the CCSet contains.
    */
    int count();
    /**
    *@brief Add a element into CCSet, it will retain the element.
    */
    void addObject(CAObject *pObject);
    /**
    *@brief Remove the given element, nothing todo if no element equals pObject.
    */
    void removeObject(CAObject *pObject);
    /**
     *@brief Remove all elements of the set
     */
    void removeAllObjects();
    /**
    *@brief Check if CCSet contains a element equals pObject.
    */
    bool containsObject(CAObject *pObject);
    /**
    *@brief Return the iterator that points to the first element.
    *@js NA
    *@lua NA
    */
    CCSetIterator begin();
    /**
    *@brief Return the iterator that points to the position after the last element.
    *@js NA
    *@lua NA
    */
    CCSetIterator end();
    /**
    *@brief Return the first element if it contains elements, or null if it doesn't contain any element.
    */
    CAObject* anyObject();

private:
    std::set<CAObject *> *m_pSet;
};

// end of data_structure group
/// @}

NS_CC_END

#endif // __CC_SET_H__

