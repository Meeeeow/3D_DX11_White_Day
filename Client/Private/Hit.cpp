#include "stdafx.h"
#include "..\Public\Hit.h"

CHit::CHit(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CRectXYObject(pGraphicDevice, pContextDevice)
{
}

HRESULT CHit::NativeConstruct()
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	Add_Component(LEVEL_STATIC, L"Hit", ComTexture, (CComponent**)&m_pTexture);

	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));
	m_pTransform->Scaling(XMVectorSet(150.f, 150.f, 0.f, 0.f));

	int iRandX = rand() % 435;
	int iRandY = rand() % 325;
	
	int	iRandXM = rand() % 2;
	int	iRandXY = rand() % 2;

	if (iRandXM == 0 && iRandXY == 0)
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(iRandX, iRandY, 0.f, 1.f));
	else if (iRandXM == 1 && iRandXY == 0)
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(iRandX * -1.f, iRandY, 0.f, 1.f));
	else if (iRandXM == 0 && iRandXY == 1)
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(iRandX, iRandY * -1.f , 0.f, 1.f));
	else if (iRandXM == 1 && iRandXY == 1)
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(iRandX * -1.f, iRandY * -1.f, 0.f, 1.f));

	m_iRenderIndex = rand() % 7;

	return S_OK;
}

_int CHit::Tick(_double dTimeDelta)
{
	if (m_fAlpha <= 0.f)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

_int CHit::LateTick(_double dTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI_FIRST, this);
	return _int();
}

HRESULT CHit::Render()
{
	m_pVIBuffer->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matView", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matProj", &XMMatrixTranspose(XMLoadFloat4x4(&m_matProj)), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("fAlpha", &m_fAlpha, sizeof(_float));
	m_pVIBuffer->Set_Texture("g_texDiffuse", m_pTexture->Get_SRV(m_iRenderIndex));

	m_pVIBuffer->Render(3);

	m_fAlpha -= 0.01f;

	return S_OK;
}

CHit * CHit::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CHit* pInstance = new CHit(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

void CHit::Free()
{
	__super::Free();
}
