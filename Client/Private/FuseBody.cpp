#include "stdafx.h"
#include "..\Public\FuseBody.h"
#include "PuzzleFirst.h"
#include "PuzzleSecond.h"
#include "PuzzleThird.h"
#include "PositionManager.h"

CFuseBody::CFuseBody(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CFuseBody::NativeConstruct(OBJ_INFO tObjInfo)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, tObjInfo.szFilePath, tObjInfo.szFileName,
		Shader_Model, DefaultTechnique);
	if (m_pModel == nullptr)
		return E_FAIL;

	CCollider::DESC tColliderDesc;
	ZeroMemory(&tColliderDesc, sizeof(tColliderDesc));
	tColliderDesc.vPivot = _float3(21.83f, 1.43f, -4.02f);
	tColliderDesc.vScale = _float3(0.05f, 0.025f, 0.025f);
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);
	Add_Component(LEVEL_STATIC, PrototypeOBB, L"Fuse0", (CComponent**)&m_pOnFuse[FUSE_FIRST], &tColliderDesc);

	tColliderDesc.vPivot = _float3(21.83f, 1.4f, -3.8f);
	Add_Component(LEVEL_STATIC, PrototypeOBB, L"Fuse1", (CComponent**)&m_pOnFuse[FUSE_SECOND], &tColliderDesc);

	tColliderDesc.vPivot = _float3(21.83f, 1.37f, -3.58f);
	Add_Component(LEVEL_STATIC, PrototypeOBB, L"Fuse2", (CComponent**)&m_pOnFuse[FUSE_THIRD], &tColliderDesc);

	Set_InitialPosition(tObjInfo);
	
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	LIGHTDESC tLightDesc;
	tLightDesc.eLightType = LIGHTDESC::LIGHT_POINT;
	tLightDesc.vPos = _float4(20.83f, 1.4f, -5.8f, 1.f);

	tLightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vAmbient = _float4(0.35f, 0.35f, 0.35f, 1.f);
	tLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.fRange = 5.5f;
	pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice, L"Light", &tLightDesc);

	tLightDesc.vPos = _float4(21.8f, 1.6f, -6.115f, 1.f);
	tLightDesc.vDiffuse = _float4(0.65f, 0.15f, 0.15f, 1.f);
	tLightDesc.fRange = 0.5f;
	pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice, L"FuseFirst", &tLightDesc);

	tLightDesc.vPos = _float4(21.8f, 1.18f, -5.6f, 1.f);
	tLightDesc.vDiffuse = _float4(0.65f, 0.15f, 0.15f, 1.f);
	tLightDesc.fRange = 0.5f;
	pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice, L"FuseSecond", &tLightDesc);

	CPositionManager::Get_Instance()->Set_FuseBox(this);

	m_bDead = false;
	m_bTrueFuse[FUSE_FIRST] = false;
	m_bTrueFuse[FUSE_SECOND] = false;
	m_bTrueFuse[FUSE_THIRD] = false;
	return S_OK;
}

_int CFuseBody::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (m_bDead)
	{
		pGameInstance->Light_DeActive(L"FuseFirst");
		pGameInstance->Light_DeActive(L"FuseSecond");
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SIREN_LOOP);
		CSoundMgr::Get_Instance()->PlaySound(L"Siren_End.wav", CSoundMgr::SIREN_END);

		CPositionManager::Get_Instance()->Get_Camera()->Set_TargetOn(CCamera_Debug::TARGET_HEEMIN);
		CPositionManager::Get_Instance()->Set_FuseClear();
		CPositionManager::Get_Instance()->Get_Player()->CanControl();
		return OBJ_DEAD;
	}
	for (_uint i = 0; i < FUSE_END; ++i)
		m_pOnFuse[i]->Update(m_pTransform->Get_WorldXM());


	
	if (m_bTrueFuse[FUSE_FIRST])
		pGameInstance->Light_Action(L"FuseFirst");
	if (m_bTrueFuse[FUSE_FIRST] && m_bTrueFuse[FUSE_SECOND] && m_bTrueFuse[FUSE_THIRD])
		pGameInstance->Light_Action(L"FuseSecond");

	if (m_bTrueFuse[FUSE_FIRST] && m_bTrueFuse[FUSE_SECOND] && m_bTrueFuse[FUSE_THIRD])
		TimeAccumulation(dTimeDelta);

	return OBJ_NOEVENT;
}

