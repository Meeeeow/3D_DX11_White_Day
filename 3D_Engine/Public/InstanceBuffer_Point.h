#pragma once
#ifndef __ENGINE_INSTANCE_BUFFER_POINT_H__
#define __ENGINE_INSTANCE_BUFFER_POINT_H__

#include "VIBuffer_Instance.h"

class ENGINE_DLL CInstanceBuffer_Point final : public CVIBuffer_Instance
{
private:
	CInstanceBuffer_Point(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, _uint iNumInstance);
	explicit CInstanceBuffer_Point(const CInstanceBuffer_Point& rhs);
	virtual ~CInstanceBuffer_Point() DEFAULT;

public:
	virtual HRESULT						NativeConstruct(const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual HRESULT						Clone_Construct(void* pArg) override;

public:
	void								Update(_double dTimeDelta);

public:
	static	CInstanceBuffer_Point*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, _uint iNumInstance
		, const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual CComponent*					Clone(void* pArg) override;
	virtual void						Free() override;
};


#endif // !__ENGINE_INSTANCE_BUFFER_POINT_H__