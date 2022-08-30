// Form.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"

#include "Form.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MultiThreadFunc.h"


// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)

	, m_strFileName(_T(""))
	, m_fCenterX(0)
	, m_fCenterY(0)
	, m_fCenterZ(0)
	, m_fExtX(0)
	, m_fExtY(0)
	, m_fExtZ(0)
	, m_fColRotX(0)
	, m_fColRotY(0)
	, m_fColRotZ(0)
	, m_fColliderRadius(0)
	, m_fGetOnePointX(0)
	, m_fGetOnePointY(0)
	, m_fGetOnePointZ(0)
	, m_fNav1X(0)
	, m_fNav1Y(0)
	, m_fNav1Z(0)
	, m_fNav2X(0)
	, m_fNav2Y(0)
	, m_fNav2Z(0)
	, m_fNav3X(0)
	, m_fNav3Y(0)
	, m_fNav3Z(0)
{

}

CForm::~CForm()
{
	Free();
}

void CForm::Free()
{
	Safe_Release(m_pGraphicDevcie);
	Safe_Release(m_pContextDevice);
	
	Safe_Delete(m_pDlg1);
	Safe_Delete(m_pDlg2);
	Safe_Delete(m_pDlg3);
	Safe_Delete(m_pDlg4);
	CGameInstance::Get_Instance()->Release_Engine();
}

void CForm::Update_CameraInfo(_float3 vPos, _float fNear, _float fFar, _float fFOV)
{
	UpdateData(TRUE);
	m_fCamPosX = vPos.x;
	m_fCamPosY = vPos.y;
	m_fCamPosZ = vPos.z;
	
	m_fNear = fNear;
	m_fFar = fFar;
	m_fFOV = XMConvertToDegrees(fFOV);
	UpdateData(FALSE);
}

OBJ_INFO CForm::Change_NavToObject(OBJ_INFO tObjInfo, NAV_INFO tNavInfo)
{
	tObjInfo.eBufferType = OBJ_INFO::VIB_NAV;
	wcscpy_s(tObjInfo.szShaderFilePath, Shader_Navigation);
	strcpy_s(tObjInfo.szTehcniqueName, DefaultTechnique);
	wcscpy_s(tObjInfo.szObjName, NaviMeshObject);

	tObjInfo.vNaviPoints[0] = tNavInfo.vPoints[0];
	tObjInfo.vNaviPoints[1] = tNavInfo.vPoints[1];
	tObjInfo.vNaviPoints[2] = tNavInfo.vPoints[2];

	return tObjInfo;
}


