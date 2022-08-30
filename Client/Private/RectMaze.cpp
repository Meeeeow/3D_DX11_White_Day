#include "stdafx.h"
#include "..\Public\RectMaze.h"


CRectMaze::CRectMaze(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CRectXYObject(pGraphicDevice, pContextDevice)
{
}

HRESULT CRectMaze::NativeConstruct()
{
	if (FAILED(Set_Components()))
		return E_FAIL;
	CTransform::DESC tTransformDesc;
	tTransformDesc.dRotationPerSec = 1.5;
	tTransformDesc.dSpeedPerSec = 1.1;
	m_pTransform->Set_TransformDesc(tTransformDesc);

	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f));

	Add_Component(LEVEL_STATIC, L"LoadMaze", ComTexture, (CComponent**)&m_pTexture);
	m_pTransform->Scaling(XMVectorSet(520.f, 520.f, 1.f, 0.f));
	m_pTransform->Set_State(CTransform::STATE_POSITION
		, XMVectorSet(0.f , 0.f, 0.f, 1.f));

	return S_OK;
}

_int CRectMaze::Tick(_double dTimeDelta)
{
	m_pTransform->Rotation_Axis(dTimeDelta, XMVectorSet(0.f, 0.f, 1.f, 0.f));
	return _int();
}

_int CRectMaze::LateTick(_double dTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_UI_THIRD, this);
	return _int();
}

HRESULT CRectMaze::Render()
{
	m_pVIBuffer->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matView", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matProj", &XMMatrixTranspose(XMLoadFloat4x4(&m_matProj)), sizeof(_float4x4));
	m_pVIBuffer->Set_Texture("g_texDiffuse", m_pTexture->Get_SRV(0));
	m_pVIBuffer->Render(1);

	return S_OK;
}

CRectMaze * CRectMaze::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CRectMaze* pInstance = new CRectMaze(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);
	return pInstance;
}

void CRectMaze::Free()
{
	__super::Free();
}
