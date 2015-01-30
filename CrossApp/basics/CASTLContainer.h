//
//  CASTLContainer.h
//  CrossApp
//
//  Created by Zhujian on 14-6-10.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//
#ifndef __CASTLContainer__
#define __CASTLContainer__

#include "ccTypes.h"
#include "ccMacros.h"
#include <vector>
#include <deque>
#include <map>
#include <list>
#include <algorithm>

NS_CC_BEGIN

template<class T>
class CADeque;

template<class T>
class CAList;

template<class T>
class CAVector
{
public:
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;

	typedef typename std::vector<T>::reverse_iterator reverse_iterator;
	typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

	iterator begin() { return _data.begin(); }
	const_iterator begin() const { return _data.begin(); }

	iterator end() { return _data.end(); }
	const_iterator end() const { return _data.end(); }

	reverse_iterator rbegin() { return _data.rbegin(); }
	const_reverse_iterator rbegin() const { return _data.rbegin(); }

	reverse_iterator rend() { return _data.rend(); }
	const_reverse_iterator rend() const { return _data.rend(); }


	CAVector<T>() {}

	explicit CAVector<T>(size_t capacity)
		: _data()
	{
		CCLOGINFO("In the default constructor with capacity of Vector.");
		reserve(capacity);
	}

	~CAVector<T>()
	{
		CCLOGINFO("In the destructor of Vector.");
		clear();
	}

	CAVector<T>(const CAVector<T>& other)
	{
		CCLOGINFO("In the copy constructor!");
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}

