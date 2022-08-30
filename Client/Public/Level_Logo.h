#pragma once
#ifndef __CLIENT_LEVEL_LOGO_H__
#define __CLIENT_LEVEL_LOGO_H__
#include "Level.h"
BEGIN(Client)
class CLevel_Logo final : public CLevel
{
private:
	CLevel_Logo(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CLevel_Logo() DEFAULT;

public:
	virtual	HRESULT			NativeConstruct(_uint iCurrentLevelID) override;
	virtual _int			Tick(_double dTimeDelta) override;
	virtual HRESULT			Render() override;

private:
	HRESULT					Ready_GameObject();
	HRESULT					Ready_BackGround(const _tchar* pLayerTag);
	_double					m_dTimeDelta = 0.0;

public:
	static	CLevel_Logo*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, _uint iCurrentLevelID);
	virtual void			Free() override;

};
END
#endif // !__CLIENT_LEVEL_LOGO_H__
