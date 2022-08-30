#include "stdafx.h"
#include "..\Public\PuzzleSecond.h"
#include "PositionManager.h"
#include "LoadDatFiles.h"

CPuzzleSecond::CPuzzleSecond(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{
}

HRESULT CPuzzleSecond::NativeConstruct(OBJ_INFO tObjInfo)
{
	if (FAILED(Set_Components()))
		return E_FAIL;
	CTransform::DESC tTransformDesc;
	tTransformDesc.dRotationPerSec = 0.0;
	tTransformDesc.dSpeedPerSec = 0.015;
	m_pTransform->Set_TransformDesc(tTransformDesc);

	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, tObjInfo.szFilePath, tObjInfo.szFileName,
		Shader_Model, DefaultTechnique);
	if (m_pModel == nullptr)
		return E_FAIL;

	CCollider::DESC tColliderDesc;
	tColliderDesc.vPivot = tObjInfo.tColliderDesc.vPivot;
	tColliderDesc.vScale = tObjInfo.tColliderDesc.vScale;

	Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc);
	Set_InitialPosition(tObjInfo);

	XMStoreFloat4(&m_vInitialPos, tObjInfo.Get_State(CTransform::STATE_POSITION));

	return S_OK;
}

_int CPuzzleSecond::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() != LEVEL_QASMOKE)
		return OBJ_NOEVENT;

	_float fDistance = 0.f;
	if (CLoadDatFiles::Get_Instance()->Get_ModelLoading())
	{
		if (CPositionManager::Get_Instance()->Get_Camera()->Get_TargetOn(CCamera_Debug::TARGET_FUSEBOX) == false)
			return 0;

		if (m_pCollider->Collision_OBB(pGameInstance->Get_Pivot(), pGameInstance->Get_Ray(), fDistance))
		{
			if (pGameInstance->Mouse_Pressing(CInputDevice::DIMB_LB) && m_bLocked == false)
			{
				_double dwMouseX = 0.0, dwMouseY = 0.0;
				dwMouseX = pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_X);
				dwMouseY = pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_Y);

				m_pTransform->Move_MouseX(dwMouseX * 0.25);
				m_pTransform->Move_MouseY(dwMouseY * 0.25);

				CPositionManager::Get_Instance()->Get_FuseBody()->Check_TrueFuse(this);
				CPositionManager::Get_Instance()->Get_FuseBody()->Check_FalseFuse(this);
			}
			if (pGameInstance->Mouse_Down(CInputDevice::DIMB_RB) && m_bLocked == false)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Fuse_Out.wav", CSoundMgr::FUSE_OUT);
				m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_vInitialPos));
			}
		}
	}


	return __super::Tick(dTimeDelta);
}

_int CPuzzleSecond::LateTick(_double dTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
	return _int();
}

HRESULT CPuzzleSecond::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CPuzzleSecond::Interaction()
{
}

void CPuzzleSecond::Event()
{
}

void CPuzzleSecond::Collect_Event()
{
	
}

void CPuzzleSecond::Correct_Position(_vector vPosition)
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
}

CPuzzleSecond * CPuzzleSecond::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo)
{
	CPuzzleSecond* pInstance = new CPuzzleSecond(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPuzzleSecond::Free()
{
	__super::Free();
}
