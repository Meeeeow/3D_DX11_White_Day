#pragma once
#ifndef __CLIENT_DEAD_HAUNTED_POT_H__
#define __CLIENT_DEAD_HAUNTED_POT_H__
#include "ModelObject.h"
BEGIN(Client)
class CDeadHauntedPot final : public CModelObject
{
private:
	CDeadHauntedPot(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CDeadHauntedPot() DEFAULT;

	//Pot_Dead.fbx
public:
	virtual HRESULT						NativeConstruct(_fvector vPosition);
	virtual _int						Tick(_double dTimeDelta);
	virtual _int						LateTick(_double dTimeDelta);
	virtual HRESULT						Render() override;

public:
	static	CDeadHauntedPot*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, _fvector vPosition);
	virtual void						Free() override;
};
END
#endif // !__CLIENT_DEAD_HAUNTED_POT_H__