#include "..\Public\Cell.h"
#include "PipeLine.h"

CCell::CCell(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: m_pGraphicDevice(pGraphicDevice), m_pContextDevice(pContextDevice)
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);
}

HRESULT CCell::NativeConstruct(_float3 * pPoints, _uint iIndex)
{
	m_iIndex = iIndex;
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	m_vCenterPos[0].x = (m_vPoints[0].x + m_vPoints[1].x) / 2.f; m_vCenterPos[0].y = (m_vPoints[0].y + m_vPoints[1].y) / 2.f; m_vCenterPos[0].z = (m_vPoints[0].z + m_vPoints[1].z) / 2.f;
	m_vCenterPos[1].x = (m_vPoints[1].x + m_vPoints[2].x) / 2.f; m_vCenterPos[1].y = (m_vPoints[1].y + m_vPoints[2].y) / 2.f; m_vCenterPos[1].z = (m_vPoints[1].z + m_vPoints[2].z) / 2.f;
	m_vCenterPos[2].x = (m_vPoints[2].x + m_vPoints[0].x) / 2.f; m_vCenterPos[2].y = (m_vPoints[2].y + m_vPoints[0].y) / 2.f; m_vCenterPos[2].z = (m_vPoints[2].z + m_vPoints[0].z) / 2.f;
	
	m_vCenter = _float4(
		(m_vCenterPos[0].x + m_vCenterPos[1].x + m_vCenterPos[2].x) / 3.f
		, (m_vCenterPos[0].y + m_vCenterPos[1].y + m_vCenterPos[2].y) / 3.f
		, (m_vCenterPos[0].z + m_vCenterPos[1].z + m_vCenterPos[2].z) / 3.f, 1.f);


	ZeroMemory(m_pNeighbor, sizeof(CCell*) * LINE_END);

#ifdef _DEBUG
	if (FAILED(Ready_Debug_Buffer()))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CCell::Transform(_fmatrix matWorld)
{
	for (_uint i = 0; i < POINT_END; ++i)
		XMStoreFloat3(&m_vPointsInWorld[i], XMVector3TransformCoord(XMLoadFloat3(&m_vPoints[i]), matWorld));

	XMStoreFloat3(&m_vLine[LINE_AB], XMLoadFloat3(&m_vPointsInWorld[POINT_B]) - XMLoadFloat3(&m_vPointsInWorld[POINT_A]));
	XMStoreFloat3(&m_vLine[LINE_BC], XMLoadFloat3(&m_vPointsInWorld[POINT_C]) - XMLoadFloat3(&m_vPointsInWorld[POINT_B]));
	XMStoreFloat3(&m_vLine[LINE_CA], XMLoadFloat3(&m_vPointsInWorld[POINT_A]) - XMLoadFloat3(&m_vPointsInWorld[POINT_C]));

	return S_OK;
}

_bool CCell::Compare(_fvector vSourPoint, _fvector vDestPoint)
{
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
	}
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
	}

	return false;
}

_bool CCell::IsIn(_fvector& vPosition, CCell ** ppNeighborOut, CCell** ppSlide)
{
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector		vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPointsInWorld[i]));
		_vector		vNormal = XMVector3Normalize(XMVectorSet(m_vLine[i].z * -1.f, 0.0f, m_vLine[i].x, 0.f));

		_float		fHeight = XMVectorGetY(vPosition);
		_float		fCenterHeigt = m_vCenter.y;
		_float		fDown = fHeight - fCenterHeigt;
		_float		fUp = fCenterHeigt - fHeight;

		_float		fX = XMVectorGetX(XMVector3Dot(vDir, vNormal));
		if (0.f < XMVectorGetX(XMVector3Dot(vDir, vNormal)))
		{
			*ppNeighborOut = m_pNeighbor[i];
			*ppSlide = this;
			return false;
		}
	}

	_vector		vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPointsInWorld[0]), XMLoadFloat3(&m_vPointsInWorld[1]), XMLoadFloat3(&m_vPointsInWorld[2]));
	_float		fPosY = (-XMVectorGetX(vPlane) * XMVectorGetX(vPosition) - XMVectorGetZ(vPlane) * XMVectorGetZ(vPosition) - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);
	const_cast<_vector&>(vPosition) = XMVectorSetY(vPosition, fPosY);
	return true;
}

