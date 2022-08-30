#include "stdafx.h"
#include "..\Public\Camera_Debug.h"
#include "LeeHeeMin.h"
#include "PositionManager.h"


CCamera_Debug::CCamera_Debug(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CCamera(pGraphicDevice, pContextDevice)
{
}

CCamera_Debug::CCamera_Debug(const CCamera_Debug & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Debug::NativeConstruct()
{
	return S_OK;
}

HRESULT CCamera_Debug::Clone_Construct(void * pArg)
{
	if (FAILED(__super::Clone_Construct(pArg)))
		return E_FAIL;

	CPositionManager::Get_Instance()->Set_Camera(this);
	for (_uint i = 0; i < TARGET_END; ++i)
		m_bTarget[i] = false;

	m_bTarget[TARGET_HEEMIN] = true;
	return S_OK;
}

_int CCamera_Debug::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Check_MouseButtonKey(pGameInstance, dTimeDelta);
	ShowCursor(FALSE);

	if (m_bTarget[TARGET_HEEMIN])
		Trace(pGameInstance, dTimeDelta);
	else if (m_bTarget[TARGET_BONGGOO])
		BongGooTrace(pGameInstance, dTimeDelta);
	else if (m_bTarget[TARGET_DALSU])
		DalSuTrace(pGameInstance, dTimeDelta);
	else if (m_bTarget[TARGET_FUSEBOX])
		FuseBody(pGameInstance, dTimeDelta);
	else if (m_bTarget[TARGET_VALVE0])
		Valve0(pGameInstance, dTimeDelta);
	else if (m_bTarget[TARGET_VALVE1])
		Valve1(pGameInstance, dTimeDelta);

	return __super::Tick(dTimeDelta);
}

_int CCamera_Debug::LateTick(_double dTimeDelta)
{

	return _int();
}

HRESULT CCamera_Debug::Center(CGameInstance * pGameInstance, _double dTimeDelta)
{
	D3D11_VIEWPORT		tViewportDesc;
	ZeroMemory(&tViewportDesc, sizeof(D3D11_VIEWPORT));
	_uint iNumViews = 1;
	m_pContextDevice->RSGetViewports(&iNumViews, &tViewportDesc);
	POINT pt = { tViewportDesc.Width / 2.0f , tViewportDesc.Height / 2.0f };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);

	return S_OK;
}

HRESULT CCamera_Debug::Trace(CGameInstance * pGameInstance, _double dTimeDelta)
{
	Center(pGameInstance, dTimeDelta);
	m_tDesc.fFOV = XMConvertToRadians(60.f);
	CLeeHeeMin* pLeeHeeMin = CPositionManager::Get_Instance()->Get_Player();
	CTransform* pTransform = CPositionManager::Get_Instance()->Get_PlayerTransform();
	_vector vLook = pTransform->Get_State(CTransform::STATE_LOOK);
	_vector vRight = pTransform->Get_State(CTransform::STATE_RIGHT);

	if (FAILED(LookOrtho(XMVector4Normalize(vLook))))
		return E_FAIL;

	_vector vTargetPos = pTransform->Get_State(CTransform::STATE_POSITION);
	if (pLeeHeeMin->Get_Vent() || pLeeHeeMin->Get_Sit())
		vTargetPos = XMVectorSetY(vTargetPos, XMVectorGetY(vTargetPos) + 0.25f);
	else
		vTargetPos = XMVectorSetY(vTargetPos, XMVectorGetY(vTargetPos) + 1.26f);

	
	vTargetPos += XMVector3Normalize(vLook) * _float(m_tDesc.tTransformDesc.dSpeedPerSec * dTimeDelta);

	vTargetPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_POSITION), vTargetPos, 0.45f);

	m_dwMouseY += pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_Y);
	Mouse_MoveControl();
	m_pTransform->Rotation_AxisLerp(dTimeDelta * m_dwMouseY * 0.2f, m_pTransform->Get_State(CTransform::STATE_RIGHT));
	m_pTransform->Set_State(CTransform::STATE_POSITION, vTargetPos);
	return S_OK;
}

HRESULT CCamera_Debug::Check_MouseButtonKey(CGameInstance* pGameInstance, _double dTimeDelta)
{
	if (pGameInstance->Mouse_Down(CInputDevice::DIMB_WB) 
		&& m_bTarget[TARGET_DALSU] == false && m_bTarget[TARGET_BONGGOO] == false && m_bTarget[TARGET_FUSEBOX] == false)
		m_bTarget[TARGET_HEEMIN] = !m_bTarget[TARGET_HEEMIN];

	return S_OK;
}

