#include "stdafx.h"
#include "ToolCamera.h"

CToolCamera::CToolCamera(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CCamera(pGraphicDevice, pContextDevice)
{

}

HRESULT CToolCamera::NativeConstruct(const DESC& tDesc)
{
	if (FAILED(__super::Clone_Construct((void*)&tDesc)))
		return E_FAIL;


	return S_OK;
}

_int CToolCamera::Tick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	D3D11_VIEWPORT		tViewportDesc;
	ZeroMemory(&tViewportDesc, sizeof(D3D11_VIEWPORT));
	_uint iNumViews = 1;
	m_pContextDevice->RSGetViewports(&iNumViews, &tViewportDesc);
	POINT pt = { tViewportDesc.Width / 2.f , tViewportDesc.Height / 2.f };

	ClientToScreen(g_hWnd, &pt);

	SetCursorPos(pt.x, pt.y);

	Move_Free(dTimeDelta, pGameInstance);



	
	return __super::Tick(dTimeDelta);
}

_int CToolCamera::FalseTick(_double dTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	Move_Free(dTimeDelta, pGameInstance);


	
	return __super::Tick(dTimeDelta);
}

_int CToolCamera::LateTick(_double dTimeDelta)
{
	return _int();
}

HRESULT CToolCamera::Move_Free(_double dTimeDelta, CGameInstance * pGameInstance)
{
	if (!m_bAction)
		return S_OK;
	

	if ((pGameInstance->Key_Pressing(DIK_LEFTARROW)))
		m_pTransform->Move_Left(dTimeDelta);
	if ((pGameInstance->Key_Pressing(DIK_RIGHTARROW)))
		m_pTransform->Move_Right(dTimeDelta);
	if ((pGameInstance->Key_Pressing(DIK_UPARROW)))
		m_pTransform->Move_Front(dTimeDelta);
	if ((pGameInstance->Key_Pressing(DIK_DOWNARROW)))
		m_pTransform->Move_Back(dTimeDelta);

	if (pGameInstance->Mouse_Pressing(CInputDevice::DIMB_RB))
	{
		_float fX = pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_X);
		_float fY = pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_Y);

		m_pTransform->Rotation_Axis(dTimeDelta * fX * 0.05f);
		m_pTransform->Rotation_Axis(dTimeDelta * fY * 0.05f, m_pTransform->Get_State(CTransform::STATE_RIGHT));
	}
	return S_OK;
}

CToolCamera * CToolCamera::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const DESC& tDesc)
{
	CToolCamera* pInstance = new CToolCamera(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(tDesc)))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject * CToolCamera::Clone(void * pArg)
{
	return nullptr;
}

void CToolCamera::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
}
