#include "..\Public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphicDevice(CGraphicDevice::Get_Instance()), m_pInputDevice(CInputDevice::Get_Instance()), m_pTimerManager(CTimerManager::Get_Instance())
	, m_pComponentManager(CComponentManager::Get_Instance()), m_pLevelManager(CLevelManager::Get_Instance()), m_pObjectManager(CObjectManager::Get_Instance())
	, m_pPipeLine(CPipeLine::Get_Instance()), m_pLightManager(CLightManager::Get_Instance()), m_pFrustum(CFrustum::Get_Instance())
	, m_pPicking(CPicking::Get_Instance()), m_pRenderTargetManager(CRenderTargetManager::Get_Instance()), m_pPickingEvent(CPickingEventManager::Get_Instance())
{
	/* System Category */
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pInputDevice);
	Safe_AddRef(m_pTimerManager);

	/* Utility Category */
	Safe_AddRef(m_pComponentManager);
	Safe_AddRef(m_pLevelManager);
	Safe_AddRef(m_pObjectManager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pLightManager);
	Safe_AddRef(m_pFrustum);
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pPickingEvent);
	Safe_AddRef(m_pRenderTargetManager);
}

HRESULT CGameInstance::Initialize(HINSTANCE hInstance, CGraphicDevice::DESC& tGraphicDeviceDesc, _uint iNumLevels, _pGraphicDevice ** ppOutGraphicDevice, _pContextDevice ** ppOutContextDevice)
{
	if (m_pGraphicDevice == nullptr)
		FAILMSG("GraphicDevice is nullptr - GameInstance Initialize");

	if (FAILED(m_pGraphicDevice->NativeConstruct(tGraphicDeviceDesc.hWnd, tGraphicDeviceDesc.eWinMode, tGraphicDeviceDesc.iWinCX, tGraphicDeviceDesc.iWinCY,
		ppOutGraphicDevice, ppOutContextDevice)))
		return E_FAIL;

	if (m_pInputDevice == nullptr)
		FAILMSG("InputDevice is nullptr - GameInstance Initialize");

	if (FAILED(m_pInputDevice->NativeConstruct(hInstance, tGraphicDeviceDesc.hWnd)))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->NativeConstruct(*ppOutContextDevice)))
		return E_FAIL;

	if (FAILED(m_pPicking->NativeConstruct(*ppOutContextDevice, tGraphicDeviceDesc.hWnd)))
		return E_FAIL;

	if(m_pObjectManager == nullptr) 
		FAILMSG("ObjectManager is nullptr - GameInstance Initialize");
	
	if(FAILED(m_pObjectManager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	if (m_pComponentManager == nullptr)
		FAILMSG("ComponentManager is nullptr - GameInstance Initialize");

	if (FAILED(m_pComponentManager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pPickingEvent->NativeConstruct(*ppOutGraphicDevice, *ppOutContextDevice, tGraphicDeviceDesc.iWinCX, tGraphicDeviceDesc.iWinCY)))
		return E_FAIL;

	return S_OK;
}

_int CGameInstance::Tick(_double dTimeDelta)
{
	if (m_pInputDevice == nullptr)
		FAILMSG("InputDevice is nullptr - GameInstance Tick");

	if (FAILED(m_pInputDevice->Update_InputDeviceState()))
		return E_FAIL;

	if(m_pPickingEvent->Tick(dTimeDelta) < 0)
		FAILMSG("PickingEvent Manager Tick return under the 0 - GameInstnace Tick");

	if (m_pObjectManager->Tick(dTimeDelta) < 0)
		FAILMSG("ObjectManager Tick return under the 0 - GameInstnace Tick");

	if (m_pPipeLine == nullptr)
		FAILMSG("PipeLine is nullptr - GameInstance Tick");
	
	if (m_pPipeLine->Update_Variables())
		return E_FAIL;

	if (FAILED(m_pFrustum->Make_Frustum(m_pPipeLine->Get_Far())))	
		return E_FAIL;

	if (FAILED(m_pPicking->PreConvert()))
		return E_FAIL;


	if (m_pObjectManager->Collect_Event() < 0)
		return E_FAIL;

	if (m_pPickingEvent->LateTick(dTimeDelta) < 0)
		FAILMSG("PickingEvent Late Tick return under the 0 - GameInstnace Tick");

	if (m_pObjectManager->LateTick(dTimeDelta) < 0)
		FAILMSG("ObjectManager LateTick return under the 0 - GameInstance Tick");

	if (m_pLevelManager == nullptr)
		FAILMSG("LevelManager is nullptr - GameInstance Tick");

	if (m_pLevelManager->Tick(dTimeDelta) < 0)
		FAILMSG("LevelManager Tick return under the 0 - GameInstacne Tick");


	return 0;
}

void CGameInstance::Clear_Container(_uint iLevelIndex)
{
	m_pComponentManager->Clear(iLevelIndex);
	m_pObjectManager->Clear(iLevelIndex);
}

HRESULT CGameInstance::Initialize(HINSTANCE hInstance, CGraphicDevice::DESC & tGraphicDeviceDesc, HWND hWnd, _uint iNumLevels, _pGraphicDevice ** ppOutGraphicDevice, _pContextDevice ** ppOutContextDevice)
{
	if (m_pGraphicDevice == nullptr)
		FAILMSG("GraphicDevice is nullptr - GameInstance Initialize");

	if (FAILED(m_pGraphicDevice->NativeConstruct(tGraphicDeviceDesc.hWnd, tGraphicDeviceDesc.eWinMode, tGraphicDeviceDesc.iWinCX, tGraphicDeviceDesc.iWinCY,
		ppOutGraphicDevice, ppOutContextDevice)))
		return E_FAIL;

	if (m_pInputDevice == nullptr)
		FAILMSG("InputDevice is nullptr - GameInstance Initialize");

	if (FAILED(m_pInputDevice->NativeConstruct(hInstance, hWnd)))
		return E_FAIL;

	if (FAILED(m_pPicking->NativeConstruct(*ppOutContextDevice, hWnd)))
		return E_FAIL;

	if (FAILED(m_pRenderTargetManager->NativeConstruct(*ppOutContextDevice)))
		return E_FAIL;

	if (m_pObjectManager == nullptr)
		FAILMSG("ObjectManager is nullptr - GameInstance Initialize");

	if (FAILED(m_pObjectManager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	if (m_pComponentManager == nullptr)
		FAILMSG("ComponentManager is nullptr - GameInstance Initialize");

	if (FAILED(m_pComponentManager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBufferView(XMFLOAT4 vClearColor)
{
	return m_pGraphicDevice->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencilView()
{
	return m_pGraphicDevice->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	return m_pGraphicDevice->Present();
}

_byte CGameInstance::Get_DIKeyState(_ubyte btKeyID)
{
	return m_pInputDevice->Get_DIKeyState(btKeyID);
}

_byte CGameInstance::Get_DIMouseButtonState(CInputDevice::DIMB eMouseButton)
{
	return m_pInputDevice->Get_DIMouseButtonState(eMouseButton);
}

_long CGameInstance::Get_DIMouseMoveState(CInputDevice::DIMM eMouseMove)
{
	return m_pInputDevice->Get_DIMouseMoveState(eMouseMove);
}

_bool CGameInstance::Key_Pressing(_ubyte btKeyID)
{
	return m_pInputDevice->Key_Pressing(btKeyID);
}

_bool CGameInstance::Key_Down(_ubyte btKeyID)
{
	return m_pInputDevice->Key_Down(btKeyID);
}

_bool CGameInstance::Key_Up(_ubyte btKeyID)
{
	return m_pInputDevice->Key_Up(btKeyID);
}

_bool CGameInstance::Mouse_Pressing(CInputDevice::DIMB eMouseButton)
{
	return m_pInputDevice->Mouse_Pressing(eMouseButton);
}

_bool CGameInstance::Mouse_Down(CInputDevice::DIMB eMouseButton)
{
	return m_pInputDevice->Mouse_Down(eMouseButton);
}

_bool CGameInstance::Mouse_Up(CInputDevice::DIMB eMouseButton)
{
	return m_pInputDevice->Mouse_Up(eMouseButton);
}

HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	return m_pTimerManager->Add_Timer(pTimerTag);
}

_double CGameInstance::Compute_Timer(const _tchar * pTimerTag)
{
	return m_pTimerManager->Compute_Timer(pTimerTag);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pComponent)
{
	return m_pComponentManager->Add_Prototype(iLevelIndex,pPrototypeTag,pComponent);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	return m_pComponentManager->Clone_Component(iLevelIndex,pPrototypeTag,pArg);
}

_bool CGameInstance::Prototype_NameCheck(_uint iLevelIndex, const _tchar * pPrototypeTag)
{
	return m_pComponentManager->Prototype_NameCheck(iLevelIndex, pPrototypeTag);
}

HRESULT CGameInstance::Open_Level(CLevel * pLevel)
{
	return m_pLevelManager->Open_Level(pLevel);
}

CNavigationMesh * CGameInstance::Get_CurrentNav()
{
	return m_pLevelManager->Get_NavMesh();
}

CLevel * CGameInstance::Get_CurrentLevel()
{
	return m_pLevelManager->Get_CurrentLevel();
}

HRESULT CGameInstance::Render_Level()
{
	return m_pLevelManager->Render();
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pGameObject)
{
	return m_pObjectManager->Add_Prototype(pPrototypeTag, pGameObject);
}

HRESULT CGameInstance::Insert_ObjectToLayer(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg)
{
	return m_pObjectManager->Insert_ObjectToLayer(iLevelIndex, pPrototypeTag, pLayerTag, pArg);
}

HRESULT CGameInstance::Insert_ObjectToLayer(_uint iLevelIndex, CGameObject * pGameObject, const _tchar * pLayerTag)
{
	return m_pObjectManager->Insert_ObjectToLayer(iLevelIndex, pGameObject, pLayerTag);
}

CGameObject * CGameInstance::Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex)
{
	return m_pObjectManager->Get_GameObject(iLevelIndex, pLayerTag, iIndex);
}

CComponent * CGameInstance::Get_Components(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex, const _tchar * pComponentTag)
{
	CGameObject* pGameObject = m_pObjectManager->Get_GameObject(iLevelIndex, pLayerTag, iIndex);
	
	return pGameObject->Get_Component(pComponentTag);
}

list<CGameObject*>* CGameInstance::Get_GameObjects(_uint iLevelIndex, const _tchar * pLayerTag)
{
	return m_pObjectManager->Get_Layer(iLevelIndex, pLayerTag);
}

_matrix CGameInstance::Get_Transform(CPipeLine::PIPESTATE eState)
{
	return m_pPipeLine->Get_Transform(eState);
}

_matrix CGameInstance::Get_Transpose(CPipeLine::PIPESTATE eState)
{
	return m_pPipeLine->Get_TransposeTransform(eState);
}

_matrix CGameInstance::Get_Inverse(CPipeLine::PIPESTATE eState)
{
	return m_pPipeLine->Get_InverseTransform(eState);
}

_vector CGameInstance::Get_CameraPosition()
{
	return m_pPipeLine->Get_CamPos();
}

_float CGameInstance::Get_Far()
{
	return m_pPipeLine->Get_Far();
}

HRESULT CGameInstance::Add_Light(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const _tchar * pLightTag, const LIGHTDESC* tLightDesc)
{
	return m_pLightManager->Add_Light(pGraphicDevice,pContextDevice, pLightTag, tLightDesc);
}

HRESULT CGameInstance::Add_Light(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, const _tchar * pLightTag, const _tchar * pShaderFilePath, const char * pTechniqueName, const LIGHTDESC & tLightDesc)
{
	return m_pLightManager->Add_Light(pGraphicDevice,pContextDevice,pLightTag,pShaderFilePath, pTechniqueName, tLightDesc);
}

HRESULT CGameInstance::Add_Light(const _tchar * pLightTag, CLight * pInstance)
{
	return m_pLightManager->Add_Light(pLightTag, pInstance);
}

HRESULT CGameInstance::Light_Action(const _tchar * pLightTag)
{
	return m_pLightManager->LightMap_Action(pLightTag);
}

HRESULT CGameInstance::Light_DeActive(const _tchar * pLightTag)
{
	return m_pLightManager->LightMap_DeActive(pLightTag);
}

LIGHTDESC & CGameInstance::Get_Light(const _tchar * pLightTag, _uint iLightIndex)
{
	return m_pLightManager->Get_LightDesc(pLightTag, iLightIndex);
}

HRESULT CGameInstance::DeActiveAll()
{
	return m_pLightManager->DeActive_All();
}

_bool CGameInstance::Culling(CTransform * pTransform, _float fRadius)
{
	return m_pFrustum->Culling_Radius(pTransform,fRadius);
}

_bool CGameInstance::Culling(_fvector vPos, _matrix matWorld, _float fRadius)
{
	return m_pFrustum->Culling_PointRadius(vPos,matWorld,fRadius);
}

_bool CGameInstance::Picking(_fvector v0, _fvector v1, _fvector v2, _float fDistance, _fmatrix matWorld)
{
	return m_pPicking->Picking(v0,v1,v2,fDistance,matWorld);
}

_fvector CGameInstance::Get_Pivot()
{
	return m_pPicking->Get_Pivot();
}

_fvector CGameInstance::Get_Ray()
{
	return m_pPicking->Get_Ray();
}

void CGameInstance::Add_Event(CPickingEventManager::PE & tEvent)
{
	m_pPickingEvent->Add_Event(tEvent);
}

void CGameInstance::Action()
{
	m_pPickingEvent->Action();
}

CPickingEventManager::FSM CGameInstance::Get_FSM()
{
	return m_pPickingEvent->Get_FSM();
}

void CGameInstance::Release_Engine()
{
	if (0 != CGameInstance::Get_Instance()->Destroy_Instance())
		MSGBOX("CGameInstance Release != 0");

	if (0 != CObjectManager::Get_Instance()->Destroy_Instance())
		MSGBOX("CObjectManager Release != 0");

	if (0 != CLevelManager::Get_Instance()->Destroy_Instance())
		MSGBOX("CLevelManager Release != 0");

	if (0 != CPicking::Get_Instance()->Destroy_Instance())
		MSGBOX("CPicking Release != 0");

	if (0 != CPickingEventManager::Get_Instance()->Destroy_Instance())
		MSGBOX("CPickingEventManager Release != 0");

	if (0 != CComponentManager::Get_Instance()->Destroy_Instance())
		MSGBOX("CComponentManager Release != 0");

	if (0 != CRenderTargetManager::Get_Instance()->Destroy_Instance())
		MSGBOX("CRenderTargetManager Release != 0");

	if (0 != CLightManager::Get_Instance()->Destroy_Instance())
		MSGBOX("CLightManager Release != 0");

	if (0 != CFrustum::Get_Instance()->Destroy_Instance())
		MSGBOX("CFrustum Release != 0");

	if (0 != CPipeLine::Get_Instance()->Destroy_Instance())
		MSGBOX("CPipeLine Release != 0");

	if (0 != CTimerManager::Get_Instance()->Destroy_Instance())
		MSGBOX("CTimerManager Release != 0");

	if (0 != CInputDevice::Get_Instance()->Destroy_Instance())
		MSGBOX("CInputDevice Release != 0");

	if (0 != CGraphicDevice::Get_Instance()->Destroy_Instance())
		MSGBOX("CGraphicDevice Release != 0");
}

void CGameInstance::Free()
{
	/* Utility Category */
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pLevelManager);
	Safe_Release(m_pPickingEvent);
	Safe_Release(m_pObjectManager);
	Safe_Release(m_pRenderTargetManager);
	Safe_Release(m_pLightManager);
	Safe_Release(m_pComponentManager);

	Safe_Release(m_pFrustum);
	Safe_Release(m_pPicking);

	/* System Category */
	Safe_Release(m_pTimerManager);
	Safe_Release(m_pInputDevice);
	Safe_Release(m_pGraphicDevice);
}
