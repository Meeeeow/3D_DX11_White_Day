#pragma once

#include "Base.h"
#include "VIBuffer_Triangle.h"

/* 네비게이션을 구성하는 삼각형 하나 */

BEGIN(Engine)
class CCell final : public CBase
{
public:
	enum POINTS { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
private:
	CCell(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CCell() DEFAULT;

public:
	HRESULT					NativeConstruct(_float3* pPoints, _uint iIndex);
	HRESULT					Transform(_fmatrix matWorld);

public:
	_uint					Get_Index() { return m_iIndex; }
	_vector					Get_PointInLocal(POINTS ePoint) { return XMLoadFloat3(&m_vPoints[ePoint]); }
	_fvector				Get_CenterPos() { return XMLoadFloat4(&m_vCenter); }
	_uint					Get_ParentIndex() { return m_iParentIndex; }
	_uint					Get_NeighborSize() { return m_iNeighborCount; }
	CCell*					Get_NeighborCell(_uint iIndex) { return m_pNeighbor[iIndex]; }


public:
	void					Set_Neighbor(LINE eNeighbor, CCell* pNeighbor) {
		m_pNeighbor[eNeighbor] = pNeighbor;	++m_iNeighborCount; }
	void					Set_ParentIndex(_uint iParentIndex) { m_iParentIndex = iParentIndex; }

public:
	_bool					Compare(_fvector vSourPoint, _fvector vDestPoint);
	_bool					IsIn(_fvector& vPosition, CCell** ppNeighborOut, CCell** ppSlide);
	_bool					IsIn_Vent(_fvector& vPosition, CCell** ppNeighborOut, CCell** ppSlide);
	_vector					IsSlide(_fvector& vPosition);

	_bool					Find_CurrentIndexCell(_fvector vPosition);
	_bool					Find_VentIndexCell(_fvector vPosition);
	
#ifdef _DEBUG
public:
	HRESULT					Render(_fmatrix matWorld);
#endif // _DEBUG

private:
	HRESULT					Ready_Debug_Buffer();
	CVIBuffer_Triangle*		m_pVIBuffer = nullptr;

private:
	_pGraphicDevice*		m_pGraphicDevice = nullptr;
	_pContextDevice*		m_pContextDevice = nullptr;

private:
	_float3					m_vPoints[POINT_END];
	_float3					m_vPointsInWorld[POINT_END];
	_float3					m_vLine[LINE_END];
	_float3					m_vCenterPos[LINE_END];
	CCell*					m_pNeighbor[LINE_END];
	_uint					m_iIndex = 0;
	_uint					m_iParentIndex = 0;
	_float4					m_vCenter;
	_uint					m_iNeighborCount = 0;


public:
	static	CCell*			Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, _float3* pPoints, _uint iIndex);
	virtual	void			Free() override;
};
END