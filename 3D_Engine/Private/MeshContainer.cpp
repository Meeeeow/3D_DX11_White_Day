#include "..\Public\MeshContainer.h"
#include "HierarchyNode.h"


CMeshContainer::CMeshContainer(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: m_pGraphicDevice(pGraphicDevice), m_pContextDevice(pContextDevice)
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);
}

CMeshContainer::CMeshContainer(const CMeshContainer & rhs)
	: m_pGraphicDevice(rhs.m_pGraphicDevice), m_pContextDevice(rhs.m_pContextDevice)
	, m_iNumVertices(rhs.m_iNumVertices), m_iNumFaces(rhs.m_iNumFaces), m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_iStartVertexIndex(rhs.m_iStartVertexIndex), m_iStartFaceIndex(rhs.m_iStartFaceIndex), m_iNumBones(rhs.m_iNumBones)
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);
}

HRESULT CMeshContainer::NativeConstruct(_uint iStartVertexIndex, _uint iNumVertices, _uint iStartFaceIndex, _uint iNumFaces, _uint iMaterialIndex)
{
	m_iStartVertexIndex = iStartVertexIndex;
	m_iStartFaceIndex = iStartFaceIndex;

	m_iNumVertices = iNumVertices;
	m_iNumFaces = iNumFaces;

	m_iMaterialIndex = iMaterialIndex;

	return S_OK;
}

HRESULT CMeshContainer::Render()
{
	/* m_iNumFaces * 3 
		: 그려야하는 도형의 개수 (m_iNumFaces) * 도형을 하나 그릴때 필요한 인덱스의 수 (3) */
	/* m_iStartFaceIndex * 3
		: 우리는 현재 Index를 TRI32 등의 구조체로 3칸을 하나씩 묶었다. 그래서 * 3 을 통해 시작 위치의 보정이 필요함 */
	/* m_iStartVertexIndex
		: 시작 정점 위치 */

	m_pContextDevice->DrawIndexed(m_iNumFaces * 3, m_iStartFaceIndex * 3, m_iStartVertexIndex);
	return S_OK;
}

HRESULT CMeshContainer::Set_CombinedMatrices(_matrix * pMatrices, _fmatrix matPivot)
{
	_uint		iIndex = 0;

	for (auto& pHierarchyNode : m_tBones)
	{
		_matrix		matOffset, matCombinedTransformation;
		matOffset = pHierarchyNode->Get_OffsetMatrix();
		matCombinedTransformation = pHierarchyNode->Get_CombinedTransformationMatrix();

		pMatrices[iIndex++] = XMMatrixTranspose(matOffset * matCombinedTransformation * matPivot);
		pMatrices[iIndex];
	}

	return S_OK;
}

CMeshContainer * CMeshContainer::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, _uint iStartVertexIndex, _uint iNumVertices, _uint iStartFaceIndex, _uint iNumFaces, _uint iMaterialIndex)
{
	CMeshContainer* pInstance = new CMeshContainer(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(iStartVertexIndex, iNumVertices, iStartFaceIndex, iNumFaces, iMaterialIndex)))
		Safe_Release(pInstance);

	return pInstance;
}

CMeshContainer * CMeshContainer::Clone()
{
	CMeshContainer* pInstance = new CMeshContainer(*this);

	return pInstance;
}

void CMeshContainer::Free()
{
	for (auto iter : m_tBones)
		Safe_Release(iter);
	m_tBones.clear();

	Safe_Release(m_pContextDevice);
	Safe_Release(m_pGraphicDevice);
}
