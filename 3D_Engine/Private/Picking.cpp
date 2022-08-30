#include "..\Public\Picking.h"
#include "PipeLine.h"

IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{

}

HRESULT CPicking::NativeConstruct(_pContextDevice * pContextDevice, HWND hWnd)
{
	m_pContextDevice = pContextDevice;
	Safe_AddRef(m_pContextDevice);

	m_hWnd = hWnd;

	return S_OK;
}

HRESULT CPicking::PreConvert()
{
	D3D11_VIEWPORT tViewportDesc;
	ZeroMemory(&tViewportDesc, sizeof(D3D11_VIEWPORT));

	_uint iNumViewports = 1;

	m_pContextDevice->RSGetViewports(&iNumViewports, &tViewportDesc);

	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	_float3	vPos;
	vPos.x = _float(pt.x / (tViewportDesc.Width / 2.f) - 1);
	vPos.y = _float(pt.y / (tViewportDesc.Height / 2.f) * -1 + 1);
	vPos.z = 0.f;
	
	CPipeLine* pPipeLine = CPipeLine::Get_Instance();
	_matrix	matInvProj = pPipeLine->Get_InverseTransform(CPipeLine::D3DTS_PROJ);
	_matrix matInvView = pPipeLine->Get_InverseTransform(CPipeLine::D3DTS_VIEW);

	_vector vMousePos;
	vMousePos = XMVector3TransformCoord(XMLoadFloat3(&vPos), matInvProj);
	_vector vMousePivot = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector vMouseRay = vMousePos - vMousePivot;

	_float3 vPivot = _float3(XMVectorGetX(vMousePivot), XMVectorGetY(vMousePivot), XMVectorGetZ(vMousePivot));
	_float3 vRay = _float3(XMVectorGetX(vMouseRay), XMVectorGetY(vMouseRay), XMVectorGetZ(vMouseRay));

	XMStoreFloat4(&m_vMousePivot, XMVector3TransformCoord(XMLoadFloat3(&vPivot), matInvView));
	XMStoreFloat4(&m_vMouseRay, XMVector4Normalize(XMVector3TransformNormal(XMLoadFloat3(&vRay), matInvView)));
	
return S_OK;
}

/* v0, v1, v2를 넘겨줄 때 애초에 matWorld를 곱해서 넘겨주자 */
_bool CPicking::Picking(_fvector v0, _fvector v1, _fvector v2, _float& fDistance, _fmatrix matWorld)
{
	/* v0, v1, v2를 넘겨줄 때 애초에 matWorld를 곱해서 넘겨주자 */


	//_matrix matInvWorld = XMMatrixInverse(nullptr, matWorld);
	//_vector vPivot = XMVector4Transform(XMLoadFloat4(&m_vMousePivot), matWorld);
	//_vector vRay = XMVector4Normalize(XMVector4Transform(XMLoadFloat4(&m_vMouseRay), matWorld));
	
	
	return TriangleTests::Intersects(XMLoadFloat4(&m_vMousePivot), XMLoadFloat4(&m_vMouseRay), v0, (_gvector)v1, (_hvector)v2, fDistance);
}


void CPicking::Free()
{
	Safe_Release(m_pContextDevice);
}
