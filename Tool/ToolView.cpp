
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "MainFrm.h"
#include "ToolView.h"
#include "Form.h"
#include "GameInstance.h"

#include "ToolBase.h"
#include "ToolTerrain.h"
#include "ToolModel.h"
#include "MultiThreadFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

HRESULT CToolView::Add_Objects(CGameObject * pObject)
{
	m_vecObjects.emplace_back(pObject);
	OBJ_INFO tObjInfo = dynamic_cast<CToolBase*>(pObject)->Get_ObjectInfo();

	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*		pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	pForm->Add_String(tObjInfo.szObjName);


	return S_OK;
}

HRESULT CToolView::Add_Navi(CGameObject * pObject)
{
	m_vecNavi.emplace_back(pObject);
	OBJ_INFO tObjInfo = dynamic_cast<CToolBase*>(pObject)->Get_ObjectInfo();

	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*		pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	pForm->Add_StringNav(tObjInfo.szObjName);

	return S_OK;
}

OBJ_INFO CToolView::Get_ObjInfo()
{
	return dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Get_ObjectInfo();
}

_fvector CToolView::Get_Position()
{
	return dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Get_Position();
}

_fvector CToolView::Get_Scale()
{
	return dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Get_Scale();
}

_float CToolView::Get_ObjectCullRadius()
{
	return dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Get_CullRadius();
}

CString CToolView::Get_ObjName()
{
	return CString(dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Get_ObjectInfo().szObjName);
}

CString CToolView::Get_FileName()
{
	return CString(dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Get_ObjectInfo().szFileName);
}

_fvector CToolView::Get_OnePoint()
{
	return dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Get_OnePoint();
}


void CToolView::Set_ObjectIndex(_uint iIndex)
{
	m_iObjectIndex = iIndex;
	if (dynamic_cast<CToolModel*>(m_vecObjects[m_iObjectIndex]) == nullptr
		|| dynamic_cast<CToolModel*>(m_vecObjects[m_iObjectIndex])->Get_HasAnimation() == false)
		return;

	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*		pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	pForm->Set_AnimationList(dynamic_cast<CToolModel*>(m_vecObjects[m_iObjectIndex]));
}

void CToolView::Set_Animation()
{
	if (dynamic_cast<CToolModel*>(m_vecObjects[m_iObjectIndex]) == nullptr)
		return;

	dynamic_cast<CToolModel*>(m_vecObjects[m_iObjectIndex])->Set_Animation(m_iAnimationIndex);
}

void CToolView::Set_ObjectColliderType(OBJ_INFO::COLLIDERTYPE eType)
{
	dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Set_ColliderType(eType);
}

void CToolView::Set_ColliderInfo(OBJ_INFO::COLLIDERTYPE eType, _float3 vCenter, _float3 vExt, _float4 vRot, _float fRadius)
{
	dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Set_Collider(eType, vCenter, vExt, vRot, fRadius);
}

void CToolView::Set_ObjectTransform(_fvector vPos)
{
	dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Set_Position(vPos);
}

void CToolView::Set_ObjectScale(_fvector vScale)
{
	dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Set_Scale(vScale);
}

void CToolView::Set_ObjectRotation(_float fRadian, _fvector vAxis)
{
	dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Set_Rotation(fRadian, vAxis);
}

void CToolView::Set_NavMeshPoint(_float3 vPos, _float fDistance)
{
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*		pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	pForm->Set_NavMeshPoint(vPos, fDistance);
}

void CToolView::Set_ObjectName(CString wstrObjectName)
{
	UpdateData(TRUE);
	dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Set_ObjectName(wstrObjectName);
	UpdateData(FALSE);
}

void CToolView::Set_ObjectCullRadius(_float fRadius)
{
	UpdateData(TRUE);
	dynamic_cast<CToolBase*>(m_vecObjects[m_iObjectIndex])->Set_CullRadius(fRadius);
	UpdateData(FALSE);
}


void CToolView::Delete_SelObject(_uint iIndex)
{
	m_vecObjects.erase(m_vecObjects.begin() + iIndex);
}

void CToolView::Delete_SelNav(_uint iIndex)
{
	m_vecNavi.erase(m_vecNavi.begin() + iIndex);
}

void CToolView::Clear_Object()
{
	for (auto& iter : m_vecObjects)
		Safe_Release(iter);
}

_float CToolView::Get_NavPointX(_uint iCurSel, _uint iIndex)
{
	return dynamic_cast<CToolNavMesh*>(m_vecNavi[iCurSel])->Get_NavPointX(iIndex);
}

_float CToolView::Get_NavPointY(_uint iCurSel, _uint iIndex)
{
	return dynamic_cast<CToolNavMesh*>(m_vecNavi[iCurSel])->Get_NavPointY(iIndex);
}

_float CToolView::Get_NavPointZ(_uint iCurSel, _uint iIndex)
{
	return dynamic_cast<CToolNavMesh*>(m_vecNavi[iCurSel])->Get_NavPointZ(iIndex);
}

