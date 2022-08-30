#pragma once
#ifndef __ENGINE_RENDER_TARGET_H__
#define __ENGINE_RENDER_TARGET_H__

#include "VIBuffer_Viewport.h"

BEGIN(Engine)
class CRenderTarget final : public CBase
{
private:
	explicit CRenderTarget(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CRenderTarget() DEFAULT;
public:
	HRESULT							NativeConsturct(_uint iWidth, _uint iHeight
		, DXGI_FORMAT ePixelFormat, _float4 vClearColor);

public:
	ID3D11RenderTargetView*			Get_RTV() { return m_pRTV; }
	ID3D11ShaderResourceView*		Get_SRV() { return m_pSRV; }

public:
	void							Clear();

public:
	HRESULT							Set_Buffer(_float4 vScreenPos);
	HRESULT							Set_Buffer(const _tchar* pShaderFilePath, const char* pTechniqueName,_float4 vScreenPos);
	HRESULT							Render();

private:
	_pGraphicDevice*				m_pGraphicDevice = nullptr;
	_pContextDevice*				m_pContextDevice = nullptr;

private:
	ID3D11Texture2D*				m_pTexture2D = nullptr;
	ID3D11RenderTargetView*			m_pRTV = nullptr;
	ID3D11ShaderResourceView*		m_pSRV = nullptr;

	CVIBuffer_Viewport*				m_pVIBuffer = nullptr;
	_float4							m_vClearColor;
	_uint							m_iRenderIndex = 0;

public:
	static	CRenderTarget*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, _uint iRenderIndex,_float4 vClearColor = _float4(0.f, 0.f, 0.f, 1.f));
	virtual	void					Free() override;
};
END
#endif // !__ENGINE_RENDER_TARGET_H__
