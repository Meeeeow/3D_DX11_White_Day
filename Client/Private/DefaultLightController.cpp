#include "stdafx.h"
#include "..\Public\DefaultLightController.h"
#include "LoadDatFiles.h"
#include "TextBox.h"

CDefaultLightController::CDefaultLightController(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CDefaultLightController::NativeConstruct(OBJ_INFO tObjInfo, const wchar_t * pLightTag)
{
	if (FAILED(Set_Components()))
		return E_FAIL;
	CTransform::DESC tTransformDesc;
	tTransformDesc.dSpeedPerSec = 0.11f;
	tTransformDesc.dRotationPerSec = 0.15f;

	m_pLightTag = pLightTag;
	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, tObjInfo.szFilePath, tObjInfo.szFileName, Shader_Model, DefaultTechnique);
	Set_InitialPosition(tObjInfo);
	m_fCullingRadius = tObjInfo.fCullRadius;

	CCollider::DESC tColliderDesc;
	if (tObjInfo.eBufferType != OBJ_INFO::COLLIDER_END)
	{
		tColliderDesc.fRadius = tObjInfo.tColliderDesc.fRadius;
		tColliderDesc.vPivot = tObjInfo.tColliderDesc.vPivot;
		tColliderDesc.vScale = tObjInfo.tColliderDesc.vScale;
		tColliderDesc.vOrientation = tObjInfo.tColliderDesc.vOrientation;
		Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc);
	}
	m_bRender = false;

	return S_OK;
}

_int CDefaultLightController::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() != LEVEL_QASMOKE)
		return OBJ_NOEVENT;

	Compute_Distance();

	if (m_bAction)
	{
		m_fCoolDown += dTimeDelta;
		if (m_fCoolDown >= 5.f)
		{
			m_bAction = false;
		}
	}
	return __super::Tick(dTimeDelta);
}

_int CDefaultLightController::LateTick(_double dTimeDelta)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_bRender = false;
	if (pGameInstance->Culling(m_pModel->Get_OnePoint(), m_pTransform->Get_WorldXM(), m_fCullingRadius) && pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() == LEVEL_QASMOKE)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
		m_bRender = true;
	}

	return _int();
}

HRESULT CDefaultLightController::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CDefaultLightController::Interaction()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (!m_bReverse)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"LightSwitch_On.wav", CSoundMgr::EFFECT);
		pGameInstance->Light_Action(m_pLightTag);
	}
	else if (m_bReverse)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"LightSwitch_Off.wav", CSoundMgr::EFFECT);
		pGameInstance->Light_DeActive(m_pLightTag);
	}

	m_bReverse = !m_bReverse;
}

void CDefaultLightController::Event()
{
	if (!lstrcmp(m_pLightTag, L"Light_PhysicalRoom"))
	{
		CSoundMgr::Get_Instance()->PlaySound(L"LightSwitch_On.wav", CSoundMgr::EFFECT);
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, CTextBox::Create(m_pGraphicDevice, m_pContextDevice, CTextBox::CONTROL_TEXT), L"LayerTextBox");
		return;
	}

	if (m_bAction)
		return;

	m_bAction = true;
	


	Interaction();
}

void CDefaultLightController::Collect_Event()
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
			tDesc.eState = CPickingEventManager::FSM_LIGHT;
			pGameInstance->Add_Event(tDesc);
		}
	}
}

CDefaultLightController * CDefaultLightController::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo, const wchar_t * pLightTag)
{
	CDefaultLightController* pInstance = new CDefaultLightController(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo, pLightTag)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDefaultLightController::Free()
{
	__super::Free();
}

void CDefaultLightController::Compute_Distance()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_vector vDistance;
	vDistance = XMVector4Length(pGameInstance->Get_CameraPosition() - m_pModel->Get_OnePoint());
	m_fCameraDistance = XMVectorGetX(vDistance);
}
