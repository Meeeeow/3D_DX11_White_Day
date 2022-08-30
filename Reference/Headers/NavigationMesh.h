#pragma once
#ifndef __ENGINE_NAVIGATION_MESH_H__
#define __ENGINE_NAVIGATION_MESH_H__
#include "Component.h"
#include "Cell.h"

BEGIN(Engine)
class ENGINE_DLL CNavigationMesh final : public CComponent
{
public:
	typedef vector<CCell*>		CELLS;
	typedef struct tagNavMesh {
		_float3	vPoints[3];
	}NAV_INFO;
private:
	CNavigationMesh(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CNavigationMesh(const CNavigationMesh& rhs);
	virtual ~CNavigationMesh() DEFAULT;

public:
	virtual	HRESULT				NativeConstruct(const _tchar* pNavDataFilePath);
	virtual HRESULT				Clone_Construct(void* pArg);
	HRESULT						Update(_fmatrix matWorld);

public:
	HRESULT						Set_Neighbor();
	_bool						IsOn(_fvector& vMovePos, CCell** pCell, _uint& iCurrentIndex);
	_bool						IsOn_Vent(_fvector& vMovePos, CCell** pCell, _uint& iCurrentIndex);
	void						In_Vent(_fvector vPosition);
	void						In_Cell(_fvector vPosition);

public:
	vector<CCell*>				Get_Cells() { return m_vecCells; }

public:
	void						Set_CurrentIndex(_uint iCurrentIndex) { m_iCurrentIndex = iCurrentIndex; }

public:

#ifdef _DEBUG
public:
	HRESULT						Render();
#endif // _DEBUG

private:
	_uint						m_iCurrentIndex;
	_float4x4					m_matWorld;
	CELLS						m_vecCells;

public:
	static	CNavigationMesh*	Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice
		, const _tchar* pNavDataFilePath);
	virtual CComponent*			Clone(void* pArg);
	virtual void				Free() override;

};
END
#endif // !__ENGINE_NAVIGATION_MESH_H__
