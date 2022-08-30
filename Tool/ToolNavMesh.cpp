#include "stdafx.h"
#include "ToolNavMesh.h"
#include "PipeLine.h"
#include "MainFrm.h"
#include "ToolView.h"


CToolNavMesh::CToolNavMesh(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CToolBase(pGraphicDevice, pContextDevice)
{

}

HRESULT CToolNavMesh::NativeConstruct(_float3 v0, _float3 v1, _float3 v2)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;
	

	m_vNavMeshPoints[0] = v0;
	m_vNavMeshPoints[1] = v1;
	m_vNavMeshPoints[2] = v2;
	CCollider::DESC tColliderDesc;

	tColliderDesc.fRadius = 0.02f;
	tColliderDesc.vPivot = v0;
	tColliderDesc.vPivot.y += 0.02f;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_pSphereCollider[0] = dynamic_cast<CCollider*>(pGameInstance->Clone_Component(LEVEL_STATIC, PrototypeSphere, (void*)&tColliderDesc));
	tColliderDesc.vPivot = v1;
	tColliderDesc.vPivot.y += 0.02f;
	m_pSphereCollider[1] = dynamic_cast<CCollider*>(pGameInstance->Clone_Component(LEVEL_STATIC, PrototypeSphere, (void*)&tColliderDesc));
	tColliderDesc.vPivot = v2;
	tColliderDesc.vPivot.y += 0.02f;
	m_pSphereCollider[2] = dynamic_cast<CCollider*>(pGameInstance->Clone_Component(LEVEL_STATIC, PrototypeSphere, (void*)&tColliderDesc));
	

	m_pVIBuffer = CVIBuffer_Triangle::Create(m_pGraphicDevice, m_pContextDevice, Shader_Navigation, DefaultTechnique, m_vNavMeshPoints);

	return S_OK;
}

HRESULT CToolNavMesh::NativeConstruct(OBJ_INFO tObjInfo)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	memcpy(&m_tObjInfo, &tObjInfo, sizeof(OBJ_INFO));

	m_vNavMeshPoints[0] = m_tObjInfo.vNaviPoints[0];
	m_vNavMeshPoints[1] = m_tObjInfo.vNaviPoints[1];
	m_vNavMeshPoints[2] = m_tObjInfo.vNaviPoints[2];
 	CCollider::DESC tColliderDesc;

	tColliderDesc.fRadius = 0.15f;
	tColliderDesc.vPivot = m_tObjInfo.vNaviPoints[0];
	tColliderDesc.vPivot.y += 0.02f;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_pSphereCollider[0] = dynamic_cast<CCollider*>(pGameInstance->Clone_Component(LEVEL_STATIC, PrototypeSphere, (void*)&tColliderDesc));
	tColliderDesc.vPivot = m_tObjInfo.vNaviPoints[1];
	tColliderDesc.vPivot.y += 0.02f;
	m_pSphereCollider[1] = dynamic_cast<CCollider*>(pGameInstance->Clone_Component(LEVEL_STATIC, PrototypeSphere, (void*)&tColliderDesc));
	tColliderDesc.vPivot = m_tObjInfo.vNaviPoints[2];
	tColliderDesc.vPivot.y += 0.02f;
	m_pSphereCollider[2] = dynamic_cast<CCollider*>(pGameInstance->Clone_Component(LEVEL_STATIC, PrototypeSphere, (void*)&tColliderDesc));
	
	m_fY = (m_vNavMeshPoints[0].y + m_vNavMeshPoints[1].y + m_vNavMeshPoints[2].y) / 3.f;
	m_pVIBuffer = CVIBuffer_Triangle::Create(m_pGraphicDevice, m_pContextDevice, Shader_Navigation, DefaultTechnique, m_vNavMeshPoints);

	return S_OK;
}

_int CToolNavMesh::Tick(_double dTimeDelta)
{
	for (_uint i = 0; i < 3; ++i)
		m_pSphereCollider[i]->Update(m_pTransform->Get_WorldXM());
	Compute_Distance();
	return _int();
}

_int CToolNavMesh::LateTick(_double dTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_BLEND, this);
	return 0;
}

HRESULT CToolNavMesh::Render()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_pVIBuffer->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_matrix));
	m_pVIBuffer->Set_RawValue("g_matView", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_VIEW), sizeof(_matrix));
	m_pVIBuffer->Set_RawValue("g_matProj", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_PROJ), sizeof(_matrix));

	if (!m_bTarget)
		m_pVIBuffer->Render();
	else
		m_pVIBuffer->Render(1);

	for (_uint i = 0; i < 3; ++i)
		m_pSphereCollider[i]->Render();
	

	return S_OK;
}

void CToolNavMesh::Picking()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_float3 vPos = _float3(0.f, 0.f, 0.f);
	_float fDistance = 0.f;
	_fvector vRay = pGameInstance->Get_Ray();
	_fvector vOrigin = pGameInstance->Get_Pivot();
	if (Picking(vOrigin, vRay, vPos, fDistance))
	{
		CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
		vPos.y -= 0.02f;
		pView->Set_NavMeshPoint(vPos, fDistance);
	}
	
}

_bool CToolNavMesh::Picking(_fvector vOrigin, _fvector vRay, _float3& vPos, _float & fDistance)
{
	if (m_pSphereCollider[0]->Collision_Sphere(vOrigin, vRay, vPos, fDistance))
		return true;
	if (m_pSphereCollider[1]->Collision_Sphere(vOrigin, vRay, vPos, fDistance))
		return true;
	if (m_pSphereCollider[2]->Collision_Sphere(vOrigin, vRay, vPos, fDistance))
		return true;
	return false;
}

void CToolNavMesh::Compute_Distance()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_vector		vCamPos = pGameInstance->Get_CameraPosition();
	_float		vCamY = XMVectorGetY(vCamPos);
	m_fCameraDistance = vCamY - m_fY;
}

CToolNavMesh * CToolNavMesh::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, _float3 v0, _float3 v1, _float3 v2)
{
	CToolNavMesh* pInstance = new CToolNavMesh(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(v0, v1, v2)))
		Safe_Release(pInstance);

 	return pInstance;
}

CToolNavMesh * CToolNavMesh::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo)
{
	CToolNavMesh* pInstance = new CToolNavMesh(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo)))
		Safe_Release(pInstance);

	return pInstance;
}

void CToolNavMesh::Free()
{
	__super::Free();
	Safe_Release(m_pVIBuffer);

	for (_uint i = 0; i < 3; ++i)
		Safe_Release(m_pSphereCollider[i]);
}