_int CFuseBody::LateTick(_double dTimeDelta)
{

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
	return _int();
}

HRESULT CFuseBody::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CFuseBody::Check_TrueFuse(CGameObject * pGameObject)
{
	CPuzzleFirst* pFirst = dynamic_cast<CPuzzleFirst*>(pGameObject);
	CPuzzleSecond* pSecond = dynamic_cast<CPuzzleSecond*>(pGameObject);
	CPuzzleThird* pThird = dynamic_cast<CPuzzleThird*>(pGameObject);
	if (pFirst != nullptr)
	{
		CCollider* pFirstCollider = dynamic_cast<CCollider*>(pFirst->Get_Component(ComCollider));
		if (m_pOnFuse[FUSE_FIRST]->Collision_OBBtoOBB(pFirstCollider->Get_OBB()))
		{
			_float3 vCenter = m_pOnFuse[FUSE_FIRST]->Get_OBB()->Center;
			_vector vPosition = XMVectorSet(vCenter.x, vCenter.y, vCenter.z, 1.f);
			
			pFirst->Set_Locked();
			pFirst->Correct_Position(vPosition);
			m_bTrueFuse[FUSE_FIRST] = true;
			CSoundMgr::Get_Instance()->PlaySound(L"Fuse_In.wav", CSoundMgr::FUSE_IN);
		}
	}
	else if (pSecond != nullptr)
	{
		CCollider* pSecondCollider = dynamic_cast<CCollider*>(pSecond->Get_Component(ComCollider));
		if (m_pOnFuse[FUSE_SECOND]->Collision_OBBtoOBB(pSecondCollider->Get_OBB()))
		{
			_float3 vCenter = m_pOnFuse[FUSE_SECOND]->Get_OBB()->Center;
			_vector vPosition = XMVectorSet(vCenter.x, vCenter.y, vCenter.z, 1.f);

			pSecond->Set_Locked();
			pSecond->Correct_Position(vPosition);
			m_bTrueFuse[FUSE_SECOND] = true;
			CSoundMgr::Get_Instance()->PlaySound(L"Fuse_In.wav", CSoundMgr::FUSE_IN);
		}
	}
	else if (pThird != nullptr)
	{
		CCollider* pThirdCollider = dynamic_cast<CCollider*>(pThird->Get_Component(ComCollider));
		if (m_pOnFuse[FUSE_THIRD]->Collision_OBBtoOBB(pThirdCollider->Get_OBB()))
		{
			_float3 vCenter = m_pOnFuse[FUSE_THIRD]->Get_OBB()->Center;
			_vector vPosition = XMVectorSet(vCenter.x, vCenter.y, vCenter.z, 1.f);

			pThird->Set_Locked();
			pThird->Correct_Position(vPosition);
			m_bTrueFuse[FUSE_THIRD] = true;
			CSoundMgr::Get_Instance()->PlaySound(L"Fuse_In.wav", CSoundMgr::FUSE_IN);
		}
	}

}

