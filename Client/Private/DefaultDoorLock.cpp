#include "stdafx.h"
#include "..\Public\DefaultDoorLock.h"
#include "LoadDatFiles.h"
#include "PositionManager.h"
#include "TextBox.h"

CDefaultDoorLock::CDefaultDoorLock(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CDefaultDoorLock::NativeConstruct(OBJ_INFO tObjInfo, _float fMove, _bool bX)
{
	if (FAILED(Set_Components()))
		return E_FAIL;
	CTransform::DESC tTransformDesc;
	tTransformDesc.dSpeedPerSec = 0.11f;
	tTransformDesc.dRotationPerSec = 0.15f;


	m_fMove = fMove;
	m_bX = bX;
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

_int CDefaultDoorLock::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() != LEVEL_QASMOKE)
		return OBJ_NOEVENT;

	Compute_Distance();
	Interaction();
	Collision();
	return __super::Tick(dTimeDelta);
}

_int CDefaultDoorLock::LateTick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_bRender = false;
	if (pGameInstance->Culling(m_pTransform, m_fCullingRadius) && m_fCameraDistance < 31.f  && pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() == LEVEL_QASMOKE)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
		m_bRender = true;
	}

	return _int();
}

HRESULT CDefaultDoorLock::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CDefaultDoorLock::Interaction()
{
	if (m_bAction)
	{
		if (!m_bReverse)
			CSoundMgr::Get_Instance()->PlaySound(L"SlideWood_Open.wav", CSoundMgr::SLIDEDOOR_ON);
		else if (m_bReverse)
			CSoundMgr::Get_Instance()->PlaySound(L"SlideWood_Close.wav", CSoundMgr::SLIDEDOOR_OFF);

		if (m_fMove != 0 && m_fRotate == 0)
		{
			if (m_fMove > 0.f)
			{
				if (m_bX)
					DoorRightX();
				else
					DoorLeftZ();
			}
			else
			{
				if (m_bX)
					DoorLeftX();
				else
					DoorRightZ();
			}
		}
	}

}

void CDefaultDoorLock::Event()
{
	if (CPositionManager::Get_Instance()->Get_Player()->Get_Key() == false)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, CTextBox::Create(m_pGraphicDevice, m_pContextDevice, CTextBox::DOOR_TEXT), L"LayerTextBox");
		return;
	}

	if (m_bAction)
		return;

	m_bAction = true;
}

void CDefaultDoorLock::Collect_Event()
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
			tDesc.eState = CPickingEventManager::FSM_KEY;
			pGameInstance->Add_Event(tDesc);
		}
	}
}

void CDefaultDoorLock::Collision()
{
	if (CLoadDatFiles::Get_Instance()->Get_ModelLoading())
	{
		if (m_bAction == false && m_bReverse == true)
			return;
		if (m_bAction == true && m_bReverse == true)
			return;
		if (m_bAction == true && m_bReverse == false)
			return;

		if (m_pCollider->Collision_OBBtoOBB(CPositionManager::Get_Instance()->Get_PlayerCollider()->Get_OBB()))
		{
			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			_vector vPos = m_pCollider->ResultCollision_OBBtoOBB(CPositionManager::Get_Instance()->Get_PlayerCollider()->Get_OBB());

			if (fabs(XMVectorGetX(vPos)) < fabs(XMVectorGetZ(vPos)))
			{
				CPositionManager::Get_Instance()->Get_PlayerTransform()->Add_Position(
					XMVectorSet(XMVectorGetX(vPos), 0.f, 0.f, 0.f), pGameInstance->Get_CurrentNav());
			}
			else
			{
				CPositionManager::Get_Instance()->Get_PlayerTransform()->Add_Position(
					XMVectorSet(0.f, 0.f, XMVectorGetZ(vPos), 0.f), pGameInstance->Get_CurrentNav());
			}
		}
		if (CPositionManager::Get_Instance()->LeeBongGooAlive() == false)
		{
			if (m_pCollider->Collision_OBBtoOBB(CPositionManager::Get_Instance()->Get_LeeBongGooCollider()->Get_OBB()))
			{
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				_vector vPos = m_pCollider->ResultCollision_OBBtoOBB(CPositionManager::Get_Instance()->Get_LeeBongGooCollider()->Get_OBB());

				if (fabs(XMVectorGetX(vPos)) < fabs(XMVectorGetZ(vPos)))
				{
					CPositionManager::Get_Instance()->Get_LeeBongGooTransform()->Add_Position(
						XMVectorSet(XMVectorGetX(vPos), 0.f, 0.f, 0.f), pGameInstance->Get_CurrentNav());
					CPositionManager::Get_Instance()->Get_LeeBongGoo()->You_Collision();
				}
				else
				{
					CPositionManager::Get_Instance()->Get_LeeBongGooTransform()->Add_Position(
						XMVectorSet(0.f, 0.f, XMVectorGetZ(vPos), 0.f), pGameInstance->Get_CurrentNav());
					CPositionManager::Get_Instance()->Get_LeeBongGoo()->You_Collision();
				}
			}
		}
	}
}