    CAVector<T>(const CADeque<T>& other)
	{
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAVector<T>(const CAList<T>& other)
	{
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAVector<T>(const std::vector<T>& other)
	{
		CCLOGINFO("In the copy constructor!");
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAVector<T>(const std::deque<T>& other)
	{
		CCLOGINFO("In the copy constructor!");
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAVector<T>(const std::list<T>& other)
	{
		CCLOGINFO("In the copy constructor!");
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
	CAVector<T>& operator=(const CAVector<T>& other)
	{
		if (this != &other)
		{
			CCLOGINFO("In the copy assignment operator!");
			clear();
			_data = other._data;
			addRefForAllObjects();
		}
		return *this;
	}

	void reserve(size_t n)
	{
		_data.reserve(n);
	}
    
    void resize(size_t n)
    {
        _data.resize(n);
    }

	size_t capacity() const
	{
		return _data.capacity();
	}

	size_t size() const
	{
		return  _data.size();
	}

	bool empty() const
	{
		return _data.empty();
	}

	size_t max_size() const
	{
		return _data.max_size();
	}

	size_t getIndex(T object) const
	{
		const_iterator iter = std::find(_data.begin(), _data.end(), object);
		if (iter != _data.end())
			return iter - _data.begin();

		return -1;
	}

	const_iterator find(T object) const
	{
		return std::find(_data.begin(), _data.end(), object);
	}

	iterator find(T object)
	{
		return std::find(_data.begin(), _data.end(), object);
	}

	T at(size_t index) const
	{
		CCAssert(index >= 0 && index < size(), "index out of range in getObjectAtIndex()");
		return _data[index];
	}

	T front() const
	{
		return _data.front();
	}

	T back() const
	{
		return _data.back();
	}

	T getRandomObject() const
	{
		if (!_data.empty())
		{
			size_t randIdx = rand() % _data.size();
			return *(_data.begin() + randIdx);
		}
		return NULL;
	}

	bool contains(T object) const
	{
		return(std::find(_data.begin(), _data.end(), object) != _data.end());
	}
	bool equals(const CAVector<T> &other)
	{
		size_t s = this->size();
		if (s != other.size())
			return false;

		for (size_t i = 0; i < s; i++)
		{
			if (this->at(i) != other.at(i))
			{
				return false;
			}
		}
		return true;
	}

    void insert(size_t index, T object)
	{
		CCAssert(index >= 0 && index <= size(), "Invalid index!");
		_data.insert(_data.begin() + index, object);
        CC_SAFE_RETAIN(object);
	}
    
	void pushBack(T object)
	{
		_data.push_back(object);
        CC_SAFE_RETAIN(object);
	}
    
	void pushBack(const CAVector<T>& other)
	{
		for (size_t i = 0; i < other.size(); i++)
		{
			pushBack(other.at(i));
		}
	}
    
	void popBack()
	{
		CCAssert(!_data.empty(), "no objects added");
		T last = _data.back();
		_data.pop_back();
        CC_SAFE_RELEASE(last);
	}

	void eraseObject(T object, bool removeAll = false)
	{
		if (removeAll)
		{

			for (iterator iter = _data.begin(); iter != _data.end();)
			{
				if ((*iter) == object)
				{
					iter = _data.erase(iter);
                    CC_SAFE_RELEASE(object);
				}
				else
				{
					++iter;
				}
			}
		}
		else
		{
			iterator iter = std::find(_data.begin(), _data.end(), object);
			if (iter != _data.end())
			{
				_data.erase(iter);
				CC_SAFE_RELEASE(object);
			}
		}
	}

	iterator erase(iterator position)
	{
		CCAssert(position >= _data.begin() && position < _data.end(), "Invalid position!");
        CC_SAFE_RELEASE(*position);
		return _data.erase(position);
	}

	iterator erase(iterator first, iterator last)
	{
		for (iterator iter = first; iter != last; ++iter)
		{
            CC_SAFE_RELEASE((*iter));
		}
		return _data.erase(first, last);
	}

	iterator erase(size_t index)
	{
		CCAssert(!_data.empty() && index >= 0 && index < size(), "Invalid index!");
		return erase(_data.begin() + index);
	}

	void clear()
	{
		for (size_t i = 0; i < _data.size(); i++)
		{
            CC_SAFE_RELEASE(_data[i]);
		}
		_data.clear();
	}

	void swap(T object1, T object2)
	{
		size_t idx1 = getIndex(object1);
		size_t idx2 = getIndex(object2);

		CCAssert(idx1 >= 0 && idx2 >= 0, "invalid object index");
		std::swap(_data[idx1], _data[idx2]);
	}

	void swap(size_t index1, size_t index2)
	{
		CCAssert(index1 >= 0 && index1 < size() && index2 >= 0 && index2 < size(), "Invalid indices");
		std::swap(_data[index1], _data[index2]);
	}

	void replace(size_t index, T object)
	{
		CCAssert(index >= 0 && index < size(), "Invalid index!");

        CC_SAFE_RELEASE(_data[index]);
		_data[index] = object;
        CC_SAFE_RETAIN(object);
	}

	void reverse()
	{
		std::reverse(_data.begin(), _data.end());
	}


protected:
	void addRefForAllObjects()
	{
		for (size_t i = 0; i < _data.size(); i++)
		{
            CC_SAFE_RETAIN(_data[i]);
		}
	}
	std::vector<T> _data;
};

template <class T>
class CAList
{
public:
public:
	typedef typename std::list<T>::iterator iterator;
	typedef typename std::list<T>::const_iterator const_iterator;

	typedef typename std::list<T>::reverse_iterator reverse_iterator;
	typedef typename std::list<T>::const_reverse_iterator const_reverse_iterator;

	iterator begin() { return _data.begin(); }
	const_iterator begin() const { return _data.begin(); }

	iterator end() { return _data.end(); }
	const_iterator end() const { return _data.end(); }

	reverse_iterator rbegin() { return _data.rbegin(); }
	const_reverse_iterator rbegin() const { return _data.rbegin(); }

	reverse_iterator rend() { return _data.rend(); }
	const_reverse_iterator rend() const { return _data.rend(); }



	CAList<T>() {}

	~CAList<T>()
	{
		clear();
	}

	CAList<T>(const CAList<T>& other)
	{
		_data = other._data;
		addRefForAllObjects();
	}

    CAList<T>(const CAVector<T>& other)
	{
		CCLOGINFO("In the copy constructor!");
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAList<T>(const CADeque<T>& other)
	{
		CCLOGINFO("In the copy constructor!");
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAList<T>(const std::vector<T>& other)
	{
		CCLOGINFO("In the copy constructor!");
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAList<T>(const std::deque<T>& other)
	{
		CCLOGINFO("In the copy constructor!");
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAList<T>(const std::list<T>& other)
	{
		CCLOGINFO("In the copy constructor!");
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
	CAList<T>& operator=(const CAList<T>& other)
	{
		if (this != &other)
		{
			clear();
			_data = other._data;
			addRefForAllObjects();
		}
		return *this;
	}

	size_t size() const
	{
		return  _data.size();
	}

	bool empty() const
	{
		return _data.empty();
	}

	const_iterator find(T object) const
	{
		return std::find(_data.begin(), _data.end(), object);
	}

	iterator find(T object)
	{
		return std::find(_data.begin(), _data.end(), object);
	}

	T front() const
	{
		return _data.front();
	}

	T back() const
	{
		return _data.back();
	}

	bool contains(T object) const
	{
		return(std::find(_data.begin(), _data.end(), object) != _data.end());
	}
	bool equals(const CAList<T> &other)
	{
		size_t s = this->size();
		if (s != other.size())
			return false;

		const_iterator it1 = this->begin();
		const_iterator it2 = other.begin();
		for (size_t i = 0; i < s; i++, ++it1, ++it2)
		{
			if (*it1 != *it2)
				return false;
		}
		return true;
	}

    void insert(size_t index, T object)
	{
		CCAssert(index >= 0 && index <= size(), "Invalid index!");
		_data.insert(_data.begin() + index, object);
        CC_SAFE_RETAIN(object);
	}
    
	void pushBack(T object)
	{
		_data.push_back(object);
        CC_SAFE_RETAIN(object);
	}
	void pushBack(const CAList<T>& other)
	{
		for (CAList<T>::const_iterator it = other.begin(); it != other.end(); ++it)
		{
			pushBack(*it);
		}
	}

	void popBack()
	{
		if (_data.empty())
			return;

		T last = _data.back();
		_data.pop_back();
        CC_SAFE_RELEASE(last);
	}


	void pushFront(T object)
	{
		_data.push_front(object);
        CC_SAFE_RETAIN(object);
	}
	void pushFront(const CAList<T>& other)
	{
		for (CAList<T>::reverse_iterator it = other.rbegin(); it != other.rend(); ++it)
		{
			pushFront(*it);
		}
	}

	void popFront()
	{
		if (_data.empty())
			return;

		T first = _data.front();
		_data.pop_front();
        CC_SAFE_RELEASE(first);
	}



	void eraseObject(T object, bool removeAll = false)
	{
		if (removeAll)
		{
			for (iterator iter = _data.begin(); iter != _data.end();)
			{
				if ((*iter) == object)
				{
					iter = _data.erase(iter);
                    CC_SAFE_RELEASE(object);
				}
				else
				{
					++iter;
				}
			}
		}
		else
		{
			iterator iter = std::find(_data.begin(), _data.end(), object);
			if (iter != _data.end())
			{
				_data.erase(iter);
				CC_SAFE_RELEASE(object);
			}
		}
	}

	iterator erase(iterator position)
	{
		CCAssert(position >= _data.begin() && position < _data.end(), "Invalid position!");
        CC_SAFE_RELEASE(*position);
		return _data.erase(position);
	}

	iterator erase(iterator first, iterator last)
	{
		for (iterator iter = first; iter != last; ++iter)
		{
            CC_SAFE_RELEASE(*iter);
		}
		return _data.erase(first, last);
	}

	void clear()
	{
		for (iterator iter = _data.begin(); iter != _data.end(); ++iter)
		{
			CC_SAFE_RELEASE((*iter));
		}
		_data.clear();
	}

	void reverse()
	{
		std::reverse(_data.begin(), _data.end());
	}


protected:


	void addRefForAllObjects()
	{
		for (iterator it = _data.begin(); it != _data.end(); ++it)
		{
            CC_SAFE_RETAIN(*it);
		}
	}
	std::list<T> _data;
};


template <class T>
class CADeque
{
public:
	typedef typename std::deque<T>::iterator iterator;
	typedef typename std::deque<T>::const_iterator const_iterator;

	typedef typename std::deque<T>::reverse_iterator reverse_iterator;
	typedef typename std::deque<T>::const_reverse_iterator const_reverse_iterator;

	iterator begin() { return _data.begin(); }
	const_iterator begin() const { return _data.begin(); }

	iterator end() { return _data.end(); }
	const_iterator end() const { return _data.end(); }

	reverse_iterator rbegin() { return _data.rbegin(); }
	const_reverse_iterator rbegin() const { return _data.rbegin(); }

	reverse_iterator rend() { return _data.rend(); }
	const_reverse_iterator rend() const { return _data.rend(); }

	CADeque<T>() {}

	~CADeque<T>()
	{
		clear();
	}

	CADeque<T>(const CADeque<T>& other)
	{
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}

    CADeque<T>(const CAVector<T>& other)
	{
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CADeque<T>(const CAList<T>& other)
	{
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CADeque<T>(const std::vector<T>& other)
	{
		CCLOGINFO("In the copy constructor!");
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CADeque<T>(const std::deque<T>& other)
	{
		CCLOGINFO("In the copy constructor!");
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CADeque<T>(const std::list<T>& other)
	{
		CCLOGINFO("In the copy constructor!");
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
	const CADeque<T>& operator=(const CADeque<T>& other)
	{
		if (this != &other)
		{
			CCLOGINFO("In the copy assignment operator!");
			clear();
			_data = other._data;
			addRefForAllObjects();
		}
		return *this;
	}

    void reserve(size_t n)
    {
        _data.reserve(n);
    }
    
    void resize(size_t n)
    {
        _data.resize(n);
    }
    
    size_t capacity() const
    {
        return _data.capacity();
    }
    
    size_t size() const
    {
        return _data.size();
    }
    
    bool empty() const
    {
        return _data.empty();
    }
    
    size_t max_size() const
    {
        return _data.max_size();
    }
    
    size_t getIndex(T object) const
    {
        const_iterator iter = std::find(_data.begin(), _data.end(), object);
        if (iter != _data.end())
            return iter - _data.begin();
        
        return -1;
    }
    
    const_iterator find(T object) const
    {
        return std::find(_data.begin(), _data.end(), object);
    }
    
    iterator find(T object)
    {
        return std::find(_data.begin(), _data.end(), object);
    }
    
    T at(size_t index) const
    {
        CCAssert(index >= 0 && index < size(), "index out of range in getObjectAtIndex()");
        return _data[index];
    }
    
    T front() const
    {
        return _data.front();
    }
    
    T back() const
    {
        return _data.back();
    }
    
    T getRandomObject() const
    {
        if (!_data.empty())
        {
            size_t randIdx = rand() % _data.size();
            return *(_data.begin() + randIdx);
        }
        return NULL;
    }
    
    bool contains(T object) const
    {
        return(std::find(_data.begin(), _data.end(), object) != _data.end());
    }
    bool equals(const CAVector<T> &other)
    {
        size_t s = this->size();
        if (s != other.size())
            return false;
        
        for (size_t i = 0; i < s; i++)
        {
            if (this->at(i) != other.at(i))
            {
                return false;
            }
        }
        return true;
    }
    
    void insert(size_t index, T object)
    {
        CCAssert(index >= 0 && index <= size(), "Invalid index!");
        _data.insert(_data.begin() + index, object);
        CC_SAFE_RETAIN(object);
    }
    
    void pushBack(T object)
    {
        _data.push_back(object);
        CC_SAFE_RETAIN(object);
    }
    
    void pushFront(T object)
    {
        _data.push_front(object);
        CC_SAFE_RETAIN(object);
    }
    
    void pushBack(const CADeque<T>& other)
    {
        for (size_t i = 0; i < other.size(); i++)
        {
            pushBack(other.at(i));
        }
    }
    
    void popFront()
    {
        if (_data.empty())
            return;
        
        T first = _data.front();
        _data.pop_front();
        CC_SAFE_RELEASE(first);
    }
    
    void popBack()
    {
        CCAssert(!_data.empty(), "no objects added");
        T last = _data.back();
        _data.pop_back();
        CC_SAFE_RELEASE(last);
    }
    
    void eraseObject(T object, bool removeAll = false)
    {
        if (removeAll)
        {
            
            for (iterator iter = _data.begin(); iter != _data.end();)
            {
                if ((*iter) == object)
                {
                    iter = _data.erase(iter);
                    CC_SAFE_RELEASE(object);
                }
                else
                {
                    ++iter;
                }
            }
        }
        else
        {
            iterator iter = std::find(_data.begin(), _data.end(), object);
            if (iter != _data.end())
            {
                _data.erase(iter);
                CC_SAFE_RELEASE(object);
            }
        }
    }
    
    iterator erase(iterator position)
    {
        CCAssert(position >= _data.begin() && position < _data.end(), "Invalid position!");
        CC_SAFE_RELEASE(*position);
        return _data.erase(position);
    }
    
    iterator erase(iterator first, iterator last)
    {
        for (iterator iter = first; iter != last; ++iter)
        {
            CC_SAFE_RELEASE((*iter));
        }
        return _data.erase(first, last);
    }
    
    iterator erase(size_t index)
    {
        CCAssert(!_data.empty() && index >= 0 && index < size(), "Invalid index!");
        return erase(_data.begin() + index);
    }
    
    void clear()
    {
        for (size_t i = 0; i < _data.size(); i++)
        {
            CC_SAFE_RELEASE(_data[i]);
        }
        _data.clear();
    }
    
    void swap(T object1, T object2)
    {
        size_t idx1 = getIndex(object1);
        size_t idx2 = getIndex(object2);
        
        CCAssert(idx1 >= 0 && idx2 >= 0, "invalid object index");
        std::swap(_data[idx1], _data[idx2]);
    }
    
    void swap(size_t index1, size_t index2)
    {
        CCAssert(index1 >= 0 && index1 < size() && index2 >= 0 && index2 < size(), "Invalid indices");
        std::swap(_data[index1], _data[index2]);
    }
    
    void replace(size_t index, T object)
    {
        CCAssert(index >= 0 && index < size(), "Invalid index!");
        
        CC_SAFE_RELEASE(_data[index]);
        _data[index] = object;
        CC_SAFE_RETAIN(object);
    }
    
    void reverse()
    {
        std::reverse(_data.begin(), _data.end());
    }

private:
    
	void addRefForAllObjects()
	{
		for (const_iterator it = _data.begin(); it != _data.end(); ++it)
		{
            CC_SAFE_RETAIN(*it);
		}
	}
	std::deque <T> _data;
};


template <class K, class T>
class CAMap
{
public:
	typedef typename std::map<K, T>::iterator iterator;
	typedef typename std::map<K, T>::const_iterator const_iterator;

	typedef typename std::map<K, T>::reverse_iterator reverse_iterator;
	typedef typename std::map<K, T>::const_reverse_iterator const_reverse_iterator;

	iterator begin() { return _data.begin(); }
	const_iterator begin() const { return _data.begin(); }

	iterator end() { return _data.end(); }
	const_iterator end() const { return _data.end(); }

	reverse_iterator rbegin() { return _data.rbegin(); }
	const_reverse_iterator rbegin() const { return _data.rbegin(); }

	reverse_iterator rend() { return _data.rend(); }
	const_reverse_iterator rend() const { return _data.rend(); }

	CAMap<K, T>() {}

	~CAMap<K, T>()
	{
		clear();
	}

	CAMap<K, T>(const CAMap<K, T>& other)
	{
		_data = other._data;
		addRefForAllObjects();
	}

	const CAMap<K, T>& operator=(const CAMap<K, T>& other)
	{
		if (this != &other)
		{
			CCLOGINFO("In the copy assignment operator!");
			clear();
			_data = other._data;
			addRefForAllObjects();
		}
		return *this;
	}

	bool insert(K key, T object)
	{
		iterator it = _data.find(key);
		if (it != _data.end())
		{
			return false;
		}
        CC_SAFE_RETAIN(object);
		_data[key] = object;
        return true;
	}

	size_t size() const
	{
		return _data.size();
	}

	bool empty() const
	{
		return _data.empty();
	}

	bool contains(K key)
	{
		return _data.find(key) != _data.end();
	}

	bool erase(K key)
	{
		iterator it = _data.find(key);
		if (it != _data.end())
		{
            CC_SAFE_RELEASE_NULL(it->second);
			_data.erase(it);
			return true;
		}
		return false;
	}

	T getValue(K key)
	{
		iterator it = _data.find(key);
		if (it != _data.end())
		{
			return it->second;
		}
		return NULL;
	}

	std::vector<K> getKeys()
	{
		std::vector<K> keys;
		for (iterator it = _data.begin(); it != _data.end(); it++)
		{
			keys.push_back(it->first);
		}
		return keys;
	}

	void clear()
	{
		for (iterator it = _data.begin(); it != _data.end(); it++)
		{
            CC_SAFE_RELEASE_NULL(it->second);
		}
		_data.clear();
	}

protected:
	void addRefForAllObjects()
	{
		for (iterator it = _data.begin(); it != _data.end(); ++it)
		{
            CC_SAFE_RETAIN(it->second);
		}
	}
	std::map <K, T> _data;
};


NS_CC_END

#endif