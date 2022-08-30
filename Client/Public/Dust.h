#pragma once
#ifndef __CLIENT_DUST_H__
#define __CLIENT_DUST_H__

#include "RectXYObject.h"
#include "Valve.h"

BEGIN(Client)
class CDust final : public CRectXYObject
{
private:
	CDust(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CDust() DEFAULT;

public:
	virtual HRESULT			NativeConstruct(CValve* pValve, _uint iLevel, _fvector vPosition);
	virtual _int			Tick(_double dTimeDelta);
	virtual _int			LateTick(_double dTimeDelta);
	virtual HRESULT			Render() override;

	virtual void			Compute_Distance() override;
private:
	CValve*					m_pValve = nullptr;
	_uint					m_iLevel = 0;

public:
	static	CDust*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, CValve* pValve, _uint iLevel,_fvector vPosition);
	virtual void			Free() override;
};
END
#endif // !__CLIENT_DUST_H__