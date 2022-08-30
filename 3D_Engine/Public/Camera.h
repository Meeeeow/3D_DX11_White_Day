#pragma once

#ifndef __ENGINE_CAMERA_H__
#define __ENGINE_CAMERA_H__

#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)
class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	enum CUSTOMPOS { SLOT_1, SLOT_2, SLOT_3, SLOT_END };
public:
	typedef struct tagCameraDesc {
		tagCameraDesc() {
			XMStoreFloat3(&vEye, XMVectorZero()); XMStoreFloat3(&vAt, XMVectorZero());
			vAxis = _float3(0.f, 1.f, 0.f); fFOV = XMConvertToRadians(90.f); fAspect = 800.f / 600.f; fNear = 0.1f; fFar = 100.f;
		};
		/* For. Initial Setting */
		_float3				vEye;
		_float3				vAt;
		_float3				vAxis;

		/* For. Changeable Setting */
		_float				fFOV;
		_float				fFar;
		_float				fNear;

		_float				fAspect;
		CTransform::DESC	tTransformDesc;
}DESC;


protected:
	CCamera(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() DEFAULT;


public:
	virtual HRESULT			NativeConstruct() override;
	virtual HRESULT			Clone_Construct(void* pArg) override;
	virtual _int			Tick(_double dTimeDelta) override;
	virtual _int			LateTick(_double dTimeDelta) override;

public:
	HRESULT					Syncronize_Camera();
	virtual void			Compute_Distance() override;

public:
	_fvector				Get_CameraPos() { return m_pTransform->Get_State(CTransform::STATE_POSITION); }
	_float					Get_FOV() { return m_tDesc.fFOV; }
	_float					Get_Far() { return m_tDesc.fFar; }

public:
	void					Change_FOV(_float fFOV) { m_tDesc.fFOV = fFOV; }
	void					Change_FOV_Add(_float fFOV) { m_tDesc.fFOV += fFOV; }
	void					Change_Near(_float fNear) { m_tDesc.fNear = fNear; }
	void					Change_Far(_float fFar) { m_tDesc.fFar = fFar; }
	void					Change_CameraTransformDesc(CTransform::DESC tCameraTransDesc) { m_tDesc.tTransformDesc = tCameraTransDesc; }

public:
	HRESULT					Setting_Target(CGameObject* pTargetObject);
	

protected:
	CTransform*				m_pTransform = nullptr;
	DESC					m_tDesc; /* Camera Init Info */
	
	_float					m_fDistance = 0.f;
	_bool					m_bInitSync = false;

	_double					m_dwMouseY = 0.0;

public:
	virtual CGameObject*	Clone(void* pArg) PURE;
	virtual	void			Free() override;

};
END

#endif // !__ENGINE_CAMERA_H__