HRESULT CCamera_Debug::LookOrtho(_fvector & vLook)
{
	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CCamera_Debug::UpOrtho(_fvector & vUp)
{
	

	return S_OK;
}

void CCamera_Debug::BongGooTrace(CGameInstance * pGameInstance, _double dTimeDelta)
{
	if (CPositionManager::Get_Instance()->Get_LeeBongGoo() == nullptr)
	{
		Set_TargetOn(CCamera_Debug::TARGET_HEEMIN);
		return;
	}

	Center(pGameInstance, dTimeDelta);
	m_tDesc.fFOV = XMConvertToRadians(60.f);
	CTransform* pTransform = CPositionManager::Get_Instance()->Get_LeeBongGooTransform();

	_vector		vLook = pTransform->Get_State(CTransform::STATE_LOOK);
	_vector		vRight = pTransform->Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = pTransform->Get_State(CTransform::STATE_UP);

	if (FAILED(LookOrtho(XMVector4Normalize(vLook))))
		return;

	_vector		vTargetPos = pTransform->Get_State(CTransform::STATE_POSITION);
	vTargetPos = XMVectorSetY(vTargetPos, XMVectorGetY(vTargetPos) + 2.5f);
	
	vTargetPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_POSITION), vTargetPos, 0.45f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vTargetPos);
	m_pTransform->Rotation_AxisRadian(XMConvertToRadians(90.f), m_pTransform->Get_State(CTransform::STATE_RIGHT));

	
}

void CCamera_Debug::DalSuTrace(CGameInstance * pGameInstance, _double dTimeDelta)
{
	if (CPositionManager::Get_Instance()->Get_SonDalSu() == nullptr)
	{
		Set_TargetOn(CCamera_Debug::TARGET_HEEMIN);
		return;
	}

	Center(pGameInstance, dTimeDelta);
	m_tDesc.fFOV = XMConvertToRadians(60.f);
	CTransform* pTransform = CPositionManager::Get_Instance()->Get_SonDalSuTransform();

	_vector		vLook = pTransform->Get_State(CTransform::STATE_LOOK);
	_vector		vRight = pTransform->Get_State(CTransform::STATE_RIGHT);
	_vector		vUp = pTransform->Get_State(CTransform::STATE_UP);

	if (FAILED(LookOrtho(XMVector4Normalize(vLook))))
		return;

	_vector		vTargetPos = pTransform->Get_State(CTransform::STATE_POSITION);
	vTargetPos = XMVectorSetY(vTargetPos, XMVectorGetY(vTargetPos) + 2.5f);

	vTargetPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_POSITION), vTargetPos, 0.45f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vTargetPos);
	m_pTransform->Rotation_AxisRadian(XMConvertToRadians(90.f), m_pTransform->Get_State(CTransform::STATE_RIGHT));
}

HRESULT CCamera_Debug::FuseBody(CGameInstance * pGameInstance, _double dTimeDelta)
{
	_vector		vLook = XMVectorSet(22.27f, 1.45f, -5.77f, 0.f) - XMVectorSet(21.27f, 1.45f, -5.77f, 0.f);
	m_pTransform->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	m_pTransform->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp = XMVector3Cross(vLook, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	_vector		vPosition = XMVectorSet(21.27f, 1.45f, -5.77f, 1.f);
	vPosition = XMVectorSetW(vPosition, 1.f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

	m_tDesc.fFOV = XMConvertToRadians(90.f);
	return S_OK;
}

void CCamera_Debug::Valve0(CGameInstance * pGameInstance, _double dTimeDelta)
{
	_vector		vLook = XMVectorSet(9.78f, 2.2f, -2.445f, 0.f) - XMVectorSet(10.1f, 2.2f, -2.445f, 0.f);
	m_pTransform->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	m_pTransform->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp = XMVector3Cross(vLook, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	_vector		vPosition = XMVectorSet(10.1f, 2.2f, -2.445f, 1.f);
	vPosition = XMVectorSetW(vPosition, 1.f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	m_tDesc.fFOV = XMConvertToRadians(45.f);
}

void CCamera_Debug::Valve1(CGameInstance * pGameInstance, _double dTimeDelta)
{
	_vector		vLook = XMVectorSet(14.7f, 2.2f, 0.56f, 0.f) - XMVectorSet(14.48f, 2.2f, 0.56f, 0.f);
	m_pTransform->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	m_pTransform->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp = XMVector3Cross(vLook, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	_vector		vPosition = XMVectorSet(14.48f, 2.2f, 0.56f, 0.f);
	vPosition = XMVectorSetW(vPosition, 1.f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	m_tDesc.fFOV = XMConvertToRadians(45.f);
}

void CCamera_Debug::Set_TargetOn(TARGETLIST eTarget)
{
	for (_uint i = 0; i < TARGET_END; ++i)
	{
		if (i != eTarget)
			m_bTarget[i] = false;
		else
			m_bTarget[eTarget] = true;
	}

}

_bool CCamera_Debug::Get_TargetOn(TARGETLIST eTarget)
{
	return m_bTarget[eTarget];
}

_float4x4 CCamera_Debug::Get_CameraMatrix()
{
	return m_pTransform->Get_World();
}


void CCamera_Debug::Mouse_MoveControl()
{
	if (m_dwMouseY >= 105.f)
		m_dwMouseY = 105.f;
	if (m_dwMouseY <= -105.f)
		m_dwMouseY = -105.f;
}

CCamera_Debug * CCamera_Debug::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CCamera_Debug* pInstance = new CCamera_Debug(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CCamera_Debug::Clone(void * pArg)
{
	CCamera_Debug* pInstance = new CCamera_Debug(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCamera_Debug::Free()
{
	__super::Free();

}
