#pragma once
#ifndef __CLIENT_BLACK_SCREEN_H__
#define __CLIENT_BLACK_SCREEN_H__

#include "RectXYObject.h"
BEGIN(Client)
class CBlackScrren final : public CRectXYObject
{
private:
	CBlackScrren(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CBlackScrren() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct();
	virtual _int				Tick(_double dTimeDelta);
	virtual _int				LateTick(_double dTimeDelta);
	virtual HRESULT				Render() override;

private:
	_float4x4					m_matProj;
	_float						m_fAlpha = 0.0f;

public:
	static	CBlackScrren*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual void				Free() override;

};
END

#endif // !__CLIENT_BLACK_SCREEN_H__