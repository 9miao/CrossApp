/****************************************************************************
Copyright (c) 2010 ForzeField Studios S.L. http://forzefield.com
Copyright (c) 2010 CrossApp.org

http://9miao.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCArray.h"
#include "platform/CCFileUtils.h"

NS_CC_BEGIN


CCArray::CCArray()
: data(NULL)
{
    init();
}

CCArray::CCArray(unsigned int capacity)
: data(NULL)
{
    initWithCapacity(capacity);
}

CCArray* CCArray::create()
{
    CCArray* pArray = new CCArray();

    if (pArray && pArray->init())
    {
        pArray->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pArray);
    }
    
    return pArray;
}

CCArray* CCArray::createWithObject(CAObject* pObject)
{
    CCArray* pArray = new CCArray();

    if (pArray && pArray->initWithObject(pObject))
    {
        pArray->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pArray);
    }

    return pArray;
}

CCArray* CCArray::create(CAObject* pObject, ...)
{
    va_list args;
    va_start(args,pObject);

    CCArray* pArray = create();
    if (pArray && pObject)
    {
        pArray->addObject(pObject);
        CAObject *i = va_arg(args, CAObject*);
        while(i) 
        {
            pArray->addObject(i);
            i = va_arg(args, CAObject*);
        }
    }
    else
    {
        CC_SAFE_DELETE(pArray);
    }

    va_end(args);

    return pArray;
}

CCArray* CCArray::createWithArray(CCArray* otherArray)
{
    CCArray* pRet = (CCArray*)otherArray->copy();
    pRet->autorelease();
    return pRet;
}

CCArray* CCArray::createWithCapacity(unsigned int capacity)
{
    CCArray* pArray = new CCArray();
    
    if (pArray && pArray->initWithCapacity(capacity))
    {
        pArray->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pArray);
    }
    
    return pArray;
}

CCArray* CCArray::createWithContentsOfFile(const char* pFileName)
{
    CCArray* pRet = CCArray::createWithContentsOfFileThreadSafe(pFileName);
    if (pRet != NULL)
    {
        pRet->autorelease();
    }
    return pRet;
}

CCArray* CCArray::createWithContentsOfFileThreadSafe(const char* pFileName)
{
    return CCFileUtils::sharedFileUtils()->createCCArrayWithContentsOfFile(pFileName);
}

bool CCArray::init()
{
    return initWithCapacity(1);
}

bool CCArray::initWithObject(CAObject* pObject)
{
    ccArrayFree(data);
    bool bRet = initWithCapacity(1);
    if (bRet)
    {
        addObject(pObject);
    }
    return bRet;
}

/** Initializes an array with some objects */
bool CCArray::initWithObjects(CAObject* pObject, ...)
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(pObject == NULL);

        va_list args;
        va_start(args, pObject);

        if (pObject)
        {
            this->addObject(pObject);
            CAObject* i = va_arg(args, CAObject*);
            while(i) 
            {
                this->addObject(i);
                i = va_arg(args, CAObject*);
            }
            bRet = true;
        }
        va_end(args);

    } while (false);

    return bRet;
}

bool CCArray::initWithCapacity(unsigned int capacity)
{
    ccArrayFree(data);
    data = ccArrayNew(capacity);
    return true;
}

bool CCArray::initWithArray(CCArray* otherArray)
{
    ccArrayFree(data);
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! initWithCapacity(otherArray->data->num));

        addObjectsFromArray(otherArray);
        bRet = true;
    } while (0);
    
    return bRet;
}

unsigned int CCArray::count() const
{
    return data->num;
}

unsigned int CCArray::capacity() const
{
    return data->max;
}

unsigned int CCArray::indexOfObject(CAObject* object) const
{
    return ccArrayGetIndexOfObject(data, object);
}

CAObject* CCArray::objectAtIndex(unsigned int index)
{
    CCAssert(index < data->num, "index out of range in objectAtIndex()");

    return data->arr[index];
}

CAObject* CCArray::lastObject()
{
    if( data->num > 0 )
        return data->arr[data->num-1];

    return NULL;
}

