#include "stdafx.h"
#include "..\Public\DefaultTriggerProps.h"
#include "LoadDatFiles.h"
#include "PositionManager.h"
#include "Lighter.h"
#include "TextBox.h"

CDefaultTriggerProps::CDefaultTriggerProps(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CDefaultTriggerProps::NativeConstruct(OBJ_INFO tObjInfo, TRIGGERTYPE eType)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, tObjInfo.szFilePath, tObjInfo.szFileName,
		Shader_Model, DefaultTechnique);
	if (m_pModel == nullptr)
		return E_FAIL;

	CCollider::DESC tColliderDesc;
	tColliderDesc.vPivot = tObjInfo.tColliderDesc.vPivot;
	tColliderDesc.vScale = tObjInfo.tColliderDesc.vScale;
	tColliderDesc.vScale.y *= 2.f;
	tColliderDesc.vOrientation = _float4(0.f,0.f,0.f,1.f);

	Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc);

	m_eType = eType;

	Set_InitialPosition(tObjInfo);


	return S_OK;
}

_int CDefaultTriggerProps::Tick(_double dTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() != LEVEL_QASMOKE)
		return OBJ_NOEVENT;

	return __super::Tick(dTimeDelta);
}

_int CDefaultTriggerProps::LateTick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if(pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() == LEVEL_QASMOKE)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
	return _int();
}

HRESULT CDefaultTriggerProps::Render()
{
	if (__super::Render())
		return E_FAIL;

	return S_OK;
}

void CDefaultTriggerProps::Interaction()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (m_eType == TRIGGERTYPE::TRG_LADDER)
	{
		pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, PrototypeLadder, L"LayerCarry", nullptr);
		m_bDead = true;
	}
	else if (m_eType == TRIGGERTYPE::TRG_KEY)
	{
		CSoundMgr::Get_Instance()->StopPlaySound(L"Get_Item.wav", CSoundMgr::GET_ITEM);
		CPositionManager::Get_Instance()->Get_Player()->Add_Key();
		m_bDead = true;
	}
	else if (m_eType == TRIGGERTYPE::TRG_TALISMAN)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Get_Item.wav", CSoundMgr::GET_ITEM);
		CPositionManager::Get_Instance()->Get_Player()->Add_Talisman();
		m_bDead = true;
	}
	else if (m_eType == TRIGGERTYPE::TRG_LIGHTER)
	{
		CSoundMgr::Get_Instance()->StopPlaySound(L"Get_Item.wav", CSoundMgr::GET_ITEM);
		CPositionManager::Get_Instance()->Get_Player()->Add_Lighter();
		pGameInstance->Insert_ObjectToLayer(LEVEL_STATIC, CLighter::Create(m_pGraphicDevice, m_pContextDevice), L"Lighter");
		m_bDead = true;
	}
	else if (m_eType == TRIGGERTYPE::TRG_POISON)
	{
		if (CPositionManager::Get_Instance()->Get_Player()->Get_Lighter() == true)
		{
			CSoundMgr::Get_Instance()->StopPlaySound(L"Get_Item.wav", CSoundMgr::GET_ITEM);
			CSoundMgr::Get_Instance()->PlaySound(L"JM_Sign.wav", CSoundMgr::JM_TALK);
			CPositionManager::Get_Instance()->Get_Player()->Add_Poison();
			m_bDead = true;
		}
		else if (CPositionManager::Get_Instance()->Get_Player()->Get_Lighter() == false)
		{
			CSoundMgr::Get_Instance()->StopPlaySound(L"ClickFail.wav", CSoundMgr::GET_ITEM);
			pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, CTextBox::Create(m_pGraphicDevice, m_pContextDevice, CTextBox::DARK_TEXT), L"LayerTextBox");
		}
	}
	else if (m_eType == TRIGGERTYPE::TRG_FUSEBOX)
	{
		CSoundMgr::Get_Instance()->StopPlaySound(L"Click_Default.wav", CSoundMgr::GET_ITEM);
		CPositionManager::Get_Instance()->Get_Camera()->Set_TargetOn(CCamera_Debug::TARGET_FUSEBOX);
		CPositionManager::Get_Instance()->Get_Player()->CantControl();
	}
}

void CDefaultTriggerProps::Event()
{
	m_bAction = true;

	Interaction();
}

void CDefaultTriggerProps::Collect_Event()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_float fDistance = 0.f;
	if (CLoadDatFiles::Get_Instance()->Get_ModelLoading())
	{
		if (m_pCollider->Collision_OBB(pGameInstance->Get_Pivot(), pGameInstance->Get_Ray(), fDistance))
		{
			CPickingEventManager::PE	tPickingEvent;
			tPickingEvent.pEventObject = this;
			tPickingEvent.fRayDistance = fDistance;
			if (m_eType == TRIGGERTYPE::TRG_LADDER)
				tPickingEvent.eState = CPickingEventManager::FSM_LADDER;
			else if (m_eType == TRIGGERTYPE::TRG_POISON)
				tPickingEvent.eState = CPickingEventManager::FSM_POISON;
			else if (m_eType == TRIGGERTYPE::TRG_LIGHTER)
				tPickingEvent.eState = CPickingEventManager::FSM_LIGHT;
			else if (m_eType == TRIGGERTYPE::TRG_KEY)
				tPickingEvent.eState = CPickingEventManager::FSM_KEY;
			else if (m_eType == TRIGGERTYPE::TRG_TALISMAN)
				tPickingEvent.eState = CPickingEventManager::FSM_TALISMAN;
			else if (m_eType == TRIGGERTYPE::TRG_FUSEBOX)
				tPickingEvent.eState = CPickingEventManager::FSM_LIGHT;

			pGameInstance->Add_Event(tPickingEvent);
		}
	}

}

CDefaultTriggerProps * CDefaultTriggerProps::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo, TRIGGERTYPE eType)
{
	CDefaultTriggerProps* pInstance = new CDefaultTriggerProps(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo, eType)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDefaultTriggerProps::Free()
{
	__super::Free();
}
