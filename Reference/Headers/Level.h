#pragma once
#ifndef __ENGINE_LEVEL_H__
#define __ENGINE_LEVEL_H__

#include "Base.h"
#include "NavigationMesh.h"

BEGIN(Engine)
class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CLevel() DEFAULT;

public:
	virtual	HRESULT			NativeConstruct(_uint iCurrentLevelID);
	virtual	_int			Tick(_double dTimeDelta);
	virtual	HRESULT			Render();

public:
	_uint					Get_CurrentLevelID() {
		return m_iCurrentLevelID;	}
	CNavigationMesh*		Get_NavMesh() {
		return m_pNavigationMesh;	}

protected:
	_pGraphicDevice*		m_pGraphicDevice = nullptr;
	_pContextDevice*		m_pContextDevice = nullptr;
	CNavigationMesh*		m_pNavigationMesh = nullptr;
	_uint					m_iCurrentLevelID;

public:
	virtual void			Free() override;

};
END
#endif // !__ENGINE_LEVEL_H__