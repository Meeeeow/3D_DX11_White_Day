#pragma once
#ifndef __ENGINE_VI_BUFFER_TRIANGLE_H__
#define __ENGINE_VI_BUFFER_TRIANGLE_H__

#include "VIBuffer.h"

class ENGINE_DLL CVIBuffer_Triangle final : public CVIBuffer
{
private:
	CVIBuffer_Triangle(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CVIBuffer_Triangle(const CVIBuffer_Triangle& rhs);
	virtual ~CVIBuffer_Triangle() DEFAULT;

public:
	virtual	HRESULT					NativeConstruct(const _tchar* pShaderFilePath, const char* pTechniqueName, _float3* pPoints);
	virtual HRESULT					Clone_Construct(void* pArg) override;

public:
	static	CVIBuffer_Triangle*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, const _tchar* pShaderFilePath, const char* pTechinqueName, _float3* pPoints);
	virtual	CComponent*				Clone(void* pArg) override;
	virtual	void					Free() override;
};


#endif // !__ENGINE_VI_BUFFER_TRIANGLE_H__
