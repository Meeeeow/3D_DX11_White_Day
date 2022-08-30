#pragma once
#ifndef __CLIENT_MOUSE_POINTER_H__
#define __CLIENT_MOUSE_POINTER_H__

#include "GameObject.h"
BEGIN(Client)
class CMousePointer final : public CGameObject
{
public:
	CMousePointer(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CMousePointer(const CMousePointer& rhs);
	virtual ~CMousePointer() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct() override;
	virtual HRESULT				Clone_Construct(void* pArg) override;
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render();

private:
	CRenderer*					m_pRenderer = nullptr;
	CTexture*					m_pTexture = nullptr;
	CVIBuffer_RectXY*			m_pVIBuffer = nullptr;
	CTransform*					m_pTransform = nullptr;
	CPickingEventManager::FSM	m_eState = CPickingEventManager::FSM_NONE;
	_float4x4					m_matProj;

public:
	static	CMousePointer*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;


	// CGameObject을(를) 통해 상속됨
	virtual void				Compute_Distance() override;

};
END
#endif // !__CLIENT_MOUSE_POINTER_H__