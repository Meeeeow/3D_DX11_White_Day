#include "stdafx.h"
#include "..\Public\WomanToiletLadder.h"
#include "LeeHeeMin.h"
#include "VentEventCollider.h"
#include "VentOutEventCollider.h"
#include "LoadDatFiles.h"

CWomanToiletLadder::CWomanToiletLadder(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

CWomanToiletLadder::CWomanToiletLadder(const CWomanToiletLadder & rhs)
	: CModelObject(rhs)
{

}

HRESULT CWomanToiletLadder::NativeConstruct()
{
	return S_OK;
}

HRESULT CWomanToiletLadder::Clone_Construct(void * pArg)
{
	if (FAILED(Set_Components()))
		return E_FAIL;
	CCollider::DESC	tColliderDesc;
	tColliderDesc.vPivot = _float3(4.8f, 1.3f, -7.4f);
	tColliderDesc.vScale = _float3(0.4f, 0.45f, 0.4f);
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_QASMOKE, L"WomanToiletLadderModel", ComVIBuffer, (CComponent**)&m_pModel)))
		return E_FAIL;

	m_bAction = false;

	return S_OK;
}

_int CWomanToiletLadder::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() != LEVEL_QASMOKE)
		return OBJ_NOEVENT;
	return __super::Tick(dTimeDelta);
}

_int CWomanToiletLadder::LateTick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Culling(m_pModel->Get_OnePoint(), m_pTransform->Get_WorldXM(), 3.f) && pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() == LEVEL_QASMOKE)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);

	return _int();
}

HRESULT CWomanToiletLadder::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CWomanToiletLadder::Interaction()
{

}

void CWomanToiletLadder::Event()
{
	m_bAction = true;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, CVentEventCollider::Create(m_pGraphicDevice, m_pContextDevice), L"ColliderEvent");
	CLeeHeeMin* pPlayer = dynamic_cast<CLeeHeeMin*>(pGameInstance->Get_GameObject(LEVEL_STATIC, LayerPlayer, 0));
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pPlayer->Get_Component(ComTransform));
	_vector pEventTransform = XMVectorSet(5.f, 0.6f, -7.795f, 1.f);
	pPlayerTransform->Set_State(CTransform::STATE_POSITION, pEventTransform);
	CSoundMgr::Get_Instance()->PlaySound(L"Ladder_Put.wav", CSoundMgr::LADDER_CREATE);
}

void CWomanToiletLadder::Collect_Event()
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
			tDesc.eState = CPickingEventManager::FSM_LADDER;
			pGameInstance->Add_Event(tDesc);
		}
	}
}

CWomanToiletLadder * CWomanToiletLadder::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CWomanToiletLadder* pInstance = new CWomanToiletLadder(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CWomanToiletLadder::Clone(void * pArg)
{
	CWomanToiletLadder* pInstance = new CWomanToiletLadder(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CWomanToiletLadder::Free()
{
	__super::Free();
}
