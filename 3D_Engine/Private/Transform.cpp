#include "..\Public\Transform.h"
#include "NavigationMesh.h"

CTransform::CTransform(_pGraphicDevice * pGraphicDeivce, _pContextDevice * pContextDevice)
	: CComponent(pGraphicDeivce,pContextDevice)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_matWorld(rhs.m_matWorld)
	, m_tDesc(rhs.m_tDesc)
{
	
}

HRESULT CTransform::NativeConstruct()
{
	XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());
	ZeroMemory(&m_tDesc, sizeof(DESC));
	
	return S_OK;
}

HRESULT CTransform::Clone_Construct(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_tDesc, pArg, sizeof(DESC));

	return S_OK;
}

void CTransform::Set_WorldMatrix(_matrix matWorld)
{
	XMStoreFloat4x4(&m_matWorld, matWorld);
}

void CTransform::Move_Front(_double dTimeDelta, CNavigationMesh* pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * _float(m_tDesc.dSpeedPerSec * dTimeDelta);

	if (pNavigation != nullptr)
	{
		CCell* pCell = nullptr;
		if(pNavigation->IsOn(vPosition,&pCell, m_iCurrentIndex))
			Set_State(CTransform::STATE_POSITION, vPosition);
		else
		{
			if (pCell != nullptr)
			{
				_vector vNormal = pCell->IsSlide(Get_State(STATE_POSITION));
				_vector vSlidingVector = XMVector3Normalize(XMVector3Normalize(vLook) - XMVector3Dot(vLook, vNormal) * vNormal);
				_float  fAngle = XMVectorGetX(XMVector3Dot(vSlidingVector, XMVector3Normalize(vLook)));

				_vector vSlidePos = Get_State(STATE_POSITION);
				vSlidePos += XMVector3Normalize(vSlidingVector) * (_float)m_tDesc.dSpeedPerSec * (_float)dTimeDelta * fAngle;
				
				if(pNavigation->IsOn(vSlidePos, &pCell, m_iCurrentIndex))
					Set_State(CTransform::STATE_POSITION, vSlidePos);
			}
		}
	}
	else
		Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Move_Back(_double dTimeDelta, CNavigationMesh* pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * _float(m_tDesc.dSpeedPerSec * dTimeDelta);

	if (pNavigation != nullptr)
	{
		CCell* pCell = nullptr;
		if (pNavigation->IsOn(vPosition, &pCell, m_iCurrentIndex))
			Set_State(CTransform::STATE_POSITION, vPosition);
		else
		{
			if (pCell != nullptr)
			{
				_vector vNormal = pCell->IsSlide(Get_State(STATE_POSITION));
				_vector vSlidingVector = XMVector3Normalize(XMVector3Normalize(vLook) - XMVector3Dot(vLook, vNormal) * vNormal);
				_float  fAngle = XMVectorGetX(XMVector3Dot(vSlidingVector, XMVector3Normalize(vLook)));

				_vector vSlidePos = Get_State(STATE_POSITION);
				vSlidePos += XMVector3Normalize(vSlidingVector) * (_float)m_tDesc.dSpeedPerSec * (_float)dTimeDelta * fAngle;

				if (pNavigation->IsOn(vSlidePos, &pCell, m_iCurrentIndex))
					Set_State(CTransform::STATE_POSITION, vSlidePos);
			}
		}
	}
	else
		Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Move_Left(_double dTimeDelta, CNavigationMesh* pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);
	_vector vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * _float(m_tDesc.dSpeedPerSec * dTimeDelta);

	if (pNavigation != nullptr)
	{
		CCell* pCell = nullptr;
		if (pNavigation->IsOn(vPosition, &pCell, m_iCurrentIndex))
			Set_State(CTransform::STATE_POSITION, vPosition);
		else
		{
			if (pCell != nullptr)
			{
				_vector vNormal = pCell->IsSlide(Get_State(STATE_POSITION));
				_vector vSlidingVector = XMVector3Normalize(XMVector3Normalize(vRight) - XMVector3Dot(vRight, vNormal) * vNormal);
				_float  fAngle = XMVectorGetX(XMVector3Dot(vSlidingVector, XMVector3Normalize(vRight)));

				_vector vSlidePos = Get_State(STATE_POSITION);
				vSlidePos += XMVector3Normalize(vSlidingVector) * (_float)m_tDesc.dSpeedPerSec * (_float)dTimeDelta * fAngle;

				if (pNavigation->IsOn(vSlidePos, &pCell, m_iCurrentIndex))
					Set_State(CTransform::STATE_POSITION, vSlidePos);
			}
		}
	}
	else
		Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Move_Right(_double dTimeDelta, CNavigationMesh* pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);
	_vector vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * _float(m_tDesc.dSpeedPerSec * dTimeDelta);

	if (pNavigation != nullptr)
	{
		CCell* pCell = nullptr;
		if (pNavigation->IsOn(vPosition, &pCell, m_iCurrentIndex))
			Set_State(CTransform::STATE_POSITION, vPosition);
		else
		{
			if (pCell != nullptr)
			{
				_vector vNormal = pCell->IsSlide(Get_State(STATE_POSITION));
				_vector vSlidingVector = XMVector3Normalize(XMVector3Normalize(vRight) - XMVector3Dot(vRight, vNormal) * vNormal);
				_float  fAngle = XMVectorGetX(XMVector3Dot(vSlidingVector, XMVector3Normalize(vRight)));

				_vector vSlidePos = Get_State(STATE_POSITION);
				vSlidePos += XMVector3Normalize(vSlidingVector) * (_float)m_tDesc.dSpeedPerSec * (_float)dTimeDelta * fAngle;

				if (pNavigation->IsOn(vSlidePos, &pCell, m_iCurrentIndex))
					Set_State(CTransform::STATE_POSITION, vSlidePos);
			}
		}
	}
	else
		Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Move_VentFront(_double dTimeDelta, CNavigationMesh * pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * _float(m_tDesc.dSpeedPerSec * dTimeDelta);

	if (pNavigation != nullptr)
	{
		CCell* pCell = nullptr;
		if (pNavigation->IsOn_Vent(vPosition, &pCell, m_iCurrentIndex))
			Set_State(CTransform::STATE_POSITION, vPosition);
		else
		{
			if (pCell != nullptr)
			{
				_vector vNormal = pCell->IsSlide(Get_State(STATE_POSITION));
				_vector vSlidingVector = XMVector3Normalize(XMVector3Normalize(vLook) - XMVector3Dot(vLook, vNormal) * vNormal);
				_float  fAngle = XMVectorGetX(XMVector3Dot(vSlidingVector, XMVector3Normalize(vLook)));

				_vector vSlidePos = Get_State(STATE_POSITION);
				vSlidePos += XMVector3Normalize(vSlidingVector) * (_float)m_tDesc.dSpeedPerSec * (_float)dTimeDelta * fAngle;

				if (pNavigation->IsOn_Vent(vSlidePos, &pCell, m_iCurrentIndex))
					Set_State(CTransform::STATE_POSITION, vSlidePos);
			}
		}
	}
	else
		Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Move_VentBack(_double dTimeDelta, CNavigationMesh * pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * _float(m_tDesc.dSpeedPerSec * dTimeDelta);

	if (pNavigation != nullptr)
	{
		CCell* pCell = nullptr;
		if (pNavigation->IsOn_Vent(vPosition, &pCell, m_iCurrentIndex))
			Set_State(CTransform::STATE_POSITION, vPosition);
		else
		{
			if (pCell != nullptr)
			{
				_vector vNormal = pCell->IsSlide(Get_State(STATE_POSITION));
				_vector vSlidingVector = XMVector3Normalize(XMVector3Normalize(vLook) - XMVector3Dot(vLook, vNormal) * vNormal);
				_float  fAngle = XMVectorGetX(XMVector3Dot(vSlidingVector, XMVector3Normalize(vLook)));

				_vector vSlidePos = Get_State(STATE_POSITION);
				vSlidePos += XMVector3Normalize(vSlidingVector) * (_float)m_tDesc.dSpeedPerSec * (_float)dTimeDelta * fAngle;

				if (pNavigation->IsOn_Vent(vSlidePos, &pCell, m_iCurrentIndex))
					Set_State(CTransform::STATE_POSITION, vSlidePos);
			}
		}
	}
	else
		Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Move_VentLeft(_double dTimeDelta, CNavigationMesh * pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);
	_vector vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * _float(m_tDesc.dSpeedPerSec * dTimeDelta);

	if (pNavigation != nullptr)
	{
		CCell* pCell = nullptr;
		if (pNavigation->IsOn_Vent(vPosition, &pCell, m_iCurrentIndex))
			Set_State(CTransform::STATE_POSITION, vPosition);
		else
		{
			if (pCell != nullptr)
			{
				_vector vNormal = pCell->IsSlide(Get_State(STATE_POSITION));
				_vector vSlidingVector = XMVector3Normalize(XMVector3Normalize(vRight) - XMVector3Dot(vRight, vNormal) * vNormal);
				_float  fAngle = XMVectorGetX(XMVector3Dot(vSlidingVector, XMVector3Normalize(vRight)));

				_vector vSlidePos = Get_State(STATE_POSITION);
				vSlidePos += XMVector3Normalize(vSlidingVector) * (_float)m_tDesc.dSpeedPerSec * (_float)dTimeDelta * fAngle;

				if (pNavigation->IsOn_Vent(vSlidePos, &pCell, m_iCurrentIndex))
					Set_State(CTransform::STATE_POSITION, vSlidePos);
			}
		}
	}
	else
		Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Move_VentRight(_double dTimeDelta, CNavigationMesh * pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);
	_vector vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * _float(m_tDesc.dSpeedPerSec * dTimeDelta);

	if (pNavigation != nullptr)
	{
		CCell* pCell = nullptr;
		if (pNavigation->IsOn_Vent(vPosition, &pCell, m_iCurrentIndex))
			Set_State(CTransform::STATE_POSITION, vPosition);
		else
		{
			if (pCell != nullptr)
			{
				_vector vNormal = pCell->IsSlide(Get_State(STATE_POSITION));
				_vector vSlidingVector = XMVector3Normalize(XMVector3Normalize(vRight) - XMVector3Dot(vRight, vNormal) * vNormal);
				_float  fAngle = XMVectorGetX(XMVector3Dot(vSlidingVector, XMVector3Normalize(vRight)));

				_vector vSlidePos = Get_State(STATE_POSITION);
				vSlidePos += XMVector3Normalize(vSlidingVector) * (_float)m_tDesc.dSpeedPerSec * (_float)dTimeDelta * fAngle;

				if (pNavigation->IsOn_Vent(vSlidePos, &pCell, m_iCurrentIndex))
					Set_State(CTransform::STATE_POSITION, vSlidePos);
			}
		}
	}
	else
		Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Move_Trace(_double dTimeDelta, _fvector vGoalPos, CNavigationMesh * pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);

	_vector vUp = Get_State(CTransform::STATE_UP);
	_vector vLook = vGoalPos - vPosition;
	_vector vScale = Get_Scale();
	_vector vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook)) * XMVectorGetX(vScale);
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp)) * XMVectorGetZ(vScale);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_LOOK, vLook);

	vPosition += XMVector3Normalize(vLook) * _float(dTimeDelta * m_tDesc.dSpeedPerSec);
	
	if (pNavigation != nullptr)
	{
		CCell* pCell = nullptr;
		if (pNavigation->IsOn(vPosition, &pCell, m_iCurrentIndex))
			Set_State(CTransform::STATE_POSITION, vPosition);
		else
		{
			if (pCell != nullptr)
			{
				_vector vNormal = pCell->IsSlide(Get_State(STATE_POSITION));
				_vector vSlidingVector = XMVector3Normalize(XMVector3Normalize(vLook) - XMVector3Dot(vLook, vNormal) * vNormal);
				_float  fAngle = XMVectorGetX(XMVector3Dot(vSlidingVector, XMVector3Normalize(vLook)));

				_vector vSlidePos = Get_State(STATE_POSITION);
				vSlidePos += XMVector3Normalize(vSlidingVector) * (_float)m_tDesc.dSpeedPerSec * (_float)dTimeDelta * fAngle;

				if (pNavigation->IsOn(vSlidePos, &pCell, m_iCurrentIndex))
					Set_State(CTransform::STATE_POSITION, vSlidePos);
			}
		}
	}
	else
		Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::LookAt(_double dTimeDelta, _fvector vTargetPos)
{
	_vector vPosition = Get_State(CTransform::STATE_POSITION);

	_vector vUp = Get_State(CTransform::STATE_UP);
	_vector vLook = vTargetPos - vPosition;
	_vector vScale = Get_Scale();
	_vector vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook)) * XMVectorGetX(vScale);
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp)) * XMVectorGetZ(vScale);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Move_MouseX(_double dPower)
{
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition = XMVectorSetZ(vPosition, XMVectorGetZ(vPosition) - (dPower * 0.2 * m_tDesc.dSpeedPerSec));

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Move_MouseY(_double dPower)
{
	_vector vPosition = Get_State(STATE_POSITION);


	vPosition = XMVectorSetY(vPosition, XMVectorGetY(vPosition) - (dPower * 0.2 * m_tDesc.dSpeedPerSec));

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Mouse_Chase(_fvector vGoalPos, _double dTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vDir = vGoalPos - vPosition;

	Set_State(STATE_POSITION, vPosition + XMVector3Normalize(vDir) * dTimeDelta);
}

void CTransform::Rotation_Axis(_double dTimeDelta, _fvector vAxis)
{
	_vector	vRight = Get_State(CTransform::STATE_RIGHT);
	_vector	vUp = Get_State(CTransform::STATE_UP);
	_vector	vLook = Get_State(CTransform::STATE_LOOK);

	_matrix	matRotation = XMMatrixRotationAxis(vAxis, _float(m_tDesc.dSpeedPerSec * dTimeDelta));

	vRight = XMVector4Transform(vRight, matRotation);
	vUp = XMVector4Transform(vUp, matRotation);
	vLook = XMVector4Transform(vLook, matRotation);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Rotation_AxisRadian(_float fRadian, _fvector vAxis)
{
	_vector	vRight = Get_State(CTransform::STATE_RIGHT);
	_vector	vUp = Get_State(CTransform::STATE_UP);
	_vector	vLook = Get_State(CTransform::STATE_LOOK);

	_matrix	matRotation = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector3TransformNormal(vRight, matRotation);
	vUp = XMVector3TransformNormal(vUp, matRotation);
	vLook = XMVector3TransformNormal(vLook, matRotation);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Rotation_Radian(_float fRadian, _fvector vAxis)
{
	_matrix		matRotation = XMMatrixRotationAxis(vAxis, fRadian);

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scale(STATE_RIGHT);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scale(STATE_UP);
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scale(STATE_LOOK);

	vRight = XMVector3TransformNormal(vRight, matRotation);
	vUp = XMVector3TransformNormal(vUp, matRotation);
	vLook = XMVector3TransformNormal(vLook, matRotation);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotation_Radian_Acc(_float fRadian, _fvector vAxis)
{
	_vector	vRight = Get_State(CTransform::STATE_RIGHT);
	_vector	vUp = Get_State(CTransform::STATE_UP);
	_vector	vLook = Get_State(CTransform::STATE_LOOK);

	_matrix	matRotation = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector4Transform(vRight, matRotation);
	vUp = XMVector4Transform(vUp, matRotation);
	vLook = XMVector4Transform(vLook, matRotation);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Scaling(_fvector vScale)
{
	_vector	vRight = Get_State(CTransform::STATE_RIGHT);
	_vector	vUp = Get_State(CTransform::STATE_UP);
	_vector	vLook = Get_State(CTransform::STATE_LOOK);

	vRight = XMVector3Normalize(vRight) * XMVectorGetX(vScale);
	vUp = XMVector3Normalize(vUp) * XMVectorGetY(vScale);
	vLook = XMVector3Normalize(vLook) * XMVectorGetZ(vScale);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Rotation_AxisLerp(_double dTimeDelta, _fvector vAxis)
{
	_matrix		matRotation = XMMatrixRotationAxis(vAxis, m_tDesc.dRotationPerSec * dTimeDelta);

	_vector	vRight = Get_State(CTransform::STATE_RIGHT);
	_vector	vUp = Get_State(CTransform::STATE_UP);
	_vector	vLook = Get_State(CTransform::STATE_LOOK);

	_vector		vResultRight = XMVector3TransformNormal(vRight, matRotation);
	_vector		vResultUp = XMVector3TransformNormal(vUp, matRotation);
	_vector		vResultLook = XMVector3TransformNormal(vLook, matRotation);

	vResultRight = XMQuaternionSlerp(vRight, vResultRight , 1.f);
	vResultUp = XMQuaternionSlerp(vUp, vResultUp, 1.f);
	vResultLook = XMQuaternionSlerp(vLook, vResultLook, 1.f);

	//vResultRight = XMVectorLerp(vRight, vResultRight, 1.f);
	//vResultUp = XMVectorLerp(vUp, vResultUp, 1.f);
	//vResultLook = XMVectorLerp(vLook, vResultLook, 1.f);

	Set_State(STATE_RIGHT, vResultRight);
	Set_State(STATE_UP, vResultUp);
	Set_State(STATE_LOOK, vResultLook);
}

void CTransform::Add_Position(_fvector vState, CNavigationMesh* pNavigation)
{
	_vector vPosition = Get_State(STATE_POSITION);
	vPosition += vState;

	if (pNavigation != nullptr)
	{
		CCell* pCell = nullptr;
		if (pNavigation->IsOn(vPosition, &pCell, m_iCurrentIndex))
			Set_State(CTransform::STATE_POSITION, vPosition);
	}
	else
		Set_State(STATE_POSITION, vPosition);
}

void CTransform::Set_State(STATE eState, _fvector vState)
{
	_float4		vStateDesc;
	XMStoreFloat4(&vStateDesc, vState);

	memcpy((_float4*)&m_matWorld.m[eState][0], &vStateDesc, sizeof(_float4));
}

void CTransform::Set_Scale(_fvector vScale)
{
	_vector	vRight	= Get_State(STATE_RIGHT);
	_vector	vUp		= Get_State(STATE_UP);
	_vector	vLook	= Get_State(STATE_LOOK);

	Set_State(STATE_RIGHT, XMVector4Normalize(vRight) * XMVectorGetX(vScale));
	Set_State(STATE_UP   , XMVector4Normalize(vUp)    * XMVectorGetY(vScale));
	Set_State(STATE_LOOK , XMVector4Normalize(vLook)  * XMVectorGetZ(vScale));
}

void CTransform::Set_MoveSpeed(_double dMoveSpeedPerSec)
{
	m_tDesc.dSpeedPerSec = dMoveSpeedPerSec;
}

void CTransform::Set_RotateSpeed(_double dRotationSpeedPerSec)
{
	m_tDesc.dRotationPerSec = dRotationSpeedPerSec;
}

CTransform * CTransform::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CTransform* pInstance = new CTransform(pGraphicDevice, pContextDevice);

	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
