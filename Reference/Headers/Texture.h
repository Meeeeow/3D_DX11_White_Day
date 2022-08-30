#pragma once
#ifndef __ENGINE_TEXTURE_H__
#define __ENGINE_TEXTURE_H__

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CTexture final : public CComponent
{
public:
	typedef vector<ID3D11ShaderResourceView*>	TEXTURES;
	typedef ID3D11ShaderResourceView			SRV;
	typedef ID3D11Resource						RESOURCE;
protected:
	CTexture(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture() DEFAULT;

public:
	virtual	HRESULT		NativeConstruct(const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT		Clone_Construct(void* pArg) override;

public:
	SRV*				Get_SRV(_uint iIndex = 0){
		return m_vecTextures[iIndex];
	}

private:
	TEXTURES			m_vecTextures;

public:
	static CTexture*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, const _tchar* pTextureFilePath, _uint iNumTexture = 1);
	virtual CComponent*	Clone(void* pArg) override;
	virtual void		Free() override;
};

END
#endif // !__ENGINE_TEXTURE_H__