#include "stdafx.h"
#include "..\Public\DefaultModel.h"


CDefaultModel::CDefaultModel(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

CDefaultModel::CDefaultModel(const CDefaultModel & rhs)
	: CModelObject(rhs)
{

}


HRESULT CDefaultModel::NativeConstruct()
{
	return S_OK;
}

HRESULT CDefaultModel::NativeConstruct(OBJ_INFO tObjInfo)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, tObjInfo.szFilePath, tObjInfo.szFileName, Shader_Model, DefaultTechnique);
	m_fCullingRadius = tObjInfo.fCullRadius;
	Set_InitialPosition(tObjInfo);
	

	m_iRenderIndex = 0;
	return S_OK;
}

HRESULT CDefaultModel::Clone_Construct(void * pArg)
{
	// No Use
	if (FAILED(Set_Components()))
		return E_FAIL;

	OBJ_INFO tObjInfo;
	memcpy(&tObjInfo, pArg, sizeof(OBJ_INFO));


	std::string str = tObjInfo.szFileName;
	wstring wstr;
	wstr.assign(str.begin(), str.end());
	Add_Component(LEVEL_QASMOKE, wstr.c_str(), ComVIBuffer, (CComponent**)&m_pModel);

	m_fCullingRadius = tObjInfo.fCullRadius;

	Set_InitialPosition(tObjInfo);
	m_iRenderIndex = 0;

	return S_OK;
	// No Use
}

_int CDefaultModel::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() != LEVEL_QASMOKE)
		return OBJ_NOEVENT;

	Compute_Distance();
	return _int();
}

_int CDefaultModel::LateTick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_bRender = false;
	if (pGameInstance->Culling(m_pModel->Get_OnePoint(), m_pTransform->Get_WorldXM(), m_fCullingRadius) && m_fCameraDistance < 25.f  && pGameInstance->Get_CurrentLevel()->Get_CurrentLevelID() == LEVEL_QASMOKE)
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
		m_bRender = true;
	}


	return 0;
}

HRESULT CDefaultModel::Render()
{
	if (__super::Render())
		return E_FAIL;

	return S_OK;
}

HRESULT CDefaultModel::Set_Components()
{
	if (__super::Set_Components())
		return E_FAIL;

	

	return S_OK;
}

CDefaultModel * CDefaultModel::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CDefaultModel* pInstance = new CDefaultModel(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CDefaultModel * CDefaultModel::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo)
{
	CDefaultModel* pInstance = new CDefaultModel(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo)))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CDefaultModel::Clone(void * pArg)
{
	CDefaultModel* pInstance = new CDefaultModel(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDefaultModel::Free()
{
	__super::Free();
}

void CDefaultModel::Compute_Distance()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_vector vDistance;
	vDistance = XMVector4Length(pGameInstance->Get_CameraPosition() - m_pModel->Get_OnePoint());
	m_fCameraDistance = XMVectorGetX(vDistance);
}
