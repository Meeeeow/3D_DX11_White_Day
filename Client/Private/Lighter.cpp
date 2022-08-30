#include "stdafx.h"
#include "..\Public\Lighter.h"
#include "PositionManager.h"

CLighter::CLighter(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CModelObject(pGraphicDevice, pContextDevice)
{
}

HRESULT CLighter::NativeConstruct()
{
	if (FAILED(Set_Components()))
		return E_FAIL;

	_matrix matPivot = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

	m_pModel = CModel::Create_ForTool(m_pGraphicDevice, m_pContextDevice,
		"../Bin/Resource/Meshes/Lighter/", "Lighter.fbx", Shader_Model, DefaultTechnique, matPivot);


	_vector vPosition = CPositionManager::Get_Instance()->Get_PlayerPosition();
	vPosition = XMVectorSetY(vPosition, XMVectorGetY(vPosition) + 0.4);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	
	CTransform::DESC tTransformDesc;
	tTransformDesc.dRotationPerSec = 2.0;
	tTransformDesc.dSpeedPerSec = 2.0;

	m_pTransform->Set_TransformDesc(tTransformDesc);

	LIGHTDESC tLightDesc;
	tLightDesc.eLightType = LIGHTDESC::LIGHT_POINT;
	tLightDesc.fRange = 2.5f;
	XMStoreFloat4(&tLightDesc.vPos, vPosition);
	tLightDesc.vDiffuse = _float4(0.6f, 0.15f, 0.15f, 1.f);
	tLightDesc.vAmbient = _float4(0.35f, 0.35f, 0.35f, 1.f);
	tLightDesc.vSpecular = _float4(1.f, 0.f, 0.f, 1.f);


	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice, L"Lighter", &tLightDesc);

	return S_OK;
}

_int CLighter::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (CPositionManager::Get_Instance()->Get_Player()->Get_Lighter() == true)
		Light(pGameInstance, dTimeDelta);
	else if (CPositionManager::Get_Instance()->Get_Player()->Get_Lighter() == false)
		NonLight(pGameInstance, dTimeDelta);

	return _int();
}

_int CLighter::LateTick(_double dTimeDelta)
{
	if (CPositionManager::Get_Instance()->Get_Player()->Get_Lighter() == true)
		m_pRenderer->Add_RenderGroup(CRenderer::RENDER_TEST, this);

	return _int();
}

HRESULT CLighter::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CLighter::Light(CGameInstance* pGameInstance, _double dTimeDelta)
{
	pGameInstance->Light_Action(L"Lighter");

	LIGHTDESC& tLightDesc = pGameInstance->Get_Light(L"Lighter");
	CTransform* pTransform = CPositionManager::Get_Instance()->Get_PlayerTransform();

	_float4x4 matLighter;
	_matrix matMix = CPositionManager::Get_Instance()->Get_Player()->Get_BoneMatrix(L"RightHand") * CPositionManager::Get_Instance()->Get_PlayerTransform()->Get_WorldXM();
	XMStoreFloat4x4(&matLighter, matMix);
	_vector vPosition = XMLoadFloat4((_float4*)&matLighter.m[3][0]);
	vPosition += XMVector3Normalize(pTransform->Get_State(CTransform::STATE_RIGHT)) * 48.1f * dTimeDelta;
	vPosition += XMVector3Normalize(pTransform->Get_State(CTransform::STATE_LOOK)) * -8.8f * dTimeDelta;
	XMStoreFloat4(&tLightDesc.vPos, vPosition);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	_double dwMouseX;
	dwMouseX = pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_X);
	m_pTransform->Rotation_Axis(dTimeDelta * dwMouseX * 0.2f);

	//CPositionManager::Get_Instance()->Get_PlayerTransform()->Get_WorldXM();


	/*_vector vLook = pTransform->Get_State(CTransform::STATE_LOOK);
	_vector vRight = pTransform->Get_State(CTransform::STATE_RIGHT);
	_vector vPosition = pTransform->Get_State(CTransform::STATE_POSITION);*/




	/*vPosition += XMVector3Normalize(vLook) * 19.f * dTimeDelta;
	vPosition += XMVector3Normalize(vRight) * 0.45f * dTimeDelta;*/

	
	
	/*vPosition = XMVector4Transform(vPosition, CPositionManager::Get_Instance()->Get_Player()->Get_BoneMatrix(L"RightHand"));
	_float fHeight = XMVectorGetY(vPosition) + 1.0f;
	vPosition = XMVectorSetY(vPosition, fHeight);
	vPosition = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_POSITION),vPosition , 0.5f);
	XMStoreFloat4(&tLightDesc.vPos, vPosition);*/

	
}

void CLighter::NonLight(CGameInstance* pGameInstance, _double dTimeDelta)
{
	pGameInstance->Light_DeActive(L"Lighter");

	LIGHTDESC& tLightDesc = pGameInstance->Get_Light(L"Lighter");

	CTransform* pTransform = CPositionManager::Get_Instance()->Get_PlayerTransform();

	_vector vLook = pTransform->Get_State(CTransform::STATE_LOOK);
	_vector vPosition = pTransform->Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vLook) * 6.6f * dTimeDelta;
	_float fHeight = XMVectorGetY(vPosition);
	vPosition = XMVectorSetY(vPosition, fHeight);

	vPosition = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_POSITION), vPosition, 0.45f);

	XMStoreFloat4(&tLightDesc.vPos, vPosition);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
}

CLighter * CLighter::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CLighter* pInstance = new CLighter(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLighter::Free()
{
	__super::Free();
}
