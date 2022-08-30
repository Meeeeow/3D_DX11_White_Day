#include "..\Public\Frustum.h"
#include "PipeLine.h"

IMPLEMENT_SINGLETON(CFrustum)

CFrustum::CFrustum()
{
}


HRESULT CFrustum::Make_Frustum(float fFar)
{
	CPipeLine* pPipeLine = CPipeLine::Get_Instance();

	/* Proj행렬과 View의 행렬을 저장 */
	_matrix		matProj = pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ);
	_matrix		matView = pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW);

	/* 투영행렬을 float4x4 형태로 1차 저장 */
	_float4x4	Proj4x4;
	XMStoreFloat4x4(&Proj4x4, matProj);

	/* 절두체에서 최소 Z거리를 계산 */
	_float		zMinimum = -Proj4x4._43 / Proj4x4._33;
	_float		r = fFar / (fFar / zMinimum);
	/* 내 기준 Far = 150.f */
	/* 업데이트한 값을 다시 투영 행렬에 설정 */
	Proj4x4._33 = r;
	Proj4x4._43 = -r * zMinimum;
	matProj = XMLoadFloat4x4(&Proj4x4);

	/* View와 Proj를 곱한다. */
	_matrix		matFinal = XMMatrixMultiply(matView, matProj);
	_float4x4	matVP;
	XMStoreFloat4x4(&matVP, matFinal);

	_vector		vPlanes[6];
	_float x, y, z, w;
	// Near
	x = (_float)(matVP._14 + matVP._13);
	y = (_float)(matVP._24 + matVP._23);
	z = (_float)(matVP._34 + matVP._33);
	w = (_float)(matVP._44 + matVP._43);
	vPlanes[0] = XMVectorSet(x, y, z, w);
	vPlanes[0] = XMPlaneNormalize(vPlanes[0]);

	// Far
	x = (_float)(matVP._14 - matVP._13);
	y = (_float)(matVP._24 - matVP._23);
	z = (_float)(matVP._34 - matVP._33);
	w = (_float)(matVP._44 - matVP._43);
	vPlanes[1] = XMVectorSet(x, y, z, w);
	vPlanes[1] = XMPlaneNormalize(vPlanes[1]);

	// Left
	x = (_float)(matVP._14 + matVP._11);
	y = (_float)(matVP._24 + matVP._21);
	z = (_float)(matVP._34 + matVP._31);
	w = (_float)(matVP._44 + matVP._41);
	vPlanes[2] = XMVectorSet(x, y, z, w);
	vPlanes[2] = XMPlaneNormalize(vPlanes[2]);

	// Right
	x = (_float)(matVP._14 - matVP._11);
	y = (_float)(matVP._24 - matVP._21);
	z = (_float)(matVP._34 - matVP._31);
	w = (_float)(matVP._44 - matVP._41);
	vPlanes[3] = XMVectorSet(x, y, z, w);
	vPlanes[3] = XMPlaneNormalize(vPlanes[3]);

	// Top
	x = (_float)(matVP._14 - matVP._12);
	y = (_float)(matVP._24 - matVP._22);
	z = (_float)(matVP._34 - matVP._32);
	w = (_float)(matVP._44 - matVP._42);
	vPlanes[4] = XMVectorSet(x, y, z, w);
	vPlanes[4] = XMPlaneNormalize(vPlanes[4]);

	// Bottom
	x = (_float)(matVP._14 + matVP._12);
	y = (_float)(matVP._24 + matVP._22);
	z = (_float)(matVP._34 + matVP._32);
	w = (_float)(matVP._44 + matVP._42);
	vPlanes[5] = XMVectorSet(x, y, z, w);
	vPlanes[5] = XMPlaneNormalize(vPlanes[5]);

	for (_uint i = 0; i < 6; i++)
		XMStoreFloat4(&m_vPlane[i], vPlanes[i]);

	return S_OK;
}

_bool CFrustum::Culling_Radius(CTransform * pTransform, _float fRadius)
{
	_vector vPos = pTransform->Get_State(CTransform::STATE_POSITION);
	for (_uint i = 0; i < 6; ++i)
	{
		_vector vPlane = XMLoadFloat4(&m_vPlane[i]);
		if (XMVectorGetX(XMPlaneDotCoord(vPlane, vPos)) < -fRadius)
			return false;
	}

	return true;
}

_bool CFrustum::Culling_PointRadius(_fvector vPos, _matrix matWorld, _float fRadius)
{
	_vector vWorldPos = XMVector4Transform(vPos, matWorld);

	for (_uint i = 0; i < 6; ++i)
	{
		_vector vPlane = XMLoadFloat4(&m_vPlane[i]);
		if (XMVectorGetX(XMPlaneDotCoord(vPlane, vWorldPos)) < -fRadius)
			return false;
	}
	return true;
}

void CFrustum::Free()
{
	
}
