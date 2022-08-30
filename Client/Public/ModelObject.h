#pragma once
#ifndef __CLIENT_MODEL_OBJECT_H__
#define __CLIENT_MODEL_OBJECT_H__



#include "GameObject.h"

BEGIN(Client)
class CModelObject abstract : public CGameObject
{
protected:
	CModelObject(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CModelObject(const CModelObject& rhs);
	virtual ~CModelObject() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct() override;
	virtual HRESULT				Clone_Construct(void* pArg) override;
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render();

public:
	virtual void				Interaction();
	virtual void				Set_InitialPosition(OBJ_INFO tObjInfo);
	

protected:
	virtual HRESULT				Set_Components();
	HRESULT						Set_ConstantTable();

protected:
	CModel*						m_pModel = nullptr;
	CRenderer*					m_pRenderer = nullptr;
	CTransform*					m_pTransform = nullptr;
	CCollider*					m_pCollider = nullptr;

	_float						m_fCullingRadius = 1.f;
	_uint						m_iRenderIndex = 0;
	_bool						m_bRender = false;

public:
	virtual	CGameObject*		Clone(void* pArg) override;
	virtual	void				Free() override;
	

	// CGameObject을(를) 통해 상속됨
	virtual void				Compute_Distance() override;

};
END
#endif // !__CLIENT_MODEL_OBJECT_H__