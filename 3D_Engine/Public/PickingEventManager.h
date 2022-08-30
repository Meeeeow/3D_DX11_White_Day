#pragma once
#ifndef __ENGINE_PICKING_EVENT_MANAGER_H__
#define __ENGINE_PICKING_EVENT_MANAGER_H__

#include "GameObject.h"
#include "VIBuffer_Viewport.h"
#include "Texture.h"

BEGIN(Engine)
class ENGINE_DLL CPickingEventManager final : public CBase
{
	DECLARE_SINGLETON(CPickingEventManager)

private:
	explicit CPickingEventManager();
	virtual ~CPickingEventManager() DEFAULT;
public:
	enum FSM { FSM_NONE, FSM_ITEM, FSM_DOOR, FSM_LIGHT, FSM_LADDER, FSM_NIPPER, FSM_POISON, FSM_VENT
		, FSM_KEY, FSM_TALISMAN ,FSM_END };

	typedef struct tagPickingEventDesc {
		class CGameObject*		pEventObject = nullptr;
		float					fRayDistance = 0.f;
		FSM						eState = FSM_END;
	}PE;


public:
	HRESULT					NativeConstruct(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, _uint iWinCX, _uint iWinCY);
	void					Add_Event(PE& tEvent);
	_int					Tick(_double dTimeDelta);
	_int					LateTick(_double dTimeDelta);

public:
	CPickingEventManager::FSM	Get_FSM();

public:
	void					Action();

private:
	vector<PE>				m_vecEvent;

	_pGraphicDevice*		m_pGraphicDevice = nullptr;
	_pContextDevice*		m_pContextDevice = nullptr;

public:
	virtual	void			Free() override;

};
END
#endif // !__ENGINE_PICKING_EVENT_MANAGER_H__