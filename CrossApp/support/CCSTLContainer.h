//
//  CCSTLContainer.h
//  CrossApp
//
//  Created by Zhujian on 14-6-10.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//
#ifndef __cocos2dx__CCSTLContainer__
#define __cocos2dx__CCSTLContainer__

#include "ccTypes.h"
#include "ccMacros.h"
#include <vector>
#include <deque>
#include <map>
#include <list>
#include <algorithm>

NS_CC_BEGIN


CC_DLL void addToObjPtrSets(void* ptr);

CC_DLL void delToObjPtrSets(void* ptr);

CC_DLL bool isCCObjectPtr(void* ptr);

CC_DLL void retainObjPtr(void* ptr);

CC_DLL void releaseObjPtr(void* ptr);



template <typename T>
class CAVector
{
public:
    
	CAVector() {}
    
	~CAVector()
	{
		clear();
	}
    
	CAVector(const CAVector& v)
	{
		clear();
		insert(v.m_Container);
	}
    
	const CAVector& operator=(const CAVector& v)
	{
		clear();
		insert(v.m_Container);
		return *this;
	}

	void push_back(T* v)
	{
		retainObjPtr(v);
		m_Container.push_back(v);
	}
    
	void insert(int index, T* v)
	{
		CCAssert(index >= 0 && index <= m_Container.size(), "");
		retainObjPtr(v);
		m_Container.insert(m_Container.begin() + index, v);
	}
    
	void insert(int index, typename std::vector<T*>::iterator itBegin, typename std::vector<T*>::iterator itEnd)
	{
		CCAssert(index >= 0 && index <= m_Container.size(), "");
		typename std::vector<T*>::iterator itTemp = itBegin;
		while (itTemp != itEnd)
		{
			retainObjPtr(*itTemp++);
		}
		m_Container.insert(m_Container.begin() + index, itBegin, itEnd);
	}
    
	void insert(int index, std::vector<T*>& v)
	{
		CCAssert(index >= 0 && index <= m_Container.size(), "");
		insert(index, v.begin(), v.end());
	}
    
	void insert(const std::vector<T*>& v)
	{
		insert(m_Container.size(), v.begin(), v.end());
	}
    
    void pop_back()
	{
		if (m_Container.empty())
		{
			return;
		}
		releaseObjPtr(back());
		m_Container.pop_back();
	}
    
	void erase(T* v)
	{
		do
        {
            int index = indexOf(v);
            CC_BREAK_IF(index == -1);
            releaseObjPtr(v);
            m_Container.erase(m_Container.begin() + index);
        }
        while (1);
	}
    
	void erase(int index)
	{
		CCAssert(index >= 0 && index < m_Container.size(), "");
		releaseObjPtr(m_Container[index]);
		m_Container.erase(m_Container.begin() + index);
	}
    
    int indexOf(T* v)
	{
		for (int i = 0; i < m_Container.size(); i++)
		{
			if (m_Container[i] == v) return i;
		}
		return -1;
	}
    
	bool contains(T* v)
	{
		return indexOf(v) != -1;
	}
    
	void clear()
	{
		for (int i = 0; i < m_Container.size(); i++)
		{
			releaseObjPtr(m_Container[i]);
		}
		m_Container.clear();
	}
    
	int size()
	{
		return m_Container.size();
	}
    
    bool empty()
    {
        return m_Container.empty();
    }
    
	typename std::vector<T*>::iterator begin()
	{
		return m_Container.begin();
	}
    
	typename std::vector<T*>::iterator end()
	{
		return m_Container.end();
	}
    
	T* at(int index)
	{
		return m_Container.at(index);
	}
    
	T* front()
	{
		if (m_Container.empty())
		{
			return NULL;
		}
		return m_Container.front();
	}
    
	T* back()
	{
		if (m_Container.empty())
		{
			return NULL;
		}
		return m_Container.back();
	}

private:
    
	std::vector<T*> m_Container;
};

