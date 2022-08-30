#include "..\Public\GameObject.h"
#include "GameInstance.h"


CGameObject::CGameObject(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: m_pGraphicDevice(pGraphicDevice), m_pContextDevice(pContextDevice)
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphicDevice(rhs.m_pGraphicDevice), m_pContextDevice(rhs.m_pContextDevice)
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);
}

HRESULT CGameObject::NativeConstruct()
{
	return S_OK;
}

HRESULT CGameObject::Clone_Construct(void * pArg)
{
	return S_OK;
}

_int CGameObject::Tick(_double dTimeDelta)
{
	return OBJ_NOEVENT;
}

_int CGameObject::LateTick(_double dTimeDelta)
{
	return OBJ_NOEVENT;
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

void CGameObject::Picking()
{

}

_bool CGameObject::OnMouse()
{
	return _bool();
}

void CGameObject::Event()
{
}

void CGameObject::Collect_Event()
{
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag)
{
	auto iter = find_if(m_mapComponents.begin(), m_mapComponents.end(), CTagFinder(pComponentTag));

	if (iter == m_mapComponents.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pComponentTag, CComponent ** ppOut, void * pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CComponent* pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (pComponent == nullptr)
		return E_FAIL;

	auto&	iter = find_if(m_mapComponents.begin(), m_mapComponents.end(), CTagFinder(pComponentTag));
	if (iter != m_mapComponents.end())
		FAILMSG("Component With the same name already exists - GameObject Add Component");

	m_mapComponents.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;
	Safe_AddRef(pComponent);


	return S_OK;
}


void CGameObject::Free()
{
	for (auto& Pair : m_mapComponents)
		Safe_Release(Pair.second);
	m_mapComponents.clear();

	Safe_Release(m_pContextDevice);
	Safe_Release(m_pGraphicDevice);
}


