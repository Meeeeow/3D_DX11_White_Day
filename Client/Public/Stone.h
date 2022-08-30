#pragma once
#ifndef __CLIENT_STONE_H__
#define __CLIENT_STONE_H__

#include "ModelObject.h"
BEGIN(Client)
class CStone final :public CModelObject
{
private:
	CStone(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CStone() DEFAULT;

public:
	virtual	HRESULT			NativeConstruct(OBJ_INFO tObjInfo);
	virtual _int			Tick(_double dTimeDelta) override;
	virtual _int			LateTick(_double dTimeDelta) override;
	virtual HRESULT			Render() override;


public:
	static	CStone*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo);
	virtual void			Free();

};
END
#endif // !__CLIENT_STONE_H__