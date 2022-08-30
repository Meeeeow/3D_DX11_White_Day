#pragma once
#ifndef __CLIENT_DEFAULT_LIGHT_CONTROLLER_H__
#define __CLIENT_DEFAULT_LIGHT_CONTROLLER_H__

#include "ModelObject.h"

BEGIN(Client)
class CDefaultLightController final : public CModelObject
{
private:
	CDefaultLightController(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CDefaultLightController() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct(OBJ_INFO tObjInfo, const wchar_t* pLightTag);
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

public:
	virtual void				Interaction() override;
	virtual void				Event() override;
	virtual void				Collect_Event() override;

	_bool						m_bAction = false;
	_bool						m_bReverse = false;
	const _tchar*				m_pLightTag = L"";
	_float						m_fTimeAcc = 0.f;
	_float						m_fCoolDown = 0.f;

public:
	static	CDefaultLightController*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, OBJ_INFO tObjInfo, const wchar_t* pLightTag);
	virtual	void							Free() override;
	virtual void							Compute_Distance() override;
};
END
#endif // !__CLIENT_DEFAULT_LIGHT_CONTROLLER_H__