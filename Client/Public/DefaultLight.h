#pragma once
#ifndef __CLIENT_DEFAULT_LIGHT_H__
#define __CLIENT_DEFAULT_LIGHT_H__

#include "ModelObject.h"
BEGIN(Client)
class CDefaultLight final : public CModelObject
{
private:
	CDefaultLight(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CDefaultLight() DEFAULT;

public:
	virtual	HRESULT					NativeConstruct(OBJ_INFO tObjInfo, const _tchar* pLightTag);
	virtual _int					Tick(_double dTimeDelta);
	virtual _int					LateTick(_double dTimeDelta);
	virtual HRESULT					Render();

public:
	virtual void					Interaction() override;
	virtual void					Event() override;
	virtual void					Collect_Event() override;

	void							Add_Light();

private:
	_tchar							m_pLightTag[MAX_PATH];
	LIGHTDESC						m_tLightDesc;

public:
	static	CDefaultLight*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo, const _tchar* pLightTag);
	virtual	void					Free() override;
	virtual void					Compute_Distance() override;
};
END
#endif // !__CLIENT_DEFAULT_LIGHT_H__