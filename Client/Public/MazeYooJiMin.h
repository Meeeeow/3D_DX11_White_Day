#pragma once
#ifndef __CLIENT_MAZE_YOO_JI_MIN_H__
#define __CLIENT_MAZE_YOO_JI_MIN_H__

#include "ModelObject.h"
BEGIN(Client)
class CMazeYooJiMin final : public CModelObject
{
private:
	CMazeYooJiMin(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CMazeYooJiMin() DEFAULT;

public:
	virtual HRESULT				NativeConstruct();
	virtual _int				Tick(_double dTimeDelta);
	virtual _int				LateTick(_double dTimeDelta);
	virtual HRESULT				Render() override;


	virtual void				Interaction(_double dTimeDelta);
	virtual void				Event() override;
	virtual void				Collect_Event() override;

	_bool						m_bAction = false;
	_double						m_dTimeAcc = 0.0;
	_bool						m_bDontAcc = false;
	_double						m_dEndTimeAcc = 0.0;
	_bool						m_bScreen = false;

public:
	static	CMazeYooJiMin*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual void				Free() override;
};
END
#endif // !__CLIENT_MAZE_YOO_JI_MIN_H__