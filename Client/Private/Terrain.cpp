#include "stdafx.h"
#include "..\Public\Terrain.h"


CTerrain::CTerrain(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CGameObject(pGraphicDevice, pContextDevice)
{

}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{

}

void CTerrain::Compute_Distance()
{
}

HRESULT CTerrain::Set_Components(OBJ_INFO tObjInfo, CGameInstance* pGameInstance)
{
	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeRenderer, ComRenderer, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	CVIBuffer::DESC tVIBufferDesc;
	tVIBufferDesc = tObjInfo.Get_VIBufferInfo(tVIBufferDesc);
	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeTerrain, ComVIBuffer, (CComponent**)&m_pVIBuffer, &tObjInfo)))
		return E_FAIL;
	
	

	
	return S_OK;
}

void CTerrain::Set_InitialPosition(OBJ_INFO tObjInfo)
{
	m_pTransform->Set_State(CTransform::STATE_RIGHT, tObjInfo.Get_State(CTransform::STATE_RIGHT));
	m_pTransform->Set_State(CTransform::STATE_UP, tObjInfo.Get_State(CTransform::STATE_UP));
	m_pTransform->Set_State(CTransform::STATE_LOOK, tObjInfo.Get_State(CTransform::STATE_LOOK));
	m_pTransform->Set_State(CTransform::STATE_POSITION, tObjInfo.Get_State(CTransform::STATE_POSITION));
}

CTerrain * CTerrain::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CTerrain* pInstance = new CTerrain(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CTerrain::Clone(void * pArg)
{
	CTerrain* pInstance = new CTerrain(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pRenderer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);
}


HRESULT CTerrain::NativeConstruct()
{
	return S_OK;
}

HRESULT CTerrain::Clone_Construct(void * pArg)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	OBJ_INFO tObjInfo;
	if (pArg != nullptr)
		tObjInfo = *(OBJ_INFO*)pArg;

	if (Add_Component(LEVEL_STATIC, PrototypeRenderer, ComRenderer, (CComponent**)&m_pRenderer))
		return E_FAIL;

	if (Add_Component(LEVEL_STATIC, PrototypeTransform, ComTransform, (CComponent**)&m_pTransform))
		return E_FAIL;
	
	m_pTexture = CTexture::Create(m_pGraphicDevice, m_pContextDevice, tObjInfo.szTextureName);

	CVIBuffer::DESC tVIBufferDesc;
	tVIBufferDesc = tObjInfo.Get_VIBufferInfo(tVIBufferDesc);

	if (Add_Component(LEVEL_STATIC, PrototypeTerrain, ComVIBuffer, (CComponent**)&m_pVIBuffer, &tVIBufferDesc))
		return E_FAIL;

	Set_InitialPosition(tObjInfo);

	return S_OK;
}

_int CTerrain::Tick(_double dTimeDelta)
{
	return _int();
}

_int CTerrain::LateTick(_double dTimeDelta)
{

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
	return __super::LateTick(dTimeDelta);
}

HRESULT CTerrain::Render()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_pVIBuffer->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_matrix));
	m_pVIBuffer->Set_RawValue("g_matView", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_VIEW), sizeof(_matrix));
	m_pVIBuffer->Set_RawValue("g_matProj", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_PROJ), sizeof(_matrix));

	/*const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc();

	m_pVIBuffer->Set_RawValue("g_vLightDir", (void*)&pLightDesc->vDir, sizeof(_float4));
	m_pVIBuffer->Set_RawValue("g_vLightDiffuse", (void*)&pLightDesc->vDiffuse, sizeof(_float4));
	m_pVIBuffer->Set_RawValue("g_vLightAmbient", (void*)&pLightDesc->vAmbient, sizeof(_float4));
	m_pVIBuffer->Set_RawValue("g_vLightSpecular", (void*)&pLightDesc->vSpecular, sizeof(_float4));
	m_pVIBuffer->Set_RawValue("g_vCamPos", &pGameInstance->Get_CameraPosition(), sizeof(_float4));*/


	m_pVIBuffer->Render();

	return S_OK;
}
