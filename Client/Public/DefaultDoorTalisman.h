#pragma once
#ifndef __CLIENT_DEFAULT_DOOR_TALISMAN_H__
#define __CLIENT_DEFAULT_DOOR_TALISMAN_H__

#include "ModelObject.h"
BEGIN(Client)
class CDefaultDoorTalisman final : public CModelObject
{
private:
	CDefaultDoorTalisman(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CDefaultDoorTalisman() DEFAULT;

public:
	virtual HRESULT				NativeConstruct(OBJ_INFO tObjInfo, _float fRotate);
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

public:
	virtual void				Interaction() override;
	virtual void				Event() override;
	virtual void				Collect_Event() override;
	void						Collision();

	void						DoorRotationPlus();
	void						DoorRotationMinus();

private:
	_float						m_fRotate = 0.f;

	_bool						m_bAction = false;

	_uint						m_iAcc = 0;
	_float						m_fTimeAcc = 0.f;

public:
	static	CDefaultDoorTalisman*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo, _float fRotate);
	virtual	void					Free() override;
	virtual void					Compute_Distance() override;


};
END
#endif // !__CLIENT_DEFAULT_DOOR_TALISMAN_H__