#pragma once
#ifndef __ENGINE_GAME_OBJECT_H__
#define __ENGINE_GAME_OBJECT_H__

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef	unordered_map<const _tchar*, CComponent*>		COMPONENTS;

protected:
	CGameObject(_pGraphicDevice* pGraphicDevice,_pContextDevice* pContextDevice);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() DEFAULT;

public:
	virtual HRESULT			NativeConstruct();
	virtual HRESULT			Clone_Construct(void* pArg);
	virtual _int			Tick(_double dTimeDelta);
	virtual _int			LateTick(_double dTimeDelta);
	virtual HRESULT			Render();

public:
	virtual void			Picking();
	virtual _bool			OnMouse();
	virtual void			Event();
	virtual void			Collect_Event();

public:
	CComponent*				Get_Component(const _tchar* pComponentTag);

protected:
	_pGraphicDevice*		m_pGraphicDevice = nullptr;
	_pContextDevice*		m_pContextDevice = nullptr;
	COMPONENTS				m_mapComponents;

protected:
	HRESULT					Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag
		, CComponent** ppOut, void* pArg = nullptr);
	


	_float					m_fCameraDistance = 0.f;
	virtual	void			Compute_Distance() PURE;

public:
	_float					Get_CameraDistance() { return m_fCameraDistance; }

public:
	virtual	CGameObject*	Clone(void* pArg) PURE;
	virtual void			Free();

};
END
#endif // !__ENGINE_GAME_OBJECT_H__