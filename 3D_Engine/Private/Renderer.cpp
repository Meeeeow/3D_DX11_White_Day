#include "..\Public\Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CComponent(pGraphicDevice,pContextDevice), m_pRenderTargetManager(CRenderTargetManager::Get_Instance())
	, m_pLightManager(CLightManager::Get_Instance())
{
}

HRESULT CRenderer::NativeConstruct()
{
	D3D11_VIEWPORT	tViewportDesc;
	ZeroMemory(&tViewportDesc, sizeof(D3D11_VIEWPORT));

	_uint			iNumViewport = 1;
	m_pContextDevice->RSGetViewports(&iNumViewport, &tViewportDesc);

	// Deffered
	if (FAILED(m_pRenderTargetManager->Add_RenderTarget(m_pGraphicDevice, m_pContextDevice
		, L"RTV_Diffuse", (_uint)tViewportDesc.Width, (_uint)tViewportDesc.Height
		, DXGI_FORMAT_B8G8R8A8_UNORM, 0, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_RenderTarget(m_pGraphicDevice, m_pContextDevice
		, L"RTV_Normal", (_uint)tViewportDesc.Width, (_uint)tViewportDesc.Height
		, DXGI_FORMAT_R16G16B16A16_UNORM,0, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_RenderTarget(m_pGraphicDevice, m_pContextDevice
		, L"RTV_Depth", (_uint)tViewportDesc.Width, (_uint)tViewportDesc.Height
		, DXGI_FORMAT_R16G16B16A16_UNORM,0, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_MRT(L"MRT_Deffered", L"RTV_Diffuse")))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_MRT(L"MRT_Deffered", L"RTV_Normal")))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_MRT(L"MRT_Deffered", L"RTV_Depth")))
		return E_FAIL;

	// Light
	if (FAILED(m_pRenderTargetManager->Add_RenderTarget(m_pGraphicDevice, m_pContextDevice
		, L"RTV_Shade", (_uint)tViewportDesc.Width, (_uint)tViewportDesc.Height
		, DXGI_FORMAT_R16G16B16A16_UNORM,1, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_RenderTarget(m_pGraphicDevice, m_pContextDevice
		, L"RTV_Specular", (_uint)tViewportDesc.Width, (_uint)tViewportDesc.Height
		, DXGI_FORMAT_R16G16B16A16_UNORM, 1, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_MRT(L"MRT_LightAcc", L"RTV_Shade")))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_MRT(L"MRT_LightAcc", L"RTV_Specular")))
		return E_FAIL;

	

	_float4 vScreenPos = _float4(0.f, 0.f, tViewportDesc.Width, tViewportDesc.Height);

	m_pVIBuffer = CVIBuffer_Viewport::Create(m_pGraphicDevice, m_pContextDevice, L"../Bin/ShaderFiles/Shader_Viewport.hlsl", "Technique_Default",vScreenPos);
	if (m_pVIBuffer == nullptr)
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Set_Buffer(TEXT("RTV_Diffuse"), _float4(0.f, 0.f, 100.f, 100.f))))
		return E_FAIL;
	if (FAILED(m_pRenderTargetManager->Set_Buffer(TEXT("RTV_Normal"), _float4(0.f, 100.f, 100.f, 100.f))))
		return E_FAIL;
	if (FAILED(m_pRenderTargetManager->Set_Buffer(TEXT("RTV_Depth"), _float4(0.f, 200.f, 100.f, 100.f))))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Set_Buffer(TEXT("RTV_Shade"), _float4(100.f, 0.f, 100.f, 100.f))))
		return E_FAIL;
	if (FAILED(m_pRenderTargetManager->Set_Buffer(TEXT("RTV_Specular"), _float4(100.f, 100.f, 100.f, 100.f))))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::NativeConstruct(const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	D3D11_VIEWPORT	tViewportDesc;
	ZeroMemory(&tViewportDesc, sizeof(D3D11_VIEWPORT));

	_uint			iNumViewport = 1;
	m_pContextDevice->RSGetViewports(&iNumViewport, &tViewportDesc);

	if (FAILED(m_pRenderTargetManager->Add_RenderTarget(m_pGraphicDevice, m_pContextDevice
		, L"RTV_Diffuse", (_uint)tViewportDesc.Width, (_uint)tViewportDesc.Height
		, DXGI_FORMAT_B8G8R8A8_UNORM, 0)))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_RenderTarget(m_pGraphicDevice, m_pContextDevice
		, L"RTV_Normal", (_uint)tViewportDesc.Width, (_uint)tViewportDesc.Height
		, DXGI_FORMAT_R16G16B16A16_UNORM, 0)))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_RenderTarget(m_pGraphicDevice, m_pContextDevice
		, L"RTV_Depth", (_uint)tViewportDesc.Width, (_uint)tViewportDesc.Height
		, DXGI_FORMAT_R16G16B16A16_UNORM,0)))
		return E_FAIL;



	if (FAILED(m_pRenderTargetManager->Add_RenderTarget(m_pGraphicDevice, m_pContextDevice
		, L"RTV_Shade", (_uint)tViewportDesc.Width, (_uint)tViewportDesc.Height
		, DXGI_FORMAT_R16G16B16A16_UNORM, 1)))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_MRT(L"MRT_Deffered", L"RTV_Diffuse")))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_MRT(L"MRT_Deffered", L"RTV_Normal")))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_MRT(L"MRT_Deffered", L"RTV_Depth")))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Add_MRT(L"MRT_LightAcc", L"RTV_Shade")))
		return E_FAIL;

	_float4 vScreenPos = _float4(0.f, 0.f, tViewportDesc.Width, tViewportDesc.Height);

	m_pVIBuffer = CVIBuffer_Viewport::Create(m_pGraphicDevice, m_pContextDevice, pShaderFilePath, pTechniqueName, vScreenPos);
	if (m_pVIBuffer == nullptr)
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Set_Buffer(TEXT("RTV_Diffuse"), pShaderFilePath, pTechniqueName, _float4(0.f, 0.f, 100.f, 100.f))))
		return E_FAIL;
	if (FAILED(m_pRenderTargetManager->Set_Buffer(TEXT("RTV_Normal"), pShaderFilePath, pTechniqueName, _float4(0.f, 100.f, 100.f, 100.f))))
		return E_FAIL;
	if (FAILED(m_pRenderTargetManager->Set_Buffer(TEXT("RTV_Depth"), pShaderFilePath, pTechniqueName, _float4(0.f, 200.f, 100.f, 100.f))))
		return E_FAIL;
	if (FAILED(m_pRenderTargetManager->Set_Buffer(TEXT("RTV_Shade"), pShaderFilePath, pTechniqueName, _float4(100.f, 0.f, 100.f, 100.f))))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Clone_Construct(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderSequence, CGameObject * pRenderObject)
{
	if (eRenderSequence >= RENDER_END
		|| pRenderObject == nullptr)
		return E_FAIL;

	m_listRenderObjects[eRenderSequence].emplace_back(pRenderObject);
	Safe_AddRef(pRenderObject);

	return S_OK;
}

