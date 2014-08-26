

#ifndef _COCOLOADER_H
#define _COCOLOADER_H

#include <stdio.h>
#include <vector>
#include <cstdio>
#include <stdint.h>
#include "ExtensionMacros.h"
#include "../rapidjson/rapidjson/rapidjson.h"
#include "../rapidjson/rapidjson/document.h"

#pragma pack (4)

NS_CC_EXT_BEGIN

struct  stExpCocoAttribDesc
{
	rapidjson::Type	m_Type;
	uint64_t m_szName;
	uint64_t m_szDefaultValue;
public:
	
	void ReBuild(char* pStringMemoryAddr)
	{
		m_szName = m_szName + (uint64_t)pStringMemoryAddr;
		m_szDefaultValue = m_szDefaultValue + (uint64_t)pStringMemoryAddr;
	}
};

struct  stExpCocoObjectDesc
{
	uint32_t		m_nAttribNum;
	uint64_t		m_szName;
	uint64_t		m_pAttribDescArray;

public:
	stExpCocoObjectDesc()
	{
		m_nAttribNum = 0;
		m_szName = 0;
		m_pAttribDescArray = 0;
	}
	void ReBuild(char* pAttribMemoryAddr,char* pStringMemoryAddr)
	{
		m_szName = m_szName + (uint64_t)pStringMemoryAddr;
		m_pAttribDescArray = m_pAttribDescArray + (uint64_t)pAttribMemoryAddr;
		stExpCocoAttribDesc* tpAttribDescArray = (stExpCocoAttribDesc*)m_pAttribDescArray;
        for (uint32_t i = 0; i < m_nAttribNum; i++)
		{
			tpAttribDescArray[i].ReBuild(pStringMemoryAddr);
		}
	}
	
};
   
class CocoLoader;

struct stExpCocoNode
{
protected:
	int32_t	m_ObjIndex;
	int32_t	m_AttribIndex;
	uint32_t	m_ChildNum;
	uint64_t	m_szValue;
	uint64_t	m_ChildArray;
    
public:
    rapidjson::Type	 GetType(CocoLoader*	pCoco);
	char*	GetName(CocoLoader*	pCoco);
	char*	GetValue();
	int	GetChildNum();
	stExpCocoNode*	GetChildArray();
    
public:
	inline  void	ReBuild(char* pCocoNodeAddr,char* pStringMemoryAddr);
	void	WriteJson(CocoLoader* pCoco, void* pFileName = NULL, int vLayer = 0, bool bEndNode = false, bool bParentNodeIsArray = false);
};


struct	stCocoFileHeader
{
	char	m_FileDesc[32];
	char	m_Version[32];
	uint32_t	m_nFirstUsed;
	uint32_t	m_ObjectCount;
	uint64_t	m_lAttribMemAddr;
	uint64_t	m_CocoNodeMemAddr;
	uint64_t	m_lStringMemAddr;
	
};


class CocoLoader
{
private:
	stCocoFileHeader*	m_pFileHeader;
	stExpCocoNode*		m_pRootNode;
	stExpCocoObjectDesc*	m_pObjectDescArray;

public:
	CocoLoader();
	~CocoLoader();

public:
	bool	ReadCocoBinBuff(char* pBinBuff);
	stCocoFileHeader*	GetFileHeader(){return m_pFileHeader;}
	stExpCocoNode*		GetRootCocoNode(){return	m_pRootNode;}
	stExpCocoObjectDesc*	GetCocoObjectDescArray(){return	m_pObjectDescArray;}
	stExpCocoObjectDesc*	GetCocoObjectDesc(const char* szObjDesc);
};

NS_CC_EXT_END


#pragma pack ()
#endif