#include "..\Public\Component.h"


CComponent::CComponent(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: m_pGraphicDevice(pGraphicDevice)
	, m_pContextDevice(pContextDevice)
	, m_bCloned(false)
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);
}

CComponent::CComponent(const CComponent & rhs)
	: m_pGraphicDevice(rhs.m_pGraphicDevice)
	, m_pContextDevice(rhs.m_pContextDevice)
	, m_bCloned(true)
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);
}

HRESULT CComponent::NativeConstruct()
{
	return S_OK;
}

HRESULT CComponent::Clone_Construct(void * pArg)
{
	return S_OK;
}

void CComponent::Free()
{
	Safe_Release(m_pContextDevice);
	Safe_Release(m_pGraphicDevice);
}
