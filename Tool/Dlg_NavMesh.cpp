// Dlg_NavMesh.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Dlg_NavMesh.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"

// CDlg_NavMesh 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_NavMesh, CDialogEx)

CDlg_NavMesh::CDlg_NavMesh(CWnd* pParent /*=NULL*/)
	: CDialogEx(TAB_NavMesh, pParent)
	, m_fPoint1_X(0)
	, m_fPoint1_Y(0)
	, m_fPoint1_Z(0)
	, m_fPoint2_X(0)
	, m_fPoint2_Y(0)
	, m_fPoint2_Z(0)
	, m_fPoint3_X(0)
	, m_fPoint3_Y(0)
	, m_fPoint3_Z(0)
{

}

CDlg_NavMesh::~CDlg_NavMesh()
{
	Free();
}

_int CDlg_NavMesh::Tick(_double dTimeDelta)
{

	return _int();
}

_int CDlg_NavMesh::LateTick(_double dTimeDelta)
{

	Make_Points();

	return _int();
}

void CDlg_NavMesh::LeftButton(_float3 vPos, _float fDistnace)
{
	if (!m_bPicking)
		return;

	PDDESC	tPointDesc;
	tPointDesc.vPoint = vPos;
	tPointDesc.fDistance = fDistnace;

	m_listDesc.push_back(tPointDesc);
}

void CDlg_NavMesh::Make_Points()
{
	if (m_listDesc.size() != 0)
	{
		m_listDesc.sort(PointDistanceCompare);
		m_vPoints[m_iCurrent] = m_listDesc.begin()->vPoint;
		m_tObjInfo.vNaviPoints[m_iCurrent] = m_listDesc.begin()->vPoint;
		m_listDesc.clear();

		Update_Points();

		++m_iCurrent;
		if (m_iCurrent == 3)
		{
			m_iCurrent = 0;
			CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
			CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
			pView->Add_Navi(CToolNavMesh::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo));
			UpdateData(TRUE);
			m_fPoint1_X = 0.f; m_fPoint1_Y = 0.f; m_fPoint1_Z = 0.f;
			m_fPoint2_X = 0.f; m_fPoint2_Y = 0.f; m_fPoint2_Z = 0.f;
			m_fPoint3_X = 0.f; m_fPoint3_Y = 0.f; m_fPoint3_Z = 0.f;
			UpdateData(FALSE);
		}
	}
}

void CDlg_NavMesh::Free()
{
	Safe_Release(m_pContextDevice);
	Safe_Release(m_pGraphicDevice);

}

void CDlg_NavMesh::Update_Points()
{
	UpdateData(TRUE);
	if (m_iCurrent == 0)
	{
		m_fPoint1_X = m_vPoints[0].x;
		m_fPoint1_Y = m_vPoints[0].y;
		m_fPoint1_Z = m_vPoints[0].z;
	}
	else if (m_iCurrent == 1)
	{
		m_fPoint2_X = m_vPoints[1].x;
		m_fPoint2_Y = m_vPoints[1].y;
		m_fPoint2_Z = m_vPoints[1].z;
	}
	else
	{
		m_fPoint3_X = m_vPoints[2].x;
		m_fPoint3_Y = m_vPoints[2].y;
		m_fPoint3_Z = m_vPoints[2].z;
	}
	UpdateData(FALSE);
}

void CDlg_NavMesh::Pick()
{
	m_bPicking = !m_bPicking;
	if (m_bPicking)
		m_CheckPicking.SetCheck(TRUE);
	else
		m_CheckPicking.SetCheck(FALSE);
}

void CDlg_NavMesh::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, Edit_Point1_X, m_fPoint1_X);
	DDX_Text(pDX, Edit_Point1_Y, m_fPoint1_Y);
	DDX_Text(pDX, Edit_Point1_Z, m_fPoint1_Z);
	DDX_Text(pDX, Edit_Point2_X, m_fPoint2_X);
	DDX_Text(pDX, Edit_Point2_Y, m_fPoint2_Y);
	DDX_Text(pDX, Edit_Point2_Z, m_fPoint2_Z);
	DDX_Text(pDX, Edit_Point3_X, m_fPoint3_X);
	DDX_Text(pDX, Edit_Point3_Y, m_fPoint3_Y);
	DDX_Text(pDX, Edit_Point3_Z, m_fPoint3_Z);
	DDX_Control(pDX, Check_Picking, m_CheckPicking);
}


