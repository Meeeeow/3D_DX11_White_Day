#include "stdafx.h"
#include "..\Public\YooJiMin.h"
#include "PositionManager.h"
#include "LoadDatFiles.h"
#include "TextBox.h"

CYooJiMin::CYooJiMin(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice,pContextDevice)
{
}

CYooJiMin::CYooJiMin(const CYooJiMin & rhs)
	: CModelObject(rhs)
{
}


HRESULT CYooJiMin::NativeConstruct()
{

	return S_OK;
}

HRESULT CYooJiMin::Clone_Construct(void * pArg)
{
	if (FAILED(Set_Components()))
		return E_FAIL;
	m_iTalkCount = 0;
	
	m_pModel->Set_Animation(3);
	LIGHTDESC tLightDesc;
	tLightDesc.eLightType = LIGHTDESC::LIGHT_POINT;
	tLightDesc.vPos = _float4(6.9f, 1.f, -6.3f, 1.f);
	tLightDesc.vDiffuse = _float4(0.55f, 0.55f, 0.55f, 1.f);
	tLightDesc.vAmbient = _float4(0.55f, 0.55f, 0.55f, 1.f);
	tLightDesc.vSpecular = _float4(0.55f, 0.55f, 0.55f, 1.f);
	tLightDesc.fRange = 1.f;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice, L"Light", &tLightDesc);
	CPositionManager::Get_Instance()->Set_YooJiMin(this);
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(6.9f, 0.f, -6.4f, 1.f));
	m_iRenderIndex = 1;
	return S_OK;
}

_int CYooJiMin::Tick(_double dTimeDelta)
{
	if (0 > __super::Tick(dTimeDelta))
		return -1;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Update(pGameInstance, dTimeDelta);

	return _int();
}

_int CYooJiMin::LateTick(_double dTimeDelta)
{
	m_pModel->Update_CombinedTransformationMatrix(dTimeDelta);

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);

	return OBJ_NOEVENT;
}

HRESULT CYooJiMin::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CYooJiMin::Set_Components()
{
	if (FAILED(__super::Set_Components()))
		return E_FAIL;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform::DESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(tTransformDesc));
	tTransformDesc.dRotationPerSec = 1.f;
	tTransformDesc.dSpeedPerSec = 1.f;

	m_pTransform->Set_TransformDesc(tTransformDesc);

	CCollider::DESC tColliderDesc;
	ZeroMemory(&tColliderDesc, sizeof(tColliderDesc));
	tColliderDesc.vPivot = _float3(0.f, 0.6f, 0.f);
	tColliderDesc.vScale = _float3(0.35f, 0.6f, 0.35f);
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Com_YooJiMin"), ComVIBuffer, (CComponent**)&m_pModel)))
		return E_FAIL;

	return S_OK;
}

void CYooJiMin::Interaction()
{

}

void CYooJiMin::Event()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, CTextBox::Create(m_pGraphicDevice, m_pContextDevice, CTextBox::JM_TEXT), L"LayerTextBox");
	
	if (m_iTalkCount == 1 && CPositionManager::Get_Instance()->Get_Player()->Get_Sit() == false)
		CSoundMgr::Get_Instance()->StopPlaySound(L"JM_WhatsUp.wav", CSoundMgr::JM_TALK);
	else if (m_iTalkCount == 2 && CPositionManager::Get_Instance()->Get_Player()->Get_Sit() == false)
		CSoundMgr::Get_Instance()->StopPlaySound(L"JM_InterestedMe.wav", CSoundMgr::JM_TALK);
	else if (m_iTalkCount >= 3 && CPositionManager::Get_Instance()->Get_Player()->Get_Sit() == false)
		CSoundMgr::Get_Instance()->StopPlaySound(L"JM_NoGrilAtHome.wav", CSoundMgr::JM_TALK);
	else if (m_iTalkCount >= 1 && CPositionManager::Get_Instance()->Get_Player()->Get_Sit() == true)
		CSoundMgr::Get_Instance()->StopPlaySound(L"JM_DoyouLikeLeg.wav", CSoundMgr::JM_TALK);

	m_iTalkCount++;
}

void CYooJiMin::Collect_Event()
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
			tPickingEvent.eState = CPickingEventManager::FSM_ITEM;
			pGameInstance->Add_Event(tPickingEvent);
		}
	}
}

HRESULT CYooJiMin::Update(CGameInstance * pGameInstance, _double dTimeDelta)
{
	m_pTransform->LookAt(dTimeDelta, CPositionManager::Get_Instance()->Get_PlayerPosition());

	return S_OK;
}

CYooJiMin * CYooJiMin::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CYooJiMin* pInstance = new CYooJiMin(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CYooJiMin::Clone(void * pArg)
{
	CYooJiMin* pInstance = new CYooJiMin(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CYooJiMin::Free()
{
	__super::Free();
}
