#pragma once

#include "ToolBase.h"

class CToolNavMesh final  : public CToolBase
{
private:
	CToolNavMesh(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	virtual ~CToolNavMesh() DEFAULT;

public:
	HRESULT						NativeConstruct(_float3 v0, _float3 v1, _float3 v2);
	HRESULT						NativeConstruct(OBJ_INFO tObjInfo);
	virtual _int				Tick(_double dTimeDelta) override;
	virtual _int				LateTick(_double dTimeDelta) override;
	virtual HRESULT				Render() override;
	virtual void				Picking();
	_bool						Picking(_fvector vOrigin, _fvector vRay, _float3& vPos,_float& fDistance);

	_float						Get_NavPointX(_uint iIndex) { return m_vNavMeshPoints[iIndex].x; }
	_float						Get_NavPointY(_uint iIndex) { return m_vNavMeshPoints[iIndex].y; }
	_float						Get_NavPointZ(_uint iIndex) { return m_vNavMeshPoints[iIndex].z; }

	virtual void				Compute_Distance() override;
	void						TargetOn() { m_bTarget = true; }
	void						TargetOff() { m_bTarget = false; }

public:
	_uint						m_iNumCounts = 0;
	_float3						m_vPoints;

private:
	_float3						m_vNavMeshPoints[3];
	CCollider*					m_pSphereCollider[3];
	CVIBuffer_Triangle*			m_pVIBuffer = nullptr;
	_float						m_fY;
	_bool						m_bTarget = false;

public:
	static CToolNavMesh*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, _float3 v0, _float3 v1, _float3 v2);
	static CToolNavMesh*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, OBJ_INFO tObjInfo);
	virtual void				Free() override;
};

