#pragma once
#ifndef __ENGINE_GRAPHIC_DEVICE_H__
#define __ENGINE_GRAPHIC_DEVICE_H__

#include "Base.h"

BEGIN(Engine)
class CGraphicDevice final : public CBase
{
public:
	DECLARE_SINGLETON(CGraphicDevice)

public:
	enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

	typedef struct tagGraphicDeviceDesc
	{
		tagGraphicDeviceDesc() {
			hWnd = nullptr;
			iWinCX = 0; iWinCY = 0;
			eWinMode = WINMODE::MODE_END;
		}
		HWND			hWnd;
		unsigned int	iWinCX, iWinCY;
		WINMODE			eWinMode;
	}DESC;

private:
	CGraphicDevice();
	virtual ~CGraphicDevice() DEFAULT;

public:
	HRESULT		NativeConstruct(HWND hWnd, WINMODE eWinMode, _uint iWinCX, _uint iWinCY, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT		Clear_BackBuffer_View(XMFLOAT4 vClearColor);
	HRESULT		Clear_DepthStencil_View();

	HRESULT		Present();

private:
	// 메모리 할당
	ID3D11Device*					m_pGraphicDevice	= nullptr;
	// 기능
	ID3D11DeviceContext*			m_pContextDevice	= nullptr;
	// 스왑체인
	IDXGISwapChain*					m_pSwapChain		= nullptr;
	// 백 버퍼
	ID3D11RenderTargetView*			m_pBackBufferView	= nullptr;
	// 스텐실 깊이 버퍼
	ID3D11DepthStencilView*			m_pDepthStencilView	= nullptr;

private:
	HRESULT		Ready_SwapChain(HWND hWnd, WINMODE eWinMode, _uint iWinCX, _uint iWinCY);
	HRESULT		Ready_BackBufferRenderTargetView();
	HRESULT		Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);
	
public:
	virtual void Free() override;
};

END
#endif // !__ENGINE_GRAPHIC_DEVICE_H__
