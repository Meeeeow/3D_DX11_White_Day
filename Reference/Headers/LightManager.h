#pragma once
#ifndef __ENGINE_LIGHT_MANAGER_H__
#define __ENGINE_LIGHT_MANAGER_H__

#include "Light.h"

BEGIN(Engine)
class CLightManager final : public CBase
{
	DECLARE_SINGLETON(CLightManager)
public:
	typedef list<CLight*>											LIGHTS;
	typedef unordered_map<const _tchar*, list<CLight*>>				LIGHTMAP;
private:
	explicit CLightManager();
	virtual ~CLightManager() DEFAULT;

public:
	HRESULT				Add_Light(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice,const _tchar* pLightTag ,const LIGHTDESC* tLightDesc);
	HRESULT				Add_Light(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, const _tchar* pLightTag
		, const _tchar* pShaderFilePath , const char* pTechniqueName , const LIGHTDESC& tLightDesc);
	HRESULT				Add_Light(const _tchar* pLightTag, CLight* pInstance);
	HRESULT				Render();

	list<CLight*>*		Find_LightMap(const _tchar* pLightTag);
	HRESULT				LightMap_Action(const _tchar* pLightTag);
	HRESULT				LightMap_DeActive(const _tchar* pLightTag);
	HRESULT				DeActive_All();

	LIGHTDESC&			Get_LightDesc(const _tchar* pLightTag, _uint iLightIndex);

private:
	LIGHTMAP			m_mapLight;

public:
	virtual	void		Free() override;
};
END
#endif // !__ENGINE_LIGHT_MANAGER_H__
