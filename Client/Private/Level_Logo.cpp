#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "Level_Qasmoke.h"
#include "Level_Loading.h"
#include "LogoScreen.h"
#include "LogoScreenRotation.h"
#include "LoadDatFiles.h"
#include "SoundMgr.h"
#include "TextBox.h"


CLevel_Logo::CLevel_Logo(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CLevel(pGraphicDevice, pContextDevice)
{

}

HRESULT CLevel_Logo::NativeConstruct(_uint iCurrentLevelID)
{
	if (FAILED(__super::NativeConstruct(iCurrentLevelID)))
		return E_FAIL;

	if (FAILED(Ready_GameObject()))
		return E_FAIL;

	if (FAILED(Ready_BackGround(LayerBackGround)))
		return E_FAIL;

	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Logo.wav");

	return S_OK;
}

_int CLevel_Logo::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	
	if (pGameInstance->Key_Down(DIK_1))
		pGameInstance->Insert_ObjectToLayer(LEVEL_LOGO, CTextBox::Create(m_pGraphicDevice, m_pContextDevice, CTextBox::JM_TEXT), L"TextBox");
	if (pGameInstance->Key_Down(DIK_2))
		pGameInstance->Insert_ObjectToLayer(LEVEL_LOGO, CTextBox::Create(m_pGraphicDevice, m_pContextDevice, CTextBox::DOOR_TEXT), L"TextBox");
	if (pGameInstance->Key_Down(DIK_3))
		pGameInstance->Insert_ObjectToLayer(LEVEL_LOGO, CTextBox::Create(m_pGraphicDevice, m_pContextDevice, CTextBox::DARK_TEXT), L"TextBox");
	if (pGameInstance->Key_Down(DIK_4))
		pGameInstance->Insert_ObjectToLayer(LEVEL_LOGO, CTextBox::Create(m_pGraphicDevice, m_pContextDevice, CTextBox::CONTROL_TEXT), L"TextBox");
	if (pGameInstance->Key_Down(DIK_5))
		pGameInstance->Insert_ObjectToLayer(LEVEL_LOGO, CTextBox::Create(m_pGraphicDevice, m_pContextDevice, CTextBox::ENTER_TEXT), L"TextBox");

	if (CLoadDatFiles::Get_Instance()->Get_TextureLoadStart() == false)
	{
		if (pGameInstance->Key_Down(DIK_SPACE))
		{
			CLoadDatFiles::Get_Instance()->TextureLoadingProcess();
		}
	}

	if (CLoadDatFiles::Get_Instance()->Get_TextureLoad() && pGameInstance->Key_Down(DIK_A))
	{
		CLevel* pLevel = CLevel_Loading::Create(m_pGraphicDevice, m_pContextDevice, LEVEL_QASMOKE, LEVEL_LOADING);
		if (pLevel == nullptr)
			return -1;

		if (FAILED(pGameInstance->Open_Level(pLevel)))
			return -1;

		return 0;
	}
	if (CLoadDatFiles::Get_Instance()->Get_TextureLoad())
	{
		m_dTimeDelta += dTimeDelta;
		if (m_dTimeDelta >= 1.3)
		{
			m_dTimeDelta = 0.0;
			pGameInstance->Insert_ObjectToLayer(LEVEL_LOGO, CTextBox::Create(m_pGraphicDevice, m_pContextDevice, CTextBox::ENTER_TEXT), L"TextBox");
		}
	}

	if (CLoadDatFiles::Get_Instance()->Get_TextureLoad() && pGameInstance->Key_Down(DIK_S))
	{
		CLevel* pLevel = CLevel_Loading::Create(m_pGraphicDevice, m_pContextDevice, LEVEL_MAZE, LEVEL_LOADING);
		if (pLevel == nullptr)
			return -1;

		if (FAILED(pGameInstance->Open_Level(pLevel)))
			return -1;

		return 0;
	}
	return 0;
}

HRESULT CLevel_Logo::Render()
{
	return S_OK;
}

HRESULT CLevel_Logo::Ready_GameObject()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (FAILED(pGameInstance->Insert_ObjectToLayer(LEVEL_STATIC, MouseObject, L"Mouse")))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Logo"), CLogoScreen::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (FAILED(pGameInstance->Insert_ObjectToLayer(LEVEL_LOGO, TEXT("Logo"), pLayerTag)))
		return E_FAIL;

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, _uint iCurrentLevelID)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(iCurrentLevelID)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
