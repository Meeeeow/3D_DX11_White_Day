#pragma once
#ifndef __CLIENT_LEVEL_MAZE_H__
#define __CLIENT_LEVEL_MAZE_H__

#include "Level.h"
class CMaze final : public CLevel
{
private:
	CMaze(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CMaze() DEFAULT;

public:
	virtual HRESULT			NativeConstruct(_uint iLevelID);
	virtual _int			Tick(_double dTimeDelta) override;

	_double					m_dTimeAcc = 0.0;

public:
	static	CMaze*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, _uint iLevelID);
	virtual void			Free() override;
		
};


#endif // !__CLIENT_LEVEL_MAZE_H__