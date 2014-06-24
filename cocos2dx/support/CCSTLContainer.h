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


void addToObjPtrSets(void* ptr);

void delToObjPtrSets(void* ptr);

bool isCCObjectPtr(void* ptr);

void retainObjPtr(void* ptr);

void releaseObjPtr(void* ptr);


template <typename T>
class CAVector
{
public:
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
		clear();
	}
	int GetCount()
	{
		return m_Container.size();
	}

private:
	std::vector<T*> m_Container;
};


template <typename T>
class CADeque
{
public:
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
		std::deque<T*>::iterator it = v.begin();
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

private:
	std::deque <T*> m_Container;
};


template <typename K, typename T>
class CAMap
{
public:
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
		std::map<K, T*>::iterator it = v.begin();
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
		std::map <K, T*>::iterator it = m_Container.find(key);
		if (it != m_Container.end())
		{
			releaseObjPtr(it->second);
			m_Container.erase(it);
		}
	}

	T* GetValueByKey(const K& key)
	{
		std::map <K, T*>::iterator it = m_Container.find(key);
		if (it != m_Container.end())
		{
			return it->second;
		}
		return NULL;
	}

	std::vector<K> getKeys()
	{
		std::vector<K> keys;
		for (std::map <K, T*>::iterator it = begin(); it != end(); it++)
		{
			keys.push_back(it->first);
		}
		return keys;
	}

	void Clear()
	{
		for (std::map <K, T*>::iterator it = begin(); it != end(); it++)
		{
			releaseObjPtr(it->second);
		}
		m_Container.clear();
	}

private:
	std::map <K, T*> m_Container;
};


template <typename T>
class CAList
{
public:
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
		clear();
	}
	int GetCount()
	{
		return m_Container.size();
	}

private:
	std::list<T*> m_Container;
};

NS_CC_END

#endif