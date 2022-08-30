#include "..\Public\VIBuffer_Terrain.h"
#include "Picking.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CVIBuffer(pGraphicDevice,pContextDevice)
{

}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{

}

HRESULT CVIBuffer_Terrain::NativeConstruct(const _tchar * pHeightMapFilePath, const _tchar * pShaderFilePath, const char * pTechinqueName)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == 0)
		FAILMSG("VIBuffer Terrain - CreateFile");

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	_ulong*		pPixel = new _ulong[m_iNumVertices];

	ReadFile(hFile, pPixel, sizeof(_ulong) * m_iNumVertices, &dwByte, nullptr);

	m_iStride = sizeof(VTXNORMALRECT);
	ZeroMemory(&m_tVB_Desc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_tIB_Desc, sizeof(D3D11_BUFFER_DESC));

	m_tVB_Desc.ByteWidth = m_iStride * m_iNumVertices;
	m_tVB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tVB_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tVB_Desc.CPUAccessFlags = 0;
	m_tVB_Desc.MiscFlags = 0;
	m_tVB_Desc.StructureByteStride = m_iStride;

	m_pVertices = new VTXNORMALRECT[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXNORMALRECT)* m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			((VTXNORMALRECT*)m_pVertices)[iIndex].vPosition = _float3(_float(j), (pPixel[iIndex] & 0x000000ff) / 10.f, _float(i));
			((VTXNORMALRECT*)m_pVertices)[iIndex].vNormal = _float3(0.f, 0.f, 0.f); // 지금 채울 수 없다. 포지션 정점을 다 채운후에 채울 수 있음.
			((VTXNORMALRECT*)m_pVertices)[iIndex].vTexUV = _float2(_float(j) / m_iNumVerticesX - 1, _float(i) / m_iNumVerticesZ - 1);
		}
	}
	
	ZeroMemory(&m_tVB_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tVB_SubResourceData.pSysMem = m_pVertices; /* m_pVertices가 void* 인 이유, pSystem 이 const void* 임 */

	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2; /* 그릴 삼각형의 개수 */
	m_eRenderType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; /* 그릴 방식 */
	m_iNumIndicesPerPT = 3; /* 도형을 하나 그릴때 필요한 개수 */
	m_iFaceIndexSize = sizeof(TRI32);

	
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_tIB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tIB_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tIB_Desc.ByteWidth = m_iFaceIndexSize * m_iNumPrimitive;
	m_tIB_Desc.MiscFlags = 0;
	m_tIB_Desc.CPUAccessFlags = 0;

	m_pIndices = new TRI32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(TRI32)* m_iNumPrimitive);

	_uint		iNumFace = 0;

	VTXNORMALRECT*	pVertices = (VTXNORMALRECT*)m_pVertices;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector		vSour, vDest, vNormal;				/* Normal 벡터 계산을 위한 Sour Dest 벡터 */

			((TRI32*)m_pIndices)[iIndex]._1 = iIndices[0];
			((TRI32*)m_pIndices)[iIndex]._2 = iIndices[1];
			((TRI32*)m_pIndices)[iIndex]._3 = iIndices[2];

			vSour = XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vPosition) -
				XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vPosition) -
				XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest)); 

			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vNormal) + vNormal));

			++iNumFace;

			((TRI32*)m_pIndices)[iIndex]._1 = iIndices[0];
			((TRI32*)m_pIndices)[iIndex]._2 = iIndices[2];
			((TRI32*)m_pIndices)[iIndex]._3 = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vPosition) -
				XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vPosition) -
				XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vNormal) + vNormal));

			++iNumFace;
		}
	}
	ZeroMemory(&m_tIB_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tIB_SubResourceData.pSysMem = m_pIndices;

	if (FAILED(__super::Create_VertexBuffer()))
		FAILMSG("VIBuffer Terrain - Create_VertexBuffer");

	if (FAILED(__super::Create_IndexBuffer()))
		FAILMSG("VIBuffer Terrain - Create_IndexBuffer");


	/*
	Element Desc 정보
		#1. Name
		#2. Index
		#3. DXGI_FORMAT
		#4. Input Slot
		#5. OffSet
		#6. Input Class
		#7. 인스턴스 별 동일한 데이터를 그리는 인스턴스 수. 단, #6. 이 D3D11_INPUT_PER_VERTEX_DATA 이면 0이어야 한다.
	*/

	D3D11_INPUT_ELEMENT_DESC	tElementDesc[] = {
		{"POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT   , 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	if (FAILED(Compile_Shader(tElementDesc, 3, pShaderFilePath, pTechinqueName)))
		FAILMSG("VIBuffer Terrain - Compile_Shader");

	Safe_Delete_Arr(pPixel);
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(const DESC& tVIBufferDesc)
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	m_iNumVerticesX = tVIBufferDesc.iNumVerticesX;
	m_iNumVerticesZ = tVIBufferDesc.iNumVerticesZ;
	
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_iStride = sizeof(VTXNORMALRECT);
	ZeroMemory(&m_tVB_Desc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_tIB_Desc, sizeof(D3D11_BUFFER_DESC));

	m_tVB_Desc.ByteWidth = m_iStride * m_iNumVertices;
	m_tVB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tVB_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tVB_Desc.CPUAccessFlags = 0;
	m_tVB_Desc.MiscFlags = 0;
	m_tVB_Desc.StructureByteStride = m_iStride;

	m_pVertices = new VTXNORMALRECT[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXNORMALRECT)* m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;
			_float	fSizeX = j * tVIBufferDesc.fIntervalX;
			_float	fSizeZ = i * tVIBufferDesc.fIntervalZ;

			((VTXNORMALRECT*)m_pVertices)[iIndex].vPosition = _float3(tVIBufferDesc.vStartPos.x + fSizeX, tVIBufferDesc.vStartPos.y, tVIBufferDesc.vStartPos.z + fSizeZ);
			((VTXNORMALRECT*)m_pVertices)[iIndex].vNormal = _float3(0.f, 0.f, 0.f); // 지금 채울 수 없다. 포지션 정점을 다 채운후에 채울 수 있음.
			((VTXNORMALRECT*)m_pVertices)[iIndex].vTexUV = _float2((fSizeX / (m_iNumVerticesX - 1)) * tVIBufferDesc.fTexU
				, (fSizeZ / (m_iNumVerticesZ - 1)) * tVIBufferDesc.fTexV);
		}
	}

	ZeroMemory(&m_tVB_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tVB_SubResourceData.pSysMem = m_pVertices; /* m_pVertices가 void* 인 이유, pSystem 이 const void* 임 */

	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2; /* 그릴 삼각형의 개수 */
	m_eRenderType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; /* 그릴 방식 */
	m_iNumIndicesPerPT = 3; /* 도형을 하나 그릴때 필요한 개수 */
	m_iFaceIndexSize = sizeof(TRI32);

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_tIB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tIB_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tIB_Desc.ByteWidth = m_iFaceIndexSize * m_iNumPrimitive;
	m_tIB_Desc.MiscFlags = 0;
	m_tIB_Desc.CPUAccessFlags = 0;

	m_pIndices = new TRI32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(TRI32)* m_iNumPrimitive);

	_uint		iNumFace = 0;

	VTXNORMALRECT*	pVertices = (VTXNORMALRECT*)m_pVertices;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector		vSour, vDest, vNormal;				/* Normal 벡터 계산을 위한 Sour Dest 벡터 */

			((TRI32*)m_pIndices)[iNumFace]._1 = iIndices[0];
			((TRI32*)m_pIndices)[iNumFace]._2 = iIndices[1];
			((TRI32*)m_pIndices)[iNumFace]._3 = iIndices[2];

			vSour = XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vPosition) -
				XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vPosition) -
				XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vNormal) + vNormal));

			++iNumFace;

			((TRI32*)m_pIndices)[iNumFace]._1 = iIndices[0];
			((TRI32*)m_pIndices)[iNumFace]._2 = iIndices[2];
			((TRI32*)m_pIndices)[iNumFace]._3 = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vPosition) -
				XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vPosition) -
				XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vNormal) + vNormal));

			++iNumFace;
		}
	}
	ZeroMemory(&m_tIB_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tIB_SubResourceData.pSysMem = m_pIndices;

	if (FAILED(__super::Create_VertexBuffer()))
		FAILMSG("VIBuffer Terrain - Create_VertexBuffer");

	if (FAILED(__super::Create_IndexBuffer()))
		FAILMSG("VIBuffer Terrain - Create_IndexBuffer");



	D3D11_INPUT_ELEMENT_DESC	tElementDesc[] = {
		{ "POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT   , 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(Compile_Shader(tElementDesc, 3, tVIBufferDesc.pShaderFilePath, tVIBufferDesc.pTechniqueName)))
		FAILMSG("VIBuffer Terrain - Compile_Shader Desc");


	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct()
{

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Clone_Construct(void * pArg)
{
	if(pArg == nullptr)
		return S_OK;
	
	DESC tVIBufferDesc = *(DESC*)pArg;
	
	m_bCloned = false;

	m_iNumVerticesX = tVIBufferDesc.iNumVerticesX;
	m_iNumVerticesZ = tVIBufferDesc.iNumVerticesZ;

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_iStride = sizeof(VTXNORMALRECT);
	ZeroMemory(&m_tVB_Desc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_tIB_Desc, sizeof(D3D11_BUFFER_DESC));

	m_tVB_Desc.ByteWidth = m_iStride * m_iNumVertices;
	m_tVB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tVB_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tVB_Desc.CPUAccessFlags = 0;
	m_tVB_Desc.MiscFlags = 0;
	m_tVB_Desc.StructureByteStride = m_iStride;

	m_pVertices = new VTXNORMALRECT[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXNORMALRECT)* m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;
			_float	fSizeX = j * tVIBufferDesc.fIntervalX;
			_float	fSizeZ = i * tVIBufferDesc.fIntervalZ;

			((VTXNORMALRECT*)m_pVertices)[iIndex].vPosition = _float3(tVIBufferDesc.vStartPos.x + fSizeX, tVIBufferDesc.vStartPos.y, tVIBufferDesc.vStartPos.z + fSizeZ);
			((VTXNORMALRECT*)m_pVertices)[iIndex].vNormal = _float3(0.f, 0.f, 0.f); // 지금 채울 수 없다. 포지션 정점을 다 채운후에 채울 수 있음.
			((VTXNORMALRECT*)m_pVertices)[iIndex].vTexUV = _float2((fSizeX / (m_iNumVerticesX - 1)) * tVIBufferDesc.fTexU
				, (fSizeZ / (m_iNumVerticesZ - 1)) * tVIBufferDesc.fTexV);
		}
	}

	ZeroMemory(&m_tVB_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tVB_SubResourceData.pSysMem = m_pVertices; /* m_pVertices가 void* 인 이유, pSystem 이 const void* 임 */

	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2; /* 그릴 삼각형의 개수 */
	m_eRenderType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; /* 그릴 방식 */
	m_iNumIndicesPerPT = 3; /* 도형을 하나 그릴때 필요한 개수 */
	m_iFaceIndexSize = sizeof(TRI32);

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_tIB_Desc.Usage = D3D11_USAGE_IMMUTABLE;
	m_tIB_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_tIB_Desc.ByteWidth = m_iFaceIndexSize * m_iNumPrimitive;
	m_tIB_Desc.MiscFlags = 0;
	m_tIB_Desc.CPUAccessFlags = 0;

	m_pIndices = new TRI32[m_iNumPrimitive];
	ZeroMemory(m_pIndices, sizeof(TRI32)* m_iNumPrimitive);

	_uint		iNumFace = 0;

	VTXNORMALRECT*	pVertices = (VTXNORMALRECT*)m_pVertices;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector		vSour, vDest, vNormal;				/* Normal 벡터 계산을 위한 Sour Dest 벡터 */

			((TRI32*)m_pIndices)[iNumFace]._1 = iIndices[0];
			((TRI32*)m_pIndices)[iNumFace]._2 = iIndices[1];
			((TRI32*)m_pIndices)[iNumFace]._3 = iIndices[2];

			vSour = XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vPosition) -
				XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vPosition) -
				XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vNormal) + vNormal));

			++iNumFace;

			((TRI32*)m_pIndices)[iNumFace]._1 = iIndices[0];
			((TRI32*)m_pIndices)[iNumFace]._2 = iIndices[2];
			((TRI32*)m_pIndices)[iNumFace]._3 = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vPosition) -
				XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vPosition) -
				XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._1].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._2].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((TRI32*)m_pIndices)[iNumFace]._3].vNormal) + vNormal));

			++iNumFace;
		}
	}
	ZeroMemory(&m_tIB_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_tIB_SubResourceData.pSysMem = m_pIndices;

	if (FAILED(__super::Create_VertexBuffer()))
		FAILMSG("VIBuffer Terrain - Create_VertexBuffer");

	if (FAILED(__super::Create_IndexBuffer()))
		FAILMSG("VIBuffer Terrain - Create_IndexBuffer");



	D3D11_INPUT_ELEMENT_DESC	tElementDesc[] = {
		{ "POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT   , 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(Compile_Shader(tElementDesc, 3, L"../Bin/ShaderFiles/Shader_Terrain.hlsl", tVIBufferDesc.pTechniqueName)))
		FAILMSG("VIBuffer Terrain - Compile_Shader Desc");


	return S_OK;
}

_bool CVIBuffer_Terrain::Picking(_float3 * pOut, _matrix matWorld, _float& fDist)
{
	CPicking* pPicking = CPicking::Get_Instance();
	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for( _uint j = 0; j< m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			_float3		iIndices[] = {
				((VTXNORMALRECT*)m_pVertices)[iIndex + m_iNumVerticesX].vPosition ,
				((VTXNORMALRECT*)m_pVertices)[iIndex + m_iNumVerticesX + 1].vPosition ,
				((VTXNORMALRECT*)m_pVertices)[iIndex + 1].vPosition ,
				((VTXNORMALRECT*)m_pVertices)[iIndex].vPosition
			};
			for (_uint k = 0; k < 4; ++k)
				XMStoreFloat3(&iIndices[k],XMVector3TransformCoord(XMLoadFloat3(&iIndices[k]), matWorld));

			if (pPicking->Picking(XMLoadFloat3(&iIndices[0]), XMLoadFloat3(&iIndices[1]), XMLoadFloat3(&iIndices[2]), fDist, matWorld))
			{
				_vector vPivot = pPicking->Get_Pivot();
				_vector vRay = pPicking->Get_Ray();
				XMStoreFloat3(pOut, vPivot + vRay * fDist);
				_float fX, fY, fZ, fW;
				fX = floor((XMVectorGetX(XMLoadFloat3(pOut)) * 100)) / 100;
				fY = floor((XMVectorGetY(XMLoadFloat3(pOut)) * 100)) / 100;
				fZ = floor((XMVectorGetZ(XMLoadFloat3(pOut)) * 100)) / 100;
				fW = floor((XMVectorGetW(XMLoadFloat3(pOut)) * 100)) / 100;
				XMStoreFloat3(pOut, XMVectorSet(fX,fY,fZ,fW));

				return true;
			}

			if (pPicking->Picking(XMLoadFloat3(&iIndices[0]), XMLoadFloat3(&iIndices[2]), XMLoadFloat3(&iIndices[3]), fDist, matWorld))
			{
				_vector vPivot = pPicking->Get_Pivot();
				_vector vRay = pPicking->Get_Ray();
				XMStoreFloat3(pOut, vPivot + vRay * fDist);
				_float fX, fY, fZ, fW;
				fX = floor((XMVectorGetX(XMLoadFloat3(pOut)) * 100)) / 100;
				fY = floor((XMVectorGetY(XMLoadFloat3(pOut)) * 100)) / 100;
				fZ = floor((XMVectorGetZ(XMLoadFloat3(pOut)) * 100)) / 100;
				fW = floor((XMVectorGetW(XMLoadFloat3(pOut)) * 100)) / 100;
				XMStoreFloat3(pOut, XMVectorSet(fX, fY, fZ, fW));

				return true;
			}

			
		}
	}

	return false;
}

_float4 CVIBuffer_Terrain::Compute_Height(_fvector vPosition)
{
	_float4		vPos;
	XMStoreFloat4(&vPos, vPosition);

	_uint		iIndex = _uint(vPos.z) * m_iNumVerticesX + _uint(vPos.x);

	_uint		iIndices[4] = {
		iIndex + m_iNumVerticesX,
		iIndex + m_iNumVerticesX + 1,
		iIndex + 1,
		iIndex
	};
	/*
		0	1
		3	2
	*/
	_float		fWidth = vPos.x - ((VTXNORMALRECT*)m_pVertices)[iIndices[0]].vPosition.x;
	_float		fHeight = ((VTXNORMALRECT*)m_pVertices)[iIndices[0]].vPosition.z - vPos.z;
	
	_vector		vPlane;


	/*
		0		1
			 p
			

		3		2
		플레이어가 오른쪽 상단 삼각형에 있을 경우(012)

		0		1
		

		   p
		3		2
		플레이어가 왼쪽 하단 삼각형에 있을 경우(023)
	*/

	if (fWidth > fHeight)
	{
		vPlane = XMPlaneFromPoints(XMLoadFloat3(&((VTXNORMALRECT*)m_pVertices)[iIndices[0]].vPosition)
			, XMLoadFloat3(&((VTXNORMALRECT*)m_pVertices)[iIndices[1]].vPosition)
			, XMLoadFloat3(&((VTXNORMALRECT*)m_pVertices)[iIndices[2]].vPosition));
	}
	else
	{
		vPlane = XMPlaneFromPoints(XMLoadFloat3(&((VTXNORMALRECT*)m_pVertices)[iIndices[0]].vPosition)
			, XMLoadFloat3(&((VTXNORMALRECT*)m_pVertices)[iIndices[2]].vPosition)
			, XMLoadFloat3(&((VTXNORMALRECT*)m_pVertices)[iIndices[3]].vPosition));
	}

	vPos.y = (-XMVectorGetX(vPlane) * vPos.x - XMVectorGetZ(vPlane) * vPos.z - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);

	return vPos;
}

_float3 CVIBuffer_Terrain::Compute_Height(_fvector vPosition, _fmatrix matTerrainMatrix)
{
	_float3		vPos;
	XMStoreFloat3(&vPos, vPosition);

	_uint		iIndex = _uint(vPos.z) * m_iNumVerticesX + _uint(vPos.x);

	_float3		iIndices[] = {
		((VTXNORMALRECT*)m_pVertices)[iIndex + m_iNumVerticesX].vPosition ,
		((VTXNORMALRECT*)m_pVertices)[iIndex + m_iNumVerticesX + 1].vPosition ,
		((VTXNORMALRECT*)m_pVertices)[iIndex + 1].vPosition ,
		((VTXNORMALRECT*)m_pVertices)[iIndex].vPosition
	};
	for (_uint k = 0; k < 4; ++k)
		XMStoreFloat3(&iIndices[k], XMVector3TransformCoord(XMLoadFloat3(&iIndices[k]), matTerrainMatrix));

	/*
	0	1
	3	2
	*/
	_float		fWidth = vPos.x - iIndices[0].x;
	_float		fHeight = iIndices[0].z - vPos.z;

	_vector		vPlane;


	/*
	0		1
	p


	3		2
	플레이어가 오른쪽 상단 삼각형에 있을 경우(012)

	0		1


	p
	3		2
	플레이어가 왼쪽 하단 삼각형에 있을 경우(023)
	*/

	if (fWidth > fHeight)
	{
		vPlane = XMPlaneFromPoints(
			  XMLoadFloat3(&iIndices[0])
			, XMLoadFloat3(&iIndices[1])
			, XMLoadFloat3(&iIndices[2]));
	}
	else
	{
		vPlane = XMPlaneFromPoints(
			  XMLoadFloat3(&iIndices[0])
			, XMLoadFloat3(&iIndices[2])
			, XMLoadFloat3(&iIndices[3]));
	}

	vPos.y = (-XMVectorGetX(vPlane) * vPos.x - XMVectorGetZ(vPlane) * vPos.z - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);

	return vPos;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const _tchar * pHeightMapFilePath, const _tchar * pShaderFilePath, const char * pTechinqueName)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphicDevice,pContextDevice);
	if (FAILED(pInstance->NativeConstruct(pHeightMapFilePath, pShaderFilePath, pTechinqueName)))
		Safe_Release(pInstance);

	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const DESC& tVIBufferDesc)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tVIBufferDesc)))
		Safe_Release(pInstance);

	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct()))
		Safe_Release(pInstance);
	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
}
