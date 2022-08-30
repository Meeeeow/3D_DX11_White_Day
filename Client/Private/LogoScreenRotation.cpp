#include "stdafx.h"
#include "..\Public\LogoScreenRotation.h"


CLogoScreenRotation::CLogoScreenRotation(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CRectXYObject(pGraphicDevice, pContextDevice)
{
}

CLogoScreenRotation::CLogoScreenRotation(const CLogoScreenRotation & rhs)
	: CRectXYObject(rhs)
{
}

HRESULT CLogoScreenRotation::NativeConstruct()
{
	return S_OK;
}

HRESULT CLogoScreenRotation::Clone_Construct(void * pArg)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	DESC tDesc;
	memcpy(&tDesc, pArg, sizeof(CLogoScreenRotation::DESC));

	if (FAILED(Add_Component(LEVEL_STATIC, tDesc.szTextureName, ComTexture, (CComponent**)&m_pTexture)))
		return E_FAIL;

	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	m_fSizeX = (_float)535;
	m_fSizeY = (_float)535;
	m_fX = (_float)(g_iWinCX >> 1) - (535 / 2.f);
	m_fY = (_float)(g_iWinCY >> 1) - (535 / 2.f);

	m_pTransform->Scaling(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.f));
	m_pTransform->Set_State(CTransform::STATE_POSITION
		, XMVectorSet(m_fX , -m_fY, 0.f, 1.f));

	return S_OK;
}

_int CLogoScreenRotation::Tick(_double dTimeDelta)
{
	m_pTransform->Rotation_Axis(dTimeDelta, XMVectorSet(0.f, 0.f, 1.f, 0.f));
	return _int();
}

_int CLogoScreenRotation::LateTick(_double dTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI_THIRD, this);
	return _int();
}

HRESULT CLogoScreenRotation::Render()
{
	m_pVIBuffer->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matView", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matProj", &XMMatrixTranspose(XMLoadFloat4x4(&m_matProj)), sizeof(_float4x4));
	m_pVIBuffer->Set_Texture("g_texDiffuse", m_pTexture->Get_SRV());

	return __super::Render();
}

HRESULT CLogoScreenRotation::Set_Components()
{
	if (FAILED(__super::Set_Components()))
		return E_FAIL;

	CTransform::DESC tTransformDesc;
	tTransformDesc.dSpeedPerSec = 1.f;
	tTransformDesc.dRotationPerSec = 1.f;

	m_pTransform->Set_TransformDesc(tTransformDesc);

	return S_OK;
}

CLogoScreenRotation * CLogoScreenRotation::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CLogoScreenRotation* pInstance = new CLogoScreenRotation(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CLogoScreenRotation::Clone(void * pArg)
{
	CLogoScreenRotation* pInstance = new CLogoScreenRotation(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLogoScreenRotation::Free()
{
	__super::Free();
}
