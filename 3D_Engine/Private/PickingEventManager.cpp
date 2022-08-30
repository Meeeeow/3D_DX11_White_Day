#include "..\Public\PickingEventManager.h"
IMPLEMENT_SINGLETON(CPickingEventManager)

CPickingEventManager::CPickingEventManager()
{

}

HRESULT CPickingEventManager::NativeConstruct(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, _uint iWinCX, _uint iWinCY)
{
	m_pGraphicDevice = pGraphicDevice;
	Safe_AddRef(m_pGraphicDevice);
	m_pContextDevice = pContextDevice;
	Safe_AddRef(m_pContextDevice);
	

	return S_OK;
}

CPickingEventManager::FSM CPickingEventManager::Get_FSM()
{
	if (m_vecEvent.size() == 0)
		return FSM_NONE;

	sort(m_vecEvent.begin(), m_vecEvent.end(), [](PE tSour, PE tDest) {
		return tSour.fRayDistance < tDest.fRayDistance;
	});
	if (m_vecEvent.begin()->fRayDistance > 0.5f)
		m_vecEvent.clear();

	if (m_vecEvent.size() == 0)
		return FSM_NONE;

	return m_vecEvent.front().eState;
}

void CPickingEventManager::Add_Event(PE & tEvent)
{
	m_vecEvent.push_back(tEvent);
}

_int CPickingEventManager::Tick(_double dTimeDelta)
{
	if (m_vecEvent.size() != 0)
		m_vecEvent.clear();

	return 0;
}

_int CPickingEventManager::LateTick(_double dTimeDelta)
{
	if (m_vecEvent.size() != 0)
	{
		sort(m_vecEvent.begin(), m_vecEvent.end(), [](PE tSour, PE tDest) {
			return tSour.fRayDistance < tDest.fRayDistance;
		});
		if (m_vecEvent.begin()->fRayDistance > 1.0f)
			m_vecEvent.clear();
	}
	
	return _int();
}


void CPickingEventManager::Action()
{
	if (m_vecEvent.size() != 0)
	{
		m_vecEvent.begin()->pEventObject->Event();
		m_vecEvent.clear();
	}
}

void CPickingEventManager::Free()
{
	m_vecEvent.clear();
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pContextDevice);
}
