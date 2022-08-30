#include "stdafx.h"
#include "..\Public\LeeBongGoo.h"
#include "PositionManager.h"

CLeeBongGoo::CLeeBongGoo(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{
}

CLeeBongGoo::CLeeBongGoo(const CLeeBongGoo & rhs)
	: CModelObject(rhs)
{
}

HRESULT CLeeBongGoo::NativeConstruct()
{
	return S_OK;
}

HRESULT CLeeBongGoo::Clone_Construct(void * pArg)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	m_pModel->Set_Animation(1);
	if (pArg != nullptr)
		Set_InitialPosition(*(OBJ_INFO*)pArg);
	m_iRenderIndex = 1;
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(5.f, 0.f, 41.f, 1.f));

	CPositionManager::Get_Instance()->Set_LeeBongGoo(this);

	m_bAttack = false;
	m_dAttackLimit = 0.0;
	m_dManagerAStarTimeAcc = 0.0;
	m_dManagerAStarLimitAcc = 0.0;
	m_bManagerAStar = false;
	m_bMakeManagerAStar = false;
	m_dRandomAStarTimeAcc = 0.0;
	m_dRandomAStarLimitAcc = 0.0;
	m_bRandomAStar = false;
	m_bMakeRandomAStar = false;
	m_dTraceLimitAcc = 0.0;
	m_bFirstWhistle = false;
	m_bWhistle = false;
	m_bView = false;
	m_bCollision = false;
	m_bAmbush = false;
	m_dAmbushLimit = 0.0;
	m_fAlpha = 1.0f;
	m_bDead = false;
	m_bAlpha = false;

	return S_OK;
}

_int CLeeBongGoo::Tick(_double dTimeDelta)
{
	if (m_bDead)
	{
		CPositionManager::Get_Instance()->Get_Camera()->Set_TargetOn(CCamera_Debug::TARGET_HEEMIN);
		CPositionManager::Get_Instance()->LeeBongGooDead();
		CGameInstance::Get_Instance()->Light_DeActive(L"BongGooLantern");
		return OBJ_DEAD;
	}

	if (__super::Tick(dTimeDelta) < 0)
		return -1;
	m_pViewCollider->Update(m_pTransform->Get_WorldXM());

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	LIGHTDESC& tDesc = pGameInstance->Get_Light(L"BongGooLantern");
	_float4x4 matLantern;
	XMStoreFloat4x4(&matLantern, XMMatrixMultiply(m_pModel->Get_BoneMatrix(L"Lantern"), m_pTransform->Get_WorldXM()));
	XMStoreFloat4(&tDesc.vPos, XMLoadFloat4((_float4*)&matLantern.m[3][0]));

	Move(pGameInstance, dTimeDelta);
	

	return OBJ_NOEVENT;
}

_int CLeeBongGoo::LateTick(_double dTimeDelta)
{
	return __super::LateTick(dTimeDelta);
}

HRESULT CLeeBongGoo::Render()
{
	if (m_bAlpha)
	{
		m_fAlpha -= 0.01f;
		if (m_fAlpha <= 0.f)
			m_bDead = true;
		m_pModel->Set_RawValue("fAlpha", &m_fAlpha, sizeof(_float));
		m_iRenderIndex = 3;
	}

	if (FAILED(__super::Render()))
		return E_FAIL;
	
	return S_OK;
}

void CLeeBongGoo::View_Check(CGameInstance * pGameInstance, _double dTimeDelta)
{
	_float fDistance = XMVectorGetX(XMVector4Length(CPositionManager::Get_Instance()->Get_PlayerPosition() - m_pTransform->Get_State(CTransform::STATE_POSITION)));

	if (m_pViewCollider->Collision_OBBtoOBB(CPositionManager::Get_Instance()->Get_Player()->Get_CurrentOBB()) == true)
	{
		m_bView = true;
		m_bWhistle = true;
		m_bManagerAStar = false;
		m_bMakeManagerAStar = false;
		m_bRandomAStar = false;
		m_bMakeRandomAStar = false;
		m_bAmbush = false;
		return;
	}
	else if (m_pViewCollider->Collision_OBBtoOBB(CPositionManager::Get_Instance()->Get_Player()->Get_CurrentOBB()) == false && fDistance < 11.f && m_bView == true)
	{
		return;
	}
	else if (m_pViewCollider->Collision_OBBtoOBB(CPositionManager::Get_Instance()->Get_Player()->Get_CurrentOBB()) == false && fDistance >= 11.f && m_bView == true)
	{
		m_dTraceLimitAcc += dTimeDelta;
		if (m_dTraceLimitAcc > 5.0)
		{
			m_dTraceLimitAcc = 0.0;
			m_bView = false;
			m_bWhistle = false;
			m_bFirstWhistle = false;
			m_bAmbush = true;
			return;
		}
	}

	if (m_bView == false && (m_bManagerAStar == false && m_bRandomAStar == false))
	{ 
		m_dManagerAStarTimeAcc += dTimeDelta;
		if (m_dManagerAStarTimeAcc >= 150.0)
		{
			m_dManagerAStarTimeAcc = 0.0;
			m_bManagerAStar = true;
		}
		m_dRandomAStarTimeAcc += dTimeDelta;
		if (m_dRandomAStarTimeAcc >= 15.0)
		{
			m_dRandomAStarTimeAcc = 0.0;
			m_bRandomAStar = true;
		}
	}

}

