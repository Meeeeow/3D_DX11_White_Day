#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "LeeHeeMin.h"
#include "LeeBongGoo.h"
#include "SonDalSu.h"
#include "YooJiMin.h"
#include "Camera_Debug.h"
#include "Terrain.h"
#include "LoadDatFiles.h"
#include "DefaultModel.h"
#include "MousePointer.h"
#include "Ladder.h"
#include "WomanToiletLadder.h"
#include "SkyBox.h"


CLoader::CLoader(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: m_pGraphicDevice(pGraphicDevice), m_pContextDevice(pContextDevice)
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;
	EnterCriticalSection(&pLoader->Get_CS());

	HRESULT			hr = 0;
	switch (pLoader->Get_NextLevel())
	{
	case LEVEL_QASMOKE:
		hr = pLoader->Loading_QASmoke();
		break;
	case LEVEL_MAZE:
		hr = pLoader->Loading_Maze();
		break;
	default:
		break;
	}
	if (FAILED(hr))
		FAILMSG("Loader hr is Failed");

	LeaveCriticalSection(&pLoader->Get_CS());
	
	return S_OK;
}

HRESULT CLoader::NativeConstruct(LEVEL eNextLevel)
{
	InitializeCriticalSection(&m_CS);

	m_eNextLevel = eNextLevel;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (m_hThread == 0)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_QASmoke()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Clear_Container(LEVEL_LOGO);

	_matrix matPivot = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	
	CLoadDatFiles* pLoadDatFiles = CLoadDatFiles::Get_Instance();
	pLoadDatFiles->Set_NewModelLoading();

	pLoadDatFiles->LoadingProcess(L"Stage1", LEVEL_QASMOKE, m_pGraphicDevice, m_pContextDevice);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Com_LeeHeeMin", CModel::Create(m_pGraphicDevice, m_pContextDevice,
		"../Bin/Resource/Friendly/LeeHeeMin/", "LeeHeeMin.fbx", Shader_Model, DefaultTechnique, matPivot))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Com_YooJiMin", CModel::Create(m_pGraphicDevice, m_pContextDevice,
		"../Bin/Resource/Friendly/YooJiMin/", "YooJiMin.fbx", Shader_Model, DefaultTechnique, matPivot))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Com_LeeBongGoo", CModel::Create(m_pGraphicDevice, m_pContextDevice,
		"../Bin/Resource/Hostile/LeeBongGoo/", "LeeBongGoo.fbx", Shader_Model, DefaultTechnique, matPivot))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"Com_SonDalSu", CModel::Create(m_pGraphicDevice, m_pContextDevice,
		"../Bin/Resource/Hostile/SonDalSu/", "SonDalSu.fbx", Shader_Model, DefaultTechnique, matPivot))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_QASMOKE, L"LadderModel", CModel::Create(m_pGraphicDevice, m_pContextDevice,
		"../Bin/Resource/Meshes/Ladder/", "Ladder_Moveable.fbx", Shader_Model, DefaultTechnique))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_QASMOKE, L"WomanToiletLadderModel", CModel::Create(m_pGraphicDevice, m_pContextDevice,
		"../Bin/Resource/Meshes/WomanToiletLadder/", "WomanToiletLadder.fbx", Shader_Model, DefaultTechnique))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(PrototypeLadder, CLadder::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(PrototypeCamera, CCamera_Debug::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(PrototypeLeeHeeMin, CLeeHeeMin::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(PrototypeLeeBongGoo, CLeeBongGoo::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(PrototypeYooJiMin, CYooJiMin::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(PrototypeSonDalSu, CSonDalSu::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(L"SkyBox", CSkyBox::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(L"PrototypeWomanLadder", CWomanToiletLadder::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_QASMOKE, L"NavigationStage1", CNavigationMesh::Create(m_pGraphicDevice, m_pContextDevice,
		L"../Bin/Data/SceneData/Stage1Nav.dat"))))
		return E_FAIL;

	m_bIsFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_Maze()
{
	CLoadDatFiles* pLoadDatFiles = CLoadDatFiles::Get_Instance();
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pLoadDatFiles->Set_NewModelLoading();

	pLoadDatFiles->LoadingProcess(L"Stage2", LEVEL_MAZE, m_pGraphicDevice, m_pContextDevice);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_MAZE, L"NavigationStage2", CNavigationMesh::Create(m_pGraphicDevice, m_pContextDevice,
		L"../Bin/Data/SceneData/Stage2Nav.dat"))))
		return E_FAIL;


	m_bIsFinished = true;
	return S_OK;
}

CLoader * CLoader::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, LEVEL eNextLevel)
{
	CLoader* pInstance = new CLoader(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(eNextLevel)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_CS);

	Safe_Release(m_pContextDevice);
	Safe_Release(m_pGraphicDevice);
}
