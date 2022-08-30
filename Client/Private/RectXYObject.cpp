#include "stdafx.h"
#include "..\Public\RectXYObject.h"

CRectXYObject::CRectXYObject(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CGameObject(pGraphicDevice, pContextDevice)
{
}

CRectXYObject::CRectXYObject(const CRectXYObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRectXYObject::NativeConstruct()
{
	return S_OK;
}

HRESULT CRectXYObject::Clone_Construct(void * pArg)
{
	return S_OK;
}

_int CRectXYObject::Tick(_double dTimeDelta)
{
	return _int();
}

_int CRectXYObject::LateTick(_double dTimeDelta)
{
	
	return _int();
}

HRESULT CRectXYObject::Render()
{

	m_pVIBuffer->Render(1);
	return S_OK;
}

HRESULT CRectXYObject::Set_Components()
{
	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeRenderer, ComRenderer, (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeTransform, ComTransform, (CComponent**)&m_pTransform)))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_STATIC, PrototypeRectXY, ComVIBuffer, (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	return S_OK;
}

CGameObject * CRectXYObject::Clone(void * pArg)
{
	return nullptr;
}

void CRectXYObject::Free()
{
	__super::Free();

	Safe_Release(m_pTexture);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransform);
}

void CRectXYObject::Compute_Distance()
{
}
