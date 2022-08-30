#include "stdafx.h"
#include "..\Public\Level_Qasmoke.h"
#include "Camera.h"
#include "LoadDatFiles.h"
#include "DefaultModel.h"
#include "DefaultLight.h"
#include "Ladder.h"
#include "PositionManager.h"
#include "SoundMgr.h"
#include "Level_Loading.h"

CLevel_Qasmoke::CLevel_Qasmoke(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CLevel(pGraphicDevice, pContextDevice)
{

}

HRESULT CLevel_Qasmoke::NativeConstruct(_uint iLevelID)
{
	if (FAILED(__super::NativeConstruct(iLevelID)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_uint i = 0;
	m_pNavigationMesh = dynamic_cast<CNavigationMesh*>(pGameInstance->Clone_Component(LEVEL_QASMOKE, L"NavigationStage1", &i));
	Safe_AddRef(m_pNavigationMesh);

	pGameInstance->Clear_Container(LEVEL_LOADING);
	CCollider::DESC tColliderDesc;
	tColliderDesc.vPivot = _float3(4.8f, 0.f, -7.4f);
	tColliderDesc.vScale = _float3(0.7f, 1.f, 0.7f);
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);
	m_pEvent1 = dynamic_cast<CCollider*>(pGameInstance->Clone_Component(LEVEL_STATIC, PrototypeOBB, &tColliderDesc));
	Safe_AddRef(m_pEvent1);
	tColliderDesc.vPivot = _float3(45.f, 3.f, 2.f);
	m_pEvent2 = dynamic_cast<CCollider*>(pGameInstance->Clone_Component(LEVEL_STATIC, PrototypeOBB, &tColliderDesc));
	Safe_AddRef(m_pEvent2);
	if (m_pNavigationMesh == nullptr)
		return E_FAIL;

	

	if (FAILED(Ready_Playable(LayerPlayer)))
		return E_FAIL;

	if (FAILED(Ready_NonPlayable(LayerNonPlayable)))
		return E_FAIL;

	if (FAILED(Ready_BackGround(LayerBackGround)))
		return E_FAIL;

	if (FAILED(Ready_Camera(LayerCamera)))
		return E_FAIL;

	if (FAILED(Ready_Light()))
		return E_FAIL;

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayLoopSound(L"Siren_Loop.wav", CSoundMgr::SIREN_LOOP);
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Main1.wav");
	CPositionManager::Get_Instance()->Get_Player()->Set_Loading(false);
	return S_OK;
}

_int CLevel_Qasmoke::Tick(_double dTimeDelta)
{
	if(m_pNavigationMesh != nullptr)
		m_pNavigationMesh->Update(XMMatrixIdentity());
	if (m_pEvent1 != nullptr)
		m_pEvent1->Update(XMMatrixIdentity());
	if (m_pEvent2 != nullptr)
		m_pEvent2->Update(XMMatrixIdentity());
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_fTimeAcc += dTimeDelta;
	if (CLoadDatFiles::Get_Instance()->Get_ModelLoading() == true)
		Collision();
	if (m_fTimeAcc > 5.f && CLoadDatFiles::Get_Instance()->Get_ModelLoading() == false)
		CLoadDatFiles::Get_Instance()->Set_ModelLoading();

	if (CLoadDatFiles::Get_Instance()->Get_ModelLoading() && CPositionManager::Get_Instance()->Get_FuseClear() == false)
	{
		m_dTimeAcc += dTimeDelta;
		if (m_dTimeAcc > 0.5 && !m_bReverse)
		{
			pGameInstance->Light_Action(L"Light_MachineRoom");
			m_dTimeAcc = 0.0;
			m_bReverse = true;
		}
		else if (m_dTimeAcc > 0.5 && m_bReverse)
		{
			pGameInstance->Light_DeActive(L"Light_MachineRoom");
			m_dTimeAcc = 0.0;
			m_bReverse = false;
		}
	}
	if (CLoadDatFiles::Get_Instance()->Get_ModelLoading() && CPositionManager::Get_Instance()->Get_FuseClear() == true)
		pGameInstance->Light_DeActive(L"Light_MachineRoom");


	return _int();
}

HRESULT CLevel_Qasmoke::Render()
{
	/*if(m_pNavigationMesh != nullptr)
		m_pNavigationMesh->Render();*/

	return S_OK;
}

HRESULT CLevel_Qasmoke::Ready_Light()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	LIGHTDESC	tLightDesc;

	tLightDesc.eLightType = LIGHTDESC::LIGHT_DIRECTIONAL;
	tLightDesc.vDiffuse = _float4(0.3f, 0.3f, 0.3f, 1.f);
	tLightDesc.vAmbient = _float4(0.15f, 0.15f, 0.15f, 1.f);
	tLightDesc.vSpecular = _float4(0.25f, 0.25f, 0.25f, 0.25f);
	tLightDesc.vDir = _float4(-1.f, -1.f, -1.f, 0.f);

	if (FAILED(pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice,L"Light" ,&tLightDesc)))
		FAILMSG("LEVEL QASmoke Ready Light - Failed to Add Light");

	tLightDesc.vDiffuse = _float4(0.5f, 0.15f, 0.15f, 1.f);
	tLightDesc.vAmbient = _float4(0.15f, 0.15f, 0.15f, 1.f);
	tLightDesc.vSpecular = _float4(0.25f, 0.25f, 0.25f, 0.25f);
	if (FAILED(pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice, L"Light_MachineRoom", &tLightDesc)))
		FAILMSG("LEVEL QASmoke Ready Light - Failed to Add Light");

	list<CGameObject*> pList = *pGameInstance->Get_GameObjects(LEVEL_QASMOKE, L"LayerLight");
	for (auto& iter : pList)
		dynamic_cast<CDefaultLight*>(iter)->Add_Light();

	if (FAILED(pGameInstance->Light_Action(L"Light")))
		return E_FAIL;

	tLightDesc.eLightType = LIGHTDESC::LIGHT_DIRECTIONAL;
	tLightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vAmbient = _float4(0.45f, 0.45f, 0.45f, 1.f);
	tLightDesc.vSpecular = _float4(0.65f, 0.65f, 0.65f, 1.f);
	tLightDesc.vDir = _float4(0.f, -1.f, -1.f, 0.f);

	if (FAILED(pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice, L"Maze", &tLightDesc)))
		FAILMSG("LEVEL Maze Ready Light - Failed to Add Light");
	

	return S_OK;
}