template <typename T>
class CAList
{
public:
    
	CAList() {}
    
	~CAList()
	{
		clear();
	}
    
	CAList(const CAList& v)
	{
		clear();
		insert(v.m_Container);
	}
    
	const CAList& operator=(const CAList& v)
	{
		clear();
		insert(v.m_Container);
		return *this;
	}

    void push_front(T* v)
	{
		retainObjPtr(v);
		m_Container.push_front(v);
	}
    
	void push_back(T* v)
	{
		retainObjPtr(v);
		m_Container.push_back(v);
	}
    
	void insert(int index, T* v)
	{
		CCAssert(index >= 0 && index <= m_Container.size(), "");
		retainObjPtr(v);
		m_Container.insert(indexOf(index), v);
	}
    
	void insert(int index, typename std::list<T*>::iterator itBegin, typename std::list<T*>::iterator itEnd)
	{
		CCAssert(index >= 0 && index <= m_Container.size(), "");
		typename std::list<T*>::iterator itTemp = itBegin;
		while (itTemp != itEnd)
		{
			retainObjPtr(*itTemp++);
		}
		m_Container.insert(indexOf(index), itBegin, itEnd);
	}

	void insert(int index, std::list<T*>& v)
	{
		insert(index, v.begin(), v.end());
	}
    
	void insert(const std::list<T*>& v)
	{
		typename std::list<T*>::iterator it = v.begin();
		for (; it != v.end(); it++)
		{
			retainObjPtr(*it);
		}
		m_Container.insert(m_Container.end(), v.begin(), v.end());
	}
    
    void pop_front()
	{
		if (m_Container.empty())
		{
			return;
		}
		releaseObjPtr(front());
		m_Container.pop_front();
	}
    
    void pop_back()
	{
		if (m_Container.empty())
		{
			return;
		}
		releaseObjPtr(back());
		m_Container.pop_back();
	}
    
    void erase(T* v)
    {
		do
        {
            int index = indexOf(v);
            CC_BREAK_IF(index == -1);
            releaseObjPtr(v);
            m_Container.erase(m_Container.begin() + index);
        }
        while (1);
    }
    
	void erase(int index)
	{
        CCAssert(index >= 0 && index < m_Container.size(), "");
		releaseObjPtr(m_Container[index]);
		m_Container.erase(m_Container.begin() + index);
	}

    typename std::list<T*>::iterator indexOf(T* v)
	{
		typename std::list<T*>::iterator it = m_Container.begin();
		for (; it != m_Container.end(); it++)
		{
			if (*it == v) break;
		}
		return it;
	}
    
	typename std::list<T*>::iterator indexOf(int index)
	{
		CCAssert(index >= 0 && index <= m_Container.size(), "");
		typename std::list<T*>::iterator it = m_Container.begin();
		for (int i = 0; i < index; i++) it++;
		return it;
	}
    
	bool contains(T* v)
	{
		return indexOf(v) != -1;
	}
    
	void clear()
	{
		typename std::list<T*>::iterator it = m_Container.begin();
		for (; it != m_Container.end(); it++)
		{
			releaseObjPtr(*it);
		}
		m_Container.clear();
	}
    
	int size()
	{
		return m_Container.size();
	}
    
    bool empty()
    {
        return m_Container.empty();
    }
    
	typename std::list<T*>::iterator begin()
	{
		return m_Container.begin();
	}
    
	typename std::list<T*>::iterator end()
	{
		return m_Container.end();
	}

    T* at(int index)
	{
		return m_Container.at(index);
	}
    
	T* front()
	{
		if (m_Container.empty())
		{
			return NULL;
		}
		return m_Container.front();
	}
    
	T* back()
	{
		if (m_Container.empty())
		{
			return NULL;
		}
		return m_Container.back();
	}

private:
    
	std::list<T*> m_Container;
};


template <typename T>
class CADeque
{
public:
    
	CADeque() {}
    
	~CADeque()
	{
		clear();
	}
    
