#pragma once
#ifndef __CLIENT_DEFAULT_DOOR_H__
#define __CLIENT_DEFAULT_DOOR_H__
#include "ModelObject.h"
/* 
	Door Filter Name
	1_ DoorLeftX - Move X Position - 0.66
	2_ DoorRightX - Move X Position + 0.66
	3_ DoorLeftZ - Move Z Position + 0.66
	4_ DoorRightZ - Move Z Position - 0.66
	5_ DoorRotationP - Rotate Y Axis 90 Degree.
	6_ DoorRotationM - Rotate Y Axis -90 Degree.
*/
BEGIN(Client)
class CDefaultDoor final : public CModelObject
{
private:
	CDefaultDoor(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CDefaultDoor() DEFAULT;

public:
	virtual HRESULT				NativeConstruct(OBJ_INFO tObjInfo, _float fMove, _float fRotate, _bool bX);
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

	void						Collision();

public:
	virtual void				Interaction() override;
	virtual void				Event() override;
	virtual void				Collect_Event() override;

	void						DoorLeftX();
	void						DoorRightX();
	void						DoorLeftZ();
	void						DoorRightZ();
	void						DoorRotationPlus();
	void						DoorRotationMinus();

private:
	_float						m_fRotate = 0.f;
	_float						m_fMove = 0.f;

	_bool						m_bAction = false;
	_bool						m_bReverse = false;

	_bool						m_bX = false;

	_uint						m_iAcc = 0;
	_float						m_fTimeAcc = 0.f;

public:
	static	CDefaultDoor*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo, _float fMove, _float fRotate, _bool bX);
	virtual	void				Free() override;
	virtual void				Compute_Distance() override;

};
END
#endif // !__CLIENT_DEFAULT_DOOR_H__