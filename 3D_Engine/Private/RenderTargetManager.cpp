#include "..\Public\RenderTargetManager.h"

IMPLEMENT_SINGLETON(CRenderTargetManager)

CRenderTargetManager::CRenderTargetManager()
{

}

HRESULT CRenderTargetManager::NativeConstruct(_pContextDevice * pContextDevice)
{
	if (pContextDevice == nullptr)
		return E_FAIL;

	m_pContextDevice = pContextDevice;
	Safe_AddRef(m_pContextDevice);

	return S_OK;
}

HRESULT CRenderTargetManager::Add_RenderTarget(_pGraphicDevice * pGrpahicDevice, _pContextDevice * pContextDevice, const _tchar * pRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, _uint iRenderIndex, _float4 vClearColor)
{
	if (Find_RenderTarget(pRenderTargetTag) != nullptr)
		return S_OK;

	CRenderTarget*	pRenderTarget = CRenderTarget::Create(pGrpahicDevice, pContextDevice
		, iWidth, iHeight, ePixelFormat, iRenderIndex, vClearColor);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	m_mapRenderTargets.emplace(pRenderTargetTag, pRenderTarget);
	return S_OK;
}

HRESULT CRenderTargetManager::Add_MRT(const _tchar * pMRTTag, const _tchar * pRenderTargetTag)
{
	CRenderTarget*			pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	list<CRenderTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (pMRTList == nullptr)
	{
		list<CRenderTarget*>	MRTList;
		MRTList.push_back(pRenderTarget);

		m_mapMRTs.emplace(pMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	return S_OK;
}

CRenderTarget * CRenderTargetManager::Find_RenderTarget(const _tchar * pRenderTargetTag)
{
	auto	iter = find_if(m_mapRenderTargets.begin(), m_mapRenderTargets.end(), CTagFinder(pRenderTargetTag));

	if(iter == m_mapRenderTargets.end())
		return nullptr;

	return iter->second;
}

list<CRenderTarget*>* CRenderTargetManager::Find_MRT(const _tchar * pMRTTag)
{
	auto	iter = find_if(m_mapMRTs.begin(), m_mapMRTs.end(), CTagFinder(pMRTTag));

	if(iter == m_mapMRTs.end())
		return nullptr;

	return &iter->second;
}

ID3D11ShaderResourceView * CRenderTargetManager::Get_SRV(const _tchar * pRenderTargetTag)
{
	CRenderTarget* pInstance = Find_RenderTarget(pRenderTargetTag);
	if (pInstance == nullptr)
		return nullptr;

	return pInstance->Get_SRV();
}

HRESULT CRenderTargetManager::Set_Buffer(const _tchar * pRenderTargetTag, _float4 vScreenPos)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	return pRenderTarget->Set_Buffer(vScreenPos);
}

HRESULT CRenderTargetManager::Set_Buffer(const _tchar * pRenderTargetTag, const _tchar * pShaderFilePath, const char * pTechniqueName, _float4 vScreenPos)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	if (pRenderTarget == nullptr)
		return E_FAIL;

	return pRenderTarget->Set_Buffer(pShaderFilePath, pTechniqueName, vScreenPos);
}

HRESULT CRenderTargetManager::Begin_MRT(const _tchar * pMRTTag)
{
	list<CRenderTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (pMRTList == nullptr)
		return E_FAIL;

	m_pContextDevice->OMGetRenderTargets(1, &m_pDefaultRTV, &m_pDefaultDSV);

	_uint iNumView = (_uint)pMRTList->size();

	ID3D11RenderTargetView*	pRTVs[8] = { nullptr };

	_uint iNumRTV = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		pRTVs[iNumRTV] = pRenderTarget->Get_RTV();
		if (pRTVs[iNumRTV] == nullptr)
			return E_FAIL;
		++iNumRTV;
	}

	m_pContextDevice->OMSetRenderTargets(iNumView, pRTVs, m_pDefaultDSV);
	return S_OK;
}

HRESULT CRenderTargetManager::End_MRT()
{
	m_pContextDevice->OMSetRenderTargets(1, &m_pDefaultRTV, m_pDefaultDSV);

	Safe_Release(m_pDefaultDSV);
	Safe_Release(m_pDefaultRTV);

	return S_OK;
}

HRESULT CRenderTargetManager::Render()
{
	if (!m_bRender)
		return S_OK;

	for (auto& Pair : m_mapMRTs)
	{
		for (auto& RenderTarget : Pair.second)
			RenderTarget->Render();
	}

	return S_OK;
}

void CRenderTargetManager::Free()
{
	for (auto& Pair : m_mapRenderTargets)
		Safe_Release(Pair.second);
	m_mapRenderTargets.clear();

	for (auto& Pair : m_mapMRTs)
	{
		for (auto& List : Pair.second)
			Safe_Release(List);
	}
	m_mapMRTs.clear();

	Safe_Release(m_pContextDevice);
}