BEGIN_MESSAGE_MAP(CDlg_NavMesh, CDialogEx)
	ON_EN_CHANGE(Edit_Point1_X, &CDlg_NavMesh::OnEnChangePoint1X)
	ON_EN_CHANGE(Edit_Point1_Y, &CDlg_NavMesh::OnEnChangePoint1Y)
	ON_EN_CHANGE(Edit_Point1_Z, &CDlg_NavMesh::OnEnChangePoint1Z)
	ON_EN_CHANGE(Edit_Point2_X, &CDlg_NavMesh::OnEnChangePoint2X)
	ON_EN_CHANGE(Edit_Point2_Y, &CDlg_NavMesh::OnEnChangePoint2Y)
	ON_EN_CHANGE(Edit_Point2_Z, &CDlg_NavMesh::OnEnChangePoint2Z)
	ON_EN_CHANGE(Edit_Point3_X, &CDlg_NavMesh::OnEnChangePoint3X)
	ON_EN_CHANGE(Edit_Point3_Y, &CDlg_NavMesh::OnEnChangePoint3Y)
	ON_EN_CHANGE(Edit_Point3_Z, &CDlg_NavMesh::OnEnChangePoint3Z)
	ON_BN_CLICKED(Check_Picking, &CDlg_NavMesh::OnBnClickedCheckPicking)
END_MESSAGE_MAP()


// CDlg_NavMesh 메시지 처리기입니다.


BOOL CDlg_NavMesh::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_pGraphicDevice = pView->Clone_GraphicDevice();
	m_pContextDevice = pView->Clone_ContextDevice();
	wcscpy_s(m_tObjInfo.szShaderFilePath, Shader_Navigation);
	strcpy_s(m_tObjInfo.szTehcniqueName, DefaultTechnique);
	wcscpy_s(m_tObjInfo.szObjName, NaviMeshObject);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlg_NavMesh::OnEnChangePoint1X()
{
	UpdateData(TRUE);

	m_vPoints[0].x = m_fPoint1_X;

	UpdateData(FALSE);
}


void CDlg_NavMesh::OnEnChangePoint1Y()
{
	UpdateData(TRUE);

	m_vPoints[0].y = m_fPoint1_Y;

	UpdateData(FALSE);
}


void CDlg_NavMesh::OnEnChangePoint1Z()
{
	UpdateData(TRUE);

	m_vPoints[0].z = m_fPoint1_Z;

	UpdateData(FALSE);
}


void CDlg_NavMesh::OnEnChangePoint2X()
{
	UpdateData(TRUE);

	m_vPoints[1].x = m_fPoint1_X;

	UpdateData(FALSE);
}


void CDlg_NavMesh::OnEnChangePoint2Y()
{
	UpdateData(TRUE);

	m_vPoints[1].y = m_fPoint1_Y;

	UpdateData(FALSE);
}


void CDlg_NavMesh::OnEnChangePoint2Z()
{
	UpdateData(TRUE);

	m_vPoints[1].z = m_fPoint1_Z;

	UpdateData(FALSE);
}


void CDlg_NavMesh::OnEnChangePoint3X()
{
	UpdateData(TRUE);

	m_vPoints[2].x = m_fPoint1_X;

	UpdateData(FALSE);
}


void CDlg_NavMesh::OnEnChangePoint3Y()
{
	UpdateData(TRUE);

	m_vPoints[2].y = m_fPoint1_Y;

	UpdateData(FALSE);
}


void CDlg_NavMesh::OnEnChangePoint3Z()
{
	UpdateData(TRUE);

	m_vPoints[2].z = m_fPoint1_Z;

	UpdateData(FALSE);
}


void CDlg_NavMesh::OnBnClickedCheckPicking()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}