HRESULT CLeeBongGoo::Set_State(BG_FSM_STATE eState, BG_FSM_KEY eKey)
{
	//enum BG_FSM_KEY { BG_KEY_IDLE, BG_KEY_WALK, BG_KEY_RUN, BG_KEY_ATTACK, BG_KEY_END };
	//enum BG_FSM_STATE { BG_STATE_PATROL, BG_STATE_CHASE, BG_STATE_AMBUSH, BG_STATE_END };
	if (m_bAttack)
	{
		m_iAnimationIndex = 3;
		return S_OK;
	}

	m_eState = eState;

	if (m_bView)
	{
		m_eState = BG_STATE_CHASE;
		m_pTransform->Set_MoveSpeed(1.8);
	}
	else if (m_bRandomAStar || m_bManagerAStar)
	{
		m_eState = BG_STATE_PATROL;
		m_pTransform->Set_MoveSpeed(1.0);
	}

	

	m_eKey = eKey;


	switch (m_eState)
	{
	case Client::CLeeBongGoo::BG_STATE_PATROL:
		State_Patrol();
		break;
	case Client::CLeeBongGoo::BG_STATE_CHASE:
		State_Chase();
		break;
	case Client::CLeeBongGoo::BG_STATE_AMBUSH:
		State_Ambush();
		break;
	case Client::CLeeBongGoo::BG_STATE_END:
		break;
	default:
		break;
	}


	return S_OK;
}

HRESULT CLeeBongGoo::Set_Components()
{
	if (FAILED(__super::Set_Components()))
		return E_FAIL;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform::DESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(tTransformDesc));
	tTransformDesc.dRotationPerSec = 1.f;
	tTransformDesc.dSpeedPerSec = 2.5f;

	m_pTransform->Set_TransformDesc(tTransformDesc);

	CCollider::DESC tColliderDesc;
	ZeroMemory(&tColliderDesc, sizeof(tColliderDesc));
	tColliderDesc.vPivot = _float3(0.f, 0.7f, 0.f);
	tColliderDesc.vScale = _float3(0.2f, 0.6f, 0.2f);
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc)))
		return E_FAIL;

	tColliderDesc.vPivot = _float3(0.f, 1.25f, 0.f);
	tColliderDesc.vScale = _float3(6.f, 0.25f, 6.f);

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeOBB, L"ComViewCollider", (CComponent**)&m_pViewCollider, &tColliderDesc)))
		return E_FAIL;
	
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Com_LeeBongGoo"), ComVIBuffer, (CComponent**)&m_pModel)))
		return E_FAIL;
	m_pModel->Create_RefNode(L"Lantern", "FX_Char_Guard_LanternLight");


	LIGHTDESC tDesc;
	tDesc.eLightType = LIGHTDESC::LIGHT_POINT;
	tDesc.fRange = 3.5f;
	tDesc.vDiffuse = _float4(0.55f, 0.55f, 0.15f, 1.f);
	tDesc.vAmbient = _float4(0.55f, 0.55f, 0.15f, 1.f);
	tDesc.vSpecular = _float4(0.55f, 0.55f, 0.15f, 1.f);
	
	_float4x4 matLantern;
	XMStoreFloat4x4(&matLantern, m_pModel->Get_BoneMatrix(L"Lantern"));
	XMStoreFloat4(&tDesc.vPos, XMLoadFloat4((_float4*)&matLantern.m[3][0]));
	
	pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice, L"BongGooLantern", &tDesc);

	_uint i = 0;

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeAStar, ComAStar, (CComponent**)&m_pAStar
		, dynamic_cast<CNavigationMesh*>(pGameInstance->Clone_Component(LEVEL_QASMOKE, L"NavigationStage1", &i)))))
		return E_FAIL;
	
	pGameInstance->Light_Action(L"BongGooLantern");

	return S_OK;
}

