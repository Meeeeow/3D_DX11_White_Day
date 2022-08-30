#pragma once
#ifndef __CLIENT_TALISMAN_CASE_H__
#define __CLIENT_TALISMAN_CASE_H__

#include "ModelObject.h"
BEGIN(Client)
class CTalismanCase final : public CModelObject
{
private:
	CTalismanCase(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CTalismanCase() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct(OBJ_INFO tObjInfo);
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

public:
	virtual void				Interaction() override;
	virtual void				Event() override;
	virtual void				Collect_Event() override;

public:
	static	CTalismanCase*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo);
	virtual void				Free() override;

};
END
#endif // !__CLIENT_TALISMAN_CASE_H__