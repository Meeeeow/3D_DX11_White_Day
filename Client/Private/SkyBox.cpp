#include "stdafx.h"
#include "..\Public\SkyBox.h"
#include "PositionManager.h"

CSkyBox::CSkyBox(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CGameObject(pGraphicDevice, pContextDevice)
{

}

CSkyBox::CSkyBox(const CSkyBox & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyBox::NativeConstruct()
{
	return S_OK;
}

HRESULT CSkyBox::Clone_Construct(void * pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeCube, ComVIBuffer, (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeTransform, ComTransform, (CComponent**)&m_pTransform)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeRenderer, ComRenderer, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, L"SkyBoxTexture", ComTexture, (CComponent**)&m_pTexture)))
		return E_FAIL;

	return S_OK;
}

_int CSkyBox::Tick(_double dTimeDelta)
{
	return _int();
}

_int CSkyBox::LateTick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_pTransform->Set_State(CTransform::STATE_POSITION, pGameInstance->Get_CameraPosition());
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);

	return 0;
}

HRESULT CSkyBox::Render()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_pVIBuffer->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_matrix));
	m_pVIBuffer->Set_RawValue("g_matView", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_VIEW), sizeof(_matrix));
	m_pVIBuffer->Set_RawValue("g_matProj", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_PROJ), sizeof(_matrix));
	m_pVIBuffer->Set_Texture("g_texDiffuse", m_pTexture->Get_SRV());

	m_pVIBuffer->Render();

	return S_OK;
}

CSkyBox * CSkyBox::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CSkyBox* pInstance = new CSkyBox(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CSkyBox::Clone(void * pArg)
{
	CSkyBox* pInstance = new CSkyBox(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSkyBox::Free()
{
	__super::Free();
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
}

void CSkyBox::Compute_Distance()
{
}