_bool CCell::IsIn_Vent(_fvector & vPosition, CCell ** ppNeighborOut, CCell ** ppSlide)
{
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector		vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPointsInWorld[i]));
		_vector		vNormal = XMVector3Normalize(XMVectorSet(m_vLine[i].z * -1.f, 0.0f, m_vLine[i].x, 0.f));

		_float		fHeight = XMVectorGetY(vPosition);
		_float		fCenterHeigt = m_vCenterPos[i].y;
		_float		fDown = fHeight - fCenterHeigt;
		_float		fUp = fCenterHeigt - fHeight;

		_float		fX = XMVectorGetX(XMVector3Dot(vDir, vNormal));
		if (0.f < XMVectorGetX(XMVector3Dot(vDir, vNormal)) || fCenterHeigt < 1.7f || fCenterHeigt > 1.9f)
		{
			*ppNeighborOut = m_pNeighbor[i];
			*ppSlide = this;
			return false;
		}
	}

	_vector		vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPointsInWorld[0]), XMLoadFloat3(&m_vPointsInWorld[1]), XMLoadFloat3(&m_vPointsInWorld[2]));
	_float		fPosY = (-XMVectorGetX(vPlane) * XMVectorGetX(vPosition) - XMVectorGetZ(vPlane) * XMVectorGetZ(vPosition) - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);
	const_cast<_vector&>(vPosition) = XMVectorSetY(vPosition, fPosY);
	return true;
}

_vector CCell::IsSlide(_fvector & vPosition)
{
	_float fAngle[3] = { 0.f };
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector		vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPointsInWorld[i]));
		_vector		vNormal = XMVector3Normalize(XMVectorSet(m_vLine[i].z * -1.f, 0.0f, m_vLine[i].x, 0.f));
		fAngle[i] = XMVectorGetX(XMVector3Dot(vDir, vNormal));
	}
	_float fMax = max(fAngle[0], fAngle[1]);
	fMax = max(fMax, fAngle[2]);
	if (fMax == fAngle[0])
	{
		_float DiffX = m_vCenterPos[0].x - XMVectorGetX(vPosition);
		_float DiffZ = m_vCenterPos[0].z - XMVectorGetZ(vPosition);
		if (fabs(DiffX) > fabs(DiffZ))
		{
			if (m_vCenterPos[0].x > XMVectorGetX(vPosition))
			{
				_vector vDir = XMLoadFloat3(&m_vPointsInWorld[1]) - XMLoadFloat3(&m_vPointsInWorld[0]);
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				return XMVector3Normalize(XMVector3Cross(vDir, vUp));
			}
			else
			{
				_vector vDir = XMLoadFloat3(&m_vPointsInWorld[0]) - XMLoadFloat3(&m_vPointsInWorld[1]);
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				return XMVector3Normalize(XMVector3Cross(vDir, vUp));
			}
		}
		else
		{
			if (m_vCenterPos[0].z > XMVectorGetZ(vPosition))
			{
				_vector vDir = XMLoadFloat3(&m_vPointsInWorld[0]) - XMLoadFloat3(&m_vPointsInWorld[1]);
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				return XMVector3Normalize(XMVector3Cross(vDir, vUp));
			}
			else
			{
				_vector vDir = XMLoadFloat3(&m_vPointsInWorld[1]) - XMLoadFloat3(&m_vPointsInWorld[0]);
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				return XMVector3Normalize(XMVector3Cross(vDir, vUp));
			}
		}
	}
	else if (fMax == fAngle[1])
	{
		_float DiffX = m_vCenterPos[1].x - XMVectorGetX(vPosition);
		_float DiffZ = m_vCenterPos[1].z - XMVectorGetZ(vPosition);
		if (fabs(DiffX) > fabs(DiffZ))
		{
			if (m_vCenterPos[1].x > XMVectorGetX(vPosition))
			{
				_vector vDir = XMLoadFloat3(&m_vPointsInWorld[2]) - XMLoadFloat3(&m_vPointsInWorld[1]);
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				return XMVector3Normalize(XMVector3Cross(vDir, vUp));
			}
			else
			{
				_vector vDir = XMLoadFloat3(&m_vPointsInWorld[1]) - XMLoadFloat3(&m_vPointsInWorld[2]);
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				return XMVector3Normalize(XMVector3Cross(vDir, vUp));
			}
		}
		else
		{
			if (m_vCenterPos[1].z > XMVectorGetZ(vPosition))
			{
				_vector vDir = XMLoadFloat3(&m_vPointsInWorld[1]) - XMLoadFloat3(&m_vPointsInWorld[2]);
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				return XMVector3Normalize(XMVector3Cross(vDir, vUp));
			}
			else
			{
				_vector vDir = XMLoadFloat3(&m_vPointsInWorld[2]) - XMLoadFloat3(&m_vPointsInWorld[1]);
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				return XMVector3Normalize(XMVector3Cross(vDir, vUp));
			}
		}
	}
	else
	{
		_float DiffX = m_vCenterPos[2].x - XMVectorGetX(vPosition);
		_float DiffZ = m_vCenterPos[2].z - XMVectorGetZ(vPosition);
		if (fabs(DiffX) > fabs(DiffZ))
		{
			if (m_vCenterPos[2].x > XMVectorGetX(vPosition))
			{
				_vector vDir = XMLoadFloat3(&m_vPointsInWorld[2]) - XMLoadFloat3(&m_vPointsInWorld[1]);
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				return XMVector3Normalize(XMVector3Cross(vDir, vUp));
			}
			else
			{
				_vector vDir = XMLoadFloat3(&m_vPointsInWorld[1]) - XMLoadFloat3(&m_vPointsInWorld[2]);
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				return XMVector3Normalize(XMVector3Cross(vDir, vUp));
			}
		}
		else
		{
			if (m_vCenterPos[2].z > XMVectorGetZ(vPosition))
			{
				_vector vDir = XMLoadFloat3(&m_vPointsInWorld[1]) - XMLoadFloat3(&m_vPointsInWorld[2]);
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				return XMVector3Normalize(XMVector3Cross(vDir, vUp));
			}
			else
			{
				_vector vDir = XMLoadFloat3(&m_vPointsInWorld[2]) - XMLoadFloat3(&m_vPointsInWorld[1]);
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				return XMVector3Normalize(XMVector3Cross(vDir, vUp));
			}
		}
	}

	return XMVectorSet(0.f,0.f,0.f,0.f);
}

