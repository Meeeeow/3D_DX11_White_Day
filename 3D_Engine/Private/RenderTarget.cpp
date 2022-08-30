#include "..\Public\RenderTarget.h"

CRenderTarget::CRenderTarget(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: m_pGraphicDevice(pGraphicDevice), m_pContextDevice(pContextDevice)
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);
}

HRESULT CRenderTarget::NativeConsturct(_uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, _float4 vClearColor)
{
	D3D11_TEXTURE2D_DESC		tTextureDesc;
	ZeroMemory(&tTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	tTextureDesc.Width = iWidth;
	tTextureDesc.Height = iHeight;
	tTextureDesc.MipLevels = 1;
	tTextureDesc.ArraySize = 1;
	tTextureDesc.Format = ePixelFormat;

	tTextureDesc.SampleDesc.Quality = 0;
	tTextureDesc.SampleDesc.Count = 1;

	tTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	tTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tTextureDesc.CPUAccessFlags = 0;
	tTextureDesc.MiscFlags = 0;

	if (FAILED(m_pGraphicDevice->CreateTexture2D(&tTextureDesc, nullptr, &m_pTexture2D)))
		FAILMSG("RenderTarget Failed to Create Texture2D - NativeConstruct");

	D3D11_RENDER_TARGET_VIEW_DESC	tRTVDesc;
	ZeroMemory(&tRTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

	tRTVDesc.Format = ePixelFormat;
	tRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	tRTVDesc.Texture2D.MipSlice = 0;

	if (FAILED(m_pGraphicDevice->CreateRenderTargetView(m_pTexture2D, &tRTVDesc, &m_pRTV)))
		FAILMSG("RenderTarget Failed to Create RTV - NativeConstruct");

	D3D11_SHADER_RESOURCE_VIEW_DESC	tSRVDesc;
	ZeroMemory(&tSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	tSRVDesc.Format = ePixelFormat;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	tSRVDesc.Texture2D.MipLevels = 1;

	if(FAILED(m_pGraphicDevice->CreateShaderResourceView(m_pTexture2D, &tSRVDesc, &m_pSRV)))
		FAILMSG("RenderTarget Failed to Create SRV - NativeConstruct");

	return S_OK;
}

void CRenderTarget::Clear()
{
	m_pContextDevice->ClearRenderTargetView(m_pRTV, (_float*)&m_vClearColor);
}

HRESULT CRenderTarget::Set_Buffer(_float4 vScreenPos)
{
	m_pVIBuffer = CVIBuffer_Viewport::Create(m_pGraphicDevice, m_pContextDevice
		,L"../Bin/ShaderFiles/Shader_Viewport.hlsl", "Technique_Default", vScreenPos);
	if (m_pVIBuffer == nullptr)
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderTarget::Set_Buffer(const _tchar * pShaderFilePath, const char * pTechniqueName, _float4 vScreenPos)
{
	m_pVIBuffer = CVIBuffer_Viewport::Create(m_pGraphicDevice, m_pContextDevice
		, pShaderFilePath, pTechniqueName, vScreenPos);
	if (m_pVIBuffer == nullptr)
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderTarget::Render()
{
	if (FAILED(m_pVIBuffer->Set_Texture("g_texDiffuse", m_pSRV)))
		return E_FAIL;

	m_pVIBuffer->Render(0);

	return S_OK;
}

CRenderTarget * CRenderTarget::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, _uint iRenderIndex ,_float4 vClearColor)
{
	CRenderTarget* pInstance = new CRenderTarget(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConsturct(iWidth, iHeight, ePixelFormat, vClearColor)))
		Safe_Release(pInstance);

	return pInstance;
}

void CRenderTarget::Free()
{
	Safe_Release(m_pVIBuffer);

	Safe_Release(m_pTexture2D);
	Safe_Release(m_pSRV);
	Safe_Release(m_pRTV);

	Safe_Release(m_pContextDevice);
	Safe_Release(m_pGraphicDevice);
}
