#include "..\Public\TimerManager.h"

IMPLEMENT_SINGLETON(CTimerManager)

CTimerManager::CTimerManager()
{
}

HRESULT CTimerManager::Add_Timer(const _tchar * pTimerTag)
{
	if (Find_Timer(pTimerTag) != nullptr)
		FAILMSG("CTimerManager::Add_Timer - Already have been Tag");

	CTimer* pInstance = CTimer::Create();

	if (pInstance == nullptr)
		FAILMSG("CTimerManager::Add_Timer - Timer Create Failed");

	m_mapTimers.emplace(pTimerTag, pInstance);

	return S_OK;
}

_double CTimerManager::Compute_Timer(const _tchar * pTimerTag)
{
	CTimer* pInstance = Find_Timer(pTimerTag);
	
	if (pInstance == nullptr)
		return 0.0;

	return pInstance->Compute_Timer();
}

CTimer * CTimerManager::Find_Timer(const _tchar * pTimerTag)
{
	auto iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), CTagFinder(pTimerTag));

	if (iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}

void CTimerManager::Free()
{
	for (auto& Pair : m_mapTimers)
		Safe_Release(Pair.second);
	m_mapTimers.clear();
}
