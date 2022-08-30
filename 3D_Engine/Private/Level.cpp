#include "..\Public\Level.h"


CLevel::CLevel(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: m_pGraphicDevice(pGraphicDevice), m_pContextDevice(pContextDevice)
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);
}

HRESULT CLevel::NativeConstruct(_uint iCurrentLevelID)
{
	if (iCurrentLevelID < 0)
		FAILMSG("Level ID is small than Zero.");
	
	m_iCurrentLevelID = iCurrentLevelID;

	
	return S_OK;
}

_int CLevel::Tick(_double dTimeDelta)
{
	return _int();
}

HRESULT CLevel::Render()
{
	return S_OK;
}

void CLevel::Free()
{
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pContextDevice);
	Safe_Release(m_pNavigationMesh);
}
