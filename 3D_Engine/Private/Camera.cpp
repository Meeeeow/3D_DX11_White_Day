#include "..\Public\Camera.h"
#include "PipeLine.h"



CCamera::CCamera(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CGameObject(pGraphicDevice, pContextDevice)
{

}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
{

}

HRESULT CCamera::NativeConstruct()
{
	return S_OK;
}

HRESULT CCamera::Clone_Construct(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_tDesc, pArg, sizeof(DESC));

	m_pTransform = CTransform::Create(m_pGraphicDevice, m_pContextDevice);
	if (m_pTransform == nullptr)
		return E_FAIL;

	if(pArg != nullptr)
		m_pTransform->Set_TransformDesc(m_tDesc.tTransformDesc);

	if (FAILED(Syncronize_Camera()))
		return E_FAIL;

	return S_OK;
}

_int CCamera::Tick(_double dTimeDelta)
{
	CPipeLine*		pPipeLine = CPipeLine::Get_Instance();

	_matrix			matWorld = m_pTransform->Get_WorldXM();
	_matrix			matView = XMMatrixInverse(nullptr, matWorld);
	pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, matView);

	_matrix			matProj;
	matProj = XMMatrixPerspectiveFovLH(m_tDesc.fFOV, m_tDesc.fAspect, m_tDesc.fNear, m_tDesc.fFar);
	pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, matProj);

	pPipeLine->Set_Far(m_tDesc.fFar);


	return _int();
}

_int CCamera::LateTick(_double dTimeDelta)
{
	return _int();
}

HRESULT CCamera::Syncronize_Camera()
{
	_vector		vLook = XMLoadFloat3(&m_tDesc.vAt) - XMLoadFloat3(&m_tDesc.vEye);
	m_pTransform->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight = XMVector3Cross(XMLoadFloat3(&m_tDesc.vAxis), vLook);
	m_pTransform->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp = XMVector3Cross(vLook, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	_vector		vPosition = XMLoadFloat3(&m_tDesc.vEye);
	vPosition = XMVectorSetW(vPosition, 1.f);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);


	return S_OK;
}

void CCamera::Compute_Distance()
{
	m_fCameraDistance = 0.f;
}


HRESULT CCamera::Setting_Target(CGameObject * pTargetObject)
{
	return S_OK;
}


void CCamera::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
}