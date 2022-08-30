#include "..\Public\Light.h"
#include "RenderTargetManager.h"
#include "PipeLine.h"

CLight::CLight(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: m_pGraphicDevice(pGraphicDevice), m_pContextDevice(pContextDevice)
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);
}

HRESULT CLight::NativeConstruct(const LIGHTDESC* tLightDesc)
{
	m_tDesc = *tLightDesc;

	if (m_tDesc.eLightType == LIGHTDESC::LIGHT_DIRECTIONAL)
		m_iPassIndex = 1;

	if (m_tDesc.eLightType == LIGHTDESC::LIGHT_POINT)
		m_iPassIndex = 2;

	D3D11_VIEWPORT			tViewportDesc;
	ZeroMemory(&tViewportDesc, sizeof(D3D11_VIEWPORT));

	_uint					iNumViewports = 1;
	m_pContextDevice->RSGetViewports(&iNumViewports, &tViewportDesc);

	m_pVIBuffer = CVIBuffer_Viewport::Create(m_pGraphicDevice, m_pContextDevice, L"../Bin/ShaderFiles/Shader_Viewport.hlsl", "Technique_Default" ,_float4(0.f, 0.f, tViewportDesc.Width, tViewportDesc.Height));
	if (m_pVIBuffer == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CLight::NativeConstruct(const _tchar * pShaderFilePath, const char * pTechniqueName, const LIGHTDESC& tLightDesc)
{
	m_tDesc = tLightDesc;

	if (m_tDesc.eLightType == LIGHTDESC::LIGHT_DIRECTIONAL)
		m_iPassIndex = 1;

	if (m_tDesc.eLightType == LIGHTDESC::LIGHT_POINT)
		m_iPassIndex = 2;

	D3D11_VIEWPORT			tViewportDesc;
	ZeroMemory(&tViewportDesc, sizeof(D3D11_VIEWPORT));

	_uint					iNumViewports = 1;
	m_pContextDevice->RSGetViewports(&iNumViewports, &tViewportDesc);

	m_pVIBuffer = CVIBuffer_Viewport::Create(m_pGraphicDevice, m_pContextDevice, pShaderFilePath, pTechniqueName, _float4(0.f, 0.f, tViewportDesc.Width, tViewportDesc.Height));
	if (m_pVIBuffer == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CLight::Render()
{
	if (!m_bLightOn)
		return S_OK;

	CRenderTargetManager*		pRenderTargetManager = CRenderTargetManager::Get_Instance();
	CPipeLine*					pPipeLine = CPipeLine::Get_Instance();
	if (m_tDesc.eLightType == LIGHTDESC::LIGHT_DIRECTIONAL)
	{
		m_iPassIndex = 1;
		m_pVIBuffer->Set_RawValue("g_vLightDir", &m_tDesc.vDir, sizeof(_float4));
	}
	else if (m_tDesc.eLightType == LIGHTDESC::LIGHT_POINT)
	{
		m_iPassIndex = 2;
		m_pVIBuffer->Set_RawValue("g_vLightPosition", &m_tDesc.vPos, sizeof(_float4));
		m_pVIBuffer->Set_RawValue("g_fRange", &m_tDesc.fRange, sizeof(_float));
	}

	m_pVIBuffer->Set_RawValue("g_vLightDiffuse", &m_tDesc.vDiffuse, sizeof(_float4));
	m_pVIBuffer->Set_RawValue("g_vLightAmbient", &m_tDesc.vAmbient, sizeof(_float4));
	m_pVIBuffer->Set_RawValue("g_vLightSpecular", &m_tDesc.vSpecular, sizeof(_float4));


	_float fFar = pPipeLine->Get_Far();
	m_pVIBuffer->Set_RawValue("g_fFar", &fFar, sizeof(_float));
	m_pVIBuffer->Set_Texture("g_texNormal", pRenderTargetManager->Get_SRV(TEXT("RTV_Normal")));
	m_pVIBuffer->Set_Texture("g_texDepth", pRenderTargetManager->Get_SRV(TEXT("RTV_Depth")));



	_matrix		matProjInv, matViewInv;

	matProjInv = pPipeLine->Get_InverseTransform(CPipeLine::D3DTS_PROJ);
	matViewInv = pPipeLine->Get_InverseTransform(CPipeLine::D3DTS_VIEW);

	m_pVIBuffer->Set_RawValue("g_matProjInv", &XMMatrixTranspose(matProjInv), sizeof(_matrix));
	m_pVIBuffer->Set_RawValue("g_matViewInv", &XMMatrixTranspose(matViewInv), sizeof(_matrix));

	m_pVIBuffer->Set_RawValue("g_vCamPos", &pPipeLine->Get_CamPos(), sizeof(_vector));

	m_pVIBuffer->Render(m_iPassIndex);


	return S_OK;
}

CLight * CLight::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const LIGHTDESC* tLightDesc)
{
	CLight* pInstance = new CLight(pGraphicDevice, pContextDevice);

	if (FAILED(pInstance->NativeConstruct(tLightDesc)))
		Safe_Release(pInstance);

	return pInstance;
}

CLight * CLight::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const _tchar * pShaderFilePath, const char * pTechniqueName, const LIGHTDESC & tLightDesc)
{
	CLight* pInstance = new CLight(pGraphicDevice, pContextDevice);

	if (FAILED(pInstance->NativeConstruct(pShaderFilePath, pTechniqueName, tLightDesc)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pContextDevice);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pVIBuffer);
}
