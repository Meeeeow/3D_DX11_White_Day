// DlgMesh.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "DlgMesh.h"
#include "afxdialogex.h"
#include "io.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "ToolModel.h"


// CDlgMesh 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMesh, CDialogEx)

CDlgMesh::CDlgMesh(CWnd* pParent /*=NULL*/)
	: CDialogEx(TAB_MESH, pParent)
	, m_strObjectName(_T(""))
{

}

CDlgMesh::~CDlgMesh()
{
	Free();
}

HRESULT CDlgMesh::Init_List(CString strFolder)
{
	CFileFind			FileFind;

	BOOL				bContinue = FileFind.FindFile(strFolder + L"/*.*");
	CString				strFilter = L".FBX";
	CString				strFolderItem = L"";
	CString				strFileExt = L"";


	while (bContinue)
	{
		bContinue = FileFind.FindNextFile();
		if (!FileFind.IsDots() && FileFind.IsDirectory())
		{
			strFolderItem = FileFind.GetFilePath();
			Init_List(strFolderItem);
		}
		strFolderItem = FileFind.GetFilePath();
		strFileExt = strFolderItem.Mid(strFolderItem.ReverseFind('.'));

		MSG msg;
		while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
		}
		if (!FileFind.IsDots())
		{
			strFileExt.MakeUpper();

			if (FileFind.IsDirectory())
				continue;

			if (lstrcmp(strFileExt, strFilter) != 0)
				continue;
			
			Add_TextureName(m_ListModel, FileFind.GetFileTitle(), strFolderItem);
		}
	}

	FileFind.Close();
	return S_OK;
}

HRESULT CDlgMesh::Add_TextureName(CListBox & ListBox, CString TextureName, CString TextureFullPath)
{
	UpdateData(TRUE);

	if (0 < ListBox.FindString(0, TextureName))
		return S_OK;

	ListBox.AddString(TextureName);
	m_vecModelName.push_back(TextureName + L".fbx");
	CString				strReplacePath;
	TextureFullPath.Replace(TextureName + L".fbx", L"");

	m_vecModelPath.emplace_back(TextureFullPath);

	UpdateData(FALSE);
	return S_OK;
}

HRESULT CDlgMesh::Add_AnimationNames()
{
	m_ListAnimation.ResetContent();
	CToolModel* pInstance = CToolModel::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo);
	if (pInstance == nullptr)
		return S_OK;


	if (pInstance->Get_HasAnimation() == true)
	{
		_uint iIndex = pInstance->Get_AnimationCount();

		for (_uint i = 0; i < iIndex; ++i)
			m_ListAnimation.AddString(CA2W(pInstance->Get_AnimationName(i)));
	}

	Safe_Release(pInstance);
	return S_OK;
}

void CDlgMesh::Free()
{
	Safe_Release(m_pContextDevice);
	Safe_Release(m_pGraphicDevice);
}

void CDlgMesh::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, List_Model, m_ListModel);
	DDX_Control(pDX, List_Animation, m_ListAnimation);
	DDX_Text(pDX, Edit_ObjectName, m_strObjectName);
}


BEGIN_MESSAGE_MAP(CDlgMesh, CDialogEx)
	ON_BN_CLICKED(Button_Mesh_Create, &CDlgMesh::OnBnClickedMeshCreate)
	ON_LBN_SELCHANGE(List_Model, &CDlgMesh::OnLbnSelchangeModelList)
	ON_EN_CHANGE(Edit_ObjectName, &CDlgMesh::OnEnChangeObjectname)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgMesh::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgMesh 메시지 처리기입니다.


BOOL CDlgMesh::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Init_List(TEXT("../Client/Bin/Resource/Meshes"));
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_pGraphicDevice = pView->Clone_GraphicDevice();
	m_pContextDevice = pView->Clone_ContextDevice();


	wcscpy_s(m_tObjInfo.szShaderFilePath, Shader_Model);
	strcpy_s(m_tObjInfo.szTehcniqueName, DefaultTechnique);

	m_tObjInfo.eBufferType = OBJ_INFO::VIB_MODEL;

	CStringA strName, strPath;
	strName = CStringA(m_vecModelName[0]);
	strPath = CStringA(m_vecModelPath[0]);


	strcpy_s(m_tObjInfo.szFileName, strName.GetBuffer());
	strcpy_s(m_tObjInfo.szFilePath, strPath.GetBuffer());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgMesh::OnBnClickedMeshCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));

	pView->Add_Objects(CToolModel::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo));
}


void CDlgMesh::OnLbnSelchangeModelList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_ListModel.GetCurSel();
	if (iIndex < 0 || iIndex > m_vecModelPath.size())
		return;

	CStringA strName, strPath;
	strName = CStringA(m_vecModelName[iIndex]);
	strPath = CStringA(m_vecModelPath[iIndex]);

	strcpy_s(m_tObjInfo.szFileName, strName.GetBuffer());
	strcpy_s(m_tObjInfo.szFilePath, strPath.GetBuffer());

	/*memcpy(m_tObjInfo.szFileName,m_vecModelName[iIndex], m_vecModelName[iIndex].GetLength());
	memcpy(m_tObjInfo.szFilePath,m_vecModelPath[iIndex], m_vecModelPath[iIndex].GetLength());*/

	//strcpy_s(m_tObjInfo.szFileName,(LPSTR)m_vecModelName[iIndex].GetBuffer());
	//strcpy_s(m_tObjInfo.szFilePath, (LPSTR)m_vecModelPath[iIndex].GetBuffer());

	
	//Add_AnimationNames();

}


void CDlgMesh::OnEnChangeObjectname()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CStringW strObjName;
	strObjName = m_strObjectName;
	
	wcscpy_s(m_tObjInfo.szObjName, strObjName);
	UpdateData(FALSE);
}


void CDlgMesh::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	_uint fZ = 0;
	for (_uint i = 0; i < 15; ++i)
	{
		CToolModel* pModel1 = CToolModel::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo);
		pModel1->Set_Position(XMVectorSet(-1.5f, 5 - (i * 0.2f), 1.5f * fZ, 1.f));
		CToolModel* pModel2 = CToolModel::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo);
		pModel2->Set_Position(XMVectorSet(  0.f, 5 - (i * 0.2f), 1.5f * fZ, 1.f));
		CToolModel* pModel3 = CToolModel::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo);
		pModel3->Set_Position(XMVectorSet( 1.5f, 5 - (i * 0.2f), 1.5f * fZ, 1.f));
		pView->Add_Objects(pModel1);
		pView->Add_Objects(pModel2);
		pView->Add_Objects(pModel3);
		fZ += 1;
	}
	_uint ffZ = 0;
	for (_uint i = 0; i < 10; ++i)
	{
		CToolModel* pModel1 = CToolModel::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo);
		pModel1->Set_Position(XMVectorSet(-1.5f, 2, 22.5f + (1.5f * ffZ), 1.f));
		CToolModel* pModel2 = CToolModel::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo);
		pModel2->Set_Position(XMVectorSet(  0.f, 2, 22.5f + (1.5f * ffZ), 1.f));
		CToolModel* pModel3 = CToolModel::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo);
		pModel3->Set_Position(XMVectorSet( 1.5f, 2, 22.5f + (1.5f * ffZ), 1.f));
		pView->Add_Objects(pModel1);
		pView->Add_Objects(pModel2);
		pView->Add_Objects(pModel3);
		ffZ += 1;
	}
	
}