	CADeque(const CADeque& v)
	{
		clear();
		insert(v.m_Container);
	}
    
	const CADeque& operator=(const CADeque& v)
	{
		clear();
		insert(v.m_Container);
		return *this;
	}
    
    void push_front(T* v)
	{
		retainObjPtr(v);
		m_Container.push_front(v);
	}
    
	void push_back(T* v)
	{
		retainObjPtr(v);
		m_Container.push_back(v);
	}
    
    void insert(int index, typename std::deque<T*>::iterator itBegin, typename std::deque<T*>::iterator itEnd)
	{
		CCAssert(index >= 0 && index <= m_Container.size(), "");
		typename std::deque<T*>::iterator itTemp = itBegin;
		while (itTemp != itEnd)
		{
			retainObjPtr(*itTemp++);
		}
		m_Container.insert(indexOf(index), itBegin, itEnd);
	}
    
	void insert(int index, std::deque<T*>& v)
	{
		insert(index, v.begin(), v.end());
	}
    
	void insert(std::deque<T*>& v)
	{
		typename std::deque<T*>::iterator it = v.begin();
		for (; it != v.end(); it++)
		{
			push_back(*it);
		}
	}
    
    void pop_front()
	{
		if (m_Container.empty())
		{
			return;
		}
		releaseObjPtr(front());
		m_Container.pop_front();
	}
    
    void pop_back()
	{
		if (m_Container.empty())
		{
			return;
		}
		releaseObjPtr(back());
		m_Container.pop_back();
	}
    
	void erase(T* v)
	{
        do
        {
            int index = indexOf(v);
            CC_BREAK_IF(index == -1);
            releaseObjPtr(v);
            m_Container.erase(m_Container.begin() + index);
        }
        while (1);
	}
    
	void erase(int index)
	{
		CCAssert(index >= 0 && index < m_Container.size(), "");
		releaseObjPtr(m_Container[index]);
		m_Container.erase(m_Container.begin() + index);
	}
    
    int  indexOf(T* v)
	{
		for (int i = 0; i < m_Container.size(); i++)
		{
			if (m_Container[i] == v) return i;
		}
		return -1;
	}
    
	bool contains(T* v)
	{
		return indexOf(v) != -1;
	}
    
    void clear()
	{
		while (!m_Container.empty())
		{
			pop_front();
		}
	}
    
	int size()
	{
		return m_Container.size();
	}

    bool empty()
    {
        return m_Container.empty();
    }
    
    typename std::deque<T*>::iterator begin()
	{
		return m_Container.begin();
	}
    
	typename std::deque<T*>::iterator end()
	{
		return m_Container.end();
	}
    
    T* at(int index)
	{
		return m_Container.at(index);
	}
    
    T* front()
	{
		if (m_Container.empty())
		{
			return NULL;
		}
		return m_Container.front();
	}
    
	T* back()
	{
		if (m_Container.empty())
		{
			return NULL;
		}
		return m_Container.back();
	}

private:
    
	std::deque <T*> m_Container;
};


template <typename K, typename T>
class CAMap
{
public:
    
	CAMap() {}
    
	~CAMap()
	{
		clear();
	}
    
	CAMap(const CAMap& v)
	{
		clear();
		insert(v.m_Container);
	}
    
	const CAMap& operator=(const CAMap& v)
	{
		clear();
		insert(v.m_Container);
		return *this;
	}
    
	void insert(const std::map<K, T*>& v)
	{
		typename std::map<K, T*>::iterator it = v.begin();
		for (; it != v.end(); it++)
		{
			insert(it->first, it->second);
		}
	}
    
	bool insert(K key, T* v)
	{
		typename std::map <K, T*>::iterator it = m_Container.find(key);
		if (it != m_Container.end())
		{
			return false;
		}

		if (v)
		{
			retainObjPtr(v);
		}
		m_Container[key] = v;
	}
    
	int size()
	{
		return m_Container.size();
	}
    
