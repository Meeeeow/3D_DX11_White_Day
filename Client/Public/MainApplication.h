#pragma once
#ifndef __CLIENT_MAIN_APP_H__
#define __CLIENT_MAIN_APP_H__

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)
class CMainApplication final : public CBase
{
private:
	CMainApplication();
	virtual ~CMainApplication() DEFAULT;

public:
	HRESULT						NativeConstruct();
	_int						Tick(_double dTimeDelta);
	HRESULT						Render();

private:
	HRESULT						Open_Level(LEVEL eLevel);
	HRESULT						Ready_Component();
	

private:
	CGameInstance*				m_pGameInstance = nullptr;
	CRenderer*					m_pRenderer = nullptr;

private:
	_pGraphicDevice*			m_pGraphicDevice = nullptr;
	_pContextDevice*			m_pContextDevice = nullptr;

public:
	static	CMainApplication*	Create();
	virtual	void				Free() override;
};
END

#endif // !__CLIENT_MAIN_APP_H__