#include "stdafx.h"
#include "..\Public\BlackScrren.h"

CBlackScrren::CBlackScrren(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CRectXYObject(pGraphicDevice, pContextDevice)
{
}

HRESULT CBlackScrren::NativeConstruct()
{
	if (FAILED(Set_Components()))
		return E_FAIL;
	CTransform::DESC tTransformDesc;
	tTransformDesc.dRotationPerSec = 1.5;
	tTransformDesc.dSpeedPerSec = 1.1;
	m_pTransform->Set_TransformDesc(tTransformDesc);

	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	Add_Component(LEVEL_STATIC, L"BlackScreen", ComTexture, (CComponent**)&m_pTexture);
	m_pTransform->Scaling(XMVectorSet(1020.f, 800.f, 1.f, 0.f));
	m_pTransform->Set_State(CTransform::STATE_POSITION
		, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	return S_OK;
}

_int CBlackScrren::Tick(_double dTimeDelta)
{

	return _int();
}

_int CBlackScrren::LateTick(_double dTimeDelta)
{
	m_fAlpha += 0.0021f;
	if (m_fAlpha >= 1.f)
		m_fAlpha = 1.f;
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI_THIRD, this);
	return 0;
}

HRESULT CBlackScrren::Render()
{
	m_pVIBuffer->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matView", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matProj", &XMMatrixTranspose(XMLoadFloat4x4(&m_matProj)), sizeof(_float4x4));
	m_pVIBuffer->Set_Texture("g_texDiffuse", m_pTexture->Get_SRV(0));
	m_pVIBuffer->Render(5);

	return S_OK;
}

CBlackScrren * CBlackScrren::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CBlackScrren* pInstance = new CBlackScrren(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

void CBlackScrren::Free()
{
	__super::Free();
}
