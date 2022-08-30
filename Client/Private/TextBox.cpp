#include "stdafx.h"
#include "..\Public\TextBox.h"

CTextBox::CTextBox(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CRectXYObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CTextBox::NativeConstruct(TEXTTYPE eType)
{
	if (FAILED(__super::Set_Components()))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("TextBox"), ComTexture, (CComponent**)&m_pTexture)))
		return E_FAIL;


	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	m_pTransform->Scaling(XMVectorSet(470.f, 148.f, 0.f, 0.f));

	m_pTransform->Set_State(CTransform::STATE_POSITION
		, XMVectorSet(0.f , (g_iWinCY / 2.f) * -1.f + 100.f, 0.f, 1.f));

	m_eType = eType;
	if(eType != TEXTTYPE::ENTER_TEXT)
		CSoundMgr::Get_Instance()->PlaySound(L"TextBox.wav", CSoundMgr::EFFECT);

	return S_OK;
}

_int CTextBox::Tick(_double dTimeDelta)
{
	if (m_fAlpha <= 0.f)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

_int CTextBox::LateTick(_double dTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI_FIRST, this);
	return _int();
}

HRESULT CTextBox::Render()
{
	m_pVIBuffer->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matView", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matProj", &XMMatrixTranspose(XMLoadFloat4x4(&m_matProj)), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("fAlpha", &m_fAlpha, sizeof(_float));
	m_pVIBuffer->Set_Texture("g_texDiffuse", m_pTexture->Get_SRV((_uint)m_eType));

	m_pVIBuffer->Render(3);

	m_fAlpha -= 0.01f;

	return S_OK;
}

CTextBox * CTextBox::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, TEXTTYPE eType)
{
	CTextBox* pInstance = new CTextBox(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(eType)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTextBox::Free()
{
	__super::Free();
}
