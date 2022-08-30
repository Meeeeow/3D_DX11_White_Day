#include "..\Public\VIBuffer_Instance.h"


CVIBuffer_Instance::CVIBuffer_Instance(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, _uint iNumInstance)
	: CVIBuffer(pGraphicDevice, pContextDevice), m_iNumInstance(iNumInstance)
{

}

CVIBuffer_Instance::CVIBuffer_Instance(const CVIBuffer_Instance & rhs)
	: CVIBuffer(rhs), m_iNumInstance(rhs.m_iNumInstance), m_iInstanceStride(rhs.m_iInstanceStride)
	, m_pInstanceBuffer(rhs.m_pInstanceBuffer), m_pInstanceVertices(rhs.m_pInstanceVertices)
	, m_tInstanceBufferDesc(rhs.m_tInstanceBufferDesc), m_tInstanceBufferSRD(rhs.m_tInstanceBufferSRD)
	, m_iIndexPerInstance(rhs.m_iIndexPerInstance)
{

}

HRESULT CVIBuffer_Instance::NativeConstruct()
{
	return S_OK;
}

HRESULT CVIBuffer_Instance::Clone_Construct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Instance::Render(_uint iIndex)
{
	if (iIndex >= m_vecPassDesc.size())
		return E_FAIL;

	ID3D11Buffer*	pVertexBuffers[] = {
		m_pVB,
		m_pInstanceBuffer
	};

	_uint			iStrides[] = {
		m_iStride,
		m_iInstanceStride
	};

	_uint			iOffsets[] = {
		0,
		0
	};
	
	m_pContextDevice->IASetVertexBuffers(0, 2, pVertexBuffers, iStrides, iOffsets);
	m_pContextDevice->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContextDevice->IASetInputLayout(m_vecPassDesc[iIndex]->pInputLayOut);
	m_pContextDevice->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/* 쉐이터 패스를 시작한다. */
	m_vecPassDesc[iIndex]->pEffectPass->Apply(0, m_pContextDevice);

	m_pContextDevice->DrawIndexedInstanced(m_iIndexPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer_Instance::Create_InstanceBuffer()
{
	if (m_pContextDevice == nullptr)
		return E_FAIL;

	if (FAILED(m_pGraphicDevice->CreateBuffer(&m_tInstanceBufferDesc, &m_tInstanceBufferSRD, &m_pInstanceBuffer)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer_Instance::Free()
{
	__super::Free();

	Safe_Release(m_pInstanceBuffer);
}
