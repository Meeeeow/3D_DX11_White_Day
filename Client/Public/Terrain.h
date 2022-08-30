#pragma once
#ifndef __CLIENT_TERRAIN_H__
#define __CLIENT_TERRAIN_H__


#include "Client_Defines.h"
#include "GameObject.h"

class CTerrain final : public CGameObject
{
public:

private:
	CTerrain(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct();
	virtual HRESULT				Clone_Construct(void* pArg) override;
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;
	virtual void				Compute_Distance() override;

public:
	HRESULT						Set_Components(OBJ_INFO tObjInfo, CGameInstance* pGameInstance);
	virtual void				Set_InitialPosition(OBJ_INFO tObjInfo);

private:
	CTransform*					m_pTransform = nullptr;
	CRenderer*					m_pRenderer = nullptr;
	CTexture*					m_pTexture = nullptr;
	CVIBuffer_Terrain*			m_pVIBuffer = nullptr;

public:
	static	CTerrain*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CGameObject*		Clone(void * pArg) override;
	virtual void				Free() override;
};


#endif // !__CLIENT_TERRAIN_H__