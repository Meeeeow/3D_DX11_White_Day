#pragma once
#ifndef __CLIENT_DEFAULT_DOOR_LOCK_H__
#define __CLIENT_DEFAULT_DOOR_LOCK_H__

#include "ModelObject.h"
BEGIN(Client)
class CDefaultDoorLock final : public CModelObject
{
private:
	CDefaultDoorLock(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CDefaultDoorLock() DEFAULT;
public:
	virtual HRESULT				NativeConstruct(OBJ_INFO tObjInfo, _float fMove, _bool bX);
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

public:
	virtual void				Interaction() override;
	virtual void				Event() override;
	virtual void				Collect_Event() override;
	void						Collision();

	void						DoorLeftX();
	void						DoorRightX();
	void						DoorLeftZ();
	void						DoorRightZ();

private:
	_float						m_fRotate = 0.f;
	_float						m_fMove = 0.f;

	_bool						m_bAction = false;
	_bool						m_bReverse = false;

	_bool						m_bX = false;

	_uint						m_iAcc = 0;
	_float						m_fTimeAcc = 0.f;

public:
	static	CDefaultDoorLock*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo, _float fMove, _bool bX);
	virtual	void				Free() override;
	virtual void				Compute_Distance() override;
};
END
#endif // !__CLIENT_DEFAULT_DOOR_LOCK_H__