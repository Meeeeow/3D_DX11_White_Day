#include "stdafx.h"
#include "..\Public\LoadDatFiles.h"
#include "DefaultDoor.h"
#include "DefaultDoorLock.h"
#include "DefaultDoorTalisman.h"
#include "DefaultLight.h"
#include "DefaultLightController.h"
#include "DefaultTriggerProps.h"
#include "DoorGuard.h"

#include "FuseBody.h"
#include "PuzzleFirst.h"
#include "PuzzleSecond.h"
#include "PuzzleThird.h"

#include "HauntedPot.h"

#include "Valve.h"
#include "TalismanCase.h"

#include "Stone.h"

#include <wchar.h>

IMPLEMENT_SINGLETON(CLoadDatFiles)

CLoadDatFiles::CLoadDatFiles()
{

}

_uint APIENTRY MakeDefaultModel(LPVOID pArg)
{
	LOADFILE*	pLoadFile = (LOADFILE*)pArg;
	EnterCriticalSection(&CLoadDatFiles::Get_Instance()->Get_CS());

	HRESULT hr = 0;
	hr = CLoadDatFiles::Get_Instance()->LoadFunc(pLoadFile->eLevel, (_uint)pLoadFile->Start, (_uint)pLoadFile->End, pLoadFile->wstrSceneName);

	if (FAILED(hr))
		FAILMSG("LoadDataFail");

	LeaveCriticalSection(&CLoadDatFiles::Get_Instance()->Get_CS());
	return S_OK;
}

_uint APIENTRY	MakeTextureFile(LPVOID pArg)
{
	CLoadDatFiles* pLoadDatFiles = (CLoadDatFiles*)pArg;
	EnterCriticalSection(&pLoadDatFiles->Get_CS());

	HRESULT hr = 0;
	hr = pLoadDatFiles->Load_MaterialFiles();

	if (FAILED(hr))
		FAILMSG("LoadTextureFiles");

	LeaveCriticalSection(&pLoadDatFiles->Get_CS());

	
	return S_OK;
}


HRESULT CLoadDatFiles::Init(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	m_pGraphicDevice = pGraphicDevice;
	Safe_AddRef(m_pGraphicDevice);
	m_pContextDevice = pContextDevice;
	Safe_AddRef(m_pContextDevice);
	m_bModelLoading = false;
	return S_OK;
}

HRESULT CLoadDatFiles::Load_DatFiles()
{
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	wstring strBuf = szBuf;
	wstring strFilter = L"\\";

	for (size_t i = strBuf.length() - 1; i > 0; --i)
	{
		if (strBuf[i] == strFilter[0])
		{
			strBuf = strBuf.substr(0, i);
			break;
		}
	}

	lstrcpy(szBuf, strBuf.c_str());
	lstrcat(szBuf, L"\\Bin\\Data\\SceneData\\");
	strBuf = szBuf;

	list<wstring> listSceneObjectsName;
	// Input Scene Object Info Name
	listSceneObjectsName.push_back(wstring(L"Stage1"));
	listSceneObjectsName.push_back(wstring(L"Stage2"));
	HANDLE hFile;
	for (auto& ObjectDatName : listSceneObjectsName)
	{
		wstring strSceneDatName = strBuf + ObjectDatName + L".dat";
		hFile = CreateFile(strSceneDatName.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hFile == INVALID_HANDLE_VALUE)
			FAILMSG("Load Dat Files - ObjectInfo");

		while (true)
		{
			OBJ_INFO* pObjInfo = new OBJ_INFO;
			_ulong dwByte = 0;
 			ReadFile(hFile, pObjInfo, sizeof(OBJ_INFO), &dwByte, nullptr);

			if (dwByte <= 0)
			{
				Safe_Delete(pObjInfo);
				break;
			}

			auto& iter = m_mapSceneObjects.find(ObjectDatName);
			if (iter == m_mapSceneObjects.end())
			{
				vector<OBJ_INFO*> pvecObjInfo;
				pvecObjInfo.push_back(pObjInfo);
				m_mapSceneObjects.emplace(ObjectDatName, pvecObjInfo);
			}
			else
				iter->second.push_back(pObjInfo);
		}
		CloseHandle(hFile);
	}


	return S_OK;
}

