#include "stdafx.h"
#include "..\Public\Poison.h"
#include "PositionManager.h"

CPoison::CPoison(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{
}

HRESULT CPoison::NativeConstruct(CHauntedPot * pPot)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	_matrix matPivot = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(180.f));

	m_pModel = CModel::Create_ForClient(m_pGraphicDevice, m_pContextDevice, "../Bin/Resource/Meshes/", "Poison_Open.fbx"
		, Shader_Model, DefaultTechnique, matPivot);

	m_pParent = pPot;

	_vector vVector = dynamic_cast<CTransform*>(pPot->Get_Component(ComTransform))->Get_State(CTransform::STATE_POSITION);
	vVector = XMVectorSetY(vVector, XMVectorGetY(vVector) + 1.3f);
	vVector = XMVectorSetX(vVector, XMVectorGetX(vVector) - 0.15f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vVector);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	LIGHTDESC tLightDesc;
	tLightDesc.eLightType = LIGHTDESC::LIGHT_POINT;
	XMStoreFloat4(&tLightDesc.vPos, vVector);
	tLightDesc.vDiffuse = _float4(0.55f, 0.55f, 0.55f, 1.f);
	tLightDesc.vAmbient = _float4(0.55f, 0.55f, 0.55f, 1.f);
	tLightDesc.vSpecular = _float4(0.55f, 0.55f, 0.55f, 1.f);
	tLightDesc.fRange = 1.5f;

	pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice, L"Light_Poison", &tLightDesc);

	pGameInstance->Light_Action(L"Light_Poison");

	return S_OK;
}

_int CPoison::Tick(_double dTimeDelta)
{
	if (m_bDead)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		pGameInstance->Light_DeActive(L"Light_Poison");
		if (m_pParent->Get_Target() == CHauntedPot::JANITOR_BONGGOO)
			CPositionManager::Get_Instance()->Get_LeeBongGoo()->Dead();
		if (m_pParent->Get_Target() == CHauntedPot::JANITOR_DALSU)
			CPositionManager::Get_Instance()->Get_SonDalSu()->Dead();

		m_pParent->Set_Finished();
		return OBJ_DEAD;
	}
	m_dTimeAcc += dTimeDelta;
	if (m_dTimeAcc >= 3.0)
		m_bDead = true;
	return OBJ_NOEVENT;
}

_int CPoison::LateTick(_double dTimeDelta)
{
	if(m_dTimeAcc >= 0.5)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);

	return 0;
}

HRESULT CPoison::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CPoison * CPoison::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, CHauntedPot * pPot)
{
	CPoison* pInstance = new CPoison(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(pPot)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPoison::Free()
{
	__super::Free();
}
