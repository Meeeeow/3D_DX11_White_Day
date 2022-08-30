#pragma once
#ifndef __ENGINE_LIGHT_H__
#define __ENGINE_LIGHT_H__

#include "VIBuffer_Viewport.h"

BEGIN(Engine)
class CLight final : public CBase
{
private:
	CLight(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CLight() DEFAULT;

public:
	HRESULT				NativeConstruct(const LIGHTDESC* tLightDesc);
	HRESULT				NativeConstruct(const _tchar* pShaderFilePath, const char* pTechniqueName, const LIGHTDESC& tLightDesc);
	HRESULT				Render();

public:
	LIGHTDESC&			Get_Desc() { return m_tDesc; }

public:
	void				Action() { m_bLightOn = true; }
	void				DeActive() { m_bLightOn = false; }
	_bool				Is_Active() { return m_bLightOn; }

private:
	_pGraphicDevice*	m_pGraphicDevice = nullptr;
	_pContextDevice*	m_pContextDevice = nullptr;
	LIGHTDESC			m_tDesc;
	CVIBuffer_Viewport*	m_pVIBuffer = nullptr;

	_uint				m_iPassIndex = 0;
	_bool				m_bLightOn = false;

public:
	static	CLight*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, const LIGHTDESC* tLightDesc);
	static	CLight*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, const _tchar* pShaderFilePath, const char* pTechniqueName , const LIGHTDESC& tLightDesc);
	virtual	void		Free() override;
};
END
#endif // !__ENGINE_LIGHT_H__