size_t CLoadDatFiles::Get_ObjVectorSize(wstring wstrSceneName)
{
	auto& iter = m_mapSceneObjects.find(wstrSceneName);
	if (iter == m_mapSceneObjects.end())
		return 0;

	return iter->second.size();
}

const OBJ_INFO * CLoadDatFiles::Get_ObjectInfo(wstring wstrSceneName, _uint iIndex)
{
	auto& iter = m_mapSceneObjects.find(wstrSceneName);
	if (iter == m_mapSceneObjects.end() || iIndex >= iter->second.size())
		return nullptr;

	return iter->second[iIndex];
}

HRESULT CLoadDatFiles::TextureLoadingProcess()
{
	m_bTextureLoadStart = true;
	InitializeCriticalSection(&m_CS);

	m_hThread[0] = (HANDLE)_beginthreadex(nullptr, 0, MakeTextureFile, this, 0, nullptr);
	if (m_hThread[0] == 0)
		return E_FAIL;

	WaitForSingleObject(m_hThread[0], INFINITE);
	//DeleteObject(m_hThread[0]); CloseHandle(m_hThread[0]);
	//DeleteCriticalSection(&m_CS);
	return S_OK;
}

HRESULT CLoadDatFiles::Load_MaterialFiles()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_finddata_t fd;

	__int64 handle = _findfirst("../Bin/Resource/Meshes/*.png", &fd);
	int Check = 0;
	int iResult = 0;

	char szCurPath[128] = "../Bin/Resource/Meshes/";
	char szFullPath[128] = "";
	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);

		_tchar	szMaterialFullPath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szMaterialFullPath, 260);
		wstring wstr(fd.name, &fd.name[260]);
		
		wchar_t* pTag = new wchar_t[MAX_PATH];
		lstrcpy(pTag, wstr.c_str());

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_DYNAMIC, pTag, CTexture::Create(m_pGraphicDevice, m_pContextDevice, szMaterialFullPath))))
			return E_FAIL;
		m_vecDynamicTag.push_back(pTag);

		iResult = _findnext(handle, &fd);
		
	}
	_findclose(handle);



	m_bTextureLoading = true;


	return S_OK;
}

HRESULT CLoadDatFiles::LoadingProcess(wstring wstrSceneName, LEVEL eLevel, _pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice)
{
	if (m_pLoadFiles != nullptr)
		Safe_Delete_Arr(m_pLoadFiles);
	m_bModelLoading = false;
	InitializeCriticalSection(&m_CS);

	m_pLoadFiles = new LOADFILE[4];
	m_pLoadFiles[0].wstrSceneName = wstrSceneName;
	m_pLoadFiles[0].eLevel = eLevel;
	m_pLoadFiles[1].wstrSceneName = wstrSceneName;
	m_pLoadFiles[1].eLevel = eLevel;
	m_pLoadFiles[2].wstrSceneName = wstrSceneName;
	m_pLoadFiles[2].eLevel = eLevel;
	m_pLoadFiles[3].wstrSceneName = wstrSceneName;
	m_pLoadFiles[3].eLevel = eLevel;

	size_t iCount = Get_ObjVectorSize(wstrSceneName);
	_uint  iResult = floor(iCount / 4);

	m_pLoadFiles[0].Start = 0;
	m_pLoadFiles[0].End = iResult;
	m_pLoadFiles[1].Start = iResult;
	m_pLoadFiles[1].End = iResult * 2;
	m_pLoadFiles[2].Start = iResult * 2;
	m_pLoadFiles[2].End = iResult * 3;
	m_pLoadFiles[3].Start = iResult * 3;
	m_pLoadFiles[3].End = iCount;

	m_hThread[0] = (HANDLE)_beginthreadex(nullptr, 0, MakeDefaultModel, &m_pLoadFiles[0], 0, nullptr);
	m_hThread[1] = (HANDLE)_beginthreadex(nullptr, 0, MakeDefaultModel, &m_pLoadFiles[1], 0, nullptr);
	m_hThread[2] = (HANDLE)_beginthreadex(nullptr, 0, MakeDefaultModel, &m_pLoadFiles[2], 0, nullptr);
	m_hThread[3] = (HANDLE)_beginthreadex(nullptr, 0, MakeDefaultModel, &m_pLoadFiles[3], 0, nullptr);


	if (m_hThread[0] == 0 && m_hThread[1] == 0 && m_hThread[2] == 0)
		return E_FAIL;

	WaitForMultipleObjects(4, m_hThread, TRUE, INFINITE);
	DeleteObject(m_hThread[0]); DeleteObject(m_hThread[1]); DeleteObject(m_hThread[2]); DeleteObject(m_hThread[3]);
	CloseHandle(m_hThread[0]); CloseHandle(m_hThread[1]); CloseHandle(m_hThread[2]); CloseHandle(m_hThread[3]);
	DeleteCriticalSection(&m_CS);


	return S_OK;
}

