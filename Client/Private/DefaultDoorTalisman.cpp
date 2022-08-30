#include "stdafx.h"
#include "..\Public\DefaultDoorTalisman.h"
#include "LoadDatFiles.h"
#include "PositionManager.h"

CDefaultDoorTalisman::CDefaultDoorTalisman(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CDefaultDoorTalisman::NativeConstruct(OBJ_INFO tObjInfo, _float fRotate)
{
	if (FAILED(Set_Components()))
		return E_FAIL;
	CTransform::DESC tTransformDesc;
	tTransformDesc.dSpeedPerSec = 0.11f;
	tTransformDesc.dRotationPerSec = 0.15f;


	m_fRotate = fRotate;
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

_int CDefaultDoorTalisman::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() != LEVEL_QASMOKE)
		return OBJ_NOEVENT;

	Compute_Distance();
	Interaction();

	return __super::Tick(dTimeDelta);
}

_int CDefaultDoorTalisman::LateTick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Culling(m_pTransform, 5.f) && pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() == LEVEL_QASMOKE)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
	return _int();
}

HRESULT CDefaultDoorTalisman::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CDefaultDoorTalisman::Interaction()
{
	if (CLoadDatFiles::Get_Instance()->Get_ModelLoading())
	{
		if (CPositionManager::Get_Instance()->Get_Player()->Get_UseTalisman() == true)
		{
			if (m_fRotate > 0.f)
				DoorRotationPlus();
			else
				DoorRotationMinus();
		}
	}
}

void CDefaultDoorTalisman::Event()
{
	if (CPositionManager::Get_Instance()->Get_Player()->Get_UseTalisman() == false)
	{
		return;
	}

	if (m_bAction)
		return;

	m_bAction = true;
}

void CDefaultDoorTalisman::Collect_Event()
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
			tDesc.eState = CPickingEventManager::FSM_TALISMAN;
			pGameInstance->Add_Event(tDesc);
		}
	}
}

void CDefaultDoorTalisman::Collision()
{
	if (CLoadDatFiles::Get_Instance()->Get_ModelLoading())
	{
		if (m_bAction == false)
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
					Event();
				}
				else
				{
					CPositionManager::Get_Instance()->Get_LeeBongGooTransform()->Add_Position(
						XMVectorSet(0.f, 0.f, XMVectorGetZ(vPos), 0.f), pGameInstance->Get_CurrentNav());
					Event();
				}
			}
		}
	}
}

void CDefaultDoorTalisman::DoorRotationPlus()
{
	if (m_iAcc != 90)
	{
		m_pTransform->Rotation_Radian_Acc(XMConvertToRadians(1));
		m_iAcc += 1;
		return;
	}
}

void CDefaultDoorTalisman::DoorRotationMinus()
{
	if (m_iAcc != -90)
	{
		m_pTransform->Rotation_Radian_Acc(XMConvertToRadians(-1));
		m_iAcc -= 1;
		return;
	}
}

CDefaultDoorTalisman * CDefaultDoorTalisman::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo, _float fRotate)
{
	CDefaultDoorTalisman* pInstance = new CDefaultDoorTalisman(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo, fRotate)))
		Safe_Release(pInstance);
	return pInstance;
}

void CDefaultDoorTalisman::Free()
{
	__super::Free();
}

void CDefaultDoorTalisman::Compute_Distance()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_vector vDistance;
	vDistance = XMVector4Length(pGameInstance->Get_CameraPosition() - m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_fCameraDistance = XMVectorGetX(vDistance);
}
