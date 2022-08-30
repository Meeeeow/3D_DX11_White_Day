#pragma once
#ifndef __ENGINE_VI_BUFFER_RECT_XY_H__
#define __ENGINE_VI_BUFFER_RECT_XY_H__

#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer_RectXY final : public CVIBuffer
{
private:
	CVIBuffer_RectXY(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CVIBuffer_RectXY(const CVIBuffer_RectXY& rhs);
	virtual ~CVIBuffer_RectXY() DEFAULT;

public:
	virtual	HRESULT					NativeConstruct(const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual HRESULT					Clone_Construct(void* pArg) override;

public:
	static	CVIBuffer_RectXY*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;
};
END
#endif // !__ENGINE_VI_BUFFER_RECT_XY_H__