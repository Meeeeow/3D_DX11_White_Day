#pragma once
#ifndef __CLIENT_VENT_EVENT_COLLIDER_H__
#define __CLIENT_VENT_EVENT_COLLIDER_H__

#include "GameObject.h"
BEGIN(Client)
class CVentEventCollider final : public CGameObject
{
private:
	CVentEventCollider(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CVentEventCollider() DEFAULT;

public:
	virtual	HRESULT					NativeConstruct();
	virtual _int					Tick(_double dTimeDelta);
	virtual _int					LateTick(_double dTimeDelta);
	virtual HRESULT					Render();

public:
	virtual void					Interaction();
	virtual void					Event() override;
	virtual void					Collect_Event() override;

private:
	CCollider*						m_pCollider = nullptr;
	_bool							m_bDead = false;

public:
	static	CVentEventCollider*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual void					Free() override;

public:
	virtual void					Compute_Distance() override;
	virtual	CGameObject*			Clone(void * pArg) override;

};
END
#endif // !__CLIENT_VENT_EVENT_COLLIDER_H__