void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, EDIT_CAMPOS_X, m_fCamPosX);
	DDX_Text(pDX, EDIT_CAMPOS_Y, m_fCamPosY);
	DDX_Text(pDX, EDIT_CAMPOS_Z, m_fCamPosZ);
	DDX_Text(pDX, EDIT_NEAR, m_fNear);
	DDX_Text(pDX, EDIT_FAR, m_fFar);
	DDX_Text(pDX, EDIT_FOV, m_fFOV);
	DDX_Control(pDX, TAB_Form_Main, m_TabCtrl);
	DDX_Control(pDX, LIST_PROTOTYPE, m_ListObjects);
	DDX_Control(pDX, List_Form_Animations, m_ListAnimations);
	DDX_Text(pDX, Edit_Poisiton_X, m_fPositionX);
	DDX_Text(pDX, Edit_Position_Y, m_fPositionY);
	DDX_Text(pDX, Edit_Position_Z, m_fPositionZ);
	DDX_Text(pDX, Edit_Scale_X, m_fScaleX);
	DDX_Text(pDX, Edit_Scale_Y, m_fScaleY);
	DDX_Text(pDX, Edit_Scale_Z, m_fScaleZ);
	DDX_Text(pDX, Edit_Rotation_X, m_fRotX);
	DDX_Text(pDX, Edit_Rotation_Y, m_fRotY);
	DDX_Text(pDX, Edit_Rotation_Z, m_fRotZ);
	DDX_Text(pDX, Edit_SceneName, m_strScene);
	DDX_Text(pDX, Edit_ObjectName, m_strObjectName);
	DDX_Text(pDX, Edit_CullingRadius, m_fCullRadius);
	DDX_Text(pDX, Eidt_FileName, m_strFileName);
	DDX_Text(pDX, Edit_Collider_CenterX, m_fCenterX);
	DDX_Text(pDX, Edit_Collider_CenterY, m_fCenterY);
	DDX_Text(pDX, Edit_Collider_CenterZ, m_fCenterZ);
	DDX_Text(pDX, Edit_Collider_ExtX, m_fExtX);
	DDX_Text(pDX, Edit_Collider_ExtY, m_fExtY);
	DDX_Text(pDX, Edit_Collider_ExtZ, m_fExtZ);
	DDX_Text(pDX, Edit_Collider_RotX, m_fColRotX);
	DDX_Text(pDX, Edit_Collider_RotY, m_fColRotY);
	DDX_Text(pDX, Edit_Collider_RotZ, m_fColRotZ);

	DDX_Control(pDX, IDC_LIST1, m_ListNav);
	DDX_Text(pDX, Edit_Collider_Radius, m_fColliderRadius);
	DDX_Text(pDX, Edit_OnePoint_X, m_fGetOnePointX);
	DDX_Text(pDX, Edit_OnePoint_Y, m_fGetOnePointY);
	DDX_Text(pDX, Edit_OnePoint_Z, m_fGetOnePointZ);
	DDX_Text(pDX, Edit_Nav1X, m_fNav1X);
	DDX_Text(pDX, Edit_Nav1Y, m_fNav1Y);
	DDX_Text(pDX, Edit_Nav1Z, m_fNav1Z);
	DDX_Text(pDX, Edit_Nav2X, m_fNav2X);
	DDX_Text(pDX, Edit_Nav2Y, m_fNav2Y);
	DDX_Text(pDX, Edit_Nav2Z, m_fNav2Z);
	DDX_Text(pDX, Edit_Nav3X, m_fNav3X);
	DDX_Text(pDX, Edit_Nav3Y, m_fNav3Y);
	DDX_Text(pDX, Edit_Nav3Z, m_fNav3Z);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, TAB_Form_Main, &CForm::OnSelchangeTabFormMain)
	ON_LBN_SELCHANGE(LIST_PROTOTYPE, &CForm::OnLbnSelchangePrototype)
	ON_LBN_SELCHANGE(List_Form_Animations, &CForm::OnLbnSelchangeFormAnimations)
	ON_EN_CHANGE(Edit_Poisiton_X, &CForm::OnEnChangePoisitonX)
	ON_EN_CHANGE(Edit_Position_Y, &CForm::OnEnChangePositionY)
	ON_EN_CHANGE(Edit_Position_Z, &CForm::OnEnChangePositionZ)
	ON_BN_CLICKED(BUTTON_Save_Scene, &CForm::OnBnClickedSaveScene)
	ON_BN_CLICKED(BUTTON_Load_Scene, &CForm::OnBnClickedLoadScene)
	ON_EN_CHANGE(Edit_Scale_X, &CForm::OnEnChangeScaleX)
	ON_EN_CHANGE(Edit_Scale_Y, &CForm::OnEnChangeScaleY)
	ON_EN_CHANGE(Edit_Scale_Z, &CForm::OnEnChangeScaleZ)
	ON_EN_CHANGE(Edit_Rotation_X, &CForm::OnEnChangeRotationX)
	ON_EN_CHANGE(Edit_Rotation_Y, &CForm::OnEnChangeRotationY)
	ON_EN_CHANGE(Edit_Rotation_Z, &CForm::OnEnChangeRotationZ)
	ON_EN_CHANGE(Edit_SceneName, &CForm::OnEnChangeScenename)
	ON_BN_CLICKED(Button_Delete, &CForm::OnBnClickedDelete)
	ON_WM_CLOSE()
	ON_BN_CLICKED(Button_NavMesh_Load, &CForm::OnBnClickedNavmeshLoad)
	ON_BN_CLICKED(Button_NavMesh_Save, &CForm::OnBnClickedNavmeshSave)
	ON_EN_CHANGE(Edit_ObjectName, &CForm::OnEnChangeObjectname)
	ON_EN_CHANGE(Edit_CullingRadius, &CForm::OnEnChangeCullingradius)
	ON_BN_CLICKED(Radio_AABB, &CForm::OnBnClickedAabb)
	ON_BN_CLICKED(Radio_OBB, &CForm::OnBnClickedObb)
	ON_BN_CLICKED(Radio_Sphere, &CForm::OnBnClickedSphere)
	ON_BN_CLICKED(Radio_None, &CForm::OnBnClickedNone)
	ON_EN_CHANGE(Edit_Collider_CenterX, &CForm::OnEnChangeColliderCenterx)
	ON_EN_CHANGE(Edit_Collider_CenterY, &CForm::OnEnChangeColliderCentery)
	ON_EN_CHANGE(Edit_Collider_CenterZ, &CForm::OnEnChangeColliderCenterz)
	ON_EN_CHANGE(Edit_Collider_ExtX, &CForm::OnEnChangeColliderExtx)
	ON_EN_CHANGE(Edit_Collider_ExtY, &CForm::OnEnChangeColliderExty)
	ON_EN_CHANGE(Edit_Collider_ExtZ, &CForm::OnEnChangeColliderExtz)
	ON_EN_CHANGE(Edit_Collider_RotX, &CForm::OnEnChangeColliderRotx)
	ON_EN_CHANGE(Edit_Collider_RotY, &CForm::OnEnChangeColliderRoty)
	ON_EN_CHANGE(Edit_Collider_RotZ, &CForm::OnEnChangeColliderRotz)
	ON_BN_CLICKED(Button_ColliderActive, &CForm::OnBnClickedCollideractive)
	ON_BN_CLICKED(Button_NavDelete, &CForm::OnBnClickedNavdelete)
	ON_EN_CHANGE(Edit_Collider_Radius, &CForm::OnEnChangeColliderRadius)
	ON_EN_CHANGE(Edit_OnePoint_X, &CForm::OnEnChangeOnepointX)
	ON_EN_CHANGE(Edit_OnePoint_Y, &CForm::OnEnChangeOnepointY)
	ON_EN_CHANGE(Edit_OnePoint_Z, &CForm::OnEnChangeOnepointZ)
	ON_LBN_SELCHANGE(IDC_LIST1, &CForm::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CForm 진단입니다.

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
void CForm::Set_Device(_pGraphicDevice * pGraphicDevice, _pContextDevice * pContextDevice)
{
	m_pGraphicDevcie = pGraphicDevice;
	Safe_AddRef(m_pGraphicDevcie);
	m_pContextDevice = pContextDevice;
	Safe_AddRef(m_pContextDevice);

}
void CForm::Set_AnimationList(CToolModel* pGameObject)
{
	_uint iAnimIndex = pGameObject->Get_AnimationCount();
	m_ListAnimations.ResetContent();

	for (_uint i = 0; i < iAnimIndex; ++i)
		m_ListAnimations.AddString(CA2W(pGameObject->Get_AnimationName(i)));

}

void CForm::Set_NavMeshPoint(_float3 vPos, _float fDistance)
{
	m_pDlg4->LeftButton(vPos, fDistance);
}
void CForm::Set_Pick()
{
	m_pDlg4->Pick();
}
void CForm::Add_String(const _tchar * szObjectName)
{
	m_ListObjects.AddString(szObjectName);
}
void CForm::Add_StringNav(const _tchar * szObjectName)
{
	m_ListNav.AddString(szObjectName);
}
_int CForm::Tick(_double dTimeDelta)
{
	return _int();
}
_int CForm::LateTick(_double dTimeDelta)
{
	m_pDlg4->LateTick(dTimeDelta);
	return _int();
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기입니다.


void CForm::OnSelchangeTabFormMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (TAB_Form_Main == pNMHDR->idFrom)
	{
		int iSelect = m_TabCtrl.GetCurSel();
		switch (iSelect)
		{
		case 0:
			m_pDlg2->ShowWindow(SW_SHOW);
			m_pDlg3->ShowWindow(SW_HIDE);
			m_pDlg4->ShowWindow(SW_HIDE);
			break;
		case 1:
			m_pDlg2->ShowWindow(SW_HIDE);
			m_pDlg3->ShowWindow(SW_SHOW);
			m_pDlg4->ShowWindow(SW_HIDE);
			break;
		case 2:
			m_pDlg2->ShowWindow(SW_HIDE);
			m_pDlg3->ShowWindow(SW_HIDE);
			m_pDlg4->ShowWindow(SW_SHOW);
			break;
		}
	}


	*pResult = 0;
}


void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CRect		tRect;
	CRect		tTabRect;
	m_TabCtrl.InsertItem(0, L"Mesh");
	m_TabCtrl.InsertItem(1, L"Effect");
	m_TabCtrl.InsertItem(2, L"NavMesh");

	m_TabCtrl.SetCurSel(0);
	m_TabCtrl.GetWindowRect(&tRect);
	m_TabCtrl.GetItemRect(0,&tTabRect);

	m_pDlg2 = new CDlgMesh;
	m_pDlg2->Create(TAB_MESH, &m_TabCtrl);
	m_pDlg2->MoveWindow(0, tTabRect.Height(), tRect.Width(), tRect.Height());
	m_pDlg2->ShowWindow(SW_SHOW);

	m_pDlg3 = new CDlgEffect;
	m_pDlg3->Create(TAB_EFFECT, &m_TabCtrl);
	m_pDlg3->MoveWindow(0, tTabRect.Height(), tRect.Width(), tRect.Height());
	m_pDlg3->ShowWindow(SW_HIDE);

	m_pDlg4 = new CDlg_NavMesh;
	m_pDlg4->Create(TAB_NavMesh, &m_TabCtrl);
	m_pDlg4->MoveWindow(0, tTabRect.Height(), tRect.Width(), tRect.Height());
	m_pDlg4->ShowWindow(SW_HIDE);
}


