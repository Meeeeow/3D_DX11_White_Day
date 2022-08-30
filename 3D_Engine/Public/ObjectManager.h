#pragma once
#ifndef __ENGINE_OBEJCT_MANAGER_H__
#define __ENGINE_OBEJCT_MANAGER_H__

#include "Layer.h"

class CObjectManager final : public CBase
{
	DECLARE_SINGLETON(CObjectManager)

public:
	typedef unordered_map<const _tchar*, CGameObject*>		PROTOTYPES;
	typedef unordered_map<const _tchar*, CLayer*>			LAYERS;

private:
	CObjectManager();
	virtual ~CObjectManager() DEFAULT;

public:
	CGameObject*		Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex);

public:
	HRESULT				Reserve_Container(_uint iNumLevels);
	HRESULT				Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pGameObject);
	HRESULT				Insert_ObjectToLayer(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg);
	HRESULT				Insert_ObjectToLayer(_uint iLevelIndex, CGameObject* pGameObject, const _tchar* pLayerTag);
	_int				Tick(_double dTimeDelta);
	_int				LateTick(_double dTimeDelta);
	_int				Collect_Event();
	void				Clear(_uint iLevelIndex);

	list<CGameObject*>*	Get_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

private:
	PROTOTYPES			m_mapPrototype;
	LAYERS*				m_pLayers = nullptr;

	_uint				m_iNumLevels = 0;

public:
	virtual	void		Free() override;
	
};


#endif // !__ENGINE_OBEJCT_MANAGER_H__
