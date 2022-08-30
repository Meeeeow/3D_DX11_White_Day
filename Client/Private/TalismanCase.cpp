#include "stdafx.h"
#include "..\Public\TalismanCase.h"
#include "LoadDatFiles.h"
#include "PositionManager.h"

CTalismanCase::CTalismanCase(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CTalismanCase::NativeConstruct(OBJ_INFO tObjInfo)
{
	if (FAILED(Set_Components()))
		return E_FAIL;
	CTransform::DESC tTransformDesc;
	tTransformDesc.dSpeedPerSec = 0.11f;
	tTransformDesc.dRotationPerSec = 0.15f;


	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, tObjInfo.szFilePath, tObjInfo.szFileName, Shader_Model, DefaultTechnique);
	if (m_pModel == nullptr)
		return E_FAIL;

	Set_InitialPosition(tObjInfo);
	m_fCullingRadius = tObjInfo.fCullRadius;

	CCollider::DESC tColliderDesc;
	tColliderDesc.vPivot = tObjInfo.tColliderDesc.vPivot;
	tObjInfo.tColliderDesc.vScale.y = 1.f;
	tColliderDesc.vScale = tObjInfo.tColliderDesc.vScale;
	
	
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);

	Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc);

	m_bRender = false;

	return S_OK;
}

_int CTalismanCase::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() != LEVEL_QASMOKE)
		return OBJ_NOEVENT;

	return __super::Tick(dTimeDelta);
}

_int CTalismanCase::LateTick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() == LEVEL_QASMOKE)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
	return _int();
}

HRESULT CTalismanCase::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CTalismanCase::Interaction()
{

}

void CTalismanCase::Event()
{
	if (CPositionManager::Get_Instance()->Get_Player()->Get_AddTalisman() == false)
		return;

	CPositionManager::Get_Instance()->Get_Player()->Use_Talisman();
}

void CTalismanCase::Collect_Event()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_float fDistance = 0.0f;
	if (CLoadDatFiles::Get_Instance()->Get_ModelLoading())
	{
		if (m_pCollider->Collision_OBB(pGameInstance->Get_Pivot(), pGameInstance->Get_Ray(), fDistance))
		{
			CPickingEventManager::PE tDesc;
			tDesc.pEventObject = this;
			tDesc.fRayDistance = fDistance;
			tDesc.eState = CPickingEventManager::FSM_TALISMAN;
			pGameInstance->Add_Event(tDesc);
		}
	}
}

CTalismanCase * CTalismanCase::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo)
{
	CTalismanCase* pInstance = new CTalismanCase(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTalismanCase::Free()
{
	__super::Free();
}
