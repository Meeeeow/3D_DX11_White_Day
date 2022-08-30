#include "stdafx.h"
#include "..\Public\MainApplication.h"
#include "Level_Logo.h"
#include "LoadDatFiles.h"
#include "MousePointer.h"
#include "PositionManager.h"
#include "SoundMgr.h"
#include <time.h>
#include "RectMaze.h"

CMainApplication::CMainApplication()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApplication::NativeConstruct()
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;

	srand(unsigned(time(NULL)));

	CGraphicDevice::DESC	tGraphicDesc;
	ZeroMemory(&tGraphicDesc, sizeof(CGraphicDevice::DESC));

	tGraphicDesc.hWnd = g_hWnd;
	tGraphicDesc.iWinCX = g_iWinCX;
	tGraphicDesc.iWinCY = g_iWinCY;
	tGraphicDesc.eWinMode = CGraphicDevice::MODE_WIN;

	if (FAILED(m_pGameInstance->Initialize(g_hInstance, tGraphicDesc, LEVEL_END, &m_pGraphicDevice, &m_pContextDevice)))
		return E_FAIL;

	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);

	if (FAILED(CLoadDatFiles::Get_Instance()->Init(m_pGraphicDevice, m_pContextDevice)))
		return E_FAIL;



	if (FAILED(CLoadDatFiles::Get_Instance()->Load_DatFiles()))
		return E_FAIL;

	CSoundMgr::Get_Instance()->Initialize();

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

_int CMainApplication::Tick(_double dTimeDelta)
{
	return m_pGameInstance->Tick(dTimeDelta);
}

HRESULT CMainApplication::Render()
{
	if (m_pGameInstance->Clear_BackBufferView(_float4(0.f, 0.15f, 0.15f, 1.f)))
		return E_FAIL;

	if (m_pGameInstance->Clear_DepthStencilView())
		return E_FAIL;

	if (FAILED(m_pRenderer->Draw_RenderGroup()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_Level()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Present()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApplication::Open_Level(LEVEL eLevel)
{
	if (FAILED(m_pGameInstance->Open_Level(CLevel_Logo::Create(m_pGraphicDevice, m_pContextDevice, LEVEL_LOGO))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApplication::Ready_Component()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeRenderer
		, m_pRenderer = CRenderer::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeTransform, CTransform::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeRectXY, CVIBuffer_RectXY::Create(m_pGraphicDevice, m_pContextDevice, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl"), DefaultTechnique))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeCube, CVIBufferCube::Create(m_pGraphicDevice, m_pContextDevice, L"../Bin/ShaderFiles/Shader_Cube.hlsl", DefaultTechnique))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeAABB, CCollider::Create(m_pGraphicDevice, m_pContextDevice, CCollider::TYPE_AABB))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeOBB, CCollider::Create(m_pGraphicDevice, m_pContextDevice, CCollider::TYPE_OBB))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeSphere, CCollider::Create(m_pGraphicDevice, m_pContextDevice, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeTerrain, CVIBuffer_Terrain::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeAStar, CAStar::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Lobby"), CTexture::Create(m_pGraphicDevice, m_pContextDevice, TEXT("../Bin/Resource/Lobby/LobbyScreen%d.png"),3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MouseState")
		, CTexture::Create(m_pGraphicDevice, m_pContextDevice, TEXT("../Bin/Resource/Sprite/Event0%d.dds"),10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SkyBoxTexture")
		, CTexture::Create(m_pGraphicDevice, m_pContextDevice, TEXT("../Bin/Resource/SkyBox/NightCubeMap.dds")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TextBox")
		, CTexture::Create(m_pGraphicDevice, m_pContextDevice, TEXT("../Bin/Resource/Sprite/Text0%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Dust")
		, CTexture::Create(m_pGraphicDevice, m_pContextDevice, TEXT("../Bin/Resource/Sprite/Dust.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Hit")
		, CTexture::Create(m_pGraphicDevice, m_pContextDevice, TEXT("../Bin/Resource/Sprite/EffectCloud%d.png"),7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("LoadMaze")
		, CTexture::Create(m_pGraphicDevice, m_pContextDevice, TEXT("../Bin/Resource/Lobby/LoadingScreen.png")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("BlackScreen")
		, CTexture::Create(m_pGraphicDevice, m_pContextDevice, TEXT("../Bin/Resource/Sprite/Black.png")))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(MouseObject, CMousePointer::Create(m_pGraphicDevice, m_pContextDevice))))
		return E_FAIL;

	return S_OK;
}

CMainApplication * CMainApplication::Create()
{
	CMainApplication* pInstance = new CMainApplication();
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainApplication::Free()
{
	CLoadDatFiles::Destroy_Instance();
	CPositionManager::Destroy_Instance();
	CSoundMgr::Destroy_Instance();

	unsigned long dwRefCnt = 0;
	dwRefCnt = Safe_Release(m_pGameInstance);
	dwRefCnt = Safe_Release(m_pRenderer);
	dwRefCnt = Safe_Release(m_pContextDevice);
	dwRefCnt = Safe_Release(m_pGraphicDevice);


	CGameInstance::Release_Engine();
}


