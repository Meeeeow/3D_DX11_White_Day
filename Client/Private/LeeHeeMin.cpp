#include "stdafx.h"
#include "..\Public\LeeHeeMin.h"
#include "PositionManager.h"
#include "Level_Loading.h"
#include "Hit.h"


CLeeHeeMin::CLeeHeeMin(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{
}

CLeeHeeMin::CLeeHeeMin(const CLeeHeeMin & rhs)
	: CModelObject(rhs)
{
}

HRESULT CLeeHeeMin::NativeConstruct()
{
	return S_OK;
}

HRESULT CLeeHeeMin::Clone_Construct(void * pArg)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	m_iRenderIndex = 1;

	m_pModel->Set_Animation(3);
	if (pArg == nullptr)
		m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, 0.f, 0.0f, 1.f));

	if (pArg != nullptr)
		Set_InitialPosition(*(OBJ_INFO*)pArg);
	m_pModel->Create_RefNode(L"RightHand", "Bip001 R Hand");
	CPositionManager::Get_Instance()->Set_Player(this);

	m_bOneCarry = false;
	m_bTwoCarry = false;
	m_bRun = false;
	m_bSit = false;
	m_bLighter = false;
	m_bUseTalisman = false;
	m_bVent = false;
	m_bKey = false;
	m_bCantControl = false;
	m_bHit = false;
	m_dHitLimit = 0.0;

	return S_OK;
}

_int CLeeHeeMin::Tick(_double dTimeDelta)
{
	if (m_bLoading == true)
		return OBJ_NOEVENT;

	if (0 > __super::Tick(dTimeDelta))
		return -1;
	m_pSitCollider->Update(m_pTransform->Get_WorldXM());
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (m_bCantControl)
		return 0;

	Check_Keyboard(pGameInstance, dTimeDelta);
	Mouse(pGameInstance, dTimeDelta);

	if (m_bHit == true)
	{
		m_dHitLimit -= dTimeDelta;
		if (m_dHitLimit < 0.0)
		{
			m_bHit = false;
			m_dHitLimit = 0.0;
		}
	}

	CPositionManager::Get_Instance()->Set_PlayerPosition(m_pTransform->Get_State(CTransform::STATE_POSITION));


	return _int();
}

_int CLeeHeeMin::LateTick(_double dTimeDelta)
{
	if (m_bLoading == true)
		return OBJ_NOEVENT;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Act(pGameInstance, dTimeDelta);

	return __super::LateTick(dTimeDelta);
}

HRESULT CLeeHeeMin::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CLeeHeeMin::Lighter(_double dTimeDelta)
{
	if (m_bGetLighter == false)
		return;

	m_bOneCarry = !m_bOneCarry;

	if (m_bOneCarry)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Lighter_On.wav", CSoundMgr::HM_LIGHTER_ON);
		m_bLighter = true;
	}

	if (!m_bOneCarry)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Lighter_Off.wav", CSoundMgr::HM_LIGHTER_OFF);
		m_bLighter = false;
	}

}

HRESULT CLeeHeeMin::Animation()
{
	return S_OK;
}

BoundingOrientedBox * CLeeHeeMin::Get_CurrentOBB()
{
	if (m_bSit || m_bVent)
		return m_pSitCollider->Get_OBB();
	else
		return m_pCollider->Get_OBB();
}

_matrix CLeeHeeMin::Get_BoneMatrix(const _tchar * pBoneTag)
{
	return m_pModel->Get_BoneMatrix(pBoneTag);
}

_matrix CLeeHeeMin::Get_CurrentAnimMatrix()
{
	return m_pModel->Get_AnimMatrix();
}

HRESULT CLeeHeeMin::Hit()
{
	if (m_bHit == true)
		return S_OK;

	m_dHitLimit = 2.0;
	m_bHit = true;
	CSoundMgr::Get_Instance()->PlaySound(L"HM_Hit.wav", CSoundMgr::HM_IDLE);
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Insert_ObjectToLayer(LEVEL_STATIC, CHit::Create(m_pGraphicDevice, m_pContextDevice), L"LayerHit");
	return S_OK;
}

