#include "stdafx.h"
#include "..\Public\Dust.h"
#include "PositionManager.h"

CDust::CDust(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CRectXYObject(pGraphicDevice, pContextDevice)
{
}

HRESULT CDust::NativeConstruct(CValve* pValve, _uint iLevel, _fvector vPosition)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	Add_Component(LEVEL_STATIC, L"Dust", ComTexture, (CComponent**)&m_pTexture);

	m_pTransform->Scaling(XMVectorSet(1.f, 1.f, 1.f, 0.f));
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	m_pTransform->Rotation_AxisRadian(XMConvertToRadians(90.f));
	m_pValve = pValve;

	m_iLevel = iLevel;
	return S_OK;
}

_int CDust::Tick(_double dTimeDelta)
{
	if (m_iLevel == 0 && CPositionManager::Get_Instance()->Get_Valve0()->Get_Clear() == false)
		CSoundMgr::Get_Instance()->PlaySound(L"Steaming.wav", CSoundMgr::GAS0);
	else if (m_iLevel == 0 && CPositionManager::Get_Instance()->Get_Valve0()->Get_Clear() == true)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Steamed.wav", CSoundMgr::GAS0);
		return OBJ_DEAD;
	}

	if (m_iLevel == 1 && CPositionManager::Get_Instance()->Get_Valve1()->Get_Clear() == false)
		CSoundMgr::Get_Instance()->PlaySound(L"Steaming.wav", CSoundMgr::GAS1);
	else if (m_iLevel == 1 && CPositionManager::Get_Instance()->Get_Valve1()->Get_Clear() == true)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Steamed.wav", CSoundMgr::GAS1);
		return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

_int CDust::LateTick(_double dTimeDelta)
{
	Compute_Distance();
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
	return _int();
}

HRESULT CDust::Render()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_pVIBuffer->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matView", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matProj", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));
	m_pVIBuffer->Set_Texture("g_texDiffuse", m_pTexture->Get_SRV(0));
	m_pVIBuffer->Render(4);
	return S_OK;
}

void CDust::Compute_Distance()
{
	_vector vCamPos = CPositionManager::Get_Instance()->Get_Camera()->Get_CameraPos();

	m_fCameraDistance = XMVectorGetX(XMVector4Length(vCamPos - m_pTransform->Get_State(CTransform::STATE_POSITION)));
}

CDust * CDust::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, CValve* pValve, _uint iLevel, _fvector vPosition)
{
	CDust* pInstance = new CDust(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(pValve, iLevel, vPosition)))
		Safe_Release(pInstance);
	return pInstance;
}

void CDust::Free()
{
	__super::Free();
}
