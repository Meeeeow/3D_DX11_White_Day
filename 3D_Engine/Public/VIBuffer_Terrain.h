#pragma once
#ifndef __ENGINE_VI_BUFFER_TERRAIN_H__
#define __ENGINE_VI_BUFFER_TERRAIN_H__

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{

private:
	CVIBuffer_Terrain(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() DEFAULT;

private:
	_uint						m_iNumVerticesX = 2;
	_uint						m_iNumVerticesZ = 2;

public:
	virtual	HRESULT				NativeConstruct(const _tchar* pHeightMapFilePath, const _tchar* pShaderFilePath, const char* pTechinqueName);
	virtual HRESULT				NativeConstruct(const DESC& tVIBufferDesc);
	virtual HRESULT				NativeConstruct() override;
	virtual HRESULT				Clone_Construct(void* pArg) override;

	virtual _bool				Picking(_float3* pOut, _matrix matWorld,_float& fDist);

public:
	/* If Terrains location is XMMatrixIdentity. */
	_float4						Compute_Height(_fvector vPosition);
	/* If Terrains location is not XMMatrixIdentity. */
	_float3						Compute_Height(_fvector vPosition, _fmatrix matTerrainMatrix);

public:
	static	CVIBuffer_Terrain*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, const _tchar* pHeightMapFilePath, const _tchar* pShaderFilePath, const char* pTechinqueName);
	static	CVIBuffer_Terrain*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, const DESC& tVIBufferDesc);
	static	CVIBuffer_Terrain*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual CComponent*			Clone(void* pArg) override;
	virtual	void				Free() override;
};

END
#endif // !__ENGINE_VI_BUFFER_TERRAIN_H__