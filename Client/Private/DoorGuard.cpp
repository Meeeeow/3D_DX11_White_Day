#include "stdafx.h"
#include "..\Public\DoorGuard.h"

CDoorGuard::CDoorGuard(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CDoorGuard::NativeConstruct(OBJ_INFO tObjInfo)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, tObjInfo.szFilePath, tObjInfo.szFileName, Shader_Model, DefaultTechnique);
	Set_InitialPosition(tObjInfo);
	m_fCullingRadius = 5.f;
	m_iRenderIndex = 2;
	return S_OK;
}

_int CDoorGuard::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() != LEVEL_QASMOKE)
		return OBJ_NOEVENT;

	return _int();
}

_int CDoorGuard::LateTick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_bRender = false;
	if (pGameInstance->Culling(m_pTransform, m_fCullingRadius) && m_fCameraDistance < 25.f && pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() == LEVEL_QASMOKE)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
		m_bRender = true;
	}
	return _int();
}

HRESULT CDoorGuard::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CDoorGuard * CDoorGuard::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo)
{
	CDoorGuard* pInstance = new CDoorGuard(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo)))
		Safe_Release(pInstance);
	return pInstance;
}

void CDoorGuard::Free()
{
	__super::Free();
}