HRESULT CLeeBongGoo::Move(CGameInstance * pGameInstance, _double dTimeDelta)
{
	if (m_bAlpha)
	{
		m_pModel->Set_Animation(0);
		return S_OK;
	}

	View_Check(pGameInstance, dTimeDelta);
	if (m_bWhistle && !m_bFirstWhistle)
		Whistle(pGameInstance, dTimeDelta);
	Set_State(BG_STATE_PATROL, BG_KEY_IDLE);

	if (m_bView)
		DirectTrace(pGameInstance, dTimeDelta);
	else if (m_bRandomAStar)
		RandomAStar(pGameInstance, dTimeDelta);
	else if (m_bManagerAStar)
		ManagerCall(pGameInstance, dTimeDelta);

	m_pModel->Set_Animation(m_iAnimationIndex);

	return S_OK;
}

HRESULT CLeeBongGoo::DirectTrace(CGameInstance * pGameInstance, _double dTimeDelta)
{
	if (m_bView == false)
		return S_OK;
	

	if (m_bAttack == true)
	{
		m_dAttackLimit += dTimeDelta;
		if (m_dAttackLimit > 2.0)
		{
			m_dAttackLimit = 0.0;
			m_bAttack = false;
		}
		return S_OK;
	}
	_vector		vPlayerPosition = CPositionManager::Get_Instance()->Get_PlayerPosition();

	m_pAStar->Start_AStar(m_pTransform->Get_State(CTransform::STATE_POSITION), vPlayerPosition);

	list<CCell*>& pBestList = m_pAStar->Get_ListBest();
	if (pBestList.empty() == false)
	{
		m_pTransform->Move_Trace(dTimeDelta, pBestList.front()->Get_CenterPos(), pGameInstance->Get_CurrentNav());
		_float fVectorDistance = XMVectorGetX(XMVector4Length(pBestList.front()->Get_CenterPos() - m_pTransform->Get_State(CTransform::STATE_POSITION)));
		if (fVectorDistance < 2.f)
			pBestList.pop_front();
	}
	Set_State(BG_STATE_CHASE, BG_KEY_RUN);

	

	_float		fDistance = XMVectorGetX(XMVector4Length(vPlayerPosition - m_pTransform->Get_State(CTransform::STATE_POSITION)));
	if(fDistance > 5.f)
		CSoundMgr::Get_Instance()->PlaySound(L"HeartBeat0.wav", CSoundMgr::HM_HEART);
	else if (fDistance < 5.f)
		CSoundMgr::Get_Instance()->PlaySound(L"HeartBeat1.wav", CSoundMgr::HM_HEART);

	if (pBestList.empty() == true)
		m_pTransform->Move_Trace(dTimeDelta, vPlayerPosition, pGameInstance->Get_CurrentNav());
	if (fDistance < 1.f)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"BG_Chase.wav", CSoundMgr::BG_VOICE);
		Attack(pGameInstance, dTimeDelta);
	}

	return S_OK;
}

HRESULT CLeeBongGoo::ManagerCall(CGameInstance * pGameInstance, _double dTimeDelta)
{
	m_bRandomAStar = false;

	if (m_bMakeManagerAStar == false)
		Make_ManagerAStarRoute(pGameInstance, dTimeDelta);
	

	m_dManagerAStarLimitAcc += dTimeDelta;
	list<CCell*>& listBestCell = m_pAStar->Get_ListBest();

	if (m_dManagerAStarLimitAcc >= 30.0 || listBestCell.empty() == true)
	{
		m_bManagerAStar = false;
		m_dManagerAStarLimitAcc = 0.0;
		return S_OK;
	}
	if (m_bCollision == true)
	{
		m_bCollision = false;
		m_bManagerAStar = false;
		m_dManagerAStarLimitAcc = 0.0;
		return S_OK;
	}


	_vector vGoalPos = listBestCell.front()->Get_CenterPos();
	m_pTransform->Move_Trace(dTimeDelta, vGoalPos);
	Set_State(BG_STATE_PATROL, BG_KEY_RUN);
	_float	fDistance = XMVectorGetX(XMVector4Length(vGoalPos - m_pTransform->Get_State(CTransform::STATE_POSITION)));

	if (fDistance < 1.0)
		listBestCell.pop_front();

	return S_OK;
}

HRESULT CLeeBongGoo::Make_ManagerAStarRoute(CGameInstance * pGameInstance, _double dTimeDelta)
{
	m_bMakeManagerAStar = true;

	m_pAStar->Start_AStar(m_pTransform->Get_State(CTransform::STATE_POSITION), CPositionManager::Get_Instance()->Get_PlayerPosition());

	return S_OK;
}

