#include "..\Public\LevelManager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevelManager)

CLevelManager::CLevelManager()
{
}

HRESULT CLevelManager::Open_Level(CLevel * pLevel)
{
	if (pLevel == nullptr)
		FAILMSG("Level is nullptr - LevelManager Open Level");

	if (m_pCurrentLevel != nullptr)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		//pGameInstance->Clear_Container(m_pCurrentLevel->Get_CurrentLevelID());
	}

	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pLevel;

	return S_OK;
}

_int CLevelManager::Tick(_double dTimeDelta)
{
	if (m_pCurrentLevel == nullptr)
		return 0;

	return m_pCurrentLevel->Tick(dTimeDelta);
}

HRESULT CLevelManager::Render()
{
	if(m_pCurrentLevel == nullptr)
		return E_FAIL;

	return m_pCurrentLevel->Render();
}

CNavigationMesh * CLevelManager::Get_NavMesh()
{
	return m_pCurrentLevel->Get_NavMesh();
}

void CLevelManager::Free()
{
	Safe_Release(m_pCurrentLevel);
}
