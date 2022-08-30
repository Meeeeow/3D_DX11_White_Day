#include "..\Public\Layer.h"



CLayer::CLayer()
{
}

CGameObject * CLayer::Get_GameObject(_uint iIndex)
{
	if (iIndex >= m_listObjects.size())
		return nullptr;

	auto iter = m_listObjects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return *iter;
}

HRESULT CLayer::NativeConstruct()
{
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (pGameObject == nullptr)
		FAILMSG("Layer Add GameObject - Argument is nullptr");

	m_listObjects.emplace_back(pGameObject);

	return S_OK;
}

_int CLayer::Tick(_double dTimeDelta)
{
	auto& iter = m_listObjects.begin();
	for (; iter != m_listObjects.end(); )
	{
		_int iResult = (*iter)->Tick(dTimeDelta);

		if (iResult < 0)
			NEGAMSG("Layer Tick - Object Error");

		if (iResult == OBJ_DEAD)
		{
			Safe_Release(*iter);
			iter = m_listObjects.erase(iter);
		}
		else if (iResult == OBJ_NOEVENT)
			++iter;

	}
	return OBJ_NOEVENT;
}

_int CLayer::LateTick(_double dTimeDelta)
{
	for (auto& pGameObject : m_listObjects)
	{
		if (pGameObject != nullptr)
		{
			if (pGameObject->LateTick(dTimeDelta) < 0)
				return -1;
		}
	}
	return 0;
}

_int CLayer::Collect_Event()
{
	for (auto& pGameObject : m_listObjects)
	{
		if (pGameObject != nullptr)
			pGameObject->Collect_Event();
	}
	return 0;
}

CLayer * CLayer::Create()
{
	CLayer*	pInstance = new CLayer();
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_listObjects)
		Safe_Release(pGameObject);

	m_listObjects.clear();
}

