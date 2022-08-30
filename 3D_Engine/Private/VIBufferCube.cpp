#include "..\Public\VIBufferCube.h"

CVIBufferCube::CVIBufferCube(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CVIBuffer(pGraphicDevice, pContextDevice)
{

}

CVIBufferCube::CVIBufferCube(const CVIBufferCube & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CVIBufferCube::NativeConstruct(const _tchar* pShaderFilePath, const char* pTechniqueName)
{
	m_iStride = sizeof(VTXCUBE);
	m_iNumVertices = 8;

	ZeroMemory(&m_tVB_Desc, sizeof(D3D11_BUFFER_DESC));
	m_tVB_Desc.ByteWidth = m_iStride * m_iNumVertices;
	m_tVB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tVB_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tVB_Desc.MiscFlags = 0;
	m_tVB_Desc.CPUAccessFlags = 0;
	m_tVB_Desc.StructureByteStride = m_iStride;
	
	m_pVertices = new VTXCUBE[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXCUBE) * m_iNumVertices);

	((VTXCUBE*)m_pVertices)[0].vPosition = _float3(-0.5f,  0.5f, -0.5f);
	((VTXCUBE*)m_pVertices)[0].vTexUV = ((VTXCUBE*)m_pVertices)[0].vPosition;

	((VTXCUBE*)m_pVertices)[1].vPosition = _float3( 0.5f,  0.5f, -0.5f);
	((VTXCUBE*)m_pVertices)[1].vTexUV = ((VTXCUBE*)m_pVertices)[1].vPosition;

	((VTXCUBE*)m_pVertices)[2].vPosition = _float3( 0.5f, -0.5f, -0.5f);
	((VTXCUBE*)m_pVertices)[2].vTexUV = ((VTXCUBE*)m_pVertices)[2].vPosition;

	((VTXCUBE*)m_pVertices)[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	((VTXCUBE*)m_pVertices)[3].vTexUV = ((VTXCUBE*)m_pVertices)[3].vPosition;

	((VTXCUBE*)m_pVertices)[4].vPosition = _float3(-0.5f,  0.5f,  0.5f);
	((VTXCUBE*)m_pVertices)[4].vTexUV = ((VTXCUBE*)m_pVertices)[4].vPosition;

	((VTXCUBE*)m_pVertices)[5].vPosition = _float3( 0.5f,  0.5f,  0.5f);
	((VTXCUBE*)m_pVertices)[5].vTexUV = ((VTXCUBE*)m_pVertices)[5].vPosition;

	((VTXCUBE*)m_pVertices)[6].vPosition = _float3( 0.5f, -0.5f,  0.5f);
	((VTXCUBE*)m_pVertices)[6].vTexUV = ((VTXCUBE*)m_pVertices)[6].vPosition;

	((VTXCUBE*)m_pVertices)[7].vPosition = _float3(-0.5f, -0.5f,  0.5f);
	((VTXCUBE*)m_pVertices)[7].vTexUV = ((VTXCUBE*)m_pVertices)[7].vPosition;

	m_tVB_SubResourceData.pSysMem = m_pVertices;
	if (FAILED(Create_VertexBuffer()))
		return E_FAIL;


	m_iNumPrimitive = 12;
	m_iNumIndicesPerPT = 3;
	m_iFaceIndexSize = sizeof(TRI16);

	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eRenderType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_tIB_Desc, sizeof(D3D11_BUFFER_DESC));
	m_tIB_Desc.ByteWidth = m_iFaceIndexSize * m_iNumPrimitive;
	m_tIB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tIB_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tIB_Desc.MiscFlags = 0;
	m_tIB_Desc.CPUAccessFlags = 0;

	m_pIndices = new TRI16[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(TRI16) * m_iNumPrimitive);

	
	((TRI16*)m_pIndices)[0]._1 = 1; ((TRI16*)m_pIndices)[0]._2 = 5; ((TRI16*)m_pIndices)[0]._3 = 6; // +X
	((TRI16*)m_pIndices)[1]._1 = 1; ((TRI16*)m_pIndices)[1]._2 = 6; ((TRI16*)m_pIndices)[1]._3 = 2; // +X

	((TRI16*)m_pIndices)[2]._1 = 4; ((TRI16*)m_pIndices)[2]._2 = 0; ((TRI16*)m_pIndices)[2]._3 = 3; // -X
	((TRI16*)m_pIndices)[3]._1 = 4; ((TRI16*)m_pIndices)[3]._2 = 3; ((TRI16*)m_pIndices)[3]._3 = 7; // -X

	((TRI16*)m_pIndices)[4]._1 = 4; ((TRI16*)m_pIndices)[4]._2 = 5; ((TRI16*)m_pIndices)[4]._3 = 1; // +Y
	((TRI16*)m_pIndices)[5]._1 = 4; ((TRI16*)m_pIndices)[5]._2 = 1; ((TRI16*)m_pIndices)[5]._3 = 0; // +Y

	((TRI16*)m_pIndices)[6]._1 = 3; ((TRI16*)m_pIndices)[6]._2 = 2; ((TRI16*)m_pIndices)[6]._3 = 6; // -Y
	((TRI16*)m_pIndices)[7]._1 = 3; ((TRI16*)m_pIndices)[7]._2 = 6; ((TRI16*)m_pIndices)[7]._3 = 7; // -Y

	((TRI16*)m_pIndices)[8]._1 = 5; ((TRI16*)m_pIndices)[8]._2 = 4; ((TRI16*)m_pIndices)[8]._3 = 7; // +Z
	((TRI16*)m_pIndices)[9]._1 = 5; ((TRI16*)m_pIndices)[9]._2 = 7; ((TRI16*)m_pIndices)[9]._3 = 6; // +Z

	((TRI16*)m_pIndices)[10]._1 = 0; ((TRI16*)m_pIndices)[10]._2 = 1; ((TRI16*)m_pIndices)[10]._3 = 2; // -Z
	((TRI16*)m_pIndices)[11]._1 = 0; ((TRI16*)m_pIndices)[11]._2 = 2; ((TRI16*)m_pIndices)[11]._3 = 3; // -Z

	m_tIB_SubResourceData.pSysMem = m_pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC			tElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(Compile_Shader(tElementDesc, 2, pShaderFilePath, pTechniqueName)))
		return E_FAIL;


	return S_OK;
}

HRESULT CVIBufferCube::Clone_Construct(void * pArg)
{
	return S_OK;
}

CVIBufferCube * CVIBufferCube::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const _tchar* pShaderFilePath, const char* pTechniqueName)
{
	CVIBufferCube* pInstnace = new CVIBufferCube(pGraphicDevice, pContextDevice);
	if (FAILED(pInstnace->NativeConstruct(pShaderFilePath, pTechniqueName)))
		Safe_Release(pInstnace);
	return pInstnace;
}

CComponent * CVIBufferCube::Clone(void * pArg)
{
	CVIBufferCube* pInstance = new CVIBufferCube(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);
	return pInstance;
}

void CVIBufferCube::Free()
{
	__super::Free();
}
