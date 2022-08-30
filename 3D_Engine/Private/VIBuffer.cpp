#include "..\Public\VIBuffer.h"


CVIBuffer::CVIBuffer(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice)
	: CComponent(pGraphicDevice, pContextDevice)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs), m_iStride(rhs.m_iStride), m_iNumVertices(rhs.m_iNumVertices), m_iNumPrimitive(rhs.m_iNumPrimitive)
	, m_iNumIndicesPerPT(rhs.m_iNumIndicesPerPT), m_iFaceIndexSize(rhs.m_iFaceIndexSize), m_eIndexFormat(rhs.m_eIndexFormat)
	, m_eRenderType(rhs.m_eRenderType), m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB),m_pVertices(rhs.m_pVertices), m_pIndices(rhs.m_pIndices)
	, m_pEffect(rhs.m_pEffect), m_vecPassDesc(rhs.m_vecPassDesc)
{
	for (auto &pPassDesc : m_vecPassDesc)
		Safe_AddRef(pPassDesc->pInputLayOut);

	Safe_AddRef(m_pEffect);
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::NativeConstruct()
{
	return S_OK;
}

HRESULT CVIBuffer::Clone_Construct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Set_RawValue(const char * pConstantName, void * pData, _uint iByteLength)
{

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (pVariable == nullptr)
		FAILMSG("CVIBuffer::Set_RawValue - GetVariableByName");

	return pVariable->SetRawValue(pData, 0, iByteLength);
}

HRESULT CVIBuffer::Set_Texture(const char * pConstantName, ID3D11ShaderResourceView * pShaderResourceView)
{
	ID3DX11EffectShaderResourceVariable*	pVariable = m_pEffect->GetVariableByName(pConstantName)->AsShaderResource();
	if (pVariable == nullptr)
		return E_FAIL;

	return pVariable->SetResource(pShaderResourceView);
}

HRESULT CVIBuffer::Render(_uint iIndex)
{
	if (m_vecPassDesc.size() <= iIndex)
		FAILMSG("CVIBuffer::Render - Index too big");

	// 하나의 버퍼를 바인드한다로 가정
	// 다수의 버퍼를 사용해야 하는 경우, override 하여 자식에서 호출

	_uint iOffSet = 0;

	m_pContextDevice->IASetVertexBuffers(0, 1, &m_pVB, &m_iStride, &iOffSet);
	m_pContextDevice->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContextDevice->IASetInputLayout(m_vecPassDesc[iIndex]->pInputLayOut);
	m_pContextDevice->IASetPrimitiveTopology(m_eRenderType);

	m_vecPassDesc[iIndex]->pEffectPass->Apply(0, m_pContextDevice);

	m_pContextDevice->DrawIndexed(m_iNumPrimitive * m_iNumIndicesPerPT, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (m_pGraphicDevice == nullptr)
		FAILMSG("CVIBuffer::Create_VertexBuffer - GraphicDevice is nullptr");

	if (FAILED(m_pGraphicDevice->CreateBuffer(&m_tVB_Desc, &m_tVB_SubResourceData, &m_pVB)))
		FAILMSG("CVIBuffer::Create_VertexBuffer - CreateBuffer");

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (m_pGraphicDevice == nullptr)
		FAILMSG("CVIBuffer::Create_IndexBuffer - GraphicDevice is nullptr");

	if (FAILED(m_pGraphicDevice->CreateBuffer(&m_tIB_Desc, &m_tIB_SubResourceData, &m_pIB)))
		FAILMSG("CVIBuffer::Create_IndexBuffer - CreateBuffer");

	return S_OK;
}

HRESULT CVIBuffer::Compile_Shader(D3D11_INPUT_ELEMENT_DESC * pElement, _uint iNumElement, const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	_uint iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif // _DEBUG
	
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE, iFlag, 0, m_pGraphicDevice, &m_pEffect, nullptr)))
		FAILMSG("CVIBuffer::Compile_Shader - D3DX11CompileEffectFromFile");
	
	ID3DX11EffectTechnique*	pTechnique = m_pEffect->GetTechniqueByName(pTechniqueName);
	if (pTechnique == nullptr)
		FAILMSG("CVIBuffer::Compile_Shader - GetTechniqueByName is Failed");

	D3DX11_TECHNIQUE_DESC	tTechniqueDesc;
	ZeroMemory(&tTechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));
	
	if (FAILED(pTechnique->GetDesc(&tTechniqueDesc)))
		FAILMSG("CVIBuffer::Compile_Shader - GetDesc");
	
	m_vecPassDesc.reserve(tTechniqueDesc.Passes);

	for (_uint i = 0; i < tTechniqueDesc.Passes; ++i)
	{
		PASSDESC*	pPassDesc = new PASSDESC;
		ZeroMemory(pPassDesc, sizeof(PASSDESC));

		pPassDesc->pEffectPass = pTechnique->GetPassByIndex(i);
		if (pPassDesc->pEffectPass == nullptr)
			FAILMSG("CVIBuffer::Compile_Shader - EffectPass is nullptr");

		D3DX11_PASS_DESC	tLayOutDesc;
		ZeroMemory(&tLayOutDesc, sizeof(D3DX11_PASS_DESC));

		pPassDesc->pEffectPass->GetDesc(&tLayOutDesc);

		if (FAILED(m_pGraphicDevice->CreateInputLayout(pElement, iNumElement
			, tLayOutDesc.pIAInputSignature, tLayOutDesc.IAInputSignatureSize, &pPassDesc->pInputLayOut)))
			FAILMSG("CVIBuffer::Compile_Shader - CreateInputLayout");

		m_vecPassDesc.emplace_back(pPassDesc);
	}

	return S_OK;
}

_bool CVIBuffer::Picking(_float3 * pOut, _matrix matWorld)
{
	return false;
}

void CVIBuffer::Free()
{
	__super::Free();

	for (auto& pPassDesc : m_vecPassDesc)
		Safe_Release(pPassDesc->pInputLayOut);

	if (m_bCloned == false)
	{
		Safe_Delete_Arr(m_pVertices);
		Safe_Delete_Arr(m_pIndices);

		for (auto& pPassDesc : m_vecPassDesc)
		{
			Safe_Release(pPassDesc->pEffectPass);
			Safe_Delete(pPassDesc);
		}
	}

	m_vecPassDesc.clear();

	Safe_Release(m_pEffect);
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
}