HRESULT CLeeHeeMin::Set_Components()
{
	if (FAILED(__super::Set_Components()))
		return E_FAIL;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform::DESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(tTransformDesc));
	tTransformDesc.dRotationPerSec = 2.0;
	tTransformDesc.dSpeedPerSec = 2.0;
	m_pTransform->Set_TransformDesc(tTransformDesc);

	CCollider::DESC tColliderDesc;
	ZeroMemory(&tColliderDesc, sizeof(tColliderDesc));
	tColliderDesc.vPivot = _float3(0.f, 0.7f, 0.f);
	tColliderDesc.vScale = _float3(0.2f, 0.6f, 0.2f);
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc)))
		return E_FAIL;

	tColliderDesc.vPivot = _float3(0.f, 0.4f, 0.f);
	tColliderDesc.vScale = _float3(0.2f, 0.4f, 0.2f);

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeOBB, L"ComSitCollider", (CComponent**)&m_pSitCollider, &tColliderDesc)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Com_LeeHeeMin"), ComVIBuffer, (CComponent**)&m_pModel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLeeHeeMin::Check_Keyboard(CGameInstance * pGameInstance, _double dTimeDelta)
{
	Set_State(FSM_UP, FSM_IDLE);

	if (pGameInstance->Key_Down(DIK_R))
		Lighter(dTimeDelta);

	if (!m_bVent)
		Move(pGameInstance, dTimeDelta);
	else
		Move_Vent(pGameInstance, dTimeDelta);

	if (pGameInstance->Key_Down(DIK_1))
		CPositionManager::Get_Instance()->Get_Camera()->Set_TargetOn(CCamera_Debug::TARGET_HEEMIN);
	if (pGameInstance->Key_Down(DIK_2))
		CPositionManager::Get_Instance()->Get_LeeBongGoo()->Dead();
	if (pGameInstance->Key_Down(DIK_3))
		CPositionManager::Get_Instance()->Get_Camera()->Set_TargetOn(CCamera_Debug::TARGET_FUSEBOX);
	if (pGameInstance->Key_Down(DIK_4))
	{
		Set_Loading(true);
		pGameInstance->Open_Level(CLevel_Loading::Create(m_pGraphicDevice, m_pContextDevice, LEVEL_MAZE, LEVEL_LOADING));
	}
	if (pGameInstance->Key_Down(DIK_5))
		pGameInstance->Get_CurrentNav()->In_Cell(m_pTransform->Get_State(CTransform::STATE_POSITION));


	m_pModel->Set_Animation(m_iAnimation);
	return S_OK;
}

HRESULT CLeeHeeMin::Move(CGameInstance * pGameInstance, _double dTimeDelta)
{
	if (pGameInstance->Key_Down(DIK_LSHIFT))
		m_bRun = !m_bRun;

	if (pGameInstance->Key_Down(DIK_LCONTROL))
		m_bSit = !m_bSit;

	if (pGameInstance->Key_Pressing(DIK_A))
	{
		Set_State(FSM_UP, FSM_WALK);
		m_pTransform->Move_Left(dTimeDelta, pGameInstance->Get_CurrentNav());
		if (m_bRun == false)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Walk.wav", CSoundMgr::HM_WALK);
		else if(m_bRun == true)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Run.wav", CSoundMgr::HM_RUN);

	}
	if (pGameInstance->Key_Pressing(DIK_D))
	{
		Set_State(FSM_UP, FSM_WALK);
		m_pTransform->Move_Right(dTimeDelta, pGameInstance->Get_CurrentNav());
		if (m_bRun == false)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Walk.wav", CSoundMgr::HM_WALK);
		else if (m_bRun == true)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Run.wav", CSoundMgr::HM_RUN);
	}
	if (pGameInstance->Key_Pressing(DIK_W))
	{
		Set_State(FSM_UP, FSM_WALK);
		m_pTransform->Move_Front(dTimeDelta, pGameInstance->Get_CurrentNav());
		if (m_bRun == false)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Walk.wav", CSoundMgr::HM_WALK);
		else if (m_bRun == true)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Run.wav", CSoundMgr::HM_RUN);
	}
	if (pGameInstance->Key_Pressing(DIK_S))
	{
		Set_State(FSM_UP, FSM_WALK);
		m_pTransform->Move_Back(dTimeDelta, pGameInstance->Get_CurrentNav());
		if (m_bRun == false)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Walk.wav", CSoundMgr::HM_WALK);
		else if (m_bRun == true)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Run.wav", CSoundMgr::HM_RUN);
	}

	


	return S_OK;
}

