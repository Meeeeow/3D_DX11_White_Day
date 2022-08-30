#include "stdafx.h"
#include "ToolModel.h"
#include "MainFrm.h"
#include "ToolView.h"
CToolModel::CToolModel(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevie)
	: CToolBase(pGraphicDevice, pContextDevie)
{

}

HRESULT CToolModel::NativeConstruct(const char * pModelFilePath, const char * pModelFileName, const _tchar * pShaderFilePath, const char * pTechniqueName, _fmatrix matPivot)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	
	m_pModel = CModel::Create(m_pGraphicDevice, m_pContextDevice, pModelFilePath, pModelFileName, pShaderFilePath, pTechniqueName, matPivot);
	if (m_pModel == nullptr)
		return E_FAIL;

	

	return S_OK;
}

HRESULT CToolModel::NativeConstruct(OBJ_INFO tObjInfo, _fmatrix matPivot)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	memcpy(&m_tObjInfo, &tObjInfo, sizeof(OBJ_INFO));

	m_pModel = CModel::Create_ForTool(m_pGraphicDevice, m_pContextDevice, m_tObjInfo.szFilePath, m_tObjInfo.szFileName
		, m_tObjInfo.szShaderFilePath, m_tObjInfo.szTehcniqueName, matPivot);

	if (m_pModel == nullptr)
		return E_FAIL;
	

	return S_OK;
}

HRESULT CToolModel::NativeConstructForTool(OBJ_INFO tObjInfo, _fmatrix matPivot)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	memcpy(&m_tObjInfo, &tObjInfo, sizeof(OBJ_INFO));

	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, m_tObjInfo.szFilePath, m_tObjInfo.szFileName
		, m_tObjInfo.szShaderFilePath, m_tObjInfo.szTehcniqueName, matPivot);

	if (m_pModel == nullptr)
		return E_FAIL;

	m_tObjInfo.tColliderDesc;
	if (m_tObjInfo.eColliderType != OBJ_INFO::COLLIDERTYPE::COLLIDER_END)
	{
		CCollider::DESC tCollDesc;
		tCollDesc = m_tObjInfo.tColliderDesc;
		Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tCollDesc);
	}
	
	return S_OK;
}

_int CToolModel::Tick(_double dTimeDelta)
{
	Compute_Distance();
	return __super::Tick(dTimeDelta);
}

_int CToolModel::LateTick(_double dTimeDelta)
{
	m_pModel->Update_CombinedTransformationMatrix(dTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Culling(m_pTransform, m_tObjInfo.fCullRadius) &&
		(!lstrcmp(m_tObjInfo.szObjName, L"Door") || !lstrcmp(m_tObjInfo.szObjName, L"DoorGuard")
			|| !lstrcmp(m_tObjInfo.szObjName, L"DoorRotationM") || !lstrcmp(m_tObjInfo.szObjName, L"DoorRotationP") 
			|| !lstrcmp(m_tObjInfo.szObjName, L"DoorRightZ") || !lstrcmp(m_tObjInfo.szObjName, L"DoorLeftZ")
			|| !lstrcmp(m_tObjInfo.szObjName, L"DoorRightX") || !lstrcmp(m_tObjInfo.szObjName, L"DoorLeftX")
			|| !lstrcmp(m_tObjInfo.szObjName, L"Ghost") || !lstrcmp(m_tObjInfo.szObjName, L"Puzzle0")
			|| !lstrcmp(m_tObjInfo.szObjName, L"Puzzle1") || !lstrcmp(m_tObjInfo.szObjName, L"Puzzle2")))
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
		m_bRender = true;
	}
	else if (pGameInstance->Culling(m_pModel->Get_OnePoint(), m_pTransform->Get_WorldXM(), m_tObjInfo.fCullRadius))
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
		m_bRender = true;
	}
	else if (!lstrcmp(m_tObjInfo.szObjName, L"InsertTalisman"))
	{
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
		m_bRender = true;
	}
	else
		m_bRender = false;

	return 0;
}