void CToolView::Set_TargetOn(_uint iCurSel)
{
	for (_uint i = 0; i < m_vecNavi.size(); ++i)
	{
		if(i != iCurSel)
			dynamic_cast<CToolNavMesh*>(m_vecNavi[i])->TargetOff();
		else
			dynamic_cast<CToolNavMesh*>(m_vecNavi[i])->TargetOn();
	}
}

void CToolView::Load_Object(OBJ_INFO * pObjInfo)
{
	CToolBase* pInstance = nullptr;

	switch (pObjInfo->eBufferType)
	{
	case OBJ_INFO::VIB_MODEL:
		pInstance = CToolModel::Create_ForLoad(m_pGraphicDevice, m_pContextDevice, *pObjInfo);
		break;
	case OBJ_INFO::VIB_TERRAIN:
		pInstance = CToolTerrain::Create(m_pGraphicDevice, m_pContextDevice, *pObjInfo);
		break;
	case OBJ_INFO::VIB_NAV:
		pInstance = CToolNavMesh::Create(m_pGraphicDevice, m_pContextDevice, *pObjInfo);
	default:
		break;
	}

	if (pInstance != nullptr && pObjInfo->eBufferType != OBJ_INFO::VIB_NAV)
	{
		pInstance->Set_ObjectInfo(*pObjInfo);
		pInstance->Update_Load();
		m_vecObjects.emplace_back(pInstance);
	}
	else
	{
		pInstance->Set_ObjectInfo(*pObjInfo);
		pInstance->Update_Load();
		m_vecNavi.emplace_back(pInstance);
	}
}

void CToolView::Load_NaviMesh(OBJ_INFO * pObjInfo)
{

}

_bool CToolView::Compare(CGameObject * pSour, CGameObject * pDest)
{
	if(dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName[0] != dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName[0])
		return dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName[0] < dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName[0];
	else if (dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName[1] != dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName[1])
		return dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName[1] < dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName[1];
	else if (dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName[2] != dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName[2])
		return dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName[2] < dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName[2];
	else if (dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName[3] != dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName[3])
		return dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName[3] < dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName[3];
	else if (dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName[4] != dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName[4])
		return dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName[4] < dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName[4];
	else if (dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName[5] != dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName[5])
		return dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName[5] < dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName[5];
	else
		return lstrlen(dynamic_cast<CToolBase*>(pSour)->Get_ObjectInfo().szObjName) < lstrlen(dynamic_cast<CToolBase*>(pDest)->Get_ObjectInfo().szObjName);
}

void CToolView::Sort_Object()
{
	sort(m_vecObjects.begin(), m_vecObjects.end(), Compare);
}

_int CToolView::Tick(_double dTimeDelta)
{
	for (auto& iter : m_vecObjects)
		iter->Tick(dTimeDelta);
	for (auto& iterr : m_vecNavi)
		iterr->Tick(dTimeDelta);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*		pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	pForm->Tick(dTimeDelta);

	return _int();
}

_int CToolView::LateTick(_double dTimeDelta)
{
	for (auto& iter : m_vecObjects)
		iter->LateTick(dTimeDelta);
	for (auto& iterr : m_vecNavi)
		iterr->LateTick(dTimeDelta);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*		pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	pForm->LateTick(dTimeDelta);

	return _int();
}

void CToolView::Free()
{
	for (auto& vectorObjects : m_vecObjects)
		Safe_Release(vectorObjects);
	m_vecObjects.clear();
	for (auto& NaviObjects : m_vecNavi)
		Safe_Release(NaviObjects);
	m_vecNavi.clear();
	_ulong	RefCnt = 0;
	
	CMultiThreadFunc::Get_Instance()->Destroy_Instance();
	RefCnt = Safe_Release(m_pGameInstance);
	RefCnt = Safe_Release(m_pCamera);
	RefCnt = Safe_Release(m_pRenderer);
	RefCnt = Safe_Release(m_pContextDevice);
	RefCnt = Safe_Release(m_pGraphicDevice);

	CGameInstance::Release_Engine();
}

void CToolView::Render()
{
	m_pGameInstance->Clear_BackBufferView(_float4(0.f, 0.15f, 0.15f, 1.f));
	m_pGameInstance->Clear_DepthStencilView();

	m_pRenderer->Draw_RenderGroup();

	m_pGameInstance->Render_Level();

	m_pGameInstance->Present();
}

void CToolView::Camera_Update()
{
	_float3 vPos;
	m_pCamera->Tick(1 / 60.f);
	XMStoreFloat3(&vPos,m_pCamera->Get_CameraPos());
	LeftButton();
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*		pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	_float	fFOV = m_pCamera->Get_FOV();
	_float	fNear = m_pCamera->Get_Near();
	_float	fFar = m_pCamera->Get_Far();
	pForm->Update_CameraInfo(vPos, fNear, fFar, fFOV);
}

