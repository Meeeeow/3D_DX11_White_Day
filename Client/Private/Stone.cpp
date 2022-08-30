#include "stdafx.h"
#include "..\Public\Stone.h"

CStone::CStone(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CStone::NativeConstruct(OBJ_INFO tObjInfo)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, tObjInfo.szFilePath, tObjInfo.szFileName
		, Shader_Model, DefaultTechnique);
	Set_InitialPosition(tObjInfo);

	
	return S_OK;
}

_int CStone::Tick(_double dTimeDelta)
{
	return _int();
}

_int CStone::LateTick(_double dTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
	return _int();
}

HRESULT CStone::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CStone * CStone::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, OBJ_INFO tObjInfo)
{
	CStone* pInstance = new CStone(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tObjInfo)))
		Safe_Release(pInstance);
	return pInstance;
}

void CStone::Free()
{
	__super::Free();
}