HRESULT CLoadDatFiles::LoadFunc(LEVEL eLevel, _uint Start, _uint End, wstring wstrSceneName)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (eLevel == LEVEL_QASMOKE)
	{
		for (_uint i = Start; i < End; ++i)
		{
			/*
			Door Filter Name
			1_ DoorLeftX - Move X Position - 0.66
			2_ DoorRightX - Move X Position + 0.66
			3_ DoorLeftZ - Move Z Position + 0.66
			4_ DoorRightZ - Move Z Position - 0.66
			5_ DoorRotationP - Rotate Y Axis 90 Degree.
			6_ DoorRotationM - Rotate Y Axis -90 Degree.
			*/
			OBJ_INFO tInfo = *CLoadDatFiles::Get_Instance()->Get_ObjectInfo(wstrSceneName, i);
			if (!lstrcmp(tInfo.szObjName, L"DoorLeftX"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultDoor::Create(m_pGraphicDevice, m_pContextDevice, tInfo, -0.66f, 0.f, true), LayerCollision)))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"DoorRightX"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultDoor::Create(m_pGraphicDevice, m_pContextDevice, tInfo, 0.66f, 0.f, true), LayerCollision)))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"DoorLeftZ"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultDoor::Create(m_pGraphicDevice, m_pContextDevice, tInfo, 0.66f, 0.f, false), LayerCollision)))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"DoorRotationP"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultDoor::Create(m_pGraphicDevice, m_pContextDevice, tInfo, 0.f, 90.f, false), LayerCollision)))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"DoorRotationM"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultDoor::Create(m_pGraphicDevice, m_pContextDevice, tInfo, 0.f, -90.f, false), LayerCollision)))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"DoorRightZ"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultDoor::Create(m_pGraphicDevice, m_pContextDevice, tInfo, -0.66f, 0.f, false), LayerCollision)))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"DoorLockLeftX"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultDoorLock::Create(m_pGraphicDevice, m_pContextDevice, tInfo, -0.66f, true), LayerCollision)))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"DoorLockRightX"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultDoorLock::Create(m_pGraphicDevice, m_pContextDevice, tInfo, 0.66f, true), LayerCollision)))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"DoorRotationMLock"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultDoorTalisman::Create(m_pGraphicDevice, m_pContextDevice, tInfo, -90.f), LayerCollision)))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"DoorRotationPLock"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultDoorTalisman::Create(m_pGraphicDevice, m_pContextDevice, tInfo, 90.f), LayerCollision)))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"DoorGuard"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDoorGuard::Create(m_pGraphicDevice, m_pContextDevice, tInfo), LayerCollision)))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_2-1"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_2-2"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_2-3"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_2-4"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_2-5"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_2-6"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_2-7"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_2-8"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_2-9"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_2-10"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_2-11"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_2-12"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_Studio1"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_PhotoRoom"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_MountainRoom"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_PhysicalRoom"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_MachineRoom"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_HouseKeepingRoom"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_ManToilet_1F"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Light_ManToilet_2F"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLight::Create(m_pGraphicDevice, m_pContextDevice, tInfo, tInfo.szObjName), L"LayerLight")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_Studio1"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_Studio1"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_PhotoRoom"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_PhotoRoom"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_MountainRoom"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_MountainRoom"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_PhysicalRoom"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_PhysicalRoom"), L"LightController")))
					return  E_FAIL;
			}

			else if (!lstrcmp(tInfo.szObjName, L"LightController_HouseKeepingRoom"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_HouseKeepingRoom"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_ManToilet_1F"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_ManToilet_1F"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_ManToilet_2F"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_ManToilet_2F"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_MachineRoom"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_MachineRoom"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_HouseKeepingRoom"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_WomanToilet"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_2-12"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_2-12"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_2-11"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_2-11"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_2-10"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_2-10"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_2-9"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_2-9"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_2-8"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_2-8"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_2-7"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_2-7"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_2-6"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_2-6"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_2-5"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_2-5"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_2-4"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_2-4"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_2-3"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_2-3"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_2-2"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_2-2"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"LightController_2-1"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultLightController::Create(m_pGraphicDevice, m_pContextDevice, tInfo, L"Light_2-1"), L"LightController")))
					return  E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"TRG_Ladder"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultTriggerProps::Create(m_pGraphicDevice, m_pContextDevice, tInfo, CDefaultTriggerProps::TRG_LADDER), L"Trigger")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"TRG_Poison"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultTriggerProps::Create(m_pGraphicDevice, m_pContextDevice, tInfo, CDefaultTriggerProps::TRG_POISON), L"Trigger")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"TRG_Key"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultTriggerProps::Create(m_pGraphicDevice, m_pContextDevice, tInfo, CDefaultTriggerProps::TRG_KEY), L"Trigger")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"TRG_Talisman"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultTriggerProps::Create(m_pGraphicDevice, m_pContextDevice, tInfo, CDefaultTriggerProps::TRG_TALISMAN), L"Trigger")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"TRG_Lighter"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultTriggerProps::Create(m_pGraphicDevice, m_pContextDevice, tInfo, CDefaultTriggerProps::TRG_LIGHTER), L"Trigger")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"TRG_FuseBody"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultTriggerProps::Create(m_pGraphicDevice, m_pContextDevice, tInfo, CDefaultTriggerProps::TRG_FUSEBOX), L"Trigger")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"PuzzleBody"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CFuseBody::Create(m_pGraphicDevice, m_pContextDevice, tInfo), L"LayerFuseBox")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Puzzle0"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CPuzzleFirst::Create(m_pGraphicDevice, m_pContextDevice, tInfo), L"LayerFuse")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Puzzle1"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CPuzzleSecond::Create(m_pGraphicDevice, m_pContextDevice, tInfo), L"LayerFuse")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Puzzle2"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CPuzzleThird::Create(m_pGraphicDevice, m_pContextDevice, tInfo), L"LayerFuse")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"DalSuPot"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CHauntedPot::Create(m_pGraphicDevice, m_pContextDevice, tInfo, CHauntedPot::JANITOR_DALSU), L"LayerPot")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"BongGooPot"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CHauntedPot::Create(m_pGraphicDevice, m_pContextDevice, tInfo, CHauntedPot::JANITOR_BONGGOO), L"LayerPot")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Valve0"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CValve::Create(m_pGraphicDevice, m_pContextDevice, tInfo, CValve::VALVE0), L"LayerValve")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"Valve1"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CValve::Create(m_pGraphicDevice, m_pContextDevice, tInfo, CValve::VALVE1), L"LayerValve")))
					return E_FAIL;
			}
			else if (!lstrcmp(tInfo.szObjName, L"InsertTalisman"))
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CTalismanCase::Create(m_pGraphicDevice, m_pContextDevice, tInfo), L"LayerTalismanCase")))
					return E_FAIL;
			}
			else
			{
				if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CDefaultModel::Create(m_pGraphicDevice, m_pContextDevice, tInfo), LayerBackGround)))
					return E_FAIL;
			}
		}
	}
	else if (eLevel == LEVEL_MAZE)
	{
		for (_uint i = Start; i < End; ++i)
		{
			OBJ_INFO tInfo = *CLoadDatFiles::Get_Instance()->Get_ObjectInfo(wstrSceneName, i);
			if (FAILED(pGameInstance->Insert_ObjectToLayer(eLevel, CStone::Create(m_pGraphicDevice, m_pContextDevice, tInfo), LayerCollision)))
				return  E_FAIL;
		}
	}


	return S_OK;
}

void CLoadDatFiles::Free()
{
	for (auto& Pair : m_mapSceneObjects)
	{
		for (auto& pObjInfo : Pair.second)
			Safe_Delete(pObjInfo);
		Pair.second.clear();
		Pair.second.shrink_to_fit();
	}
	for (auto& Vector : m_vecDynamicTag)
		Safe_Delete_Arr(Vector);

	
	Safe_Release(m_pContextDevice);
	Safe_Release(m_pGraphicDevice);
	Safe_Delete_Arr(m_pLoadFiles);
}

