#include "..\Public\ObjectManager.h"


IMPLEMENT_SINGLETON(CObjectManager)

CObjectManager::CObjectManager()
{
}

CGameObject * CObjectManager::Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex)
{
	if (m_iNumLevels <= iLevelIndex)
		NULLMSG("LevelIndex is too big - ObjectManager Get GameObject");

	auto&	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	if (iter == m_pLayers[iLevelIndex].end())
		NULLMSG("LayerTag is wrong - ObjectManager Get GameObject");

	return iter->second->Get_GameObject(iIndex);
}

HRESULT CObjectManager::Reserve_Container(_uint iNumLevels)
{
	if (m_pLayers != nullptr)
		FAILMSG("Already Reserved - ObjectManager Reserve Container");

	m_pLayers = new LAYERS[iNumLevels];
	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CObjectManager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pGameObject)
{
	if (pGameObject == nullptr)
		FAILMSG("Attempt Insert Nullptr - ObjectManager Add Prototype");

	if (m_mapPrototype.end() != find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CTagFinder(pPrototypeTag)))
		FAILMSG("Prototype With the same name already exists - ObjectManager's Add Prototype");
	
	m_mapPrototype.emplace(pPrototypeTag, pGameObject);

	return S_OK;
}

HRESULT CObjectManager::Insert_ObjectToLayer(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg)
{
	if (m_iNumLevels <= iLevelIndex)
		FAILMSG("LevelIndex is too big - ObjectManager Insert Object to Layer");

	auto&	pIterPrototype = find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CTagFinder(pPrototypeTag));
	if(pIterPrototype == m_mapPrototype.end())
		FAILMSG("No Prototype with the same name - CObjectManager Insert Object to Layer");

	CGameObject*	pGameObject = pIterPrototype->second->Clone(pArg);
	if (pGameObject == nullptr)
		return E_FAIL;

	auto&	pIterLayer = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	if (pIterLayer == m_pLayers[iLevelIndex].end())
	{
		CLayer*	pLayer = CLayer::Create();
		if (pLayer == nullptr)
			return E_FAIL;

		pLayer->Add_GameObject(pGameObject);
		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
		pIterLayer->second->Add_GameObject(pGameObject);

	return S_OK;
}

HRESULT CObjectManager::Insert_ObjectToLayer(_uint iLevelIndex, CGameObject * pGameObject, const _tchar * pLayerTag)
{
	if (m_iNumLevels <= iLevelIndex)
		return E_FAIL;

	auto&	pIterLayer = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	if (pIterLayer == m_pLayers[iLevelIndex].end())
	{
		CLayer*	pLayer = CLayer::Create();
		if (pLayer == nullptr)
			return E_FAIL;

		pLayer->Add_GameObject(pGameObject);
		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
		pIterLayer->second->Add_GameObject(pGameObject);

	return S_OK;
}

_int CObjectManager::Tick(_double dTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (Pair.second->Tick(dTimeDelta) < 0)
				return -1;
		}
	}

	return _int();
}

_int CObjectManager::LateTick(_double dTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			if (Pair.second->LateTick(dTimeDelta) < 0)
				return -1;
		}
	}

	return _int();
}

_int CObjectManager::Collect_Event()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Collect_Event();
	}
	return 0;
}

void CObjectManager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();
}

list<CGameObject*>* CObjectManager::Get_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	auto& iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTagFinder(pLayerTag));
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;


	return iter->second->Get_Objects();
}

void CObjectManager::Free()
{
	for (size_t i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);
		m_pLayers[i].clear();
	}
	for (auto& Pair : m_mapPrototype)
		Safe_Release(Pair.second);
	m_mapPrototype.clear();

	Safe_Delete_Arr(m_pLayers);
}