void CForm::OnLbnSelchangePrototype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_ListObjects.GetCurSel();
	if (iIndex < 0 || m_ListObjects.GetCount() < iIndex)
		return;
	

	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_ListAnimations.ResetContent();
	pView->Set_ObjectIndex(iIndex);
	UpdateData(TRUE);
	_fvector vPos = pView->Get_Position();
	_fvector vScale = pView->Get_Scale();
	m_fPositionX = XMVectorGetX(vPos); m_fPositionY = XMVectorGetY(vPos); m_fPositionZ = XMVectorGetZ(vPos);
	m_fScaleX = XMVectorGetX(vScale); m_fScaleY = XMVectorGetY(vScale); m_fScaleZ = XMVectorGetZ(vScale);
	m_fCullRadius = pView->Get_ObjectCullRadius();
	m_strObjectName = pView->Get_ObjName();
	m_strFileName = pView->Get_FileName();

	m_fCenterX = pView->Get_ObjInfo().tColliderDesc.vPivot.x;
	m_fCenterY = pView->Get_ObjInfo().tColliderDesc.vPivot.y;
	m_fCenterZ = pView->Get_ObjInfo().tColliderDesc.vPivot.z;

	m_fExtX = pView->Get_ObjInfo().tColliderDesc.vScale.x;
	m_fExtY = pView->Get_ObjInfo().tColliderDesc.vScale.y;
	m_fExtZ = pView->Get_ObjInfo().tColliderDesc.vScale.z;
		
	m_fColRotX = pView->Get_ObjInfo().tColliderDesc.vOrientation.x;
	m_fColRotY = pView->Get_ObjInfo().tColliderDesc.vOrientation.y;
	m_fColRotZ = pView->Get_ObjInfo().tColliderDesc.vOrientation.z;
	_fvector vOnePoint = pView->Get_OnePoint();
	m_fGetOnePointX = XMVectorGetX(vOnePoint);
	m_fGetOnePointY = XMVectorGetY(vOnePoint);
	m_fGetOnePointZ = XMVectorGetZ(vOnePoint);

	UpdateData(FALSE);
}


