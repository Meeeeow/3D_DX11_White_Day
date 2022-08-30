#include "stdafx.h"
#include "..\Public\Maze.h"
#include "PositionManager.h"
#include "LoadDatFiles.h"
#include "MazeYooJiMin.h"

CMaze::CMaze(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CLevel(pGraphicDevice, pContextDevice)
{
}

HRESULT CMaze::NativeConstruct(_uint iLevelID)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	LIGHTDESC	tLightDesc;


	_uint i = 0;
	m_pNavigationMesh = dynamic_cast<CNavigationMesh*>(pGameInstance->Clone_Component(LEVEL_MAZE, L"NavigationStage2", &i));
	Safe_AddRef(m_pNavigationMesh);
	pGameInstance->Clear_Container(LEVEL_LOADING);
	CPositionManager::Get_Instance()->Get_PlayerTransform()->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 6.f, 0.f, 1.f));
	CPositionManager::Get_Instance()->Get_Player()->Set_Loading(false);


	pGameInstance->Insert_ObjectToLayer(LEVEL_MAZE, CMazeYooJiMin::Create(m_pGraphicDevice, m_pContextDevice), L"LayerMazeObject");
	CSoundMgr::Get_Instance()->StopAll();

	pGameInstance->Light_Action(L"Maze");
	return S_OK;
}

_int CMaze::Tick(_double dTimeDelta)
{
	m_pNavigationMesh->Update(XMMatrixIdentity());

	m_dTimeAcc += dTimeDelta;
	if (m_dTimeAcc > 5.f && CLoadDatFiles::Get_Instance()->Get_ModelLoading() == false)
		CLoadDatFiles::Get_Instance()->Set_ModelLoading();

	return _int();
}

CMaze * CMaze::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, _uint iLevelID)
{
	CMaze* pInstance = new CMaze(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(iLevelID)))
		Safe_Release(pInstance);
	return pInstance;
}

void CMaze::Free()
{
	__super::Free();
}
