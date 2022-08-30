#pragma once
#ifndef __ENGINE_VI_BUFFER_INSTANCE_H__
#define __ENGINE_VI_BUFFER_INSTANCE_H__

#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Instance abstract : public CVIBuffer
{
protected:
	CVIBuffer_Instance(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, _uint iNumInstance);
	explicit CVIBuffer_Instance(const CVIBuffer_Instance& rhs);
	virtual ~CVIBuffer_Instance() DEFAULT;

public:
	virtual	HRESULT					NativeConstruct() override;
	virtual HRESULT					Clone_Construct(void* pArg) override;
	virtual HRESULT					Render(_uint iIndex = 0);

protected:
	virtual HRESULT					Create_InstanceBuffer();

protected:
	_uint							m_iNumInstance = 0;
	_uint							m_iInstanceStride = 0;
	_uint							m_iIndexPerInstance = 0;

	ID3D11Buffer*					m_pInstanceBuffer = nullptr;
	D3D11_BUFFER_DESC				m_tInstanceBufferDesc;
	D3D11_SUBRESOURCE_DATA			m_tInstanceBufferSRD;

	void*							m_pInstanceVertices = nullptr;

public:
	virtual CComponent*				Clone(void* pArg) PURE;
	virtual void					Free() override;

};
END
#endif // !__ENGINE_VI_BUFFER_INSTANCE_H__