void CDefaultDoorLock::DoorLeftX()
{
	if (m_iAcc != -66 && !m_bReverse)
	{
		m_pTransform->Add_Position(XMVectorSet(-0.01f, 0.f, 0.f, 0));
		m_iAcc -= 1;
		return;
	}
	else if (m_iAcc != 0 && m_bReverse)
	{
		m_pTransform->Add_Position(XMVectorSet(0.01f, 0.f, 0.f, 0));
		m_iAcc += 1;
		return;
	}

	if (m_bAction)
	{
		m_bAction = false;
		m_bReverse = !m_bReverse;
	}
}

void CDefaultDoorLock::DoorRightX()
{
	if (m_iAcc != 66 && !m_bReverse)
	{
		m_pTransform->Add_Position(XMVectorSet(0.01f, 0.f, 0.f, 0));
		m_iAcc += 1;
		return;
	}
	else if (m_iAcc != 0 && m_bReverse)
	{
		m_pTransform->Add_Position(XMVectorSet(-0.01f, 0.f, 0.f, 0));
		m_iAcc -= 1;
		return;
	}

	if (m_bAction)
	{
		m_bAction = false;
		m_bReverse = !m_bReverse;
	}
}

void CDefaultDoorLock::DoorLeftZ()
{
	if (m_iAcc != 66 && !m_bReverse)
	{
		m_pTransform->Add_Position(XMVectorSet(0.f, 0.f, 0.01f, 0));
		m_iAcc += 1;
		return;
	}
	else if (m_iAcc != 0 && m_bReverse)
	{
		m_pTransform->Add_Position(XMVectorSet(0.f, 0.f, -0.01f, 0));
		m_iAcc -= 1;
		return;
	}

	if (m_bAction)
	{
		m_bAction = false;
		m_bReverse = !m_bReverse;
	}
}

void CDefaultDoorLock::DoorRightZ()
{
	if (m_iAcc != -66 && !m_bReverse)
	{
		m_pTransform->Add_Position(XMVectorSet(0.f, 0.f, -0.01f, 0));
		m_iAcc -= 1;
		return;
	}
	else if (m_iAcc != 0 && m_bReverse)
	{
		m_pTransform->Add_Position(XMVectorSet(0.f, 0.f, 0.01f, 0));
		m_iAcc += 1;
		return;
	}

	if (m_bAction)
	{
		m_bAction = false;
		m_bReverse = !m_bReverse;
	}
}

CDefaultDoorLock * CDefaultDoorLock::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo, _float fMove, _bool bX)
{
	CDefaultDoorLock* pInstance = new CDefaultDoorLock(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo, fMove, bX)))
		Safe_Release(pInstance);
	return pInstance;
}

void CDefaultDoorLock::Free()
{
	__super::Free();
}

void CDefaultDoorLock::Compute_Distance()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_vector vDistance;
	vDistance = XMVector4Length(pGameInstance->Get_CameraPosition() - m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_fCameraDistance = XMVectorGetX(vDistance);
}