_bool CCell::Find_CurrentIndexCell(_fvector vPosition)
{
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector		vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPointsInWorld[i]));
		_vector		vNormal = XMVector3Normalize(XMVectorSet(m_vLine[i].z * -1.f, 0, m_vLine[i].x, 0.f));

		_float		fX = XMVectorGetX(XMVector3Dot(vDir, vNormal));
		_float		fHeight = XMVectorGetY(vPosition);
		_float		fNaviHeight = m_vCenter.y;
		_float		fDistance = fHeight - fNaviHeight;
		if ( (0.f < XMVectorGetX(XMVector3Dot(vDir, vNormal))) || (fDistance > 1.f) || (fDistance < -1.f))
		{
			return false;
		}
	}
	return true;
}

_bool CCell::Find_VentIndexCell(_fvector vPosition)
{
	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector		vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPointsInWorld[i]));
		_vector		vNormal = XMVector3Normalize(XMVectorSet(m_vLine[i].z * -1.f, 0, m_vLine[i].x, 0.f));

		_float		fNaviHeight = m_vCenter.y;
		if ((0.f < XMVectorGetX(XMVector3Dot(vDir, vNormal))) || fNaviHeight > 1.9f || fNaviHeight < 1.7f )
		{
			return false;
		}
	}
	return true;
}

#ifdef _DEBUG
HRESULT CCell::Render(_fmatrix matWorld)
{
	CPipeLine* pPipeLine = CPipeLine::Get_Instance();

	m_pVIBuffer->Set_RawValue("g_matWorld", &XMMatrixTranspose(matWorld), sizeof(_matrix));
	m_pVIBuffer->Set_RawValue("g_matView", &pPipeLine->Get_TransposeTransform(CPipeLine::D3DTS_VIEW), sizeof(_matrix));
	m_pVIBuffer->Set_RawValue("g_matProj", &pPipeLine->Get_TransposeTransform(CPipeLine::D3DTS_PROJ), sizeof(_matrix));

	m_pVIBuffer->Render();


	return S_OK;
}
#endif // _DEBUG
HRESULT CCell::Ready_Debug_Buffer()
{
	m_pVIBuffer = CVIBuffer_Triangle::Create(m_pGraphicDevice, m_pContextDevice, TEXT("../Bin/ShaderFiles/Shader_NavigationMesh.hlsl"), "Technique_Default", m_vPoints);
	if (m_pVIBuffer == nullptr)
		return E_FAIL;

	return S_OK;
}

CCell * CCell::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, _float3 * pPoints, _uint iIndex)
{
	CCell* pInstance = new CCell(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(pPoints, iIndex)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pContextDevice);
	Safe_Release(m_pVIBuffer);
}