HRESULT CLeeHeeMin::Move_Vent(CGameInstance * pGameInstance, _double dTimeDelta)
{
	if (pGameInstance->Key_Pressing(DIK_A))
	{
		Set_State(FSM_UP, FSM_WALK);
		m_pTransform->Move_VentLeft(dTimeDelta, pGameInstance->Get_CurrentNav());
		if (m_bRun == false)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Vent.wav", CSoundMgr::HM_WALK);
		else if (m_bRun == true)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Vent.wav", CSoundMgr::HM_RUN);
	}
	if (pGameInstance->Key_Pressing(DIK_D))
	{
		Set_State(FSM_UP, FSM_WALK);
		m_pTransform->Move_VentRight(dTimeDelta, pGameInstance->Get_CurrentNav());
		if (m_bRun == false)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Vent.wav", CSoundMgr::HM_WALK);
		else if (m_bRun == true)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Vent.wav", CSoundMgr::HM_RUN);
	}
	if (pGameInstance->Key_Pressing(DIK_W))
	{
		Set_State(FSM_UP, FSM_WALK);
		m_pTransform->Move_VentFront(dTimeDelta, pGameInstance->Get_CurrentNav());
		if (m_bRun == false)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Vent.wav", CSoundMgr::HM_WALK);
		else if (m_bRun == true)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Vent.wav", CSoundMgr::HM_RUN);
	}
	if (pGameInstance->Key_Pressing(DIK_S))
	{
		Set_State(FSM_UP, FSM_WALK);
		m_pTransform->Move_VentBack(dTimeDelta, pGameInstance->Get_CurrentNav());
		if (m_bRun == false)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Vent.wav", CSoundMgr::HM_WALK);
		else if (m_bRun == true)
			CSoundMgr::Get_Instance()->PlaySound(L"HM_Vent.wav", CSoundMgr::HM_RUN);
	}

	return S_OK;
}

HRESULT CLeeHeeMin::Act(CGameInstance * pGameInstance, _double dTimeDelta)
{
	if (pGameInstance->Key_Down(DIK_E))
		pGameInstance->Action();

	return S_OK;
}

HRESULT CLeeHeeMin::Mouse(CGameInstance * pGameInstance, _double dTimeDelta)
{
	_double dwMouseX = 0.0;
	if (dwMouseX = pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_X))
		m_pTransform->Rotation_AxisLerp(dTimeDelta * dwMouseX * 0.2f);

	return S_OK;
}

