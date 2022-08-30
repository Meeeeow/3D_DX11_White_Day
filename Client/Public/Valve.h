#pragma once
#ifndef __CLIENT_VALVE_H__
#define __CLIENT_VALVE_H__

#include "ModelObject.h"
BEGIN(Client)
class CValve final : public CModelObject
{
	// Rotation Axis Z
private:
	CValve(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CValve() DEFAULT;

public:
	enum VALVEPOS { VALVE0, VALVE1, VALVE_END };

public:
	virtual	HRESULT				NativeConstruct(OBJ_INFO tObjInfo, VALVEPOS eValveType);
	virtual _int				Tick(_double dTimeDelta);
	virtual _int				LateTick(_double dTimeDelta);
	virtual HRESULT				Render();

	virtual void				Interaction() override;
	virtual void				Event() override;
	virtual void				Collect_Event() override;

	void						On_Gas();

	_bool						Get_Clear() { return m_bClear; }

private:
	VALVEPOS					m_eValveType = VALVE_END;
	_float						m_fRotate = 0.f;
	_bool						m_bAction = false;
	_bool						m_bClear = false;

	_long						m_dwMouseX = 0.0, m_dwMouseY = 0.0;

public:
	static	CValve*				Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo, VALVEPOS eValveType);
	virtual void				Free() override;

};
END
#endif // !__CLIENT_VALVE_H__