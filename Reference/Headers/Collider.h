#pragma once
#ifndef __ENGINE_COLLIDER_H__
#define __ENGINE_COLLIDER_H__

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)
class ENGINE_DLL CCollider final : public CComponent
{
public:
	typedef DirectX::PrimitiveBatch<DirectX::VertexPositionColor> BATCH;


public:
	enum COLLIDERTYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
	enum COLLIDERSTATE { STATE_PIVOT, STATE_TRANSFORM, STATE_END };

public:
	typedef	struct	tagColliderDesc {
		tagColliderDesc() {
			ZeroMemory(this, sizeof(tagColliderDesc));
			vOrientation = _float4(0.f, 0.f, 0.f, 1.f);
		}
		_float3		vScale;
		_float3		vPivot;
		_float4		vOrientation;
		_float		fRadius;
	}DESC;

private:
	CCollider(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() DEFAULT;

public:
	virtual HRESULT			NativeConstruct(COLLIDERTYPE eType);
	virtual HRESULT			Clone_Construct(void* pArg) override;
	void					Update(_fmatrix matTransformation);
	HRESULT					Render();

public: /* Ray Collision */
	_bool					Collision_Sphere(_fvector vOrigin, _fvector vRay, _float3& vPos,_float& fDistance);
	_bool					Collision_OBB(_fvector vOrigin, _fvector vRay, _float& fDistance);
	_bool					Collision_AABB(_fvector vOrigin, _fvector vRay, _float& fDistance);

public: /* AABB Collision */
	_bool					Collision_AABBtoAABB(BoundingBox* pAABB);
	_bool					Collision_AABBtoOBB(BoundingOrientedBox* pOBB);
	_bool					Collision_AABBtoSphere(BoundingSphere* pSphere);

public: /* OBB Collision */
	_bool					Collision_OBBtoAABB(BoundingBox* pAABB);
	_bool					Collision_OBBtoOBB(BoundingOrientedBox* pOBB);
	_bool					Collision_OBBtoSphere(BoundingSphere* pSphere);

public: /* Sphere Collision */
	_bool					Collision_SpheretoAABB(BoundingBox* pAABB);
	_bool					Collision_SpheretoOBB(BoundingOrientedBox* pOBB);
	_bool					Collision_SpheretoSphere(BoundingSphere* pSphere);

public:
	BoundingOrientedBox*	Get_OBB() { return m_pOBB[STATE_TRANSFORM]; }
	BoundingBox*			Get_AABB() { return m_pAABB[STATE_TRANSFORM]; }
	BoundingSphere*			Get_Sphere() { return m_pSphere[STATE_TRANSFORM]; }

public:
	_fvector				ResultCollision_OBBtoOBB(BoundingOrientedBox* pOBB);

private:
	_matrix					Remove_MatrixsRatation(_fmatrix matWorld);

private:
	COLLIDERTYPE			m_eColliderType = TYPE_END;
	DESC					m_tDesc;
	_bool					m_bIsCollision = false;

	BoundingBox*			m_pAABB[STATE_END] = { nullptr };
	BoundingOrientedBox*	m_pOBB[STATE_END] = { nullptr};
	BoundingSphere*			m_pSphere[STATE_END] = { nullptr };

private:
	BasicEffect*			m_pEffect = nullptr;
	ID3D11InputLayout*		m_pInputLayout = nullptr;
	BATCH*					m_pBatch = nullptr;

public:
	static	CCollider*		Create(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice, COLLIDERTYPE eType);
	virtual CComponent*		Clone(void* pArg) override;
	virtual	void			Free() override;

};

END
#endif // !__ENGINE_COLLIDER_H__