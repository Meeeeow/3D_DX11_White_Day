#include "..\Public\LightManager.h"

IMPLEMENT_SINGLETON(CLightManager)

CLightManager::CLightManager()
{

}


HRESULT CLightManager::Add_Light(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const _tchar* pLightTag, const LIGHTDESC* tLightDesc)
{
	if (pGraphicDevice == nullptr ||
		pContextDevice == nullptr)
		FAILMSG("LightManager Add_Light - Device is nullptr");

	CLight*	pInstance = CLight::Create(pGraphicDevice, pContextDevice, tLightDesc);
	if (pInstance == nullptr)
		return E_FAIL;

	list<CLight*>* pLightList = Find_LightMap(pLightTag);
	if (pLightList == nullptr)
	{
		list<CLight*>	LightList;
		LightList.push_back(pInstance);

		m_mapLight.emplace(pLightTag, LightList);
	}
	else
		pLightList->push_back(pInstance);

	return S_OK;
}

HRESULT CLightManager::Add_Light(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const _tchar* pLightTag, const _tchar * pShaderFilePath, const char * pTechniqueName, const LIGHTDESC & tLightDesc)
{
	if (pGraphicDevice == nullptr ||
		pContextDevice == nullptr)
		FAILMSG("LightManager Add_Light - Device is nullptr");

	CLight*	pInstance = CLight::Create(pGraphicDevice, pContextDevice, pShaderFilePath, pTechniqueName, tLightDesc);
	if (pInstance == nullptr)
		return E_FAIL;


	list<CLight*>* pLightList = Find_LightMap(pLightTag);
	if (pLightList == nullptr)
	{
		list<CLight*>	LightList;
		LightList.push_back(pInstance);

		m_mapLight.emplace(pLightTag, LightList);
	}
	else
		pLightList->push_back(pInstance);

	return S_OK;
}

HRESULT CLightManager::Add_Light(const _tchar* pLightTag, CLight * pInstance)
{
	if (pInstance == nullptr)
		FAILMSG("LightManager Add_Light - Light is nullptr");


	list<CLight*>* pLightList = Find_LightMap(pLightTag);
	if (pLightList == nullptr)
	{
		list<CLight*>	LightList;
		LightList.push_back(pInstance);

		m_mapLight.emplace(pLightTag, LightList);
	}
	else
		pLightList->push_back(pInstance);


	return S_OK;
}

HRESULT CLightManager::Render()
{
	for (auto& Pair : m_mapLight)
	{
		for (auto& pLight : Pair.second)
		{
			if(pLight != nullptr)
				pLight->Render();
		}
	}
	/*list<CLight*>* pLights = Find_LightMap(L"Light");
	if (pLights != nullptr)
	{
		for (auto& pLight : *pLights)
			pLight->Render();
	}*/

	return S_OK;
}

list<CLight*>* CLightManager::Find_LightMap(const _tchar * pLightTag)
{
	auto& iter = find_if(m_mapLight.begin(), m_mapLight.end(), CTagFinder(pLightTag));

	if (iter == m_mapLight.end())
		return nullptr;

	return &iter->second;
}

HRESULT CLightManager::LightMap_Action(const _tchar * pLightTag)
{
	list<CLight*>* pLightList = Find_LightMap(pLightTag);
	if (pLightList == nullptr)
		return E_FAIL;

	for (auto& pLight : *pLightList)
		pLight->Action();

	return S_OK;
}

HRESULT CLightManager::LightMap_DeActive(const _tchar * pLightTag)
{
	list<CLight*>* pLightList = Find_LightMap(pLightTag);
	if (pLightList == nullptr)
		return E_FAIL;

	for (auto& pLight : *pLightList)
		pLight->DeActive();

	return S_OK;
}

HRESULT CLightManager::DeActive_All()
{
	for (auto& Pair : m_mapLight)
	{
		for (auto& pLight : Pair.second)
			pLight->DeActive();
	}

	return S_OK;
}

LIGHTDESC & CLightManager::Get_LightDesc(const _tchar * pLightTag, _uint iLightIndex)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	list<CLight*>* pLightList = Find_LightMap(pLightTag);
	if (pLightList == nullptr)
		return LIGHTDESC();
	
	CLight* pLight = pLightList->front() + iLightIndex;

	return pLight->Get_Desc();
}

void CLightManager::Free()
{
	for (auto& Pair : m_mapLight)
	{
		for (auto& pLight : Pair.second)
			Safe_Release(pLight);
		Pair.second.clear();
	}
	m_mapLight.clear();
}
