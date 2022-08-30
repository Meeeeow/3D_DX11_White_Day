#pragma once
#ifndef __CLIENT_DOOR_GUARD_H__
#define __CLIENT_DOOR_GUARD_H__

#include "ModelObject.h"
BEGIN(Client)
class CDoorGuard final : public CModelObject
{
private:
	CDoorGuard(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CDoorGuard() DEFAULT;

public:
	virtual HRESULT					NativeConstruct(OBJ_INFO tObjInfo);
	virtual _int					Tick(_double dTimeDelta) override;
	virtual _int					LateTick(_double dTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	static	CDoorGuard*				Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo);
	virtual void					Free() override;
};

END
#endif // !__CLIENT_DOOR_GUARD_H__