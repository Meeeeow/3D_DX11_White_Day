#include "..\Public\InstanceBuffer_Point.h"

CInstanceBuffer_Point::CInstanceBuffer_Point(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, _uint iNumInstance)
	: CVIBuffer_Instance(pGraphicDevice, pContextDevice, iNumInstance)
{
}

CInstanceBuffer_Point::CInstanceBuffer_Point(const CInstanceBuffer_Point & rhs)
	: CVIBuffer_Instance(rhs)
{
}

HRESULT CInstanceBuffer_Point::NativeConstruct(const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	m_iStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;

	ZeroMemory(&m_tVB_Desc, sizeof(D3D11_BUFFER_DESC));

	m_tVB_Desc.ByteWidth = m_iStride * m_iNumVertices;
	m_tVB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tVB_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tVB_Desc.CPUAccessFlags = 0;
	m_tVB_Desc.MiscFlags = 0;
	m_tVB_Desc.StructureByteStride = m_iStride;

	m_pVertices = new VTXPOINT;
	ZeroMemory(m_pVertices, sizeof(VTXPOINT));

	((VTXPOINT*)m_pVertices)[0].vPosition = _float3(0.f, 0.f, 0.f);
	((VTXPOINT*)m_pVertices)[0].fSize = 1.f;


	m_tVB_SubResourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_iNumPrimitive = m_iNumInstance;
	m_iNumIndicesPerPT = 1; /* �����ϳ��׸��� �ʿ��� �ε��� ����. */
	m_iIndexPerInstance = 1;
	m_iFaceIndexSize = sizeof(_ushort);

	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eRenderType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_tIB_Desc, sizeof(D3D11_BUFFER_DESC));

	m_tIB_Desc.ByteWidth = m_iFaceIndexSize * m_iNumPrimitive;
	m_tIB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tIB_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tIB_Desc.CPUAccessFlags = 0;
	m_tIB_Desc.MiscFlags = 0;

	m_pIndices = new _ushort[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(_ushort) * m_iNumPrimitive);

	m_tIB_SubResourceData.pSysMem = m_pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	m_iInstanceStride = sizeof(VTXMATRIX);
	ZeroMemory(&m_tInstanceBufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_tInstanceBufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_tInstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_tInstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tInstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_tInstanceBufferDesc.MiscFlags = 0;
	m_tInstanceBufferDesc.StructureByteStride = m_iInstanceStride;

	m_pInstanceVertices = new VTXMATRIX[m_iNumInstance];
	ZeroMemory(m_pInstanceVertices, sizeof(VTXMATRIX) * m_iNumInstance);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXMATRIX*)m_pInstanceVertices)[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		((VTXMATRIX*)m_pInstanceVertices)[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		((VTXMATRIX*)m_pInstanceVertices)[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		((VTXMATRIX*)m_pInstanceVertices)[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	}

	m_tInstanceBufferSRD.pSysMem = m_pInstanceVertices;

	if (FAILED(Create_InstanceBuffer()))
		return E_FAIL;

	Safe_Delete_Arr(m_pInstanceVertices);

	D3D11_INPUT_ELEMENT_DESC			ElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA  , 0 },
		{ "PSIZE"   , 0, DXGI_FORMAT_R32_FLOAT		    , 0, 12, D3D11_INPUT_PER_VERTEX_DATA  , 0 },

		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT	, 1, 0 ,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT	, 1, 16,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT	, 1, 32,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT	, 1, 48,  D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	if (FAILED(__super::Compile_Shader(ElementDesc, 6, pShaderFilePath, pTechniqueName)))
		return E_FAIL;


	return S_OK;
}

HRESULT CInstanceBuffer_Point::Clone_Construct(void * pArg)
{
	return S_OK;
}

void CInstanceBuffer_Point::Update(_double dTimeDelta)
{

}

CInstanceBuffer_Point * CInstanceBuffer_Point::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, _uint iNumInstance, const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	CInstanceBuffer_Point* pInstance = new CInstanceBuffer_Point(pGraphicDevice, pContextDevice, iNumInstance);
	if (FAILED(pInstance->NativeConstruct(pShaderFilePath, pTechniqueName)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CInstanceBuffer_Point::Clone(void * pArg)
{
	CInstanceBuffer_Point* pInstance = new CInstanceBuffer_Point(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CInstanceBuffer_Point::Free()
{
	__super::Free();
}
