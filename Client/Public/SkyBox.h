#pragma once
#ifndef __CLIENT_SKY_BOX_H__
#define __CLIENT_SKY_BOX_H__
 

#include "GameObject.h"
BEGIN(Client)
class CSkyBox final : public CGameObject
{
public:
	CSkyBox(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct() override;
	virtual HRESULT				Clone_Construct(void* pArg) override;
	virtual _int				Tick(_double dTimeDelta);
	virtual _int				LateTick(_double dTimeDelta);
	virtual HRESULT				Render() override;


private:
	CTexture*					m_pTexture = nullptr;
	CRenderer*					m_pRenderer = nullptr;
	CTransform*					m_pTransform = nullptr;
	CVIBufferCube*				m_pVIBuffer = nullptr;

public:
	static	CSkyBox*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;


	virtual void				Compute_Distance() override;

};
END
#endif // !__CLIENT_SKY_BOX_H__