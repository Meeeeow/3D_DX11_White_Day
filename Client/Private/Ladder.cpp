#include "stdafx.h"
#include "..\Public\Ladder.h"
#include "LoadDatFiles.h"
#include "LeeHeeMin.h"
#include "Level_Qasmoke.h"
#include "WomanToiletLadder.h"
#include "PositionManager.h"

CLadder::CLadder(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{
}

CLadder::CLadder(const CLadder & rhs)
	: CModelObject(rhs)
{

}

HRESULT CLadder::NativeConstruct()
{
	return S_OK;
}

HRESULT CLadder::Clone_Construct(void * pArg)
{
	if (FAILED(Set_Components()))
		return E_FAIL;


	CTransform::DESC tTransformDesc;
	tTransformDesc.dRotationPerSec = 2.f;
	tTransformDesc.dSpeedPerSec = 2.f;
	m_pTransform->Set_TransformDesc(tTransformDesc);

	Add_Component(LEVEL_QASMOKE, L"LadderModel", ComVIBuffer, (CComponent**)&m_pModel);

	CCollider::DESC tColliderDesc;
	tColliderDesc.vPivot = _float3(0.f, 0.45f, 0.f);
	tColliderDesc.vScale = _float3(0.4f, 0.45f, 0.4f);
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);

	Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc);
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CLeeHeeMin* pInstance = dynamic_cast<CLeeHeeMin*>(pGameInstance->Get_GameObject(LEVEL_STATIC, LayerPlayer, 0));
	pInstance->TwoCarry_On();
	m_bActive = false;
	CSoundMgr::Get_Instance()->PlaySound(L"Ladder_Create.wav", CSoundMgr::LADDER_CREATE);
	return S_OK;
}

_int CLadder::Tick(_double dTimeDelta)
{
	if (m_bActive)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Carry(pGameInstance, dTimeDelta);
	Interaction(pGameInstance);
	return __super::Tick(dTimeDelta);
}

_int CLadder::LateTick(_double dTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
	return _int();
}

HRESULT CLadder::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CLadder::Interaction(CGameInstance* pGameInstance)
{
	CLevel_Qasmoke* pLevel = dynamic_cast<CLevel_Qasmoke*>(pGameInstance->Get_CurrentLevel());
	CCollider* pLevelCollider = pLevel->Get_Event1();
	if (m_pCollider->Collision_OBBtoOBB(pLevelCollider->Get_OBB()))
	{
		m_bActive = true;
		pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, L"PrototypeWomanLadder", L"LayerWomanToilet");
		CPositionManager::Get_Instance()->Get_Player()->TwoCarry_Off();
	}
}

void CLadder::Event()
{
	

}

void CLadder::Collect_Event()
{

}

void CLadder::Carry(CGameInstance * pGameInstance, _double dTimeDelta)
{
	CLeeHeeMin* pInstance = CPositionManager::Get_Instance()->Get_Player();
	CTransform* pPlayerTransform = CPositionManager::Get_Instance()->Get_PlayerTransform();

	_vector		vLook = pPlayerTransform->Get_State(CTransform::STATE_LOOK);
	_vector		vPosition = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_double dwMouseX = 0.0;
	dwMouseX = pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_X);

	m_pTransform->Rotation_Axis(dTimeDelta * dwMouseX * 0.2f);
	vPosition += XMVector3Normalize(vLook) * 21.6f * dTimeDelta;
	_float fHeight = XMVectorGetY(vPosition) + 0.2f;
	vPosition = XMVectorSetY(vPosition, fHeight);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	CSoundMgr::Get_Instance()->PlaySound(L"Ladder_Carry.wav", CSoundMgr::LADDER_CREATE);
}

CLadder * CLadder::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CLadder* pInstance = new CLadder(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CLadder::Clone(void * pArg)
{
	CLadder* pInstance = new CLadder(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLadder::Free()
{
	__super::Free();
}
