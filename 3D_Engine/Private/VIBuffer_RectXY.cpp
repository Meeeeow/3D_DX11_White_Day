#include "..\Public\VIBuffer_RectXY.h"

CVIBuffer_RectXY::CVIBuffer_RectXY(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CVIBuffer(pGraphicDevice, pContextDevice)
{
}

CVIBuffer_RectXY::CVIBuffer_RectXY(const CVIBuffer_RectXY & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_RectXY::NativeConstruct(const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;

	ZeroMemory(&m_tVB_Desc, sizeof(D3D11_BUFFER_DESC));

	m_tVB_Desc.ByteWidth = m_iStride * m_iNumVertices;
	m_tVB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tVB_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tVB_Desc.CPUAccessFlags = 0;
	m_tVB_Desc.MiscFlags = 0;
	m_tVB_Desc.StructureByteStride = m_iStride;

	m_pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXTEX) * m_iNumVertices);

	((VTXTEX*)m_pVertices)[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	((VTXTEX*)m_pVertices)[0].vTexUV = _float2(0.0f, 0.f);

	((VTXTEX*)m_pVertices)[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	((VTXTEX*)m_pVertices)[1].vTexUV = _float2(1.0f, 0.f);

	((VTXTEX*)m_pVertices)[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	((VTXTEX*)m_pVertices)[2].vTexUV = _float2(1.0f, 1.f);

	((VTXTEX*)m_pVertices)[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	((VTXTEX*)m_pVertices)[3].vTexUV = _float2(0.0f, 1.f);

	m_tVB_SubResourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_iNumPrimitive = 2;
	m_iNumIndicesPerPT = 3; /* 도형하나그릴때 필요한 인덱스 갯수. */
	m_iFaceIndexSize = sizeof(TRI16);

	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eRenderType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_tIB_Desc, sizeof(D3D11_BUFFER_DESC));

	m_tIB_Desc.ByteWidth = m_iFaceIndexSize * m_iNumPrimitive;
	m_tIB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tIB_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tIB_Desc.CPUAccessFlags = 0;
	m_tIB_Desc.MiscFlags = 0;

	m_pIndices = new TRI16[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(TRI16) * m_iNumPrimitive);

	((TRI16*)m_pIndices)[0]._1 = 0;
	((TRI16*)m_pIndices)[0]._2 = 1;
	((TRI16*)m_pIndices)[0]._3 = 2;

	((TRI16*)m_pIndices)[1]._1 = 0;
	((TRI16*)m_pIndices)[1]._2 = 2;
	((TRI16*)m_pIndices)[1]._3 = 3;

	m_tIB_SubResourceData.pSysMem = m_pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC			ElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(__super::Compile_Shader(ElementDesc, 2, pShaderFilePath, pTechniqueName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_RectXY::Clone_Construct(void * pArg)
{
	return S_OK;
}

CVIBuffer_RectXY * CVIBuffer_RectXY::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	CVIBuffer_RectXY* pInstance = new CVIBuffer_RectXY(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(pShaderFilePath, pTechniqueName)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CVIBuffer_RectXY::Clone(void * pArg)
{
	CVIBuffer_RectXY* pInstance = new CVIBuffer_RectXY(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CVIBuffer_RectXY::Free()
{
	__super::Free();
}
