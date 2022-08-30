#include "stdafx.h"
#include "..\Public\ModelObject.h"

CModelObject::CModelObject(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CGameObject(pGraphicDevice, pContextDevice)
{

}

CModelObject::CModelObject(const CModelObject & rhs)
	: CGameObject(rhs), m_fCullingRadius(rhs.m_fCullingRadius), m_iRenderIndex(rhs.m_iRenderIndex)
{

}

HRESULT CModelObject::NativeConstruct()
{
	return S_OK;
}

HRESULT CModelObject::Clone_Construct(void * pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	OBJ_INFO tObjectInfo;
	memcpy(&tObjectInfo, pArg, sizeof(OBJ_INFO));

	return S_OK;
}

_int CModelObject::Tick(_double dTimeDelta)
{
	m_pCollider->Update(m_pTransform->Get_WorldXM());
	return OBJ_NOEVENT;
}

_int CModelObject::LateTick(_double dTimeDelta)
{
	m_pModel->Update_CombinedTransformationMatrix(dTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Culling(m_pTransform, m_fCullingRadius))
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);

	return _int();
}

HRESULT CModelObject::Render()
{
	if (FAILED(Set_ConstantTable()))
		return E_FAIL;

	_uint iNumMeshes = m_pModel->Get_MeshContainersSize();

	if (FAILED(m_pModel->Bind_Buffer()))
		return E_FAIL;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModel->Set_MaterialsIndex(i, aiTextureType_DIFFUSE, "g_texDiffuse");
		m_pModel->Render(i, m_iRenderIndex);
	}

	return S_OK;
}

void CModelObject::Interaction()
{

}

void CModelObject::Set_InitialPosition(OBJ_INFO tObjInfo)
{
	m_pTransform->Set_State(CTransform::STATE_RIGHT, tObjInfo.Get_State(CTransform::STATE_RIGHT));
	m_pTransform->Set_State(CTransform::STATE_UP, tObjInfo.Get_State(CTransform::STATE_UP));
	m_pTransform->Set_State(CTransform::STATE_LOOK, tObjInfo.Get_State(CTransform::STATE_LOOK));
	m_pTransform->Set_State(CTransform::STATE_POSITION, tObjInfo.Get_State(CTransform::STATE_POSITION));
}

HRESULT CModelObject::Set_Components()
{
	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeRenderer, ComRenderer, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeTransform, ComTransform, (CComponent**)&m_pTransform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModelObject::Set_ConstantTable()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_pModel->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_matrix));
	m_pModel->Set_RawValue("g_matView", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_VIEW), sizeof(_matrix));
	m_pModel->Set_RawValue("g_matProj", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_PROJ), sizeof(_matrix));

	_float fFar = pGameInstance->Get_Far();
	m_pModel->Set_RawValue("g_fFar", &fFar, sizeof(_float));
	/*const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc();

	m_pModel->Set_RawValue("g_vLightDir", (void*)&pLightDesc->vDir, sizeof(_float4));
	m_pModel->Set_RawValue("g_vLightDiffuse", (void*)&pLightDesc->vDiffuse, sizeof(_float4));
	m_pModel->Set_RawValue("g_vLightAmbient", (void*)&pLightDesc->vAmbient, sizeof(_float4));
	m_pModel->Set_RawValue("g_vLightSpecular", (void*)&pLightDesc->vSpecular, sizeof(_float4));
	m_pModel->Set_RawValue("g_vCamPos", &pGameInstance->Get_CameraPosition(), sizeof(_float4));*/

	return S_OK;
}

CGameObject * CModelObject::Clone(void * pArg)
{
	return nullptr;
}

void CModelObject::Free()
{
	Safe_Release(m_pCollider);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);

	__super::Free();

}

void CModelObject::Compute_Distance()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_vector vV;
	vV = XMVector4Length(pGameInstance->Get_CameraPosition() - m_pTransform->Get_State(CTransform::STATE_POSITION));
	m_fCameraDistance = XMVectorGetX(vV);

}
