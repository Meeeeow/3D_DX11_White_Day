#pragma once
#ifndef __ENGINE_TRANSFORM_H__
#define __ENGINE_TRANSFORM_H__

#include "Component.h"
#include "NavigationMesh.h"

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

	typedef struct tagTransformDesc
	{
		tagTransformDesc() {
			dSpeedPerSec = 0.0;
			dRotationPerSec = 0.0;
		};
		_double		dSpeedPerSec;
		_double		dRotationPerSec;
	}DESC;
private:
	CTransform(_pGraphicDevice* pGraphicDeivce, _pContextDevice* pContextDevice);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() DEFAULT;

public:
	virtual	HRESULT		NativeConstruct() override;
	virtual HRESULT		Clone_Construct(void* pArg) override;

public:
	_fvector			Get_State(STATE eState) {
		return XMLoadFloat4((_float4*)&m_matWorld.m[eState][0]); }
	_fvector			Get_Scale() {
		return XMVectorSet(
			XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
			XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))),
			0.f); }
	_float				Get_Scale(STATE eState) {
		return XMVectorGetX(XMVector3Length(Get_State(eState)));
	}
	_fmatrix			Get_WorldXM() const {
		return XMLoadFloat4x4(&m_matWorld);	}
	_float4x4			Get_World() const {
		return m_matWorld; }
	_float4x4			Get_WorldTranspose() const {
		_float4x4		fMatrix;	XMStoreFloat4x4(&fMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_matWorld)));
		return fMatrix;
	}
	_uint				Get_CurrentIndex() { return m_iCurrentIndex; }
	_double				Get_CurrentSpeed() { return m_tDesc.dSpeedPerSec; }

	void				Set_WorldMatrix(_matrix matWorld);

public:
	void				Move_Front(_double dTimeDelta, CNavigationMesh* pNavigation = nullptr);
	void				Move_Back(_double dTimeDelta, CNavigationMesh* pNavigation = nullptr);
	void				Move_Left(_double dTimeDelta, CNavigationMesh* pNavigation = nullptr);
	void				Move_Right(_double dTimeDelta, CNavigationMesh* pNavigation = nullptr);

	void				Move_VentFront(_double dTimeDelta, CNavigationMesh* pNavigation = nullptr);
	void				Move_VentBack(_double dTimeDelta, CNavigationMesh* pNavigation = nullptr);
	void				Move_VentLeft(_double dTimeDelta, CNavigationMesh* pNavigation = nullptr);
	void				Move_VentRight(_double dTimeDelta, CNavigationMesh* pNavigation = nullptr);

	void				Move_Trace(_double dTimeDelta, _fvector vGoalPos,CNavigationMesh* pNavigation = nullptr);
	void				LookAt(_double dTimeDelta, _fvector vTargetPos);

	void				Move_MouseX(_double dPower);
	void				Move_MouseY(_double dPower);
	void				Mouse_Chase(_fvector vGoalPos, _double dTimeDelta);

	void				Rotation_Axis(_double dTimeDelta, _fvector vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f));
	void				Rotation_AxisRadian(_float fRadian, _fvector vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f));
	void				Rotation_Radian(_float fRadian, _fvector vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f));
	void				Rotation_Radian_Acc(_float fRadian, _fvector vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f));
	void				Scaling(_fvector vScale);

	void				Rotation_AxisLerp(_double dTimeDelta, _fvector vAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f));

public:
	void				Add_Position(_fvector vState, CNavigationMesh* pNavigation = nullptr);
	void				Set_State(STATE eState, _fvector vState);
	void				Set_Scale(_fvector vScale);
	void				Set_TransformDesc(const DESC& tDesc) {
		m_tDesc = tDesc; }
	void				Set_MoveSpeed(_double dMoveSpeedPerSec);
	void				Set_RotateSpeed(_double dRotationSpeedPerSec);

private:
	_float4x4			m_matWorld;
	DESC				m_tDesc;
	_uint				m_iCurrentIndex = 0;

public:
	static CTransform*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CComponent*	Clone(void* pArg) override;
	virtual void		Free() override;
};


#endif // !__ENGINE_TRANSFORM_H__
