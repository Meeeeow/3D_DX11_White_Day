#pragma once
#ifndef __ENGINE_TIMER_MANAGER_H__
#define __ENGINE_TIMER_MANAGER_H__

#include "Timer.h"

BEGIN(Engine)
class CTimerManager final : public CBase
{
	DECLARE_SINGLETON(CTimerManager)
private:
	CTimerManager();
	virtual ~CTimerManager() DEFAULT;

public:
	HRESULT			Add_Timer(const _tchar* pTimerTag);
	_double			Compute_Timer(const _tchar* pTimerTag);

private:
	typedef			unordered_map<const _tchar*, CTimer*>	TIMERS;
	TIMERS			m_mapTimers;

private:
	CTimer*			Find_Timer(const _tchar* pTimerTag);

public:
	virtual void	Free() override;
};

END
#endif // !__ENGINE_TIMER_MANAGER_H__
