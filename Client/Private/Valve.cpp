#include "stdafx.h"
#include "..\Public\Valve.h"
#include "PositionManager.h"
#include "LoadDatFiles.h"
#include "Dust.h"

CValve::CValve(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CValve::NativeConstruct(OBJ_INFO tObjInfo, VALVEPOS eValveType)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	CTransform::DESC tTransformDesc;
	tTransformDesc.dRotationPerSec = 1.2;
	tTransformDesc.dSpeedPerSec = 0.0;
	m_pTransform->Set_TransformDesc(tTransformDesc);

	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, tObjInfo.szFilePath, tObjInfo.szFileName
		, Shader_Model, DefaultTechnique);
	if (m_pModel == nullptr)
		return E_FAIL;

	CCollider::DESC tColliderDesc;
	tColliderDesc.vPivot = tObjInfo.tColliderDesc.vPivot;
	tColliderDesc.vScale = tObjInfo.tColliderDesc.vScale;
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc)))
		return E_FAIL;

	Set_InitialPosition(tObjInfo);

	m_eValveType = eValveType;
	if(eValveType == VALVE0)
		CPositionManager::Get_Instance()->Set_Valve0(this);
	else
		CPositionManager::Get_Instance()->Set_Valve1(this);

	return S_OK;
}

_int CValve::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() != LEVEL_QASMOKE)
		return OBJ_NOEVENT;

	if (m_bAction && !m_bClear)
		Interaction();

	return __super::Tick(dTimeDelta);
}

_int CValve::LateTick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Culling(m_pTransform, 3.f) && pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() == LEVEL_QASMOKE)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);

	return OBJ_NOEVENT;
}

HRESULT CValve::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CValve::Interaction()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Mouse_Pressing(CInputDevice::DIMB_LB))
	{
		m_dwMouseX += pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_X) * 0.2;
		m_dwMouseY += pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_Y) * 0.2;
		_float fRotate = (m_dwMouseX - m_dwMouseY);
		m_fRotate = fRotate - m_fRotate;

		m_pTransform->Rotation_Radian_Acc(XMConvertToRadians(m_fRotate * 0.2f), XMVectorSet(1.f, 0.f, 0.f, 0.f));
	}
	if (m_fRotate >= 180.f)
	{
		m_bClear = true;
		CPositionManager::Get_Instance()->Get_Player()->CanControl();
		CPositionManager::Get_Instance()->Get_Camera()->Set_TargetOn(CCamera_Debug::TARGET_HEEMIN);
	}
}

void CValve::Event()
{
	CPositionManager::Get_Instance()->Get_Player()->CantControl();
	CSoundMgr::Get_Instance()->PlaySound(L"Valve_Pick.wav", CSoundMgr::EFFECT);
	if (m_eValveType == VALVE0)
	{
		CPositionManager::Get_Instance()->Get_Camera()->Set_TargetOn(CCamera_Debug::TARGET_VALVE0);
		m_bAction = true;
	}
	else
	{
		CPositionManager::Get_Instance()->Get_Camera()->Set_TargetOn(CCamera_Debug::TARGET_VALVE1);
		m_bAction = true;
	}
}

void CValve::Collect_Event()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_float fDistance = 0.f;
	if (CLoadDatFiles::Get_Instance()->Get_ModelLoading())
	{
		if (m_bClear == true)
			return;

		if (m_pCollider->Collision_OBB(pGameInstance->Get_Pivot(), pGameInstance->Get_Ray(), fDistance))
		{
			CPickingEventManager::PE	tPickingEvent;
			tPickingEvent.pEventObject = this;
			tPickingEvent.fRayDistance = fDistance;
			tPickingEvent.eState = CPickingEventManager::FSM_ITEM;
			pGameInstance->Add_Event(tPickingEvent);
		}
	}
}

void CValve::On_Gas()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (m_eValveType == VALVE0)
		pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, CDust::Create(m_pGraphicDevice, m_pContextDevice, this, 0, XMVectorSet(13.55f, 2.14f, -4.58, 1.f)),L"LayerDust");
	else
		pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, CDust::Create(m_pGraphicDevice, m_pContextDevice, this, 1, XMVectorSet(14.06f, 2.14f, -4.58, 1.f)), L"LayerDust");
}

CValve * CValve::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo, VALVEPOS eValveType)
{
	CValve* pInstance = new CValve(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo, eValveType)))
		Safe_Release(pInstance);
	return pInstance;
}

void CValve::Free()
{
	__super::Free();
}