void CForm::OnLbnSelchangeFormAnimations()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iAnimIndex = m_ListAnimations.GetCurSel();
	if (iAnimIndex < 0 || m_ListAnimations.GetCount() < iAnimIndex)
		return;

	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));

	pView->Set_AnimationIndex(iAnimIndex);
	pView->Set_Animation();
}


void CForm::OnEnChangePoisitonX()
{
	UpdateData(TRUE);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));

	_fvector vPos = pView->Get_Position();
	pView->Set_ObjectTransform(XMVectorSetX(vPos, m_fPositionX));
	UpdateData(FALSE);
}


void CForm::OnEnChangePositionY()
{
	UpdateData(TRUE);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));

	_fvector vPos = pView->Get_Position();
	
	pView->Set_ObjectTransform(XMVectorSetY(vPos, m_fPositionY));
	UpdateData(FALSE);
}


void CForm::OnEnChangePositionZ()
{
	UpdateData(TRUE);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));

	_fvector vPos = pView->Get_Position();
	
	pView->Set_ObjectTransform(XMVectorSetZ(vPos, m_fPositionZ));
	UpdateData(FALSE);
}


void CForm::OnBnClickedSaveScene()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\Bin\\Data\\SceneData\\");
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));

	pView->Sort_Object();

	vector<CGameObject*> pList = pView->Get_ObjectList();
	
	CString wstrPath = szBuf;
	wstrPath += m_strScene + L".dat";
	HANDLE hFile = CreateFile(wstrPath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	
	if (INVALID_HANDLE_VALUE == hFile)
		VOIDMSG("CForm OnBnClicked_SaveScene - hFile is INVALID_HANDLE_VALUE");

	_ulong dwByte = 0;
	for (auto& pGameObj : pList)
	{
		CToolBase* pInstance = dynamic_cast<CToolBase*>(pGameObj);
		if (pGameObj == nullptr)
			VOIDMSG("CForm OnBnClicked_SaveScene - pToolObj is null");

		OBJ_INFO tObjInfo = pInstance->Get_ObjectInfo();
		if(!lstrcmp(tObjInfo.szObjName, NaviMeshObject))
			continue;

		if(tObjInfo.tColliderDesc.vPivot.x == 0)
			tObjInfo.eColliderType = OBJ_INFO::COLLIDERTYPE::COLLIDER_END;


		WriteFile(hFile, &tObjInfo, sizeof(OBJ_INFO), &dwByte, nullptr);
	}
	CloseHandle(hFile);

	return;
}


