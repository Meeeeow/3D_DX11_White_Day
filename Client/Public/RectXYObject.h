#pragma once
#ifndef __CLIENT_RECT_XY_OBJECT_H__
#define __CLIENT_RECT_XY_OBJECT_H__

#include "Client_Defines.h"
#include "GameObject.h"
BEGIN(Client)
class CRectXYObject abstract : public CGameObject
{
protected:
	CRectXYObject(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CRectXYObject(const CRectXYObject& rhs);
	virtual ~CRectXYObject() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct() override;
	virtual HRESULT				Clone_Construct(void* pArg) override;
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

protected:
	CVIBuffer_RectXY*			m_pVIBuffer = nullptr;
	CTexture*					m_pTexture = nullptr;
	CRenderer*					m_pRenderer = nullptr;
	CTransform*					m_pTransform = nullptr;
	
protected:
	virtual	HRESULT				Set_Components();

public:
	virtual	CGameObject*		Clone(void* pArg) override;
	virtual	void				Free() override;
	virtual void				Compute_Distance() override;

};
END
#endif // !__CLIENT_RECT_XY_OBJECT_H__