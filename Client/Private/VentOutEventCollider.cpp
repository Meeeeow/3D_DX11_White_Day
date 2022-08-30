#include "stdafx.h"
#include "..\Public\VentOutEventCollider.h"
#include "PositionManager.h"
#include "Level_Qasmoke.h"

CVentOutEventCollider::CVentOutEventCollider(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CGameObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CVentOutEventCollider::NativeConstruct()
{
	CCollider::DESC tColliderDesc;
	tColliderDesc.vPivot = _float3(15.2f, 2.1f, -4.71f);
	tColliderDesc.vScale = _float3(0.15f, 0.45f, 0.15f);
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);
	Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc);

	return S_OK;
}

_int CVentOutEventCollider::Tick(_double dTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_pCollider->Update(XMMatrixIdentity());
	if (m_pCollider->Collision_OBBtoOBB(CPositionManager::Get_Instance()->Get_PlayerCollider()->Get_OBB()))
	{
		CPositionManager::Get_Instance()->Get_PlayerTransform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(16.16f, 0.f, -4.35f,1.f));
		CPositionManager::Get_Instance()->Get_Player()->Off_Vent();
		pGameInstance->Get_CurrentLevel()->Get_NavMesh()->In_Cell(CPositionManager::Get_Instance()->Get_PlayerTransform()->Get_State(CTransform::STATE_POSITION));
		m_bDead = true;
	}
	
	return _int();
}

_int CVentOutEventCollider::LateTick(_double dTimeDelta)
{
	return _int();
}

HRESULT CVentOutEventCollider::Render()
{
	return S_OK;
}

void CVentOutEventCollider::Interaction()
{

}

void CVentOutEventCollider::Event()
{
}

void CVentOutEventCollider::Collect_Event()
{
}

CVentOutEventCollider * CVentOutEventCollider::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CVentOutEventCollider* pInstance = new CVentOutEventCollider(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);
	return pInstance;
}

void CVentOutEventCollider::Free()
{
	__super::Free();
	Safe_Release(m_pCollider);
}

void CVentOutEventCollider::Compute_Distance()
{

}

CGameObject * CVentOutEventCollider::Clone(void * pArg)
{
	return nullptr;
}