HRESULT CRenderer::Add_DebugGroup(DEBUGGROUP eDebugSequence, CComponent * pDebugObjects)
{
	if (eDebugSequence >= DEBUG_END
		|| pDebugObjects == nullptr)
		return E_FAIL;

	m_listDebugObjects[eDebugSequence].emplace_back(pDebugObjects);
	Safe_AddRef(pDebugObjects);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	if (FAILED(Sequence_Priority()))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->Begin_MRT(L"MRT_Deffered")))
		return E_FAIL;

	if (FAILED(Sequence_Test()))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->End_MRT()))
		return E_FAIL;

	if (FAILED(Sequence_Light()))
		return E_FAIL;

	if (FAILED(Sequence_Result()))
		return E_FAIL;

	if (FAILED(Sequence_Blend()))
		return E_FAIL;

	if (FAILED(Sequence_UI_First()))
		return E_FAIL;

	if (FAILED(Sequence_UI_Second()))
		return E_FAIL;

	if (FAILED(Sequence_UI_Third()))
		return E_FAIL;
#ifdef __DEBUG
	if (FAILED(m_pRenderTargetManager->Render()))
		return E_FAIL;
#endif
	return S_OK;
}

HRESULT CRenderer::Render(RENDERGROUP eRenderSequence)
{
	for (auto& pGameObject : m_listRenderObjects[eRenderSequence])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
				return E_FAIL;

			Safe_Release(pGameObject);
		}
	}
	m_listRenderObjects[eRenderSequence].clear();

	return S_OK;
}

HRESULT CRenderer::Sequence_Priority()
{
	return Render(RENDER_PRIORITY);
	
}

HRESULT CRenderer::Sequence_Test()
{
	return Render(RENDER_TEST);
}

HRESULT CRenderer::Sequence_Blend()
{
	m_listRenderObjects[RENDER_BLEND].sort([](CGameObject* pSour, CGameObject* pDest)->bool
	{
		return pSour->Get_CameraDistance() > pDest->Get_CameraDistance();
	});
	return Render(RENDER_BLEND);
}

HRESULT CRenderer::Sequence_UI_First()
{
	return Render(RENDER_UI_FIRST);
}

HRESULT CRenderer::Sequence_UI_Second()
{
	return Render(RENDER_UI_SECOND);
}

HRESULT CRenderer::Sequence_UI_Third()
{
	return Render(RENDER_UI_THIRD);
}

HRESULT CRenderer::Sequence_Light()
{
	if(FAILED(m_pRenderTargetManager->Begin_MRT(L"MRT_LightAcc")))
		return E_FAIL;

	m_pLightManager->Render();
		

	if (FAILED(m_pRenderTargetManager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Sequence_Result()
{

	if (FAILED(m_pVIBuffer->Set_Texture("g_texDiffuse", m_pRenderTargetManager->Get_SRV(L"RTV_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Set_Texture("g_texShade", m_pRenderTargetManager->Get_SRV(L"RTV_Shade"))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Set_Texture("g_texSpecular", m_pRenderTargetManager->Get_SRV(L"RTV_Specular"))))
		return E_FAIL;

	m_pVIBuffer->Render(3);

	return S_OK;
}

CRenderer * CRenderer::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CRenderer* pComponent = new CRenderer(pGraphicDevice, pContextDevice);
	if (FAILED(pComponent->NativeConstruct()))
		Safe_Release(pComponent);

	return pComponent;
}

CRenderer * CRenderer::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	CRenderer* pComponent = new CRenderer(pGraphicDevice, pContextDevice);
	if (FAILED(pComponent->NativeConstruct(pShaderFilePath, pTechniqueName)))
		Safe_Release(pComponent);

	return pComponent;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();
	return this;
}

void CRenderer::Free()
{
	__super::Free();

	for (auto& iter : m_listRenderObjects)
	{
		for (auto& pGameObject : iter)
			Safe_Release(pGameObject);
		iter.clear();
	}
	unsigned long dwRefCnt = 0;
	Safe_Release(m_pVIBuffer);
}
