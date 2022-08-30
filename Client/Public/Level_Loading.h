#pragma once
#ifndef __CLIENT_LEVEL_LOADING_H__
#define __CLIENT_LEVEL_LOADING_H__

#include "Client_Defines.h"
#include "Level.h"
#include "Loader.h"

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CLevel_Loading() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct(LEVEL eNextLevel, _uint iLevelID);
	virtual	_int				Tick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	LEVEL						m_eNextLevel = LEVEL_END;
	CLoader*					m_pLoader = nullptr;

public:
	static	CLevel_Loading*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, LEVEL eNextLevel, _uint iLevelIndex);
	virtual	void				Free() override;
};


#endif // !__CLIENT_LEVEL_LOADING_H__