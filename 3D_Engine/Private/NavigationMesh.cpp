#include "..\Public\NavigationMesh.h"

CNavigationMesh::CNavigationMesh(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CComponent(pGraphicDevice, pContextDevice)
{

}

CNavigationMesh::CNavigationMesh(const CNavigationMesh & rhs)
	: CComponent(rhs), m_vecCells(rhs.m_vecCells)
{
	for (auto& pCell : m_vecCells)
		Safe_AddRef(pCell);
}

HRESULT CNavigationMesh::NativeConstruct(const _tchar * pNavDataFilePath)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pNavDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == 0)
		return E_FAIL;

	while (true)
	{
		NAV_INFO tInfo;

		ReadFile(hFile, &tInfo, sizeof(NAV_INFO), &dwByte, nullptr);
		if (dwByte == 0)
			break;
		_float3 vPoints[3];

		vPoints[0].x = floor(tInfo.vPoints[0].x * 100) / 100; vPoints[0].y = floor(tInfo.vPoints[0].y * 100) / 100; vPoints[0].z = floor(tInfo.vPoints[0].z * 100) / 100;
		vPoints[1].x = floor(tInfo.vPoints[1].x * 100) / 100; vPoints[1].y = floor(tInfo.vPoints[1].y * 100) / 100; vPoints[1].z = floor(tInfo.vPoints[1].z * 100) / 100;
		vPoints[2].x = floor(tInfo.vPoints[2].x * 100) / 100; vPoints[2].y = floor(tInfo.vPoints[2].y * 100) / 100; vPoints[2].z = floor(tInfo.vPoints[2].z * 100) / 100;

		CCell* pCell = CCell::Create(m_pGraphicDevice, m_pContextDevice, vPoints, (_uint)m_vecCells.size());
		if (pCell == nullptr)
			return E_FAIL;

		m_vecCells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(Set_Neighbor()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigationMesh::Clone_Construct(void * pArg)
{
	if (pArg != nullptr)
		m_iCurrentIndex = *(_uint*)pArg;

	return S_OK;
}

HRESULT CNavigationMesh::Update(_fmatrix matWorld)
{
	XMStoreFloat4x4(&m_matWorld, matWorld);

	for (auto& pCell : m_vecCells)
		pCell->Transform(matWorld);

	return S_OK;
}

HRESULT CNavigationMesh::Set_Neighbor()
{
	for (auto& pSourCell : m_vecCells)
	{
		for (auto& pDestCell : m_vecCells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (pDestCell->Compare(pSourCell->Get_PointInLocal(CCell::POINT_A), pSourCell->Get_PointInLocal(CCell::POINT_B)))
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);

			if (pDestCell->Compare(pSourCell->Get_PointInLocal(CCell::POINT_B), pSourCell->Get_PointInLocal(CCell::POINT_C)))
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);

			if (pDestCell->Compare(pSourCell->Get_PointInLocal(CCell::POINT_C), pSourCell->Get_PointInLocal(CCell::POINT_A)))
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
		}
	}
	return S_OK;
}

_bool CNavigationMesh::IsOn(_fvector& vMovePos, CCell** pCell, _uint& iCurrentIndex)
{
	CCell*		pNeighbor = nullptr;
	if (m_vecCells[m_iCurrentIndex]->IsIn(vMovePos, &pNeighbor, pCell) == false)
	{
		if (pNeighbor != nullptr)
		{
			while (true)
			{
				if (pNeighbor == nullptr)
					return false;
				if (pNeighbor->IsIn(vMovePos, &pNeighbor, pCell))
					break;
			}
			m_iCurrentIndex = pNeighbor->Get_Index();
			iCurrentIndex = pNeighbor->Get_Index();
			return true;
		}
		else
			return false;
	}
	iCurrentIndex = m_iCurrentIndex;
	return true;
}

_bool CNavigationMesh::IsOn_Vent(_fvector & vMovePos, CCell ** pCell, _uint & iCurrentIndex)
{
	CCell*		pNeighbor = nullptr;
	if (m_vecCells[m_iCurrentIndex]->IsIn_Vent(vMovePos, &pNeighbor, pCell) == false)
	{
		if (pNeighbor != nullptr)
		{
			while (true)
			{
				if (pNeighbor == nullptr)
					return false;
				if (pNeighbor->IsIn_Vent(vMovePos, &pNeighbor, pCell))
					break;
			}
			m_iCurrentIndex = pNeighbor->Get_Index();
			iCurrentIndex = pNeighbor->Get_Index();
			return true;
		}
		else
			return false;
	}
	iCurrentIndex = m_iCurrentIndex;
	return true;
}

void CNavigationMesh::In_Vent(_fvector vPosition)
{
	for (_uint i = 0; i < m_vecCells.size(); ++i)
	{
		if (m_vecCells[i]->Find_VentIndexCell(vPosition))
		{
			Set_CurrentIndex(i);
			break;
		}
	}
}

void CNavigationMesh::In_Cell(_fvector vPosition)
{
	for (_uint i = 0; i < m_vecCells.size(); ++i)
	{
		if (m_vecCells[i]->Find_CurrentIndexCell(vPosition))
		{
			Set_CurrentIndex(i);
			break;
		}
	}
}


#ifdef _DEBUG
HRESULT CNavigationMesh::Render()
{
	for (auto& pCell : m_vecCells)
		pCell->Render(XMLoadFloat4x4(&m_matWorld));

	return S_OK;
}
#endif // _DEBUG

CNavigationMesh * CNavigationMesh::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const _tchar* pNavDataFilePath)
{
	CNavigationMesh* pInstance = new CNavigationMesh(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(pNavDataFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CNavigationMesh::Clone(void * pArg)
{
	CNavigationMesh* pInstance = new CNavigationMesh(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CNavigationMesh::Free()
{
	__super::Free();
	for (auto& pCell : m_vecCells)
		Safe_Release(pCell);
	m_vecCells.clear();
}
