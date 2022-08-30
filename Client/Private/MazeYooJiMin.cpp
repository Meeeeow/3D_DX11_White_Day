#include "stdafx.h"
#include "..\Public\MazeYooJiMin.h"
#include "LoadDatFiles.h"
#include "PositionManager.h"
#include "LeeHeeMin.h"
#include "BlackScrren.h"

CMazeYooJiMin::CMazeYooJiMin(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{
}

HRESULT CMazeYooJiMin::NativeConstruct()
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	CCollider::DESC tColliderDesc;
	tColliderDesc.vPivot = _float3(0.f, 0.6f, 0.f);
	tColliderDesc.vScale = _float3(0.35f, 1.2f, 0.35f);
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Com_YooJiMin"), ComVIBuffer, (CComponent**)&m_pModel)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc)))
		return E_FAIL;

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, 6.f, 1.f, 1.f));
	CTransform::DESC tTransformDesc;
	tTransformDesc.dRotationPerSec = 2.0;
	tTransformDesc.dSpeedPerSec = 0.6;
	m_pTransform->Set_TransformDesc(tTransformDesc);

	m_iRenderIndex = 1;

	m_pModel->Set_Animation(2);

	m_pTransform->Rotation_Radian(XMConvertToRadians(90.f));

	return S_OK;
}

_int CMazeYooJiMin::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_pCollider->Update(m_pTransform->Get_WorldXM());
	if (m_bAction)
	{
		Interaction(dTimeDelta);
		if (m_bDontAcc == false)
		{
			m_dTimeAcc += dTimeDelta;
			if (m_dTimeAcc >= 4.0)
			{
				CSoundMgr::Get_Instance()->PlayBGM(L"JM_End.wav");
				m_bDontAcc = true;
			}
		}

	}
	if (m_bDontAcc == true)
	{
		m_dEndTimeAcc += dTimeDelta;
		if (m_dEndTimeAcc >= 10.0 && m_bScreen == false)
		{
			m_bScreen = true;
			pGameInstance->Insert_ObjectToLayer(LEVEL_MAZE, CBlackScrren::Create(m_pGraphicDevice, m_pContextDevice), L"LayerCredit");
		}
	}

	return _int();
}

_int CMazeYooJiMin::LateTick(_double dTimeDelta)
{
	m_pModel->Update_CombinedTransformationMatrix(dTimeDelta);
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
	return _int();
}

HRESULT CMazeYooJiMin::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CMazeYooJiMin::Interaction(_double dTimeDelta)
{
	CLeeHeeMin* pInstance = CPositionManager::Get_Instance()->Get_Player();
	CTransform* pPlayerTransform = CPositionManager::Get_Instance()->Get_PlayerTransform();
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_vector		vLook = pPlayerTransform->Get_State(CTransform::STATE_LOOK);
	_vector		vPosition = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_double		dwMouseX = 0.0;
	dwMouseX = pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_X);

	m_pTransform->Rotation_Axis(dTimeDelta * dwMouseX * 0.2f);
	vPosition = XMVectorSetZ(vPosition, XMVectorGetZ(vPosition) + 0.25f);
	_float fHeight = XMVectorGetY(vPosition) + 0.75f;
	vPosition = XMVectorSetY(vPosition, fHeight);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
}


void CMazeYooJiMin::Event()
{
	if (m_bAction)
		return;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_bAction = true;
	CSoundMgr::Get_Instance()->PlaySound(L"JM_Thanks.wav", CSoundMgr::JM_TALK);

}
void CMazeYooJiMin::Collect_Event()
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

CMazeYooJiMin * CMazeYooJiMin::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CMazeYooJiMin* pInstance = new CMazeYooJiMin(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);
	return pInstance;
}

void CMazeYooJiMin::Free()
{
	__super::Free();
}
