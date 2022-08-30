#include "stdafx.h"
#include "..\Public\DefaultLight.h"

CDefaultLight::CDefaultLight(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CDefaultLight::NativeConstruct(OBJ_INFO tObjInfo, const _tchar* pLightTag)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, tObjInfo.szFilePath, tObjInfo.szFileName, Shader_Model, DefaultTechnique);
	m_fCullingRadius = tObjInfo.fCullRadius;
	Set_InitialPosition(tObjInfo);
	m_iRenderIndex = 0;


	m_tLightDesc.eLightType = LIGHTDESC::LIGHT_POINT;
	_vector vModelPos = m_pModel->Get_OnePoint();
	m_tLightDesc.vPos = _float4(XMVectorGetX(vModelPos), XMVectorGetY(vModelPos) - 0.6f, XMVectorGetZ(vModelPos), 1.f);
	if (!lstrcmp(L"Light", pLightTag))
	{
		m_tLightDesc.fRange = 2.9f;
		m_tLightDesc.vDiffuse = _float4(0.55f, 0.55f, 0.55f, 1.f);
		m_tLightDesc.vAmbient = _float4(0.45f, 0.45f, 0.45f, 1.f);
		m_tLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
		wcscpy_s(m_pLightTag, pLightTag);
	}
	else if (!lstrcmp(L"Light_MachineRoom", pLightTag))
	{
		m_tLightDesc.fRange = 3.9f;
		m_tLightDesc.vDiffuse = _float4(0.65f, 0.f, 0.f, 1.f);
		m_tLightDesc.vAmbient = _float4(0.45f, 0.f, 0.f, 1.f);
		m_tLightDesc.vSpecular = _float4(1.f, 0.f, 0.f, 1.f);
		wcscpy_s(m_pLightTag, pLightTag);
	}
	else
	{
		m_tLightDesc.fRange = 3.6f;
		m_tLightDesc.vDiffuse = _float4(0.55f, 0.55f, 0.55f, 1.f);
		m_tLightDesc.vAmbient = _float4(0.45f, 0.45f, 0.45f, 1.f);
		m_tLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
		wcscpy_s(m_pLightTag, pLightTag);
	}

	return S_OK;
}

_int CDefaultLight::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() != LEVEL_QASMOKE)
		return OBJ_NOEVENT;

	Compute_Distance();
	
	return _int();
}

_int CDefaultLight::LateTick(_double dTimeDelta)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_bRender = false;
	if (pGameInstance->Culling(m_pModel->Get_OnePoint(), m_pTransform->Get_WorldXM(), m_fCullingRadius) && m_fCameraDistance < 31.f  && pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() == LEVEL_QASMOKE)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
		m_bRender = true;
	}

	return 0;
}

HRESULT CDefaultLight::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CDefaultLight::Interaction()
{

}

void CDefaultLight::Event()
{

}

void CDefaultLight::Collect_Event()
{

}

void CDefaultLight::Add_Light()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (FAILED(pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice, m_pLightTag, &m_tLightDesc)))
		VOIDMSG("Failed to Create Light");
}

CDefaultLight * CDefaultLight::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo, const _tchar * pLightTag)
{
	CDefaultLight* pInstance = new CDefaultLight(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo, pLightTag)))
		Safe_Release(pInstance);
	return pInstance;
}

void CDefaultLight::Free()
{
	__super::Free();
}

void CDefaultLight::Compute_Distance()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_vector vDistance;
	vDistance = XMVector4Length(pGameInstance->Get_CameraPosition() - m_pModel->Get_OnePoint());
	m_fCameraDistance = XMVectorGetX(vDistance);
}
