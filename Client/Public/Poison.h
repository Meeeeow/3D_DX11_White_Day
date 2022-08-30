#pragma once
#ifndef __CLIENT_POISON_H__
#define __CLIENT_POISON_H__

#include "ModelObject.h"
#include "HauntedPot.h"
BEGIN(Client)
class CPoison final : public CModelObject
{
private:
	CPoison(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CPoison() DEFAULT;

public:
	virtual HRESULT				NativeConstruct(CHauntedPot* pPot);
	virtual _int				Tick(_double dTimeDelta);
	virtual _int				LateTick(_double dTimeDelta);
	virtual HRESULT				Render() override;

public:
	CHauntedPot*				m_pParent = nullptr;
	_double						m_dTimeAcc = 0.0;
	_bool						m_bDead = false;

public:
	static	CPoison*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, CHauntedPot* pPot);
	virtual	void				Free() override;

};
END
#endif // !__CLIENT_POISON_H__