void CFuseBody::Check_FalseFuse(CGameObject * pGameObject)
{
	CPuzzleFirst* pFirst = dynamic_cast<CPuzzleFirst*>(pGameObject);
	CPuzzleSecond* pSecond = dynamic_cast<CPuzzleSecond*>(pGameObject);
	CPuzzleThird* pThird = dynamic_cast<CPuzzleThird*>(pGameObject);

	if (pFirst != nullptr)
	{
		CCollider* pFirstCollider = dynamic_cast<CCollider*>(pFirst->Get_Component(ComCollider));
		if (m_bTrueFuse[FUSE_SECOND] == false)
		{
			if (m_pOnFuse[FUSE_SECOND]->Collision_OBBtoOBB(pFirstCollider->Get_OBB()))
			{
				_float3 vCenter = m_pOnFuse[FUSE_SECOND]->Get_OBB()->Center;
				_vector vPosition = XMVectorSet(vCenter.x, vCenter.y, vCenter.z, 1.f);
				pFirst->Correct_Position(vPosition);
				CSoundMgr::Get_Instance()->PlaySound(L"Fuse_In.wav", CSoundMgr::FUSE_IN);
			}
		}
		if (m_bTrueFuse[FUSE_THIRD] == false)
		{
			if (m_pOnFuse[FUSE_THIRD]->Collision_OBBtoOBB(pFirstCollider->Get_OBB()))
			{
				_float3 vCenter = m_pOnFuse[FUSE_THIRD]->Get_OBB()->Center;
				_vector vPosition = XMVectorSet(vCenter.x, vCenter.y, vCenter.z, 1.f);
				pFirst->Correct_Position(vPosition);
				CSoundMgr::Get_Instance()->PlaySound(L"Fuse_In.wav", CSoundMgr::FUSE_IN);
			}
		}
	}
	if (pSecond != nullptr)
	{
		CCollider* pSecondCollider = dynamic_cast<CCollider*>(pSecond->Get_Component(ComCollider));
		if (m_bTrueFuse[FUSE_FIRST] == false)
		{
			if (m_pOnFuse[FUSE_FIRST]->Collision_OBBtoOBB(pSecondCollider->Get_OBB()))
			{
				_float3 vCenter = m_pOnFuse[FUSE_FIRST]->Get_OBB()->Center;
				_vector vPosition = XMVectorSet(vCenter.x, vCenter.y, vCenter.z, 1.f);
				pSecond->Correct_Position(vPosition);
				CSoundMgr::Get_Instance()->PlaySound(L"Fuse_In.wav", CSoundMgr::FUSE_IN);
			}
		}
		if (m_bTrueFuse[FUSE_THIRD] == false)
		{
			if (m_pOnFuse[FUSE_THIRD]->Collision_OBBtoOBB(pSecondCollider->Get_OBB()))
			{
				_float3 vCenter = m_pOnFuse[FUSE_THIRD]->Get_OBB()->Center;
				_vector vPosition = XMVectorSet(vCenter.x, vCenter.y, vCenter.z, 1.f);
				pSecond->Correct_Position(vPosition);
				CSoundMgr::Get_Instance()->PlaySound(L"Fuse_In.wav", CSoundMgr::FUSE_IN);
			}
		}
	}
	if (pThird != nullptr)
	{
		CCollider* pThirdCollider = dynamic_cast<CCollider*>(pThird->Get_Component(ComCollider));
		if (m_bTrueFuse[FUSE_FIRST] == false)
		{
			if (m_pOnFuse[FUSE_FIRST]->Collision_OBBtoOBB(pThirdCollider->Get_OBB()))
			{
				_float3 vCenter = m_pOnFuse[FUSE_FIRST]->Get_OBB()->Center;
				_vector vPosition = XMVectorSet(vCenter.x, vCenter.y, vCenter.z, 1.f);
				pThird->Correct_Position(vPosition);
				CSoundMgr::Get_Instance()->PlaySound(L"Fuse_In.wav", CSoundMgr::FUSE_IN);
			}
		}
		if (m_bTrueFuse[FUSE_SECOND] == false)
		{
			if (m_pOnFuse[FUSE_SECOND]->Collision_OBBtoOBB(pThirdCollider->Get_OBB()))
			{
				_float3 vCenter = m_pOnFuse[FUSE_SECOND]->Get_OBB()->Center;
				_vector vPosition = XMVectorSet(vCenter.x, vCenter.y, vCenter.z, 1.f);
				pThird->Correct_Position(vPosition);
				CSoundMgr::Get_Instance()->PlaySound(L"Fuse_In.wav", CSoundMgr::FUSE_IN);
			}
		}
	}

}

void CFuseBody::TimeAccumulation(_double dTimeDelta)
{
	m_dTime += dTimeDelta;
	
	if (m_dTime > 3.0)
		m_bDead = true;
}

CFuseBody * CFuseBody::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo)
{
	CFuseBody* pInstance = new CFuseBody(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFuseBody::Free()
{
	__super::Free();
	Safe_Release(m_pOnFuse[FUSE_FIRST]);
	Safe_Release(m_pOnFuse[FUSE_SECOND]);
	Safe_Release(m_pOnFuse[FUSE_THIRD]);

}