    bool empty()
    {
        return m_Container.empty();
    }
    
	bool contains(K key)
	{
		return m_Container.find(key) != m_Container.end();
	}
    
	void erase(K key)
	{
		typename std::map <K, T*>::iterator it = m_Container.find(key);
		if (it != m_Container.end())
		{
			releaseObjPtr(it->second);
			m_Container.erase(it);
		}
	}

	T* getValue(const K& key)
	{
		typename std::map <K, T*>::iterator it = m_Container.find(key);
		if (it != m_Container.end())
		{
			return it->second;
		}
		return NULL;
	}

	std::vector<K> getKeys()
	{
		std::vector<K> keys;
		for (typename std::map <K, T*>::iterator it = m_Container.begin(); it != m_Container.end(); it++)
		{
			keys.push_back(it->first);
		}
		return keys;
	}

	void clear()
	{
		for (typename std::map <K, T*>::iterator it = m_Container.begin(); it != m_Container.end(); it++)
		{
			releaseObjPtr(it->second);
		}
		m_Container.clear();
	}

	typename std::map <K, T*>::iterator begin()
	{
		return m_Container.begin();
	}
    
	typename std::map <K, T*>::iterator end()
	{
		return m_Container.end();
	}

private:
    
	std::map <K, T*> m_Container;
};


template <typename K, typename T>
class CAMultimap
{
public:
    
	CAMultimap() {}
    
	~CAMultimap()
	{
		clear();
	}
    
	CAMultimap(const CAMultimap& v)
	{
		clear();
		insert(v.m_Container);
	}
    
	const CAMultimap& operator=(const CAMultimap& v)
	{
		clear();
		insert(v.m_Container);
		return *this;
	}
    
	void insert(const std::multimap<K, T*>& v)
	{
		typename std::multimap<K, T*>::iterator it = v.begin();
		for (; it != v.end(); it++)
		{
			insert(it->first, it->second);
		}
	}
    
	void insert(K key, T* v)
	{
		retainObjPtr(v);
		m_Container.insert(std::make_pair(key, v));
	}
    
	int size()
	{
		return m_Container.size();
	}
    
    bool empty()
    {
        return m_Container.empty();
    }
    
	bool contains(K key)
	{
		std::pair<typename std::multimap<K, T*>::iterator, typename std::multimap<K, T*>::iterator> pos = m_Container.equal_range(key);
		return pos.first != m_Container.end();
	}
    
	void erase(K key)
	{
		std::pair<typename std::multimap<K, T*>::iterator, typename std::multimap<K, T*>::iterator> pos = m_Container.equal_range(key);
		if (pos.first == m_Container.end())
		{
			return;
		}

		while (pos.first != pos.second)
		{
			releaseObjPtr(pos.first++->second);
		}
		m_Container.erase(key);
	}

	std::vector<T*> getValueByKey(const K& key)
	{
		std::vector<T*> v;
		std::pair<typename std::multimap<K, T*>::iterator, typename std::multimap<K, T*>::iterator> pos = m_Container.equal_range(key);
		while (pos.first != pos.second)
		{
			v.push_back(pos.first++->second);
		}
		return v;
	}

	std::vector<K> getKeys()
	{
		std::vector<K> keys;
		for (typename std::multimap <K, T*>::iterator it = m_Container.begin(); it != m_Container.end(); it++)
		{
			keys.push_back(it->first);
		}
		return keys;
	}

	void clear()
	{
		for (typename std::multimap <K, T*>::iterator it = m_Container.begin(); it != m_Container.end(); it++)
		{
			releaseObjPtr(it->second);
		}
		m_Container.clear();
	}

	typename std::multimap <K, T*>::iterator begin()
	{
		return m_Container.begin();
	}
    
	typename std::multimap <K, T*>::iterator end()
	{
		return m_Container.end();
	}
    
private:
    
	std::multimap <K, T*> m_Container;
};

NS_CC_END

#endif