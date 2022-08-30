#include "..\Public\AStar.h"


CAStar::CAStar(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CComponent(pGraphicDevice, pContextDevice)
{

}

CAStar::CAStar(const CAStar & rhs)
	: CComponent(rhs)
{

}

_uint APIENTRY MakingRouteFunc(LPVOID pArg)
{
	CAStar* pAstar = static_cast<CAStar*>(pArg);

	EnterCriticalSection(&pAstar->Get_CS());

	if (pAstar->PathFinder(pAstar->Get_End(), pAstar->Get_Start()))
		pAstar->MakeRoute(pAstar->Get_End());

	LeaveCriticalSection(&pAstar->Get_CS());

	return 0;
}

HRESULT CAStar::NativeConstruct()
{
	return S_OK;
}

HRESULT CAStar::Clone_Construct(void * pArg)
{
	m_pNavigation = (CNavigationMesh*)pArg;

	return S_OK;
}

void CAStar::Start_AStar(_fvector vStartPos, _fvector vGoalPos)
{
	m_listOpen.clear(); m_listClose.clear(); m_listBest.clear();

	m_iStart = Get_CellIndex(vStartPos);
	m_iEnd = Get_CellIndex(vGoalPos);

	vector<CCell*> vecCells = m_pNavigation->Get_Cells();

	if (m_iStart == -1 || m_iEnd == -1 || m_iStart == m_iEnd || m_iStart >= vecCells.size() || m_iEnd >= vecCells.size())
		return;

	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, MakingRouteFunc, this, 0, nullptr);

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CS);
}

void CAStar::Start_Random(_fvector vStartPos)
{
	m_listOpen.clear(); m_listClose.clear(); m_listBest.clear();

	m_iStart = Get_CellIndex(vStartPos);
	m_iEnd = Get_RandomIndex();

	vector<CCell*> vecCells = m_pNavigation->Get_Cells();

	if (m_iStart == -1 || m_iEnd == -1 || m_iStart == m_iEnd || m_iStart >= vecCells.size() || m_iEnd >= vecCells.size())
		return;

	InitializeCriticalSection(&m_CS);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, MakingRouteFunc, this, 0, nullptr);

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CS);
}

_int CAStar::Get_CellIndex(_fvector vPosition)
{
	vector<CCell*> vecCells = m_pNavigation->Get_Cells();
	size_t iSize = vecCells.size();
	for (size_t i = 0; i < iSize; ++i)
	{
		if (vecCells[i]->Find_CurrentIndexCell(vPosition))
			return (_int)i;
	}

	return -1;
}

_int CAStar::Get_RandomIndex()
{
	vector<CCell*> vecCells = m_pNavigation->Get_Cells();
	size_t iSize = vecCells.size();
	size_t t = rand() % iSize;

	return (_int)t;
}

HRESULT CAStar::MakeRoute(_uint iGoalIndex)
{
	vector<CCell*> vecCells = m_pNavigation->Get_Cells();
	m_listBest.emplace_back(vecCells[iGoalIndex]);
	_uint iParentIndex = vecCells[iGoalIndex]->Get_ParentIndex();
	while (true)
	{
		if (iParentIndex == m_iStart)
			break;
		m_listBest.emplace_front(vecCells[iParentIndex]);
		iParentIndex = vecCells[iParentIndex]->Get_ParentIndex();
	}


	return S_OK;
}

_bool CAStar::PathFinder(_uint iGoalIndex, _uint iStartIndex)
{
	if (!m_listOpen.empty())
		m_listOpen.pop_front();

	m_listClose.emplace_back(iStartIndex);
	vector<CCell*> vecCells = m_pNavigation->Get_Cells();
	for (_uint i = 0; i < 3; ++i)
	{
		CCell* pCell = vecCells[iStartIndex]->Get_NeighborCell(i);

		if (pCell == nullptr)
			continue;

		if (pCell->Get_Index() == iGoalIndex)
		{
			pCell->Set_ParentIndex(iStartIndex);
			return true;
		}

		if (Is_OpenList(pCell->Get_Index()) || Is_CloseList(pCell->Get_Index()))
			continue;

		pCell->Set_ParentIndex(iStartIndex);
		m_listOpen.emplace_back(pCell->Get_Index());
	}

	m_listOpen.sort([&](_uint iFirstIndex, _uint iSecondIndex)
	{
		_vector		vFirstStart = vecCells[iFirstIndex]->Get_CenterPos() - vecCells[m_iStart]->Get_CenterPos();
		_vector		vFirstEnd = vecCells[iFirstIndex]->Get_CenterPos() - vecCells[m_iEnd]->Get_CenterPos();
		_float		fFirstCost = XMVectorGetX(XMVector3Length(vFirstStart) + XMVector3Length(vFirstEnd));

		_vector		vSecondStart = vecCells[iSecondIndex]->Get_CenterPos() - vecCells[m_iStart]->Get_CenterPos();
		_vector		vSecondEnd = vecCells[iSecondIndex]->Get_CenterPos() - vecCells[m_iEnd]->Get_CenterPos();
		_float		fSecondCost = XMVectorGetX(XMVector3Length(vSecondStart) + XMVector3Length(vSecondEnd));

		return fFirstCost < fSecondCost;
	});

	if (m_listOpen.empty())
		return false;

	return PathFinder(iGoalIndex, m_listOpen.front());
}

_bool CAStar::Is_OpenList(_uint iIndex)
{
	auto& iter = find(m_listOpen.begin(), m_listOpen.end(), iIndex);
	if (iter != m_listOpen.end())
		return true;

	return false;
}

_bool CAStar::Is_CloseList(_uint iIndex)
{
	auto& iter = find(m_listClose.begin(), m_listClose.end(), iIndex);
	if (iter != m_listClose.end())
		return true;

	return false;
}

CAStar * CAStar::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CAStar* pInstance = new CAStar(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);
	return pInstance;
}

CComponent * CAStar::Clone(void * pArg)
{
	CAStar* pInstance = new CAStar(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);
	return pInstance;
}

void CAStar::Free()
{
	__super::Free();
	m_listBest.clear();
	Safe_Release(m_pNavigation);
}
