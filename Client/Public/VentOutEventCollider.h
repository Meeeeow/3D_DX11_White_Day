#pragma once
#ifndef __CLIENT_VENT_OUT_EVENT_COLLIDER_H__
#define __CLIENT_VENT_OUT_EVENT_COLLIDER_H__

#include "GameObject.h"
BEGIN(Client)
class CVentOutEventCollider final : public CGameObject
{
private:
	CVentOutEventCollider(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CVentOutEventCollider() DEFAULT;

public:
	virtual	HRESULT					NativeConstruct();
	virtual _int					Tick(_double dTimeDelta);
	virtual _int					LateTick(_double dTimeDelta);
	virtual HRESULT					Render();

public:
	virtual void					Interaction();
	virtual void					Event() override;
	virtual void					Collect_Event() override;

	void							Set_Dead() { m_bDead = true; }

private:
	CCollider*						m_pCollider = nullptr;
	_bool							m_bAction = false;
	_bool							m_bDead = false;
	_double							m_dTimeAcc = 0.0;
	_bool							m_bReverse = false;

public:
	static	CVentOutEventCollider*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual void					Free() override;

public:
	virtual void					Compute_Distance() override;
	virtual	CGameObject*			Clone(void * pArg) override;
};
END
#endif // !__CLIENT_VENT_OUT_EVENT_COLLIDER_H__