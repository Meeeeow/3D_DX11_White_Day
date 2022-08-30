#include "stdafx.h"
#include "..\Public\DeadHauntedPot.h"

CDeadHauntedPot::CDeadHauntedPot(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CDeadHauntedPot::NativeConstruct(_fvector vPosition)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	_matrix matPivot = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, "../Bin/Resource/Meshes/", "Pot_Dead.fbx"
		, Shader_Model, DefaultTechnique, matPivot);
	if (m_pModel == nullptr)
		return E_FAIL;

	m_pTransform->Set_State(CTransform::STATE_POSITION,vPosition);

	return S_OK;
}

_int CDeadHauntedPot::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() != LEVEL_QASMOKE)
		return OBJ_NOEVENT;

	return _int();
}

_int CDeadHauntedPot::LateTick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Culling(m_pTransform, 3.f) && pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() == LEVEL_QASMOKE)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);

	return _int();
}

HRESULT CDeadHauntedPot::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CDeadHauntedPot * CDeadHauntedPot::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, _fvector vPosition)
{
	CDeadHauntedPot* pInstance = new CDeadHauntedPot(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(vPosition)))
		Safe_Release(pInstance);
	return pInstance;
}

void CDeadHauntedPot::Free()
{
	__super::Free();
}
