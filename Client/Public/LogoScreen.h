#pragma once
#ifndef __CLIENT_LOGO_SCREEN_H__
#define __CLIENT_LOGO_SCREEN_H__

#include "Client_Defines.h"
#include "RectXYObject.h"

class CLogoScreen final : public CRectXYObject
{
public:
	typedef struct tagScreenDesc {
		const wchar_t*		szTextureName;
	}DESC;

private:
	CLogoScreen(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CLogoScreen(const CLogoScreen& rhs);
	virtual ~CLogoScreen() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct() override;
	virtual HRESULT				Clone_Construct(void* pArg) override;
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	_float4x4					m_matProj;
	_float						m_fX, m_fY, m_fSizeX, m_fSizeY;

private:
	CVIBuffer_RectXY*			m_pTreeVIBuffer = nullptr;
	CTransform*					m_pTreeTransform = nullptr;
	CVIBuffer_RectXY*			m_pSelectBuffer = nullptr;
	CTransform*					m_pSelectTransform = nullptr;


private:
	virtual HRESULT				Set_Components();

public:
	static	CLogoScreen*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual	void				Free() override;



};

#endif // !__CLIENT_LOGO_SCREEN_H__