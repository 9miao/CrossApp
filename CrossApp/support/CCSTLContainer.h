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
		Clear();
	}
	CAVector(const CAVector& v)
	{
		Clear();
		AddRange(v.m_Container);
	}
	const CAVector& operator=(const CAVector& v)
	{
		Clear();
		AddRange(v.m_Container);
		return *this;
	}

	void Add(T* v)
	{
		retainObjPtr(v);
		m_Container.push_back(v);
	}
	void Insert(int index, T* v)
	{
		CCAssert(index >= 0 && index < m_Container.size(), "");
		retainObjPtr(v);
		m_Container.insert(m_Container.begin() + index, v);
	}

	void AddRange(const std::vector<T*>& v)
	{
		for (int i = 0; i < v.size(); i++)
		{
			retainObjPtr(v[i]);
		}
		m_Container.insert(m_Container.end(), v.begin(), v.end());
	}
	int  IndexOf(T* v)
	{
		for (int i = 0; i < m_Container.size(); i++)
		{
			if (m_Container[i] == v) return i;
		}
		return -1;
	}
	bool Contains(T* v)
	{
		return IndexOf(v) != -1;
	}

	void Remove(T* v)
	{
		int index = IndexOf(v);
		if (index != -1)
		{
			releaseObjPtr(v);
			m_Container.erase(m_Container.begin() + index);
		}
	}
	void RemoveAt(int index)
	{
		CCAssert(index >= 0 && index < m_Container.size(), "");
		releaseObjPtr(m_Container[index]);
		m_Container.erase(m_Container.begin() + index);
	}

	void Clear()
	{
		for (int i = 0; i < m_Container.size(); i++)
		{
			releaseObjPtr(m_Container[i]);
		}
		m_Container.clear();
	}
	int GetCount()
	{
		return m_Container.size();
	}

public:
	std::vector<T*> m_Container;
};


template <typename T>
class CADeque
{
public:
	CADeque() {}
	~CADeque()
	{
		Clear();
	}
	CADeque(const CADeque& v)
	{
		Clear();
		AddRange(v.m_Container);
	}
	const CADeque& operator=(const CADeque& v)
	{
		Clear();
		AddRange(v.m_Container);
		return *this;
	}
	void AddRange(std::deque<T*>& v)
	{
		typename std::deque<T*>::iterator it = v.begin();
		for (; it != v.end(); it++)
		{
			push_back(*it);
		}
	}
	int GetCount()
	{
		return m_Container.size();
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
	void pop_back()
	{
		if (m_Container.empty())
		{
			return;
		}

		releaseObjPtr(m_Container.back());
		m_Container.pop_back();
	}
	void pop_front()
	{
		if (m_Container.empty())
		{
			return;
		}

		releaseObjPtr(m_Container.front());
		m_Container.pop_front();
	}

	void Clear()
	{
		while (!m_Container.empty())
		{
			pop_front();
		}
	}

public:
	std::deque <T*> m_Container;
};


template <typename K, typename T>
class CAMap
{
public:
	CAMap() {}
	~CAMap()
	{
		Clear();
	}
	CAMap(const CAMap& v)
	{
		Clear();
		AddRange(v.m_Container);
	}
	const CAMap& operator=(const CAMap& v)
	{
		Clear();
		AddRange(v.m_Container);
		return *this;
	}
	void AddRange(const std::map<K, T*>& v)
	{
		typename std::map<K, T*>::iterator it = v.begin();
		for (; it != v.end(); it++)
		{
			Add(it->first, it->second);
		}
	}
	void Add(K key, T* v)
	{
		T* p = GetValueByKey(key);
		if (p)
		{
			retainObjPtr(p);
		}

		if (v)
		{
			retainObjPtr(v);
		}
		m_Container[key] = v;
	}
	int GetCount()
	{
		return m_Container.size();
	}
	bool ContainsKey(K key)
	{
		return m_Container.find(key) != m_Container.end();
	}
	void Remove(K key)
	{
		typename std::map <K, T*>::iterator it = m_Container.find(key);
		if (it != m_Container.end())
		{
			releaseObjPtr(it->second);
			m_Container.erase(it);
		}
	}

	T* GetValueByKey(const K& key)
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

	void Clear()
	{
		for (typename std::map <K, T*>::iterator it = m_Container.begin(); it != m_Container.end(); it++)
		{
			releaseObjPtr(it->second);
		}
		m_Container.clear();
	}

public:
	std::map <K, T*> m_Container;
};


template <typename T>
class CAList
{
public:
	CAList() {}
	~CAList()
	{
		Clear();
	}
	CAList(const CAList& v)
	{
		Clear();
		AddRange(v.m_Container);
	}
	const CAList& operator=(const CAList& v)
	{
		Clear();
		AddRange(v.m_Container);
		return *this;
	}

	void Add(T* v)
	{
		retainObjPtr(v);
		m_Container.push_back(v);
	}
	void Insert(int index, T* v)
	{
		CCAssert(index >= 0 && index < m_Container.size(), "");
		retainObjPtr(v);
		m_Container.insert(m_Container.begin() + index, v);
	}

	void AddRange(const std::vector<T*>& v)
	{
		for (int i = 0; i < v.size(); i++)
		{
			retainObjPtr(v[i]);
		}
		m_Container.insert(m_Container.end(), v.begin(), v.end());
	}
	int  IndexOf(T* v)
	{
		for (int i = 0; i < m_Container.size(); i++)
		{
			if (m_Container[i] == v) return i;
		}
		return -1;
	}
	bool Contains(T* v)
	{
		return IndexOf(v) != -1;
	}

	void Remove(T* v)
	{
		int index = IndexOf(v);
		if (index != -1)
		{
			releaseObjPtr(v);
			m_Container.erase(m_Container.begin() + index);
		}
	}

	void Clear()
	{
		for (int i = 0; i < m_Container.size(); i++)
		{
			releaseObjPtr(m_Container[i]);
		}
		m_Container.clear();
	}
	int GetCount()
	{
		return m_Container.size();
	}

public:
	std::list<T*> m_Container;
};

template <typename K, typename T>
class CAMultimap
{
public:
	CAMultimap() {}
	~CAMultimap()
	{
		Clear();
	}
	CAMultimap(const CAMultimap& v)
	{
		Clear();
		AddRange(v.m_Container);
	}
	const CAMultimap& operator=(const CAMultimap& v)
	{
		Clear();
		AddRange(v.m_Container);
		return *this;
	}
	void AddRange(const std::multimap<K, T*>& v)
	{
		typename std::multimap<K, T*>::iterator it = v.begin();
		for (; it != v.end(); it++)
		{
			Add(it->first, it->second);
		}
	}
	void Add(K key, T* v)
	{
		retainObjPtr(v);
		m_Container.insert(std::make_pair(key, v));
	}
	int GetCount()
	{
		return m_Container.size();
	}
	bool ContainsKey(K key)
	{
		std::pair<typename std::multimap<K, T*>::iterator, typename std::multimap<K, T*>::iterator> pos = m_Container.equal_range(key);
		return pos.first != m_Container.end();
	}
	void Remove(K key)
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

	std::vector<T*> GetValueByKey(const K& key)
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

	void Clear()
	{
		for (typename std::multimap <K, T*>::iterator it = m_Container.begin(); it != m_Container.end(); it++)
		{
			releaseObjPtr(it->second);
		}
		m_Container.clear();
	}

public:
	std::multimap <K, T*> m_Container;
};

NS_CC_END

#endif