HRESULT CLeeBongGoo::RandomAStar(CGameInstance * pGameInstance, _double dTimeDelta)
{
	if (m_bMakeRandomAStar == false)
		Make_RandomAStarRoute(pGameInstance, dTimeDelta);

	m_dRandomAStarLimitAcc += dTimeDelta;
	list<CCell*>& listBestCell = m_pAStar->Get_ListBest();

	if (m_dRandomAStarLimitAcc >= 30.0 || listBestCell.empty() == true)
	{
		m_bRandomAStar = false;
		m_dRandomAStarLimitAcc = 0.0;
		return S_OK;
	}
	if (m_bCollision == true)
	{
		m_bCollision = false;
		m_bManagerAStar = false;
		m_dManagerAStarLimitAcc = 0.0;
		return S_OK;
	}
	_vector vGoalPos = listBestCell.front()->Get_CenterPos();
	m_pTransform->Move_Trace(dTimeDelta, vGoalPos);
	Set_State(BG_STATE_PATROL, BG_KEY_WALK);
	_float	fDistance = XMVectorGetX(XMVector4Length(vGoalPos - m_pTransform->Get_State(CTransform::STATE_POSITION)));

	if (fDistance < 1.0)
		listBestCell.pop_front();

	return S_OK;
}

HRESULT CLeeBongGoo::Make_RandomAStarRoute(CGameInstance * pGameInstance, _double dTimeDelta)
{
	m_bMakeRandomAStar = true;

	m_pAStar->Start_Random(m_pTransform->Get_State(CTransform::STATE_POSITION));
	return S_OK;
}

HRESULT CLeeBongGoo::Whistle(CGameInstance * pGameInstance, _double dTimeDelta)
{
	CSoundMgr::Get_Instance()->PlaySound(L"Whistle.wav", CSoundMgr::BG_WHISTLE);

	m_bFirstWhistle = true;
	return S_OK;
}

HRESULT CLeeBongGoo::Attack(CGameInstance * pGameInstance, _double dTimeDelta)
{
	m_bAttack = true;
	CPositionManager::Get_Instance()->Get_Player()->Hit();
	CSoundMgr::Get_Instance()->PlaySound(L"BG_Attack.wav", CSoundMgr::BG_EFFECT);
	return S_OK;
}

HRESULT CLeeBongGoo::Dead()
{
	m_bAttack = false;
	m_bManagerAStar = false;
	m_bRandomAStar = false;
	m_bView = false;
	m_bCollision = true;
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(6.9f, 0, -6.4f, 1.f));
	m_pTransform->LookAt(0.16, XMVectorSet(6.5f, 0, -5.2f, 1.f));
	CPositionManager::Get_Instance()->Get_Camera()->Set_TargetOn(CCamera_Debug::TARGET_BONGGOO);
	CPositionManager::Get_Instance()->Get_YooJiMin()->Set_DeadAnim();
	m_bAlpha = true;

	return S_OK;
}

void CLeeBongGoo::Add_ManagerTime(_double dTimeDelta)
{
	if(m_bManagerAStar == false)
		m_dManagerAStarTimeAcc += dTimeDelta;
}

void CLeeBongGoo::State_Patrol()
{
	switch (m_eKey)
	{
	case Client::CLeeBongGoo::BG_KEY_IDLE:
		m_iAnimationIndex = 0;
		break;
	case Client::CLeeBongGoo::BG_KEY_WALK:
		m_iAnimationIndex = 2;
		break;
	case Client::CLeeBongGoo::BG_KEY_RUN:
		m_iAnimationIndex = 1;
		break;
	case Client::CLeeBongGoo::BG_KEY_ATTACK:
		break;
	case Client::CLeeBongGoo::BG_KEY_END:
		break;
	default:
		break;
	}
}

void CLeeBongGoo::State_Chase()
{
	switch (m_eKey)
	{
	case Client::CLeeBongGoo::BG_KEY_IDLE:

		break;
	case Client::CLeeBongGoo::BG_KEY_WALK:
		m_iAnimationIndex = 9;
		break;
	case Client::CLeeBongGoo::BG_KEY_RUN:
		m_iAnimationIndex = 9;
		break;
	case Client::CLeeBongGoo::BG_KEY_ATTACK:
		m_iAnimationIndex = 3;
		break;
	case Client::CLeeBongGoo::BG_KEY_END:
		break;
	default:
		break;
	}
}

void CLeeBongGoo::State_Ambush()
{
	_uint i = rand() % 2;
	CSoundMgr::Get_Instance()->PlaySound(L"BG_Missing.wav", CSoundMgr::BG_EFFECT);
	if(i == 0)
		m_iAnimationIndex = 7;
	else
		m_iAnimationIndex = 8;

}

CLeeBongGoo * CLeeBongGoo::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CLeeBongGoo* pInstance = new CLeeBongGoo(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CLeeBongGoo::Clone(void * pArg)
{
	CLeeBongGoo* pInstance = new CLeeBongGoo(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);
	return pInstance;
}

void CLeeBongGoo::Free()
{
	Safe_Release(m_pAStar);

	__super::Free();
}
