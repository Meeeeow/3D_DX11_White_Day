#include "stdafx.h"
#include "..\Public\LogoScreen.h"

CLogoScreen::CLogoScreen(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CRectXYObject(pGraphicDevice, pContextDevice)
{
}

CLogoScreen::CLogoScreen(const CLogoScreen & rhs)
	: CRectXYObject(rhs)
{
}

HRESULT CLogoScreen::NativeConstruct()
{
	return S_OK;
}

HRESULT CLogoScreen::Clone_Construct(void * pArg)
{
	if (FAILED(Set_Components()))
		return E_FAIL;
	
	if (FAILED(Add_Component(LEVEL_STATIC, L"Lobby", ComTexture, (CComponent**)&m_pTexture)))
		return E_FAIL;

	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	m_fSizeX = (_float)800;
	m_fSizeY = (_float)g_iWinCY;
	m_fX = (_float)(800 >> 1);
	m_fY = (_float)(g_iWinCY >> 1);

	m_pTransform->Scaling(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.f));
	m_pTreeTransform->Scaling(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.f));
	m_pSelectTransform->Scaling(XMVectorSet(220.f, m_fSizeY, 1.f, 0.f));

	m_pTransform->Set_State(CTransform::STATE_POSITION
		, XMVectorSet(m_fX - 200 , -m_fY + (g_iWinCY >> 1), 0.f, 1.f));
	m_pTreeTransform->Set_State(CTransform::STATE_POSITION
		, XMVectorSet(m_fX - 200, -m_fY + (g_iWinCY >> 1), 0.f, 1.f));
	m_pSelectTransform->Set_State(CTransform::STATE_POSITION
		, XMVectorSet((g_iWinCX /2.f - 220.f) * -1.f, -m_fY + (g_iWinCY >> 1), 0.f, 1.f));


	return S_OK;
}

_int CLogoScreen::Tick(_double dTimeDelta)
{
	
	return _int();
}

_int CLogoScreen::LateTick(_double dTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI_FIRST, this);
	return _int();
}

HRESULT CLogoScreen::Render()
{
	m_pVIBuffer->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matView", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matProj", &XMMatrixTranspose(XMLoadFloat4x4(&m_matProj)), sizeof(_float4x4));
	m_pVIBuffer->Set_Texture("g_texDiffuse", m_pTexture->Get_SRV(0));
	m_pVIBuffer->Render(0);

	m_pTreeVIBuffer->Set_RawValue("g_matWorld", &m_pTreeTransform->Get_WorldTranspose(), sizeof(_float4x4));
	m_pTreeVIBuffer->Set_RawValue("g_matView", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pTreeVIBuffer->Set_RawValue("g_matProj", &XMMatrixTranspose(XMLoadFloat4x4(&m_matProj)), sizeof(_float4x4));
	m_pTreeVIBuffer->Set_Texture("g_texDiffuse", m_pTexture->Get_SRV(1));
	m_pTreeVIBuffer->Render(1);

	m_pSelectBuffer->Set_RawValue("g_matWorld", &m_pSelectTransform->Get_WorldTranspose(), sizeof(_float4x4));
	m_pSelectBuffer->Set_RawValue("g_matView", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pSelectBuffer->Set_RawValue("g_matProj", &XMMatrixTranspose(XMLoadFloat4x4(&m_matProj)), sizeof(_float4x4));
	m_pSelectBuffer->Set_Texture("g_texDiffuse", m_pTexture->Get_SRV(2));
	m_pSelectBuffer->Render(1);

	return S_OK;
}

HRESULT CLogoScreen::Set_Components()
{
	if (FAILED(__super::Set_Components()))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeTransform, L"ComTreeTransform", (CComponent**)&m_pTreeTransform)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeRectXY, L"ComTreeVIBuffer", (CComponent**)&m_pTreeVIBuffer)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeTransform, L"ComSelectTransform", (CComponent**)&m_pSelectTransform)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeRectXY, L"ComSelectVIBuffer", (CComponent**)&m_pSelectBuffer)))
		return E_FAIL;

	return S_OK;
}

CLogoScreen * CLogoScreen::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CLogoScreen* pInstance = new CLogoScreen(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CLogoScreen::Clone(void * pArg)
{
	CLogoScreen* pInstance = new CLogoScreen(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLogoScreen::Free()
{
	__super::Free();
	Safe_Release(m_pTreeTransform);
	Safe_Release(m_pTreeVIBuffer);
	Safe_Release(m_pSelectBuffer);
	Safe_Release(m_pSelectTransform);
}