HRESULT CLevel_Qasmoke::Ready_Camera(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CCamera::DESC tCameraDesc;
	tCameraDesc.fAspect = (g_iWinCX) / (_float)g_iWinCY;
	tCameraDesc.vAt = _float3(1.f, 0.f, 0.f);
	tCameraDesc.vEye = _float3(1.f, 1.5f, -1.f);
	tCameraDesc.fFOV = XMConvertToRadians(60.f);

	tCameraDesc.tTransformDesc.dSpeedPerSec = 2.f;
	tCameraDesc.tTransformDesc.dRotationPerSec = 2.f;

	if (FAILED(pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, PrototypeCamera, pLayerTag, &tCameraDesc)))
		return E_FAIL;



	/*if (FAILED(pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, L"SkyBox", L"LayerSkyBox")))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_Qasmoke::Ready_Playable(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (FAILED(pGameInstance->Insert_ObjectToLayer(LEVEL_STATIC, PrototypeLeeHeeMin, pLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Qasmoke::Ready_NonPlayable(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (FAILED(pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, PrototypeLeeBongGoo, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, PrototypeSonDalSu, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Insert_ObjectToLayer(LEVEL_QASMOKE, PrototypeYooJiMin, L"LayerYooJiMin")))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Qasmoke::Ready_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	

	return S_OK;
}

void CLevel_Qasmoke::Set_VentIndex(_fvector vPosition)
{
	m_pNavigationMesh->In_Vent(vPosition);
}

void CLevel_Qasmoke::Collision()
{
	if (m_pEvent2->Collision_OBBtoOBB(CPositionManager::Get_Instance()->Get_PlayerCollider()->Get_OBB()))
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CPositionManager::Get_Instance()->Get_Player()->Set_Loading(true);
		pGameInstance->Open_Level(CLevel_Loading::Create(m_pGraphicDevice, m_pContextDevice, LEVEL_MAZE, LEVEL_LOADING));
	}
}

CLevel_Qasmoke * CLevel_Qasmoke::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, _uint iLevelID)
{
	CLevel_Qasmoke* pInstance = new CLevel_Qasmoke(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(iLevelID)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLevel_Qasmoke::Free()
{
	__super::Free();
	Safe_Release(m_pEvent1);
	Safe_Release(m_pEvent2);
}