CAObject* CCArray::randomObject()
{
    if (data->num==0)
    {
        return NULL;
    }

    float r = CCRANDOM_0_1();
    
    if (r == 1) // to prevent from accessing data-arr[data->num], out of range.
    {
        r = 0;
    }
    
    return data->arr[(int)(data->num * r)];
}

bool CCArray::containsObject(CAObject* object) const
{
    return ccArrayContainsObject(data, object);
}

bool CCArray::isEqualToArray(CCArray* otherArray)
{
    for (unsigned int i = 0; i< this->count(); i++)
    {
        if (!this->objectAtIndex(i)->isEqual(otherArray->objectAtIndex(i)))
        {
            return false;
        }
    }
    return true;
}

void CCArray::addObject(CAObject* object)
{
    ccArrayAppendObjectWithResize(data, object);
}

void CCArray::addObjectsFromArray(CCArray* otherArray)
{
    ccArrayAppendArrayWithResize(data, otherArray->data);
}

void CCArray::insertObject(CAObject* object, unsigned int index)
{
    ccArrayInsertObjectAtIndex(data, object, index);
}

void CCArray::removeLastObject(bool bReleaseObj)
{
    CCAssert(data->num, "no objects added");
    ccArrayRemoveObjectAtIndex(data, data->num-1, bReleaseObj);
}

void CCArray::removeObject(CAObject* object, bool bReleaseObj/* = true*/)
{
    ccArrayRemoveObject(data, object, bReleaseObj);
}

void CCArray::removeObjectAtIndex(unsigned int index, bool bReleaseObj)
{
    ccArrayRemoveObjectAtIndex(data, index, bReleaseObj);
}

void CCArray::removeObjectsInArray(CCArray* otherArray)
{
    ccArrayRemoveArray(data, otherArray->data);
}

void CCArray::removeAllObjects()
{
    ccArrayRemoveAllObjects(data);
}

void CCArray::fastRemoveObjectAtIndex(unsigned int index)
{
    ccArrayFastRemoveObjectAtIndex(data, index);
}

void CCArray::fastRemoveObject(CAObject* object)
{
    ccArrayFastRemoveObject(data, object);
}

void CCArray::exchangeObject(CAObject* object1, CAObject* object2)
{
    unsigned int index1 = ccArrayGetIndexOfObject(data, object1);
    if(index1 == UINT_MAX)
    {
        return;
    }

    unsigned int index2 = ccArrayGetIndexOfObject(data, object2);
    if(index2 == UINT_MAX)
    {
        return;
    }

    ccArraySwapObjectsAtIndexes(data, index1, index2);
}

void CCArray::exchangeObjectAtIndex(unsigned int index1, unsigned int index2)
{
    ccArraySwapObjectsAtIndexes(data, index1, index2);
}

void CCArray::replaceObjectAtIndex(unsigned int index, CAObject* pObject, bool bReleaseObject/* = true*/)
{
    ccArrayInsertObjectAtIndex(data, pObject, index);
    ccArrayRemoveObjectAtIndex(data, index+1);
}

void CCArray::reverseObjects()
{
    if (data->num > 1)
    {
        // floorf(), since in the case of an even number, the number of swaps stays the same
        int count = (int) floorf(data->num/2.f); 
        unsigned int maxIndex = data->num - 1;

        for (int i = 0; i < count ; i++)
        {
            ccArraySwapObjectsAtIndexes(data, i, maxIndex);
            maxIndex--;
        }
    }
}

void CCArray::reduceMemoryFootprint()
{
    ccArrayShrink(data);
}

CCArray::~CCArray()
{
    ccArrayFree(data);
}

CAObject* CCArray::copyWithZone(CAZone* pZone)
{
    CCAssert(pZone == NULL, "CCArray should not be inherited.");
    CCArray* pArray = new CCArray();
    pArray->initWithCapacity(this->data->num > 0 ? this->data->num : 1);

    CAObject* pObj = NULL;
    CAObject* pTmpObj = NULL;
    CCARRAY_FOREACH(this, pObj)
    {
        pTmpObj = pObj->copy();
        pArray->addObject(pTmpObj);
        pTmpObj->release();
    }
    return pArray;
}

NS_CC_END