void CForm::OnBnClickedLoadScene()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMultiThreadFunc::Get_Instance()->Call_LoadButton();

	CMultiThreadFunc::Get_Instance()->LoadingProcess();
}


void CForm::OnEnChangeScaleX()
{
	UpdateData(TRUE);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));

	_fvector vScale = pView->Get_Scale();
	_vector vChangeScale = vScale;
	

	pView->Set_ObjectScale(XMVectorSetX(vChangeScale, m_fScaleX));
	UpdateData(FALSE);
}


void CForm::OnEnChangeScaleY()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));

	_fvector vScale = pView->Get_Scale();
	_vector vChangeScale = vScale;
	

	pView->Set_ObjectScale(XMVectorSetY(vChangeScale, m_fScaleY));
	UpdateData(FALSE);
}


void CForm::OnEnChangeScaleZ()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));

	_fvector vScale = pView->Get_Scale();
	_vector vChangeScale = vScale;

	pView->Set_ObjectScale(XMVectorSetZ(vChangeScale, m_fScaleZ));
	UpdateData(FALSE);
}


void CForm::OnEnChangeRotationX()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	
	pView->Set_ObjectRotation(XMConvertToRadians(m_fRotX), XMVectorSet(1.f, 0.f, 0.f, 0.f));

	UpdateData(FALSE);
}


void CForm::OnEnChangeRotationY()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));

	pView->Set_ObjectRotation(XMConvertToRadians(m_fRotY), XMVectorSet(0.f, 1.f, 0.f, 0.f));

	UpdateData(FALSE);
}


void CForm::OnEnChangeRotationZ()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));

	pView->Set_ObjectRotation(XMConvertToRadians(m_fRotZ), XMVectorSet(0.f, 0.f, 1.f, 0.f));

	UpdateData(FALSE);
}


void CForm::OnEnChangeScenename()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}


void CForm::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_ListObjects.GetCurSel();
	if (iIndex < 0 || m_ListObjects.GetCount() < iIndex)
		return;


	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_ListObjects.DeleteString(iIndex);
	pView->Delete_SelObject(iIndex);

}


void CForm::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnClose();
}


void CForm::OnBnClickedNavmeshLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L"dat", L"*.dat");
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\Bin\\Data\\SceneData\\");
	Dlg.m_ofn.lpstrInitialDir = szBuf;
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	OBJ_INFO tObjInfo;
	NAV_INFO tNavInfo;
	UpdateData(TRUE);
	if (IDOK == Dlg.DoModal())
	{
		CString wstrPath = Dlg.GetPathName();
		HANDLE	hFile = CreateFile(wstrPath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		_ulong	dwByte = 0;
		while (true)
		{

			ReadFile(hFile, &tNavInfo, sizeof(NAV_INFO), &dwByte, nullptr);
			if (dwByte <= 0)
				break;

			

			pView->Load_Object(&Change_NavToObject(tObjInfo, tNavInfo));
			Add_StringNav(NaviMeshObject);
		}
		CloseHandle(hFile);

	}
	UpdateData(FALSE);
}


void CForm::OnBnClickedNavmeshSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\Bin\\Data\\SceneData\\");
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	vector<CGameObject*> pList = pView->Get_NaviList();

	CString wstrPath = szBuf;


	_ulong dwByte = 0;
	CString wstrNavPath = szBuf;
	wstrNavPath += m_strScene + L"Nav.dat";
	HANDLE hFile = CreateFile(wstrNavPath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		VOIDMSG("CForm OnBnClicked_SaveScene - hFile is INVALID_HANDLE_VALUE");


	for (auto& pGameObj : pList)
	{
		CToolBase* pInstance = dynamic_cast<CToolBase*>(pGameObj);
		if (pGameObj == nullptr)
			VOIDMSG("CForm OnBnClicked_SaveScene - pToolObj is null");

		OBJ_INFO tObjInfo = pInstance->Get_ObjectInfo();
		if (lstrcmp(tObjInfo.szObjName, NaviMeshObject))
			continue;

		NAV_INFO tNavInfo;
		tNavInfo = tObjInfo.Get_NavInfo(tNavInfo);
		WriteFile(hFile, &tNavInfo, sizeof(NAV_INFO), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	return;
}


void CForm::OnEnChangeObjectname()
{
	UpdateData(TRUE);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	pView->Set_ObjectName(m_strObjectName);
	UpdateData(FALSE);
}




void CForm::OnEnChangeCullingradius()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	pView->Set_ObjectCullRadius(m_fCullRadius);
	UpdateData(FALSE);
}


void CForm::OnBnClickedAabb()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_eColliderType = OBJ_INFO::COLLIDER_AABB;
}


void CForm::OnBnClickedObb()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_eColliderType = OBJ_INFO::COLLIDER_OBB;
}


void CForm::OnBnClickedSphere()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_eColliderType = OBJ_INFO::COLLIDER_SPHERE;
}


void CForm::OnBnClickedNone()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_eColliderType = OBJ_INFO::COLLIDER_END;
}


void CForm::OnEnChangeColliderCenterx()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CForm::OnEnChangeColliderCentery()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CForm::OnEnChangeColliderCenterz()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CForm::OnEnChangeColliderExtx()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CForm::OnEnChangeColliderExty()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CForm::OnEnChangeColliderExtz()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CForm::OnEnChangeColliderRotx()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CForm::OnEnChangeColliderRoty()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CForm::OnEnChangeColliderRotz()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CForm::OnBnClickedCollideractive()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	pView->Set_ColliderInfo(m_eColliderType,_float3(m_fCenterX, m_fCenterY, m_fCenterZ), _float3(m_fExtX, m_fExtY, m_fExtZ)
		, _float4(m_fColRotX, m_fColRotY, m_fColRotZ, 1.f),m_fColliderRadius);
}


void CForm::OnBnClickedNavdelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_ListNav.GetCurSel();
	if (iIndex < 0 || m_ListNav.GetCount() < iIndex)
		return;


	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_ListNav.DeleteString(iIndex);
	pView->Delete_SelNav(iIndex);
}


void CForm::OnEnChangeColliderRadius()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	UpdateData(FALSE);
}


void CForm::OnEnChangeOnepointX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	UpdateData(FALSE);
}


void CForm::OnEnChangeOnepointY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	UpdateData(FALSE);
}


void CForm::OnEnChangeOnepointZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	UpdateData(FALSE);
}


void CForm::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_uint iCurSel = m_ListNav.GetCurSel();
	if (iCurSel > m_ListNav.GetCount() || iCurSel < 0)
		return;

	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	UpdateData(TRUE);

	m_fNav1X = pView->Get_NavPointX(iCurSel, 0);
	m_fNav1Y = pView->Get_NavPointY(iCurSel, 0);
	m_fNav1Z = pView->Get_NavPointZ(iCurSel, 0);
	
	m_fNav2X = pView->Get_NavPointX(iCurSel, 1);
	m_fNav2Y = pView->Get_NavPointY(iCurSel, 1);
	m_fNav2Z = pView->Get_NavPointZ(iCurSel, 1);

	m_fNav3X = pView->Get_NavPointX(iCurSel, 2);
	m_fNav3Y = pView->Get_NavPointY(iCurSel, 2);
	m_fNav3Z = pView->Get_NavPointZ(iCurSel, 2);

	pView->Set_TargetOn(iCurSel);
	
	UpdateData(FALSE);
}
