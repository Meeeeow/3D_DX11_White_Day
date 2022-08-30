#pragma once
#ifndef __CLIENT_HAUNTED_POT_H__
#define __CLIENT_HAUNTED_POT_H__

#include "ModelObject.h"
BEGIN(Client)
class CHauntedPot final : public CModelObject
{
private:
	CHauntedPot(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CHauntedPot() DEFAULT;

public:
	enum JANITOR { JANITOR_BONGGOO, JANITOR_DALSU, JANITOR_END };

public:
	virtual HRESULT				NativeConstruct(OBJ_INFO tObjInfo, JANITOR eTarget);
	virtual _int				Tick(_double dTimeDelta);
	virtual _int				LateTick(_double dTimeDelta);
	virtual HRESULT				Render() override;

public:
	virtual void				Interaction() override;
	virtual void				Event() override;
	virtual void				Collect_Event() override;
	JANITOR						Get_Target() { return m_eTarget; }

public:
	void						Set_Dead() { m_bDead = true; }
	void						Set_Finished() { m_bSetFinished = true; }
	

private:
	JANITOR						m_eTarget = JANITOR_END;
	_bool						m_bAction = false;
	_bool						m_bDead = false;
	_bool						m_bSetFinished = false;
	_double						m_dTimeAcc = 0.0;

public:
	static	CHauntedPot*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo, JANITOR eTarget);
	virtual void				Free() override;
};
END
#endif // !__CLIENT_HAUNTED_POT_H__