HRESULT CToolModel::Render()
{
	if (FAILED(Set_ConstantTable()))
		return E_FAIL;

	_uint iNumMeshes = m_pModel->Get_MeshContainersSize();

	if (FAILED(m_pModel->Bind_Buffer()))
		return E_FAIL;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		m_pModel->Set_MaterialsIndex(i, aiTextureType_DIFFUSE, "g_texDiffuse");
		if(m_pModel->Get_HasAnimation() == true)
			m_pModel->Render(i, 1);
		else
			m_pModel->Render(i, 0);
	}

	if (m_pCollider != nullptr)
		m_pCollider->Render();

	return S_OK;
}

HRESULT CToolModel::Set_ConstantTable()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_pModel->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_matrix));
	m_pModel->Set_RawValue("g_matView", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_VIEW), sizeof(_matrix));
	m_pModel->Set_RawValue("g_matProj", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_PROJ), sizeof(_matrix));


	
	return S_OK;
}

void CToolModel::Set_Animation(_uint iIndex)
{
	if (iIndex > Get_AnimationCount())
		return;

	m_pModel->Set_Animation(iIndex);
}

_uint CToolModel::Get_AnimationCount()
{
	return m_pModel->Get_AnimationCount();
}

const char * CToolModel::Get_AnimationName(_uint iIndex)
{
	return m_pModel->Get_AnimationNames(iIndex);
}

_fvector CToolModel::Get_OnePoint()
{
	return m_pModel->Get_OnePoint();
}

void CToolModel::Picking()
{
	if (m_bRender == false || !lstrcmp(m_tObjInfo.szObjName, L"Ceiling") || !lstrcmp(m_tObjInfo.szObjName, L"Wall")
		|| (!lstrcmp(&m_tObjInfo.szObjName[0],L"D") && !lstrcmp(&m_tObjInfo.szObjName[1], L"o") && !lstrcmp(&m_tObjInfo.szObjName[2], L"o")))
		return;

	_float3 vPos = _float3(0.f, 0.f, 0.f);
	_float fDistance = 0.f;

	if (Picking(&vPos, fDistance))
	{
		CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
		pView->Set_NavMeshPoint(vPos, fDistance);
	}
}

_bool CToolModel::Picking(_float3 * pOut, _float & fDistance)
{
	return m_pModel->Picking(pOut, m_pTransform->Get_WorldXM(), fDistance);
}

void CToolModel::Compute_Distance()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (!lstrcmp(m_tObjInfo.szObjName, L"Door") || !lstrcmp(m_tObjInfo.szObjName, L"DoorGuard")
		|| !lstrcmp(m_tObjInfo.szObjName, L"DoorRotationM") || !lstrcmp(m_tObjInfo.szObjName, L"DoorRotationP")
		|| !lstrcmp(m_tObjInfo.szObjName, L"DoorRightZ") || !lstrcmp(m_tObjInfo.szObjName, L"DoorLeftZ")
		|| !lstrcmp(m_tObjInfo.szObjName, L"DoorRightX") || !lstrcmp(m_tObjInfo.szObjName, L"DoorLeftX"))
	{
		_vector vMyPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
		_vector vCamPos = pGameInstance->Get_CameraPosition();
		m_fCameraDistance = XMVectorGetX(XMVector4Length(vCamPos - vMyPos));
	}
	else
	{
		_vector vMyPos = m_pModel->Get_OnePoint();
		_vector vCamPos = pGameInstance->Get_CameraPosition();
		m_fCameraDistance = XMVectorGetX(XMVector4Length(vCamPos - vMyPos));
	}
	
}

CToolModel * CToolModel::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const char * pModelFilePath, const char * pModelFileName, const _tchar * pShaderFilePath, const char * pTechniqueName, _fmatrix matPivot)
{
	CToolModel* pInstance = new CToolModel(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(pModelFilePath, pModelFileName, pShaderFilePath, pTechniqueName, matPivot)))
		Safe_Release(pInstance);

	return pInstance;
}

CToolModel * CToolModel::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo, _fmatrix matPivot)
{
	CToolModel* pInstance = new CToolModel(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo, matPivot)))
		Safe_Release(pInstance);

	return pInstance;
}

CToolModel * CToolModel::Create_ForLoad(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo, _fmatrix matPivot)
{
	CToolModel* pInstance = new CToolModel(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstructForTool(tObjInfo, matPivot)))
		Safe_Release(pInstance);

	return pInstance;
}

void CToolModel::Free()
{
	__super::Free();
	Safe_Release(m_pModel);
}
