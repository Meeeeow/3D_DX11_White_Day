#include "stdafx.h"
#include "ToolTerrain.h"
#include "MainFrm.h"
#include "ToolView.h"

CToolTerrain::CToolTerrain(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CToolBase(pGraphicDevice, pContextDevice)
{
}

HRESULT CToolTerrain::NativeConstruct(OBJ_INFO tObjInfo)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	memcpy(&m_tObjInfo, &tObjInfo, sizeof(OBJ_INFO));
	m_tObjInfo.eBufferType = OBJ_INFO::VIB_TERRAIN;
	m_tObjInfo.eColliderType = OBJ_INFO::COLLIDER_END;
	
	CVIBuffer::DESC tDesc;
	tDesc = m_tObjInfo.Get_VIBufferInfo(tDesc);

	m_pVIBuffer = CVIBuffer_Terrain::Create(m_pGraphicDevice, m_pContextDevice, tDesc);
	m_pTexture = CTexture::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo.szTextureName);

	

	return S_OK;
}

_int CToolTerrain::Tick(_double dTimeDelta)
{
	
	return _int();
}

_int CToolTerrain::LateTick(_double dTimeDelta)
{
	

	

	
	return __super::LateTick(dTimeDelta);
}

HRESULT CToolTerrain::Render()
{
	if (FAILED(Set_ConstantTable()))
		return E_FAIL;

	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CToolTerrain::Change_VIBuffer()
{
	if (m_pVIBuffer != nullptr)
		Safe_Release(m_pVIBuffer);
	CVIBuffer::DESC	tDesc;
	tDesc = m_tObjInfo.Get_VIBufferInfo(tDesc);

	m_pVIBuffer = CVIBuffer_Terrain::Create(m_pGraphicDevice, m_pContextDevice, tDesc);
	m_pTexture = CTexture::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo.szTextureName);
	return S_OK;
}

HRESULT CToolTerrain::Change_Texture(const _tchar * pTextureFilePath)
{
	if (m_pTexture != nullptr)
		Safe_Release(m_pTexture);

	wcscpy_s(m_tObjInfo.szTextureName, pTextureFilePath);
	m_pTexture = CTexture::Create(m_pGraphicDevice, m_pContextDevice, pTextureFilePath);
	
	if (m_pTexture == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CToolTerrain::Set_ConstantTable()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_pVIBuffer->Set_RawValue("g_matWorld", &m_pTransform->Get_WorldTranspose(), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matView", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pVIBuffer->Set_RawValue("g_matProj", &pGameInstance->Get_Transpose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));
	m_pVIBuffer->Set_Texture("g_texDiffuse", m_pTexture->Get_SRV());


	m_pVIBuffer->Set_RawValue("g_vCamPos", (void*)&pGameInstance->Get_CameraPosition(), sizeof(_float4));

	
	return S_OK;
}

HRESULT CToolTerrain::Set_Texture(CTexture * pTexture)
{
	if (m_pTexture != nullptr)
		Safe_Release(m_pTexture);

	m_pTexture = pTexture;

	return S_OK;
}

void CToolTerrain::Picking()
{
	_float3 vPos = _float3(0.f, 0.f, 0.f);
	_float fDistance = 0.f;
	if (Picking(&vPos, fDistance))
	{
		CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
		pView->Set_NavMeshPoint(vPos, fDistance);
	}
}

_bool CToolTerrain::Picking(_float3 * pOut, _float& fDistance)
{
	return m_pVIBuffer->Picking(pOut, m_pTransform->Get_WorldXM(),fDistance);
}



CToolTerrain * CToolTerrain::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo)
{
	CToolTerrain* pInstance = new CToolTerrain(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo)))
		Safe_Release(pInstance);

	return pInstance;
}

void CToolTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pVIBuffer);
}
