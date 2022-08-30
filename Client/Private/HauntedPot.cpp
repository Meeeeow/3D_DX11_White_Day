#include "stdafx.h"
#include "..\Public\HauntedPot.h"
#include "LoadDatFiles.h"
#include "PositionManager.h"
#include "Poison.h"
#include "DeadHauntedPot.h"

CHauntedPot::CHauntedPot(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{
}

HRESULT CHauntedPot::NativeConstruct(OBJ_INFO tObjInfo, JANITOR eTarget)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, tObjInfo.szFilePath, tObjInfo.szFileName
		, Shader_Model, DefaultTechnique);
	if (m_pModel == nullptr)
		return E_FAIL;

	m_pModel->Set_Animation(0);

	CCollider::DESC tColliderDesc;
	
	tColliderDesc.vPivot = tObjInfo.tColliderDesc.vPivot;
	tColliderDesc.vScale = tObjInfo.tColliderDesc.vScale;
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);

	Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc);
	
	m_eTarget = eTarget;

	Set_InitialPosition(tObjInfo);

	m_iRenderIndex = 1;

	return S_OK;
}

_int CHauntedPot::Tick(_double dTimeDelta)
{
	if (m_bDead)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, CDeadHauntedPot::Create(m_pGraphicDevice, m_pContextDevice, m_pTransform->Get_State(CTransform::STATE_POSITION)), L"LayerDeadPot");
		return OBJ_DEAD;
	}

	if (m_bSetFinished)
	{
		m_dTimeAcc += dTimeDelta;
		if (m_dTimeAcc >= 1.5)
			m_bDead = true;
	}

	return __super::Tick(dTimeDelta);
}

_int CHauntedPot::LateTick(_double dTimeDelta)
{
	m_pModel->Update_CombinedTransformationMatrix(dTimeDelta);
	
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Culling(m_pTransform, 5.f))
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
	

	return OBJ_NOEVENT;
}

HRESULT CHauntedPot::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CHauntedPot::Interaction()
{

}

void CHauntedPot::Event()
{
	if (m_bAction)
		return;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_bAction = true;
	if (FAILED(pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, CPoison::Create(m_pGraphicDevice, m_pContextDevice, this), L"LayerPoison")))
		return;
		
}

void CHauntedPot::Collect_Event()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_float fDistance = 0.0f;
	if (CLoadDatFiles::Get_Instance()->Get_ModelLoading())
	{
		if (m_pCollider->Collision_OBB(pGameInstance->Get_Pivot(), pGameInstance->Get_Ray(), fDistance)
			&& CPositionManager::Get_Instance()->Get_Player()->Get_Poison())
		{
			CPickingEventManager::PE tDesc;
			tDesc.pEventObject = this;
			tDesc.fRayDistance = fDistance;
			tDesc.eState = CPickingEventManager::FSM_POISON;
			pGameInstance->Add_Event(tDesc);
		}
	}
}

CHauntedPot * CHauntedPot::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo, JANITOR eTarget)
{
	CHauntedPot* pInstance = new CHauntedPot(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo, eTarget)))
		Safe_Release(pInstance);

	return pInstance;
}

void CHauntedPot::Free()
{
	__super::Free();
}
