#pragma once
#ifndef __ENGINE_ASTAR_H__
#define __ENGINE_ASTAR_H__

#include "NavigationMesh.h"

class ENGINE_DLL CAStar final : public CComponent
{
private:
	CAStar(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CAStar(const CAStar& rhs);
	virtual ~CAStar() DEFAULT;

public:
	virtual	HRESULT			NativeConstruct();
	virtual HRESULT			Clone_Construct(void* pArg);

public:
	void					Start_AStar(_fvector vStartPos, _fvector vGoalPos);
	void					Start_Random(_fvector vStartPos);
	_int					Get_CellIndex(_fvector vPosition);
	_int					Get_RandomIndex();
public:
	HRESULT					MakeRoute(_uint iGoalIndex);
	_bool					PathFinder(_uint iGoalIndex, _uint iStartIndex);

public:
	_uint					Get_Start() { return m_iStart; }
	_uint					Get_End() { return m_iEnd; }
	CRITICAL_SECTION		Get_CS() { return m_CS; }
	list<CCell*>&			Get_ListBest() { return m_listBest; }


public:
	_bool					Is_OpenList(_uint iIndex);
	_bool					Is_CloseList(_uint iIndex);

private:
	CNavigationMesh*		m_pNavigation = nullptr;

	list<_uint>				m_listOpen;
	list<_uint>				m_listClose;
	list<CCell*>			m_listBest;

	_uint					m_iStart = 0;
	_uint					m_iEnd = 0;

	HANDLE					m_hThread = 0;
	CRITICAL_SECTION		m_CS;


public:
	static	CAStar*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CComponent*		Clone(void* pArg) override;
	virtual void			Free() override;
};


#endif // !__ENGINE_ASTAR_H__