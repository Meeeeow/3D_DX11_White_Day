#include "stdafx.h"
#include "..\Public\VentEventCollider.h"
#include "LeeHeeMin.h"
#include "LoadDatFiles.h"
#include "Level_Qasmoke.h"
#include "VentOutEventCollider.h"
#include "PositionManager.h"

CVentEventCollider::CVentEventCollider(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CGameObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CVentEventCollider::NativeConstruct()
{
	CCollider::DESC tColliderDesc;
	tColliderDesc.vPivot = _float3(5.9f, 2.1f, -7.77f);
	tColliderDesc.vScale = _float3(0.6f, 0.45f, 0.35f);
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);
	Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc);

	return S_OK;
}

_int CVentEventCollider::Tick(_double dTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	m_pCollider->Update(XMMatrixIdentity());
	return _int();
}

_int CVentEventCollider::LateTick(_double dTimeDelta)
{
	
	return _int();
}

HRESULT CVentEventCollider::Render()
{


	return S_OK;
}

void CVentEventCollider::Interaction()
{

}

void CVentEventCollider::Event()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CLeeHeeMin* pPlayer = dynamic_cast<CLeeHeeMin*>(pGameInstance->Get_GameObject(LEVEL_STATIC, LayerPlayer, 0));
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pPlayer->Get_Component(ComTransform));

	_vector	vVentPosition = XMVectorSet(6.93f, 1.78f, -7.87f, 1.f);
	pPlayerTransform->Set_State(CTransform::STATE_POSITION, vVentPosition);
	pPlayer->On_Vent();
	
	CLevel_Qasmoke* pLevel = dynamic_cast<CLevel_Qasmoke*>(pGameInstance->Get_CurrentLevel());
	pLevel->Set_VentIndex(pPlayerTransform->Get_State(CTransform::STATE_POSITION));
	pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, CVentOutEventCollider::Create(m_pGraphicDevice, m_pContextDevice), L"VentOutCollider");

	CPositionManager::Get_Instance()->Get_Valve0()->On_Gas();
	CPositionManager::Get_Instance()->Get_Valve1()->On_Gas();

	m_bDead = true;
}

void CVentEventCollider::Collect_Event()
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
			tDesc.eState = CPickingEventManager::FSM_VENT;
			pGameInstance->Add_Event(tDesc);
		}
	}
}

CVentEventCollider * CVentEventCollider::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CVentEventCollider* pInstance = new CVentEventCollider(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);
	return pInstance;
}

void CVentEventCollider::Free()
{
	__super::Free();
	Safe_Release(m_pCollider);
}

void CVentEventCollider::Compute_Distance()
{

}

CGameObject * CVentEventCollider::Clone(void * pArg)
{
	return nullptr;
}
