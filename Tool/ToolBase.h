#pragma once

#include "GameObject.h"

class CToolBase abstract : public CGameObject
{
protected:
	CToolBase(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CToolBase() DEFAULT;

public:
	virtual	HRESULT		NativeConstruct();
	virtual _int		Tick(_double dTimeDelta);
	virtual _int		LateTick(_double dTimeDelta);
	virtual HRESULT		Render();

public:
	virtual HRESULT		Set_ConstantTable();

protected:
	void				Update_Info();

public:
	void				Update_Load();

public:
	OBJ_INFO			Get_ObjectInfo() { return m_tObjInfo; }
	_fvector			Get_Position() { return m_pTransform->Get_State(CTransform::STATE_POSITION); }
	_fvector			Get_Scale() { return m_pTransform->Get_Scale(); }
	_float				Get_Scale(CTransform::STATE eState) { return m_pTransform->Get_Scale(eState); }
	_float				Get_CullRadius() { return m_tObjInfo.fCullRadius; }
	virtual _fvector	Get_OnePoint();

public:
	void				Set_ColliderType(OBJ_INFO::COLLIDERTYPE eType);
	void				Set_Collider(OBJ_INFO::COLLIDERTYPE eType,_float3 vCenter, _float3 vExt = _float3(0.f, 0.f, 0.f), _float4 vRot = _float4(0.f, 0.f, 0.f, 1.f), _float fRadius = 0.f);
	void				Set_ObjectInfo(const OBJ_INFO& tInfo) { m_tObjInfo = tInfo; }
	void				Set_Position(_fvector vPosition) { 
		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition); 
		Update_Info();	}
	void				Set_Scale(_fvector vScale) { 
		m_pTransform->Set_Scale(vScale);
		Update_Info();	}
	void				Set_Rotation(_float fRadian, _fvector vAxis) {
		m_pTransform->Rotation_Radian(fRadian, vAxis);
	}
	void				Set_ObjectName(CString ObjectName);
	void				Set_CullRadius(_float fRadius) { m_tObjInfo.fCullRadius = fRadius; }

	virtual	void		Picking();
protected:
	CTransform*			m_pTransform = nullptr;
	CRenderer*			m_pRenderer = nullptr;
	CCollider*			m_pCollider = nullptr;
	OBJ_INFO			m_tObjInfo;
	_bool				m_bRender = false;
public:
	virtual void		Free() override;
	// CGameObject을(를) 통해 상속됨
	virtual void Compute_Distance() override;

	// 사용 안함
	virtual CGameObject * Clone(void * pArg) override;
};

