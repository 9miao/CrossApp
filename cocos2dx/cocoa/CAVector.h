//
//  CAVector.h
//  project
//
//  Created by lh on 14-5-14.
//
//

#ifndef __project__CAVector__
#define __project__CAVector__

#include <iostream>
#include <vector>
#include "cocos2d.h"
USING_NS_CC;
template<class T>
class CC_DLL CAVector:public CCObject
{
public:
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    
    typedef typename std::vector<T>::reverse_iterator reverse_iterator;
    typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;
    
    iterator begin() { return m_data.begin(); }
    const_iterator begin() const { return m_data.begin(); }
    
    iterator end() { return m_data.end(); }
    const_iterator end() const { return m_data.end(); }
    
    const_iterator cbegin() const { return m_data.cbegin(); }
    const_iterator cend() const { return m_data.cend(); }
    
    reverse_iterator rbegin() { return m_data.rbegin(); }
    const_reverse_iterator rbegin() const { return m_data.rbegin(); }
    
    reverse_iterator rend() { return m_data.rend(); }
    const_reverse_iterator rend() const { return m_data.rend(); }
    
    const_reverse_iterator crbegin() const { return m_data.crbegin(); }
    const_reverse_iterator crend() const { return m_data.crend(); }
    
    CAVector<T>()
    : m_data()
    {
        
    }
    CAVector<T>(ssize_t capacity)
    : m_data()
    {//
        CCLOGINFO("In the default constructor with capacity of Vector.");
        reserve(capacity);
    }
    void reserve(ssize_t n)
    {
        m_data.reserve(n);
    }
    ~CAVector<T>()
    {
        clear();
    }
    
    ssize_t capacity() const
    {
        return m_data.capacity();
    }
    ssize_t size() const
    {
        return  m_data.size();
    }
    bool empty() const
    {
        return m_data.empty();
    }
    ssize_t max_size() const
    {
        return m_data.max_size();
    }
    
    ssize_t getIndex(T object) const
    {
        iterator iter = std::find(m_data.begin(), m_data.end(), object);
        if (iter != m_data.end())
            return iter - m_data.begin();
        
        return -1;
    }
    CAVector<T>(const CAVector<T>& other)
    {//
        CCLOGINFO("In the copy constructor!");
        m_data = other.m_data;
        addRefForAllObjects();
    }
    CAVector<T>& operator=(const CAVector<T>& other)
    {//
        if (this != &other) {
            CCLOGINFO("In the copy assignment operator!");
            clear();
            m_data = other.m_data;
            addRefForAllObjects();
        }
        return *this;
    }
    CAVector<T>& operator=(CAVector<T>&& other)
    {
        if (this != &other) {
            CCLOGINFO("In the move assignment operator!");
            clear();
            m_data = m_data.move(other.m_data);
        }
        return *this;
    }
    const_iterator find(T object) const
    {
        return std::find(m_data.begin(), m_data.end(), object);
    }
    iterator find(T object)
    {
        return std::find(m_data.begin(), m_data.end(), object);
    }
    T at(ssize_t index) const
    {
       // CCASSERT( index >= 0 && index < size(), "index out of range in getObjectAtIndex()");
        return m_data[index];
    }
    T front() const
    {
        return m_data.front();
    }
    T back() const
    {
        return m_data.back();
    }
    bool contains(T object) const
    {
        return( std::find(m_data.begin(), m_data.end(), object) != m_data.end() );
    }
    T getRandomObject() const
    {
        if (!m_data.empty())
        {
            ssize_t randIdx = rand() % m_data.size();
            return *(m_data.begin() + randIdx);
        }
        return NULL;
    }
    void pushBack(const CAVector<T>& other)
    {
        for (iterator i = other.begin(); i!= other.end(); ++i) {
            CCObject *obj = (CCObject *)(*i);
            m_data.push_back(obj);
            obj->retain();
        }
    }
    void insert(ssize_t index, T object)
    {

        m_data.insert((m_data.begin() + index), object);
        object->retain();
    }
    bool equals(const CAVector<T> &other)
    {
        ssize_t s = this->size();
        if (s != other.size())
            return false;
        
        for (ssize_t i = 0; i < s; i++)
        {
            if (this->at(i) != other.at(i))
            {
                return false;
            }
        }
        return true;
    }
    void clear()
    {
        for( iterator it = m_data.begin(); it != m_data.end(); ++it ) {
            (*it)->release();
        }
        m_data.clear();
    }
    
    
    
    
    void pushBack(T object)
    {
    //CCASSERT(object != nullptr, "The object should not be nullptr");
        m_data.push_back( object );
        
        object->retain();
    }
    
    void popBack()
    {
        auto last = m_data.back();
        m_data.pop_back();
        last->release();
    }
    
    void eraseObject(T object, bool removeAll = false)
    {
        CCAssert(object != NULL, "The object should not be nullptr");
        
        if (removeAll)
        {
            for (auto iter = m_data.begin(); iter != m_data.end();)
            {
                if ((*iter) == object)
                {
                    iter = m_data.erase(iter);
                    object->release();
                }
                else
                {
                    ++iter;
                }
            }
        }
        else
        {
            auto iter = std::find(m_data.begin(), m_data.end(), object);
            if (iter != m_data.end())
            {
                m_data.erase(iter);
                object->release();
            }
        }
    }
    
    iterator erase(iterator position)
    {
        CCASSERT(position >= m_data.begin() && position < m_data.end(), "Invalid position!");
        (*position)->release();
        return m_data.erase(position);
    }
    
    iterator erase(iterator first, iterator last)
    {
        for (auto iter = first; iter != last; ++iter)
        {
            (*iter)->release();
        }
        
        return m_data.erase(first, last);
    }
    
    iterator erase(ssize_t index)
    {
        CCASSERT(!m_data.empty() && index >=0 && index < size(), "Invalid index!");
        iterator it =m_data.next(begin(),index);
        //std::next( begin(), index );
    
        (*it)->release();
        return m_data.erase(it);
    }
    void swap(T object1, T object2)
    {
        ssize_t idx1 = getIndex(object1);
        ssize_t idx2 = getIndex(object2);
        
        std::swap( m_data[idx1], m_data[idx2] );
    }
    
    void swap(ssize_t index1, ssize_t index2)
    {
        
        
        std::swap( m_data[index1], m_data[index2] );
    }
    
    void replace(ssize_t index, T object)
    {
        
        m_data[index]->release();
        m_data[index] = object;
        object->retain();
    }
    void reverse()
    {
        std::reverse( m_data.begin(), m_data.end() );
    }
    void shrinkToFit()
    {
        m_data.shrink_to_fit();
    }
protected:
    
    void addRefForAllObjects()
    {
        for(iterator i = m_data.begin();i!=m_data.end();++i)
        {
            CCObject* obj =(CCObject *)(*i);
            obj->retain();
        }
    }
    std::vector<T> m_data;
};
#endif /* defined(__project__CAVector__) */
