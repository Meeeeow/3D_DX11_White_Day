#pragma once
#ifndef __CLIENT_LIGHTER_H__
#define __CLIENT_LIGHTER_H__

#include "ModelObject.h"
BEGIN(Client)
class CLighter final : public CModelObject
{
private:
	CLighter(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CLighter() DEFAULT;

public:
	virtual HRESULT				NativeConstruct() override;
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;

public:
	void						Light(CGameInstance* pGameInstance, _double dTimeDelta);
	void						NonLight(CGameInstance* pGameInstance, _double dTimeDelta);

public:
	static	CLighter*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual void				Free() override;

};
END
#endif // !__CLIENT_LIGHTER_H__