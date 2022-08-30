#include "..\Public\Collider.h"
#include "PipeLine.h"

CCollider::CCollider(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CComponent(pGraphicDevice, pContextDevice)
{
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs), m_eColliderType(rhs.m_eColliderType), m_pBatch(rhs.m_pBatch), m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
{
	Safe_AddRef(m_pInputLayout);
}

HRESULT CCollider::NativeConstruct(COLLIDERTYPE eType)
{
	m_eColliderType = eType;
	if (m_eColliderType == TYPE_END)
		return E_FAIL;

	m_pEffect = new BasicEffect(m_pGraphicDevice);
	if (m_pEffect == nullptr)
		return E_FAIL;

	m_pEffect->SetVertexColorEnabled(true);

	const void*		pShaderByteCode = nullptr;
	size_t			iShaderByteCodeLength = 0;
	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);
	
	if (FAILED(m_pGraphicDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

	m_pBatch = new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(m_pContextDevice);
	if (m_pBatch == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider::Clone_Construct(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_tDesc, pArg, sizeof(DESC));

	switch (m_eColliderType)
	{
	case Engine::CCollider::TYPE_AABB:
		m_pAABB[STATE_PIVOT] = new BoundingBox(m_tDesc.vPivot, m_tDesc.vScale);
		m_pAABB[STATE_TRANSFORM] = new BoundingBox(*m_pAABB[STATE_PIVOT]);
		break;
	case Engine::CCollider::TYPE_OBB:
		m_pOBB[STATE_PIVOT] = new BoundingOrientedBox(m_tDesc.vPivot, m_tDesc.vScale, m_tDesc.vOrientation);
		m_pOBB[STATE_TRANSFORM] = new BoundingOrientedBox(*m_pOBB[STATE_PIVOT]);
		break;
	case Engine::CCollider::TYPE_SPHERE:
		m_pSphere[STATE_PIVOT] = new BoundingSphere(m_tDesc.vPivot, m_tDesc.fRadius);
		m_pSphere[STATE_TRANSFORM] = new BoundingSphere(*m_pSphere[STATE_PIVOT]);
		break;
	case Engine::CCollider::TYPE_END:
		break;
	default:
		break;
	}

	return S_OK;
}

void CCollider::Update(_fmatrix matTransformation)
{
	
	switch (m_eColliderType)
	{
	case Engine::CCollider::TYPE_AABB:
		m_pAABB[STATE_PIVOT]->Transform(*m_pAABB[STATE_TRANSFORM], Remove_MatrixsRatation(matTransformation));
		break;
	case Engine::CCollider::TYPE_OBB:
		m_pOBB[STATE_PIVOT]->Transform(*m_pOBB[STATE_TRANSFORM], matTransformation);
		break;
	case Engine::CCollider::TYPE_SPHERE:
		m_pSphere[STATE_PIVOT]->Transform(*m_pSphere[STATE_TRANSFORM], matTransformation);
		break;
	default:
		break;
	}
	

}

HRESULT CCollider::Render()
{
	CPipeLine*	pPipeLine = CPipeLine::Get_Instance();
	m_pBatch->Begin();

	m_pContextDevice->IASetInputLayout(m_pInputLayout);

	// View, Proj 행렬 세팅
	m_pEffect->SetView(pPipeLine->Get_Transform(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_Transform(CPipeLine::D3DTS_PROJ));
	
	m_pEffect->Apply(m_pContextDevice);

	_vector		vColor = m_bIsCollision == true ? DirectX::Colors::Coral : DirectX::Colors::Azure;


	switch (m_eColliderType)
	{
	case Engine::CCollider::TYPE_AABB:
		DX::Draw(m_pBatch, *m_pAABB[STATE_TRANSFORM], vColor);
		break;
	case Engine::CCollider::TYPE_OBB:
		DX::Draw(m_pBatch, *m_pOBB[STATE_TRANSFORM], vColor);
		break;
	case Engine::CCollider::TYPE_SPHERE:
		DX::Draw(m_pBatch, *m_pSphere[STATE_TRANSFORM], vColor);
		break;
	default:
		break;
	}

	m_pBatch->End();
	return S_OK;
}

_bool CCollider::Collision_Sphere(_fvector vOrigin, _fvector vRay, _float3& vPos, _float & fDistance)
{
	
	if (m_pSphere[STATE_TRANSFORM]->Intersects(vOrigin, vRay, fDistance))
	{
		vPos =  m_pSphere[STATE_TRANSFORM]->Center;
		return true;
	}
	return false;
}

_bool CCollider::Collision_OBB(_fvector vOrigin, _fvector vRay, _float & fDistance)
{
	return m_pOBB[STATE_TRANSFORM]->Intersects(vOrigin, vRay, fDistance);
}

_bool CCollider::Collision_AABB(_fvector vOrigin, _fvector vRay, _float & fDistance)
{
	return m_pAABB[STATE_TRANSFORM]->Intersects(vOrigin, vRay, fDistance);
}

_bool CCollider::Collision_AABBtoAABB(BoundingBox * pAABB)
{
	return m_pAABB[STATE_TRANSFORM]->Intersects(*pAABB);
}

_bool CCollider::Collision_AABBtoOBB(BoundingOrientedBox * pOBB)
{
	return m_pAABB[STATE_TRANSFORM]->Intersects(*pOBB);
}

_bool CCollider::Collision_AABBtoSphere(BoundingSphere * pSphere)
{
	return m_pAABB[STATE_TRANSFORM]->Intersects(*pSphere);
}

_bool CCollider::Collision_OBBtoAABB(BoundingBox * pAABB)
{
	return m_pOBB[STATE_TRANSFORM]->Intersects(*pAABB);
}

_bool CCollider::Collision_OBBtoOBB(BoundingOrientedBox * pOBB)
{
	return m_pOBB[STATE_TRANSFORM]->Intersects(*pOBB);
}

_bool CCollider::Collision_OBBtoSphere(BoundingSphere * pSphere)
{
	return m_pOBB[STATE_TRANSFORM]->Intersects(*pSphere);
}

_bool CCollider::Collision_SpheretoAABB(BoundingBox * pAABB)
{
	return m_pSphere[STATE_TRANSFORM]->Intersects(*pAABB);
}

_bool CCollider::Collision_SpheretoOBB(BoundingOrientedBox * pOBB)
{
	return m_pSphere[STATE_TRANSFORM]->Intersects(*pOBB);
}

_bool CCollider::Collision_SpheretoSphere(BoundingSphere * pSphere)
{
	return m_pSphere[STATE_TRANSFORM]->Intersects(*pSphere);
}

_fvector CCollider::ResultCollision_OBBtoOBB(BoundingOrientedBox * pOBB)
{
	/*_vector vMyCenter = XMLoadFloat3(&m_pOBB[STATE_TRANSFORM]->Center);
	_vector vTargetCenter = XMLoadFloat3(&pOBB->Center);

	_vector vResultCenter = vTargetCenter - vMyCenter;
	_vector vReturnCenter = XMVectorSet(0.f,0.f,0.f,0.f);

	if (fabs(XMVectorGetX(vResultCenter) > fabs(XMVectorGetZ(vResultCenter))))
		vReturnCenter = XMVectorSetX(vReturnCenter, XMVectorGetX(vResultCenter));
	else
		vReturnCenter = XMVectorSetZ(vReturnCenter, XMVectorGetZ(vResultCenter));
	return vReturnCenter;*/
	_float3 vCorners[8];
	_vector vCenter = XMLoadFloat3(&pOBB->Center);
	pOBB->GetCorners(vCorners);
	_float3 vMyCorners[8];
	_vector vMyCenter = XMLoadFloat3(&m_pOBB[STATE_TRANSFORM]->Center);
	m_pOBB[STATE_TRANSFORM]->GetCorners(vMyCorners);

	_vector vCorner[8];
	_vector vMyCorner[8];
	for (_uint i = 0; i < 8; ++i)
	{
		vCorner[i] = XMLoadFloat3(&vCorners[i]);
		vMyCorner[i] = XMLoadFloat3(&vMyCorners[i]);
	}
	_vector fVector1 = vMyCenter - vCorner[0];
	_vector fVector2 = vMyCenter - vCorner[1];
	_vector fVector3 = vMyCenter - vCorner[4];
	_vector fVector4 = vMyCenter - vCorner[5];

	fVector1 = XMVectorSetY(fVector1 ,0.f);
	fVector2 = XMVectorSetY(fVector2 ,0.f);
	fVector3 = XMVectorSetY(fVector3 ,0.f);
	fVector4 = XMVectorSetY(fVector4 ,0.f);

	_float fDistance1 = XMVectorGetX(XMVector3Length(fVector1));
	_float fDistance2 = XMVectorGetX(XMVector3Length(fVector2));
	_float fDistance3 = XMVectorGetX(XMVector3Length(fVector3));
	_float fDistance4 = XMVectorGetX(XMVector3Length(fVector4));

	_vector fMyVector1 = vCenter - vMyCorner[0];
	_vector fMyVector2 = vCenter - vMyCorner[1];
	_vector fMyVector3 = vCenter - vMyCorner[4];
	_vector fMyVector4 = vCenter - vMyCorner[5];

	fMyVector1 = XMVectorSetY(fMyVector1, 0.f);
	fMyVector2 = XMVectorSetY(fMyVector2, 0.f);
	fMyVector3 = XMVectorSetY(fMyVector3, 0.f);
	fMyVector4 = XMVectorSetY(fMyVector4, 0.f);

	_float fMyDistance1 = XMVectorGetX(XMVector3Length(fMyVector1));
	_float fMyDistance2 = XMVectorGetX(XMVector3Length(fMyVector2));
	_float fMyDistance3 = XMVectorGetX(XMVector3Length(fMyVector3));
	_float fMyDistance4 = XMVectorGetX(XMVector3Length(fMyVector4));

	_float fMin = min(fDistance1, fDistance2);
	fMin = min(fMin, fDistance3);
	fMin = min(fMin, fDistance4);

	_float fMyMin = min(fMyDistance1, fMyDistance2);
	fMyMin = min(fMyMin, fMyDistance3);
	fMyMin = min(fMyMin, fMyDistance4);

	if (fMin == fDistance1)
	{
		if (fMyMin == fMyDistance1)
			return fMyVector1 - fVector1;
		else if (fMyMin == fMyDistance2)
			return fMyVector2 - fVector1;
		else if (fMyMin == fMyDistance3)
			return fMyVector3 - fVector1;
		else
			return fMyVector4 - fVector1;
	}
	else if (fMin == fDistance2)
	{
		if (fMyMin == fMyDistance1)
			return fMyVector1 - fVector2;
		else if (fMyMin == fMyDistance2)
			return fMyVector2 - fVector2;
		else if (fMyMin == fMyDistance3)
			return fMyVector3 - fVector2;
		else
			return fMyVector4 - fVector2;
	}
	else if (fMin == fDistance3)
	{
		if (fMyMin == fMyDistance1)
			return fMyVector1 - fVector3;
		else if (fMyMin == fMyDistance2)
			return fMyVector2 - fVector3;
		else if (fMyMin == fMyDistance3)
			return fMyVector3 - fVector3;
		else
			return fMyVector4 - fVector3;
	}
	else
	{
		if (fMyMin == fMyDistance1)
			return fMyVector1 - fVector4;
		else if (fMyMin == fMyDistance2)
			return fMyVector2 - fVector4;
		else if (fMyMin == fMyDistance3)
			return fMyVector3 - fVector4;
		else
			return fMyVector4 - fVector4;
	}

	return XMVectorSet(0.f, 0.f, 0.f, 0.f);
}

_matrix CCollider::Remove_MatrixsRatation(_fmatrix matWorld)
{
	// fmatrix가 const이므로 새로 받아서 변환 후 반환 해줘야 한다.
	
	_matrix			matTransformation;
	matTransformation = matWorld;

	// XMMATRIX는 .r 을 통해 행 접근이 가능하다.
	matTransformation.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(matWorld.r[0]));
	matTransformation.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetX(XMVector3Length(matWorld.r[1]));
	matTransformation.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetX(XMVector3Length(matWorld.r[2]));

	return matTransformation;
}

CCollider * CCollider::Create(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice, COLLIDERTYPE eType)
{
	CCollider* pInstance = new CCollider(pGraphicDevice, pContextDevice);
	if (FAILED(pInstance->NativeConstruct(eType)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CCollider::Clone(void * pArg)
{
	CCollider* pInstance = new CCollider(*this);
	if (FAILED(pInstance->Clone_Construct(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCollider::Free()
{
	Safe_Release(m_pInputLayout);
	if (m_bCloned == false)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}
	for (_uint i = 0; i < STATE_END; ++i)
	{
		Safe_Delete(m_pAABB[i]);
		Safe_Delete(m_pOBB[i]);
		Safe_Delete(m_pSphere[i]);
	}

	__super::Free();
}
