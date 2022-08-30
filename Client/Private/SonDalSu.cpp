#include "stdafx.h"
#include "..\Public\SonDalSu.h"
#include "LoadDatFiles.h"
#include "PositionManager.h"

CSonDalSu::CSonDalSu(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevcie)
	: CModelObject(pGraphicDevice, pContextDevcie)
{
}

CSonDalSu::CSonDalSu(const CSonDalSu & rhs)
	: CModelObject(rhs)
{
}

HRESULT CSonDalSu::NativeConstruct()
{
	return S_OK;
}

HRESULT CSonDalSu::Clone_Construct(void * pArg)
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(45.f, 3.f, 2.f, 1.f));

	m_iRenderIndex = 1;
	m_bDead = false;
	m_bAlpha = false;
	m_fAlpha = 1.0f;
	m_pModel->Set_Animation(1);
	CPositionManager::Get_Instance()->Set_SonDalSu(this);

	return S_OK;
}

_int CSonDalSu::Tick(_double dTimeDelta)
{
	if (m_bDead)
	{
		CPositionManager::Get_Instance()->Get_Camera()->Set_TargetOn(CCamera_Debug::TARGET_HEEMIN);
		CPositionManager::Get_Instance()->SonDalSuDead();
		CGameInstance::Get_Instance()->Light_DeActive(L"DalSuLantern");
		return OBJ_DEAD;
	}

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	LIGHTDESC& tDesc = pGameInstance->Get_Light(L"DalSuLantern");
	_float4x4 matLantern;
	XMStoreFloat4x4(&matLantern, XMMatrixMultiply(m_pModel->Get_BoneMatrix(L"Lantern"), m_pTransform->Get_WorldXM()));
	XMStoreFloat4(&tDesc.vPos, XMLoadFloat4((_float4*)&matLantern.m[3][0]));

	m_pCollider->Update(m_pTransform->Get_WorldXM());
	

	if (CPositionManager::Get_Instance()->Get_Player()->Get_UseTalisman() == true)
		Move(pGameInstance, dTimeDelta);

	return OBJ_NOEVENT;
}

_int CSonDalSu::LateTick(_double dTimeDelta)
{
	m_pModel->Update_CombinedTransformationMatrix(dTimeDelta);

	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);
	return _int();
}

HRESULT CSonDalSu::Render()
{
	if (m_bAlpha)
	{
		m_fAlpha -= 0.01f;
		if (m_fAlpha <= 0.f)
			m_bDead = true;
		m_pModel->Set_RawValue("fAlpha", &m_fAlpha, sizeof(_float));
		m_iRenderIndex = 3;
	}

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSonDalSu::Set_Components()
{
	if (FAILED(__super::Set_Components()))
		return E_FAIL;
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CTransform::DESC tTransformDesc;
	ZeroMemory(&tTransformDesc, sizeof(tTransformDesc));
	tTransformDesc.dRotationPerSec = 1.f;
	tTransformDesc.dSpeedPerSec = 2.5f;

	m_pTransform->Set_TransformDesc(tTransformDesc);

	CCollider::DESC tColliderDesc;
	ZeroMemory(&tColliderDesc, sizeof(tColliderDesc));
	tColliderDesc.vPivot = _float3(0.f, 0.7f, 0.f);
	tColliderDesc.vScale = _float3(0.2f, 0.6f, 0.2f);
	tColliderDesc.vOrientation = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeOBB, ComCollider, (CComponent**)&m_pCollider, &tColliderDesc)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("Com_SonDalSu"), ComVIBuffer, (CComponent**)&m_pModel)))
		return E_FAIL;
	m_pModel->Create_RefNode(L"Lantern", "FX_Char_Guard_LanternLight");


	LIGHTDESC tDesc;
	tDesc.eLightType = LIGHTDESC::LIGHT_POINT;
	tDesc.fRange = 3.5f;
	tDesc.vDiffuse = _float4(0.55f, 0.55f, 0.15f, 1.f);
	tDesc.vAmbient = _float4(0.55f, 0.55f, 0.15f, 1.f);
	tDesc.vSpecular = _float4(0.55f, 0.55f, 0.15f, 1.f);

	_float4x4 matLantern;
	XMStoreFloat4x4(&matLantern, m_pModel->Get_BoneMatrix(L"Lantern"));
	XMStoreFloat4(&tDesc.vPos, XMLoadFloat4((_float4*)&matLantern.m[3][0]));

	pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice, L"DalSuLantern", &tDesc);

	pGameInstance->Light_Action(L"DalSuLantern");

	return S_OK;
}

void CSonDalSu::Move(CGameInstance * pGameInstance, _double dTimeDelta)
{
	m_pTransform->Move_Trace(dTimeDelta, CPositionManager::Get_Instance()->Get_PlayerPosition()
		, pGameInstance->Get_CurrentNav());

	
}

HRESULT CSonDalSu::Dead()
{
	CPositionManager::Get_Instance()->Get_Camera()->Set_TargetOn(CCamera_Debug::TARGET_DALSU);
	m_bAlpha = true;
	return S_OK;
}

CSonDalSu * CSonDalSu::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CSonDalSu* pInstance = new CSonDalSu(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);
	return pInstance;
}

CGameObject * CSonDalSu::Clone(void * pArg)
{
	CSonDalSu* pInstance = new CSonDalSu(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);
	return pInstance;
}

void CSonDalSu::Free()
{
	__super::Free();
}
