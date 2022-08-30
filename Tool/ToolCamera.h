#pragma once

#include "Camera.h"

class CToolCamera final : public CCamera
{
private:
	CToolCamera(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CToolCamera() DEFAULT;

public:
	virtual	HRESULT			NativeConstruct(const DESC& tDesc);
	virtual _int			Tick(_double dTimeDelta) override;
	virtual _int			FalseTick(_double dTimeDelta);
	virtual _int			LateTick(_double dTimeDelta) override;
	
public:
	_bool					Get_Action() { return m_bAction; }
	void					Action() { m_bAction = !m_bAction; }

public:
	_float					Get_Near() { return m_tDesc.fNear; }
	_float					Get_Far() { return m_tDesc.fFar; }
	

private:
	HRESULT					Move_Free(_double dTimeDelta, CGameInstance* pGameInstance);
	
private:
	_bool					m_bAction = false;

public:
	// CCamera을(를) 통해 상속됨
	static	CToolCamera*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, const DESC& tDesc);
	virtual CGameObject*	Clone(void * pArg) override;
	virtual void			Free() override;
};

