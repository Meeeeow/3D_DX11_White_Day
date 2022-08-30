#include "stdafx.h"
#include "..\Public\MousePointer.h"



CMousePointer::CMousePointer(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CGameObject(pGraphicDevice, pContextDevice)
{

}

CMousePointer::CMousePointer(const CMousePointer & rhs)
	: CGameObject(rhs)
{

}

HRESULT CMousePointer::NativeConstruct()
{
	return S_OK;
}

HRESULT CMousePointer::Clone_Construct(void * pArg)
{
	Add_Component(LEVEL_STATIC, PrototypeRectXY, ComVIBuffer, (CComponent**)&m_pVIBuffer);
	Add_Component(LEVEL_STATIC, PrototypeRenderer, ComRenderer, (CComponent**)&m_pRenderer);
	Add_Component(LEVEL_STATIC, PrototypeTransform, ComTransform, (CComponent**)&m_pTransform);
	Add_Component(LEVEL_STATIC, L"MouseState", ComTexture, (CComponent**)&m_pTexture);

	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	m_pTransform->Scaling(XMVectorSet(20.f, 20.f, 0.f, 0.f));
	m_pTransform->Set_Scale(XMVectorSet(20.f, 20.f, 0.f, 0.f));

	return S_OK;
}

_int CMousePointer::Tick(_double dTimeDelta)
{
	D3D11_VIEWPORT tViewPort;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(pt.x - (g_iWinCX/2.f)
		, -pt.y + (g_iWinCY / 2.f), 0.f, 1.f));

	ShowCursor(FALSE);

	return 0;
}

_int CMousePointer::LateTick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_eState = pGameInstance->Get_FSM();
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI_THIRD, this);
	return 0;
}

HRESULT CMousePointer::Render()
{
	m_pVIBuffer->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matView", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matProj", &XMMatrixTranspose(XMLoadFloat4x4(&m_matProj)), sizeof(_float4x4));
	m_pVIBuffer->Set_Texture("g_texDiffuse", m_pTexture->Get_SRV((_uint)m_eState));
	m_pVIBuffer->Render(2);
	return S_OK;
}

CMousePointer * CMousePointer::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CMousePointer* pInstance = new CMousePointer(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CMousePointer::Clone(void * pArg)
{
	CMousePointer* pInstance = new CMousePointer(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CMousePointer::Free()
{
	__super::Free();
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);
}

void CMousePointer::Compute_Distance()
{

}
