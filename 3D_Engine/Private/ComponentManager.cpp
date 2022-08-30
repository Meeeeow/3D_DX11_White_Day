#include "..\Public\ComponentManager.h"

IMPLEMENT_SINGLETON(CComponentManager)

CComponentManager::CComponentManager()
{

}

HRESULT CComponentManager::Reserve_Container(_uint iNumLevels)
{
	if (m_pPrototypes != nullptr)
		FAILMSG("Already reserved PROTOTYPES");

	m_pPrototypes = new PROTOTYPES[iNumLevels];
	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CComponentManager::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pInstance)
{
	if (m_iNumLevels <= iLevelIndex)
		FAILMSG("Level Index is too big - ComponentManager's Add Prototype");

	if (pInstance == nullptr)
		FAILMSG("Prototype is nullptr - ComponentManager's Add Prototype");

	if (m_pPrototypes[iLevelIndex].end() != find_if(m_pPrototypes[iLevelIndex].begin(), m_pPrototypes[iLevelIndex].end(), CTagFinder(pPrototypeTag)))
		return S_OK;

	m_pPrototypes[iLevelIndex].emplace(pPrototypeTag, pInstance);

	return S_OK;
}

CComponent * CComponentManager::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (m_iNumLevels <= iLevelIndex)
		NULLMSG("Level Index is too big - ComponentManager's Clone Component");

	auto iter = find_if(m_pPrototypes[iLevelIndex].begin(), m_pPrototypes[iLevelIndex].end(), CTagFinder(pPrototypeTag));
	if (iter == m_pPrototypes[iLevelIndex].end())
		NULLMSG("No Prototype with the same name - ComponentManager's Clone Component");

	CComponent*		pInstance = iter->second->Clone(pArg);

	return pInstance;
}

_bool CComponentManager::Prototype_NameCheck(_uint iLevelIndex, const _tchar * pPrototypeTag)
{
	if (m_iNumLevels <= iLevelIndex)
		return false;

	auto iter = find_if(m_pPrototypes[iLevelIndex].begin(), m_pPrototypes[iLevelIndex].end(), CTagFinder(pPrototypeTag));
	if (iter == m_pPrototypes[iLevelIndex].end())
		return true;

	return false;
}

void CComponentManager::Clear(_uint iLevelIndex)
{
	if (m_iNumLevels <= iLevelIndex)
		VOIDMSG("Level Index is too big - ComponentManager's Clear");

	for (auto& Pair : m_pPrototypes[iLevelIndex])
		Safe_Release(Pair.second);

	m_pPrototypes[iLevelIndex].clear();
	
}

void CComponentManager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pPrototypes[i])
			Safe_Release(Pair.second);

		m_pPrototypes[i].clear();
	}
	Safe_Delete_Arr(m_pPrototypes);
}

