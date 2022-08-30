#pragma once
#ifndef __ENGINE_RENDER_TARGET_MANAGER_H__
#define __ENGINE_RENDER_TARGET_MANAGER_H__

#include "RenderTarget.h"
BEGIN(Engine)
class CRenderTargetManager final : public CBase
{
	DECLARE_SINGLETON(CRenderTargetManager)

public:
	typedef	unordered_map<const _tchar*, CRenderTarget*>		RENDERTARGETS;
	typedef unordered_map<const _tchar*, list<CRenderTarget*>>	MULTIRENDERTARGETS;

private:
	explicit CRenderTargetManager();
	virtual ~CRenderTargetManager() DEFAULT;

public:
	HRESULT						NativeConstruct(_pContextDevice* pContextDevice);
	HRESULT						Add_RenderTarget(_pGraphicDevice* pGrpahicDevice, _pContextDevice* pContextDevice
		, const _tchar* pRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat,_uint iRenderIndex, _float4 vClearColor = _float4(0.f, 0.15f, 0.15f, 1.f));
	HRESULT						Add_MRT(const _tchar* pMRTTag, const _tchar* pRenderTargetTag);

public:
	CRenderTarget*				Find_RenderTarget(const _tchar* pRenderTargetTag);
	list<CRenderTarget*>*		Find_MRT(const _tchar* pMRTTag);
	ID3D11ShaderResourceView*	Get_SRV(const _tchar* pRenderTargetTag);

public:
	HRESULT						Set_Buffer(const _tchar* pRenderTargetTag, _float4 vScreenPos);
	HRESULT						Set_Buffer(const _tchar* pRenderTargetTag, const _tchar* pShaderFilePath, const char* pTechniqueName, _float4 vScreenPos);
	void						IsRender() { m_bRender = !m_bRender; }

public:
	HRESULT						Begin_MRT(const _tchar* pMRTTag);
	HRESULT						End_MRT();

public:
	HRESULT						Render();

private:
	RENDERTARGETS				m_mapRenderTargets;
	MULTIRENDERTARGETS			m_mapMRTs;

private: /* Default Device */
	_pContextDevice*			m_pContextDevice = nullptr;
	ID3D11RenderTargetView*		m_pDefaultRTV = nullptr;
	ID3D11DepthStencilView*		m_pDefaultDSV = nullptr;
	
private:
	_bool						m_bRender = true;

public:
	virtual	void				Free() override;

};
END
#endif // !__ENGINE_RENDER_TARGET_MANAGER_H__