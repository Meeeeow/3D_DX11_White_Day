#pragma once
#ifndef __ENGINE_INSTANCE_BUFFER_RECT_H__
#define __ENGINE_INSTANCE_BUFFER_RECT_H__

#include "VIBuffer_Instance.h"
BEGIN(Engine)
class ENGINE_DLL CInstanceBuffer_Rect final : public CVIBuffer_Instance
{
private:
	CInstanceBuffer_Rect(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, _uint iNumInstance);
	explicit CInstanceBuffer_Rect(const CInstanceBuffer_Rect& rhs);
	virtual ~CInstanceBuffer_Rect() DEFAULT;

public:
	virtual	HRESULT						NativeConstruct(const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual HRESULT						Clone_Construct(void* pArg) override;

public:
	void								Update(_double dTimeDelta);

public:
	static	CInstanceBuffer_Rect*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, _uint iNumInstance
		, const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual	CComponent*					Clone(void* pArg) override;
	virtual void						Free() override;

};
END
#endif // !__ENGINE_INSTANCE_BUFFER_RECT_H__