#include "stdafx.h"
#include "ToolBase.h"


CToolBase::CToolBase(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
	: CGameObject(pGraphicDevice, pContextDevice)
{

}

HRESULT CToolBase::NativeConstruct()
{
	m_pTransform = CTransform::Create(m_pGraphicDevice, m_pContextDevice);
	if (m_pTransform == nullptr)
		FAILMSG("Failed to Create Transform");

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	m_pRenderer = dynamic_cast<CRenderer*>(pGameInstance->Clone_Component(LEVEL_STATIC, PrototypeRenderer));

	if (m_pRenderer == nullptr)
		FAILMSG("Failed to Create Renderer");


	return S_OK;
}

_int CToolBase::Tick(_double dTimeDelta)
{
	if(m_pCollider != nullptr)
		m_pCollider->Update(m_pTransform->Get_WorldXM());

	return _int();
}

_int CToolBase::LateTick(_double dTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
	return 0;
}

HRESULT CToolBase::Render()
{
	
	return S_OK;
}

HRESULT CToolBase::Set_ConstantTable()
{

	return S_OK;
}

void CToolBase::Update_Info()
{
	XMStoreFloat4x4(&m_tObjInfo.matWorld, m_pTransform->Get_WorldXM());
}

void CToolBase::Update_Load()
{
	m_pTransform->Set_State(CTransform::STATE_RIGHT   , m_tObjInfo.Get_State(CTransform::STATE_RIGHT));
	m_pTransform->Set_State(CTransform::STATE_UP, m_tObjInfo.Get_State(CTransform::STATE_UP));
	m_pTransform->Set_State(CTransform::STATE_LOOK, m_tObjInfo.Get_State(CTransform::STATE_LOOK));
	m_pTransform->Set_State(CTransform::STATE_POSITION, m_tObjInfo.Get_State(CTransform::STATE_POSITION));
}

_fvector CToolBase::Get_OnePoint()
{
	return _fvector();
}

void CToolBase::Set_ColliderType(OBJ_INFO::COLLIDERTYPE eType)
{
	m_tObjInfo.eColliderType = eType;
}

void CToolBase::Set_Collider(OBJ_INFO::COLLIDERTYPE eType,_float3 vCenter, _float3 vExt, _float4 vRot, _float fRadius)
{
	if (m_pCollider != nullptr)
		Safe_Release(m_pCollider);

	m_tObjInfo.eColliderType = eType;

	if (m_tObjInfo.eColliderType == OBJ_INFO::COLLIDER_END)
		return;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CCollider::DESC tDesc;
	ZeroMemory(&tDesc, sizeof(CCollider::DESC));
	
	tDesc.vPivot = vCenter;
	tDesc.vScale = vExt;
	tDesc.vOrientation = vRot;
	tDesc.fRadius = fRadius;
	m_tObjInfo.tColliderDesc = tDesc;
	if(m_tObjInfo.eColliderType == OBJ_INFO::COLLIDER_OBB)
		m_pCollider = dynamic_cast<CCollider*>(pGameInstance->Clone_Component(LEVEL_STATIC, PrototypeOBB, &tDesc));
	else if(m_tObjInfo.eColliderType == OBJ_INFO::COLLIDER_SPHERE)
		m_pCollider = dynamic_cast<CCollider*>(pGameInstance->Clone_Component(LEVEL_STATIC, PrototypeSphere, &tDesc));

}

void CToolBase::Set_ObjectName(CString ObjectName)
{
	wcscpy_s(m_tObjInfo.szObjName, ObjectName);
}

void CToolBase::Picking()
{

}

void CToolBase::Free()
{
	__super::Free();
	if(m_pCollider != nullptr)
		Safe_Release(m_pCollider);

	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
}

void CToolBase::Compute_Distance()
{
	
}

CGameObject * CToolBase::Clone(void * pArg)
{
	// 사용안함
	return nullptr;
}
