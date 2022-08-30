#pragma once
#ifndef __ENGINE_BASE_H__
#define __ENGINE_BASE_H__

#include "3DEngine_Defines.h"

class ENGINE_DLL CBase abstract
{
public:
	CBase();
	virtual ~CBase() DEFAULT;
	
public:
	virtual	unsigned	long	AddRef();
	virtual unsigned	long	Release();

private:
	unsigned long				m_dwRefCnt = 0;

public:
	virtual void				Free() PURE;

};


#endif // !__ENGINE_BASE_H__