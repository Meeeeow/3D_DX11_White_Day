#pragma once
#ifndef __ENGINE_LEVEL_MANAGER_H__
#define __ENGINE_LEVEL_MANAGER_H__

#include "Level.h"

BEGIN(Engine)

class CLevelManager final : public CBase
{
	DECLARE_SINGLETON(CLevelManager)
private:
	explicit CLevelManager();
	virtual ~CLevelManager() DEFAULT;

public:
	HRESULT				Open_Level(CLevel* pLevel);
	_int				Tick(_double dTimeDelta);
	HRESULT				Render();
	CNavigationMesh*	Get_NavMesh();
	CLevel*				Get_CurrentLevel() { return m_pCurrentLevel; }

private:
	CLevel*				m_pCurrentLevel = nullptr;

public:
	virtual	void		Free() override;
};

END
#endif // !__ENGINE_LEVEL_MANAGER_H__
