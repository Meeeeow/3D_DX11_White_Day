#pragma once
#ifndef __CLIENT_BONGGOO_POT_H__
#define __CLIENT_BONGGOO_POT_H__

#include "ModelObject.h"
BEGIN(Client)
class CBongGooPot final : public CModelObject
{
private:
	CBongGooPot(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CBongGooPot() DEFAULT;

public:
	virtual HRESULT				NativeConstruct(OBJ_INFO tObjInfo);
	virtual _int				Tick(_double dTimeDelta);
	virtual _int				LateTick(_double dTimeDelta);


public:
	static	CBongGooPot*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo);
	virtual void				Free() override;
};
END
#endif // !__CLIENT_BONGGOO_POT_H__