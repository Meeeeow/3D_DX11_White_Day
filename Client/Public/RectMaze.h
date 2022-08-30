#pragma once
#ifndef __CLIENT_RECT_MAZE_H__
#define __CLIENT_RECT_MAZE_H__

#include "RectXYObject.h"
BEGIN(Client)
class CRectMaze final : public CRectXYObject
{
private:
	CRectMaze(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CRectMaze() DEFAULT;

public:
	virtual HRESULT				NativeConstruct() override;
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	_float4x4					m_matProj;

public:
	static	CRectMaze*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual void				Free();

};
END
#endif // !__CLIENT_RECT_MAZE_H__