void CToolView::Update()
{
	LeftButton();
}

void CToolView::LeftButton()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (pGameInstance->Key_Down(DIK_P))
	{
		CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CForm* pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
		pForm->Set_Pick();
	}

	

	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*		pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	if (pForm->Get_Pick() == false)
		return;
	if (pGameInstance->Mouse_Down(CInputDevice::DIMB_LB))
	{
		for (auto& pObjects : m_vecObjects)
			pObjects->Picking();
		for (auto& pNavis : m_vecNavi)
			pNavis->Picking();
	}
}

	

CToolView::~CToolView()
{
	Free();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	m_pGameInstance->Tick(1 / 60.f);
	
	Tick(1 / 60.f);
	LateTick(1 / 60.f);

	Render();
	
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*		pForm =	dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	RECT rcMain = {};
	pMain->GetWindowRect(&rcMain);
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	RECT rcView = {};
	GetClientRect(&rcView);

	int	iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, g_iWinCX + iGapX, g_iWinCY + iGapY, SWP_NOMOVE);

	if (m_pGameInstance != nullptr)
		return;

	m_pGameInstance = GET_INSTANCE(CGameInstance);
	CGraphicDevice::DESC tGraphicDesc;
	tGraphicDesc.eWinMode = CGraphicDevice::MODE_WIN;
	tGraphicDesc.iWinCX = g_iWinCX;
	tGraphicDesc.iWinCY = g_iWinCY;
	tGraphicDesc.hWnd = m_hWnd;

	if (FAILED(m_pGameInstance->Initialize(AfxGetInstanceHandle(), tGraphicDesc, pMain->m_hWnd, LEVEL_END, &m_pGraphicDevice, &m_pContextDevice)))
		return;

	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pContextDevice);
	CMultiThreadFunc::Get_Instance()->Init(this, pForm, m_pGraphicDevice, m_pContextDevice);

	pForm->Set_Device(m_pGraphicDevice, m_pContextDevice);

	CToolCamera::DESC tCameraDesc;
	tCameraDesc.fAspect = g_iWinCX / (_float)g_iWinCY;
	tCameraDesc.fFar = 350.f;
	tCameraDesc.fNear = 0.2f;
	tCameraDesc.fFOV = XMConvertToRadians(90.f);

	tCameraDesc.vAt = _float3(0.f, 0.f, -1.f);
	tCameraDesc.vEye = _float3(0.f, 3.f, 0.f);
	tCameraDesc.vAxis = _float3(0.f, 1.f, 0.f);

	tCameraDesc.tTransformDesc.dRotationPerSec = 10.f;
	tCameraDesc.tTransformDesc.dSpeedPerSec = 30.f;

	m_pCamera = CToolCamera::Create(m_pGraphicDevice, m_pContextDevice, tCameraDesc);
	if (m_pCamera == nullptr)
		return;
	

	m_pGameInstance->Add_Timer(TimerDefault);
	m_pGameInstance->Add_Timer(Timer60);

	LIGHTDESC			tLightDesc;

	tLightDesc.eLightType = LIGHTDESC::LIGHT_DIRECTIONAL;
	tLightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 0.4f);
	tLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vDir = _float4(-1.f, -1.f, -1.f, 0.f);

	if (FAILED(m_pGameInstance->Add_Light(m_pGraphicDevice, m_pContextDevice, L"AnyWay", Shader_Viewport, DefaultTechnique,tLightDesc)))
		return;	

	if (FAILED(m_pGameInstance->Light_Action(L"AnyWay")))
		return;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeRenderer, m_pRenderer = CRenderer::Create(m_pGraphicDevice, m_pContextDevice, L"../Client/Bin/ShaderFiles/Shader_Viewport.hlsl", DefaultTechnique))))
		return;
	Safe_AddRef(m_pRenderer);

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeSphere, CCollider::Create(m_pGraphicDevice, m_pContextDevice, CCollider::TYPE_SPHERE))))
		return;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeAABB, CCollider::Create(m_pGraphicDevice, m_pContextDevice, CCollider::TYPE_AABB))))
		return;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PrototypeOBB, CCollider::Create(m_pGraphicDevice, m_pContextDevice, CCollider::TYPE_OBB))))
		return;

	return;
}


void CToolView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pCamera->Action();
	if (m_pCamera->Get_Action())
	{
		SetTimer(0, 100, NULL);
		KillTimer(1);
	}
	else
	{
		SetTimer(1, 100, NULL);
		KillTimer(0);
	}
	CView::OnMButtonDown(nFlags, point);
}


void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 0:
		Camera_Update();
		break;
	case 1:
		Update();
		break;
	default:
		break;
	}
	CView::OnTimer(nIDEvent);
}


int CToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.


	return 0;
}
