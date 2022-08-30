#pragma once
#ifndef __ENGINE_GAME_INSTANCE_H__
#define __ENGINE_GAME_INSTANCE_H__

/* System Category Manager */
#include "GraphicDevice.h"
#include "InputDevice.h"
#include "TimerManager.h"

/* Utility Category Manager */
#include "ComponentManager.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "PipeLine.h"
#include "LightManager.h"
#include "RenderTargetManager.h"
#include "Frustum.h"
#include "Picking.h"
#include "PickingEventManager.h"


BEGIN(Engine)
class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() DEFAULT;

public: /* Common Function */
	HRESULT				Initialize(HINSTANCE hInstance, CGraphicDevice::DESC& tGraphicDeviceDesc,
		_uint iNumLevels, _pGraphicDevice** ppOutGraphicDevice, _pContextDevice** ppOutContextDevice);
	_int				Tick(_double dTimeDelta);
	void				Clear_Container(_uint iLevelIndex);

public: /* MFC */
	HRESULT				Initialize(HINSTANCE hInstance, CGraphicDevice::DESC& tGraphicDeviceDesc,
		HWND hWnd, _uint iNumLevels, _pGraphicDevice** ppOutGraphicDevice, _pContextDevice** ppOutContextDevice);

public: 
	/* System Category Function */

/*1. GraphicDevice */
	HRESULT				Clear_BackBufferView(XMFLOAT4 vClearColor = XMFLOAT4(1.f,1.f,1.f,1.f));
	HRESULT				Clear_DepthStencilView();
	HRESULT				Present();

/*2. InputDevice */
	_byte				Get_DIKeyState(_ubyte btKeyID);
	_byte				Get_DIMouseButtonState(CInputDevice::DIMB eMouseButton);
	_long				Get_DIMouseMoveState(CInputDevice::DIMM eMouseMove);

	_bool				Key_Pressing(_ubyte btKeyID);
	_bool				Key_Down(_ubyte btKeyID);
	_bool				Key_Up(_ubyte btKeyID);

	_bool				Mouse_Pressing(CInputDevice::DIMB eMouseButton);
	_bool				Mouse_Down(CInputDevice::DIMB eMouseButton);
	_bool				Mouse_Up(CInputDevice::DIMB eMouseButton);

/*3. Timer Manager */
	HRESULT				Add_Timer(const _tchar* pTimerTag);
	_double				Compute_Timer(const _tchar* pTimerTag);

public:
	/* Utiltiy Category Fuction */

/*1. Component Manager */
	HRESULT				Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pComponent);
	CComponent*			Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
	_bool				Prototype_NameCheck(_uint iLevelIndex, const _tchar* pPrototypeTag);

/*2. Level Manager */
	HRESULT				Open_Level(CLevel* pLevel);
	CNavigationMesh*	Get_CurrentNav();
	CLevel*				Get_CurrentLevel();
	HRESULT				Render_Level();


/*3. Object Manager */
	HRESULT				Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pGameObject);
	HRESULT				Insert_ObjectToLayer(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);
	HRESULT				Insert_ObjectToLayer(_uint iLevelIndex, CGameObject* pGameObject, const _tchar* pLayerTag);
	CGameObject*		Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex);
	CComponent*			Get_Components(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex, const _tchar* pComponentTag);
	list<CGameObject*>*	Get_GameObjects(_uint iLevelIndex, const _tchar* pLayerTag);

/*4. Pipe Line */
	_matrix				Get_Transform(CPipeLine::PIPESTATE eState);
	_matrix				Get_Transpose(CPipeLine::PIPESTATE eState);
	_matrix				Get_Inverse(CPipeLine::PIPESTATE eState);
	_vector				Get_CameraPosition();
	_float				Get_Far();

/*5. Light Manager */
	HRESULT				Add_Light(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, const _tchar* pLightTag, const LIGHTDESC* tLightDesc);
	HRESULT				Add_Light(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, const _tchar* pLightTag, const _tchar* pShaderFilePath, const char* pTechniqueName, const LIGHTDESC& tLightDesc);
	HRESULT				Add_Light(const _tchar* pLightTag, CLight* pInstance);
	HRESULT				Light_Action(const _tchar* pLightTag);
	HRESULT				Light_DeActive(const _tchar* pLightTag);
	LIGHTDESC&			Get_Light(const _tchar* pLightTag, _uint iLightIndex = 0);
	HRESULT				DeActiveAll();

/*6. Render Target Manager */


/*7. Frustum Manager */
	_bool				Culling(CTransform* pTransform, _float fRadius);
	_bool				Culling(_fvector vPos, _matrix matWorld, _float fRadius);

/* 8. Picking */
	_bool				Picking(_fvector v0, _fvector v1, _fvector v2, _float fDistance, _fmatrix matWorld);
	_fvector			Get_Pivot();
	_fvector			Get_Ray();

/* 9. Picking Event Manager */
	void				Add_Event(CPickingEventManager::PE& tEvent);
	void				Action();
	CPickingEventManager::FSM Get_FSM();

private: // Linked Manager
	/* System Category */
	CGraphicDevice*						m_pGraphicDevice = nullptr;
	CInputDevice*						m_pInputDevice = nullptr;
	CTimerManager*						m_pTimerManager = nullptr;
	
	/* Utility Category */
	CComponentManager*					m_pComponentManager = nullptr;
	CLevelManager*						m_pLevelManager = nullptr;
	CObjectManager*						m_pObjectManager = nullptr;
	CPipeLine*							m_pPipeLine = nullptr;
	CLightManager*						m_pLightManager = nullptr;
	CFrustum*							m_pFrustum = nullptr;
	CPicking*							m_pPicking = nullptr;
	CPickingEventManager*				m_pPickingEvent = nullptr;
	CRenderTargetManager*				m_pRenderTargetManager = nullptr;

public:
	static	void						Release_Engine();
	virtual void						Free() override;
};
END

#endif // !__ENGINE_GAME_INSTANCE_H__
