#pragma once
#ifndef __CLIENT_LOGO_SCREEN_ROTATION_H__
#define __CLIENT_LOGO_SCREEN_ROTATION_H__

#include "RectXYObject.h"

BEGIN(Client)
class CLogoScreenRotation final : public CRectXYObject
{
public:
	typedef struct tagScreenDesc {
		const wchar_t*		szTextureName;
	}DESC;

private:
	CLogoScreenRotation(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CLogoScreenRotation(const CLogoScreenRotation& rhs);
	virtual ~CLogoScreenRotation() DEFAULT;

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
	virtual HRESULT				Set_Components();

public:
	static CLogoScreenRotation* Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual	void				Free() override;

};
END
#endif // !__CLIENT_LOGO_SCREEN_ROTATION_H__