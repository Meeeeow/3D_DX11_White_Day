#include "stdafx.h"
#include "..\Public\Level_Loading.h"
#include "Level_Qasmoke.h"
#include "Maze.h"
#include "RectMaze.h"

CLevel_Loading::CLevel_Loading(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CLevel(pGraphicDevice, pContextDevice)
{

}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel, _uint iLevelID)
{
	if (FAILED(__super::NativeConstruct(iLevelID)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	pGameInstance->Insert_ObjectToLayer(LEVEL_LOADING, CRectMaze::Create(m_pGraphicDevice, m_pContextDevice), L"LayerLoading");
	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pGraphicDevice, m_pContextDevice, m_eNextLevel);
	if (m_pLoader == nullptr)
		return E_FAIL;

	return S_OK;
}

_int CLevel_Loading::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (m_pLoader->Get_Finished() == true)
	{
		CLevel* pLevel = nullptr;

		switch (m_eNextLevel)
		{
		case LEVEL_STATIC:
			break;
		case LEVEL_QASMOKE:
			pLevel = CLevel_Qasmoke::Create(m_pGraphicDevice, m_pContextDevice, LEVEL_QASMOKE);
			break;
		case LEVEL_MAZE:
			pLevel = CMaze::Create(m_pGraphicDevice, m_pContextDevice, LEVEL_MAZE);
			break;
		case LEVEL_END:
			break;
		default:
			break;
		}

		if (pLevel == nullptr)
			return -1;

		if (FAILED(pGameInstance->Open_Level(pLevel)))
			return -1;

		return 0;
	}

	return 0;
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, LEVEL eNextLevel, _uint iLevelIndex)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(eNextLevel, iLevelIndex)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();
	Safe_Release(m_pLoader);
}
