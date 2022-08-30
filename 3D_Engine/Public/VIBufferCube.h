#pragma once
#ifndef __ENGINE_VI_BUFFER_CUBE_H__
#define __ENGINE_VI_BUFFER_CUBE_H__

#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBufferCube final : public CVIBuffer
{
private:
	CVIBufferCube(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CVIBufferCube(const CVIBufferCube& rhs);
	virtual ~CVIBufferCube() DEFAULT;

public:
	virtual	HRESULT					NativeConstruct(const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual HRESULT					Clone_Construct(void* pArg);

public:
	static	CVIBufferCube*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual	CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;

};
END
#endif // !__ENGINE_VI_BUFFER_CUBE_H__