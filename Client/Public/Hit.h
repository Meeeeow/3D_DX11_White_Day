#pragma once
#ifndef __CLIENT_HIT_H__
#define __CLIENT_HIT_H__

#include "RectXYObject.h"

class CHit final : public CRectXYObject
{
private:
	CHit(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CHit() DEFAULT;

public:
	virtual HRESULT			NativeConstruct();
	virtual _int			Tick(_double dTimeDelta);
	virtual _int			LateTick(_double dTimeDelta);
	virtual HRESULT			Render() override;

private:
	_float					m_fAlpha = 1.0f;
	_float4x4				m_matProj;
	_uint					m_iRenderIndex = 0;

public:
	static	CHit*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual void			Free() override;

};


#endif // !__CLIENT_HIT_H__