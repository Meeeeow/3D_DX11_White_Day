#include "..\Public\HierarchyNode.h"



CHierarchyNode::CHierarchyNode()
{
}

HRESULT CHierarchyNode::NativeConstruct(const char * pName, _fmatrix fmatTransformation, CHierarchyNode * pParentNode, _uint iDepth)
{
	m_iDepth = iDepth;
	strcpy_s(m_szName, pName);
	XMStoreFloat4x4(&m_matTransformation, fmatTransformation);
	m_pParentNode = pParentNode;
	Safe_AddRef(m_pParentNode);
	return S_OK;
}

void CHierarchyNode::Update_CombinedTransformMatrix(_uint iCurredAnimIndex)
{
	if (m_vecAnimationChannels.size() <= iCurredAnimIndex)
		return;

	/* 애니메이션이 있다면 */
	if (m_vecAnimationChannels[iCurredAnimIndex] != nullptr)
		XMStoreFloat4x4(&m_matTransformation, m_vecAnimationChannels[iCurredAnimIndex]->Get_TransformationMatrix());
	/* 부모가 있다면 */
	if (m_pParentNode != nullptr)
		XMStoreFloat4x4(&m_matCombinedTransformation
			, XMLoadFloat4x4(&m_matTransformation) * XMLoadFloat4x4(&m_pParentNode->m_matCombinedTransformation));
	else /* 부모가 없다면, 루트 노드 */
		m_matCombinedTransformation = m_matTransformation;


}

void CHierarchyNode::Update_CombinedTransformMatrix()
{
	if (m_pParentNode != nullptr)
	{
		XMStoreFloat4x4(&m_matCombinedTransformation,
			XMLoadFloat4x4(&m_matTransformation) * m_pParentNode->Get_CombinedTransformationMatrix());
	}
	else
	{
		XMStoreFloat4x4(&m_matCombinedTransformation,
			XMLoadFloat4x4(&m_matTransformation));
	}
}

CHierarchyNode * CHierarchyNode::Create(const char * pName, _fmatrix fmatTransformation, CHierarchyNode * pParentNode, _uint iDepth)
{
	CHierarchyNode* pInstance = new CHierarchyNode();
	if (FAILED(pInstance->NativeConstruct(pName, fmatTransformation, pParentNode, iDepth)))
		Safe_Release(pInstance);
	return pInstance;
}

void CHierarchyNode::Free()
{
	Safe_Release(m_pParentNode);
	for (auto& pVector : m_vecAnimationChannels)
		Safe_Release(pVector);
}

