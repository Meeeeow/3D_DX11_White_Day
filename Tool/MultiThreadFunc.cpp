#include "stdafx.h"
#include "MultiThreadFunc.h"


IMPLEMENT_SINGLETON(CMultiThreadFunc)

CMultiThreadFunc::CMultiThreadFunc()
{
}

_uint APIENTRY MakeLoadObject(LPVOID pArg)
{
	TOOLLOADFILE* pLoadFile = (TOOLLOADFILE*)pArg;
	EnterCriticalSection(&CMultiThreadFunc::Get_Instance()->Get_CS());

	HRESULT hr = 0;
	hr = CMultiThreadFunc::Get_Instance()->LoadFunc(pLoadFile->Start, pLoadFile->End);

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&CMultiThreadFunc::Get_Instance()->Get_CS());
	return S_OK;
}

HRESULT CMultiThreadFunc::Init(CToolView * pToolView, CForm * pForm, _pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice)
{
	m_pToolView = pToolView;
	m_pForm = pForm;
	m_pGraphicDevice = pGraphicDevice;
	m_pContextDevice = pContextDevice;
	Safe_AddRef(m_pContextDevice);
	Safe_AddRef(m_pGraphicDevice);
	return S_OK;
}

void CMultiThreadFunc::Load_MaterialFiles()
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_finddata_t fd;

	__int64 handle = _findfirst("../Client/Bin/Resource/Meshes/*.png", &fd);
	int Check = 0;
	int iResult = 0;

	char szCurPath[128] = "../Client/Bin/Resource/Meshes/";
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
			return;
		m_vecDynamicTag.push_back(pTag);

		iResult = _findnext(handle, &fd);

	}
	_findclose(handle);
	
}

HRESULT CMultiThreadFunc::LoadingProcess()
{
	InitializeCriticalSection(&m_CS);
	m_pLoadFile = new TOOLLOADFILE;
	m_pLoadFile->Start = 0;
	m_pLoadFile->End = m_vecSceneObject.size();


	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, MakeLoadObject, m_pLoadFile, 0, nullptr);

	if (m_hThread == 0)
		return E_FAIL;

	return S_OK;
}

HRESULT CMultiThreadFunc::LoadFunc(_uint iStart, _uint iEnd)
{
	m_bLoad = true;
	for (_uint i = iStart; i < iEnd; ++i)
	{
		m_pToolView->Load_Object(m_vecSceneObject[i]);
		m_pForm->Add_String(m_vecSceneObject[i]->szObjName);
	}
	m_bLoad = false;
	return S_OK;
}

void CMultiThreadFunc::Call_LoadButton()
{
	Load_MaterialFiles();

	CFileDialog Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\Bin\\Data\\SceneData\\");
	Dlg.m_ofn.lpstrInitialDir = szBuf;
	if (IDOK == Dlg.DoModal())
	{

		CString wstrPath = Dlg.GetPathName();
		HANDLE	hFile = CreateFile(wstrPath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);


		if (INVALID_HANDLE_VALUE == hFile)
			return;

		_ulong	dwByte = 0;
		while (true)
		{
			OBJ_INFO* pObjInfo = new OBJ_INFO;
			ReadFile(hFile, pObjInfo, sizeof(OBJ_INFO), &dwByte, nullptr);

			if (dwByte <= 0)
			{
				Safe_Delete(pObjInfo);
				break;
			}
			m_vecSceneObject.push_back(pObjInfo);
		}
		CloseHandle(hFile);
	}
}

void CMultiThreadFunc::Free()
{
	for (auto& iter : m_vecSceneObject)
		Safe_Delete(iter);
	m_vecSceneObject.clear();

	for (auto& iter : m_vecDynamicTag)
		Safe_Delete(iter);
	m_vecDynamicTag.clear();

	DeleteCriticalSection(&m_CS);

	Safe_Delete(m_pLoadFile);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pContextDevice);
}
