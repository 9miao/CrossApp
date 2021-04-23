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
class CAVector: public CAObject
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
		reserve(capacity);
	}

	~CAVector<T>()
	{
		clear();
	}

	CAVector<T>(const CAVector<T>& other)
	{
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
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAVector<T>(const std::deque<T>& other)
	{
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAVector<T>(const std::list<T>& other)
	{
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
	CAVector<T>& operator=(const CAVector<T>& other)
	{
		if (this != &other)
		{
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
        if (index < 0 || index >= size())
        {
            return NULL;
        }
		return _data[index];
	}

	T front() const
	{
        if (_data.empty())
        {
            return NULL;
        }
		return _data.front();
	}

	T back() const
	{
        if (_data.empty())
        {
            return NULL;
        }
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
		for (auto it = other.begin(); it != other.end(); it++)
		{
			pushBack(*it);
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
			CC_SAFE_RELEASE((CAObject*)_data[i]);
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

	void reserve()
	{
		reserve(_data.begin(), _data.end());
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
class CAList: public CAObject
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
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAList<T>(const CADeque<T>& other)
	{
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAList<T>(const std::vector<T>& other)
	{
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAList<T>(const std::deque<T>& other)
	{
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CAList<T>(const std::list<T>& other)
	{
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
        if (_data.empty())
        {
            return NULL;
        }
        return _data.front();
    }
    
    T back() const
    {
        if (_data.empty())
        {
            return NULL;
        }
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
		for (auto it = other.begin(); it != other.end(); ++it)
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
			CC_SAFE_RELEASE((CAObject*)(*iter));
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

template <class T1, class T2>
class CAPair : public CAObject
{
public:
	CAPair<T1, T2>() {}

	CAPair<T1, T2>(T1 obj1, T2 obj2)
	{
		CC_SAFE_RETAIN(obj1);
		CC_SAFE_RETAIN(obj2);
		_cpair = std::make_pair(obj1, obj2);
	}

	CAPair<T1, T2>(const std::pair<T1, T2>& c)
	{
		CC_SAFE_RETAIN(c.first);
		CC_SAFE_RETAIN(c.second);
		_cpair = c;
	}


	CAPair<T1, T2>(const CAPair<T1, T2>& other)
	{
		clear();
		_cpair = other;
		CC_SAFE_RETAIN(_cpair.first);
		CC_SAFE_RETAIN(_cpair.second);
	}

	CAPair<T1, T2>& operator=(const CAPair<T1, T2>& other)
	{
		if (this != &other)
		{
			clear();
			_cpair = other._cpair;
			CC_SAFE_RETAIN(_cpair.first);
			CC_SAFE_RETAIN(_cpair.second);
		}
		return *this;
	}

	~CAPair<T1, T2>()
	{
		clear();
	}

	void clear()
	{
		CC_SAFE_RELEASE(_cpair.first);
		CC_SAFE_RELEASE(_cpair.second);
		_cpair.first = _cpair.first = NULL;
	}

	T1 first() { return _cpair.first; }
	T2 second() { return _cpair.second; }

private:
	std::pair<T1, T2> _cpair;
};


template <class T>
class CADeque: public CAObject
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
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CADeque<T>(const std::deque<T>& other)
	{
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
    CADeque<T>(const std::list<T>& other)
	{
		_data.insert(_data.begin(), other.begin(), other.end());
		addRefForAllObjects();
	}
    
	const CADeque<T>& operator=(const CADeque<T>& other)
	{
		if (this != &other)
		{
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
        if (index < 0 || index >= size())
        {
            return NULL;
        }
        return _data[index];
    }
    
    T front() const
    {
        if (_data.empty())
        {
            return NULL;
        }
        return _data.front();
    }
    
    T back() const
    {
        if (_data.empty())
        {
            return NULL;
        }
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
        for (auto it = other.begin(); it != other.end(); it++)
        {
            pushBack(*it);
        }
    }
    
    void pushFront(const CADeque<T>& other)
    {
        for (auto it = other.rbegin(); it != other.rend(); it++)
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
			CC_SAFE_RELEASE((CAObject*)_data[i]);
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
class CAMap: public CAObject
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
			clear();
			_data = other._data;
			addRefForAllObjects();
		}
		return *this;
	}

	void insert(K key, T object)
	{
        CC_SAFE_RETAIN(object);
        erase(key);
        _data.insert(std::make_pair(key, object));
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

    iterator erase(iterator it)
    {
        if (_data.count(it->first) > 0)
        {
            CAObject* o = (CAObject*)it->second;
            CC_SAFE_RELEASE_NULL(o);
            return _data.erase(it);
        }
        return _data.end();
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

	T at(K key)
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

    void erase(const std::vector<K>& keys)
    {
        for(const auto &key : keys)
        {
            this->erase(key);
        }
    }
    
	void clear()
	{
		for (iterator it = _data.begin(); it != _data.end(); it++)
		{
			CAObject* o = (CAObject*)it->second;
			CC_SAFE_RELEASE_NULL(o);
		}
		_data.clear();
	}
    
    void reserve(int rsize)
    {
        _data.reserve(rsize);
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