HRESULT CLeeHeeMin::State_Up()
{
	switch (m_eKey)
	{
	case Client::CLeeHeeMin::FSM_IDLE:
		m_iAnimation = 3;
		break;
	case Client::CLeeHeeMin::FSM_WALK:
		m_iAnimation = 14;
		break;
	case Client::CLeeHeeMin::FSM_RUN:
		m_iAnimation = 0;
		break;
	case Client::CLeeHeeMin::FSM_END:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CLeeHeeMin::State_OneCarry()
{
	switch (m_eKey)
	{
	case Client::CLeeHeeMin::FSM_IDLE:
		m_iAnimation = 4;
		break;
	case Client::CLeeHeeMin::FSM_WALK:
		m_iAnimation = 15;
		break;
	case Client::CLeeHeeMin::FSM_RUN:
		m_iAnimation = 15;
		break;
	case Client::CLeeHeeMin::FSM_END:
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CLeeHeeMin::State_TwoCarry()
{
	switch (m_eKey)
	{
	case Client::CLeeHeeMin::FSM_IDLE:
		m_iAnimation = 6;
		break;
	case Client::CLeeHeeMin::FSM_WALK:
		m_iAnimation = 17;
		break;
	case Client::CLeeHeeMin::FSM_RUN:
		m_iAnimation = 17;
		break;
	case Client::CLeeHeeMin::FSM_END:
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CLeeHeeMin::State_Sit()
{
	switch (m_eKey)
	{
	case Client::CLeeHeeMin::FSM_IDLE:
		m_iAnimation = 12;
		break;
	case Client::CLeeHeeMin::FSM_WALK:
		m_iAnimation = 21;
		break;
	case Client::CLeeHeeMin::FSM_RUN:
		m_iAnimation = 21;
		break;
	case Client::CLeeHeeMin::FSM_END:
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CLeeHeeMin::State_Jimin()
{
	switch (m_eKey)
	{
	case Client::CLeeHeeMin::FSM_IDLE:
		m_iAnimation = 5;
		break;
	case Client::CLeeHeeMin::FSM_WALK:
		m_iAnimation = 16;
		break;
	case Client::CLeeHeeMin::FSM_RUN:
		m_iAnimation = 16;
		break;
	case Client::CLeeHeeMin::FSM_END:
		break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CLeeHeeMin::Set_State(FSM_STATE eState, FSM_KEY eKey)
{
	if (!m_bOneCarry && !m_bTwoCarry && !m_bVent && !m_bSit && !m_bJiMinCarry)
	{
		m_eState = eState;
		m_pTransform->Set_MoveSpeed(1.3);
	}

	if (m_bOneCarry)
	{
		m_eState = FSM_STATE::FSM_ONECARRY;
		m_pTransform->Set_MoveSpeed(1.2);
	}
	if (m_bTwoCarry)
	{
		m_eState = FSM_STATE::FSM_TWOCARRY;
		m_pTransform->Set_MoveSpeed(0.8);
	}

	if (m_bVent || m_bSit)
	{
		m_eState = FSM_STATE::FSM_SIT;
		m_pTransform->Set_MoveSpeed(0.6);
	}

	if (m_bJiMinCarry)
	{
		m_eState = FSM_STATE::FSM_JIMINCARRY;
		m_pTransform->Set_MoveSpeed(0.6);
	}

	m_eKey = eKey;

	if (m_bRun && eKey == FSM_WALK)
	{
		m_eKey = FSM_RUN;
		if (m_bSit || m_bVent)
			m_pTransform->Set_MoveSpeed(m_pTransform->Get_CurrentSpeed() + 0.0);
		else
			m_pTransform->Set_MoveSpeed(m_pTransform->Get_CurrentSpeed() + 0.5);
	}
	if (m_bHit)
	{
		m_pTransform->Set_MoveSpeed(m_pTransform->Get_CurrentSpeed() + 0.5);
	}

	switch (m_eState)
	{
	case Client::CLeeHeeMin::FSM_UP:
		State_Up();
		break;
	case Client::CLeeHeeMin::FSM_ONECARRY:
		State_OneCarry();
		break;
	case Client::CLeeHeeMin::FSM_TWOCARRY:
		State_TwoCarry();
		break;
	case Client::CLeeHeeMin::FSM_SIT:
		State_Sit();
		break;
	case Client::CLeeHeeMin::FSM_JIMINCARRY:
		State_Jimin();
		break;
	case Client::CLeeHeeMin::FSMSTATE_END:
		break;
	default:
		break;
	}

	return S_OK;
}

CLeeHeeMin * CLeeHeeMin::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CLeeHeeMin* pInstance = new CLeeHeeMin(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CLeeHeeMin::Clone(void * pArg)
{
	CLeeHeeMin* pInstance = new CLeeHeeMin(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLeeHeeMin::Free()
{
	__super::Free();
}

