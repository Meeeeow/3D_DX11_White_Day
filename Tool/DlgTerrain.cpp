// DlgTerrain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "DlgTerrain.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"


// CDlgTerrain 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTerrain, CDialogEx)

CDlgTerrain::CDlgTerrain(CWnd* pParent /*=NULL*/)
	: CDialogEx(TAB_TERRAIN, pParent)
	, m_strTextureName(_T(""))
{

}

CDlgTerrain::~CDlgTerrain()
{
	Free();
}

_int CDlgTerrain::Tick(_double dTimeDelta)
{
	Invalidate(FALSE);
	return m_pTerrain->Tick(dTimeDelta);
}

_int CDlgTerrain::LateTick(_double dTimeDelta)
{
	Terrain_Render();
	return _int();
}

HRESULT CDlgTerrain::Change_Values()
{
	UpdateData(TRUE);
	
	m_tObjInfo.iNumVerticesX = m_iNumVerticesX;
	m_tObjInfo.iNumVerticesZ = m_iNumVerticesZ;

	m_tObjInfo.fIntervalX = m_fSizeX;
	m_tObjInfo.fIntervalZ = m_fSizeZ;

	m_tObjInfo.fTexU = m_fTexU;
	m_tObjInfo.fTexV = m_fTexV;

	m_tObjInfo.vStartPos.x = m_fStartPosX;
	m_tObjInfo.vStartPos.y = m_fStartPosY;
	m_tObjInfo.vStartPos.z = m_fStartPosZ;

	if (m_pTerrain != nullptr)
		Safe_Release(m_pTerrain);

	m_pTerrain = CToolTerrain::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo);

	UpdateData(FALSE);
	return S_OK;
}

void CDlgTerrain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, Terrain_List_Texture, m_ListBoxTexture);
	DDX_Text(pDX, Edit_Vertices_X, m_iNumVerticesX);
	DDX_Text(pDX, Edit_Vertices_Z, m_iNumVerticesZ);
	DDX_Text(pDX, Edit_Size_X, m_fSizeX);
	DDX_Text(pDX, Edit_Size_Z, m_fSizeZ);
	DDX_Control(pDX, Pic_Terrain, m_PicControl);
	DDX_Text(pDX, Edit_Tex_U, m_fTexU);
	DDX_Text(pDX, Edit_Tex_V, m_fTexV);
	DDX_Text(pDX, Edit_StartPos_X, m_fStartPosX);
	DDX_Text(pDX, Edit_StartPos_Y, m_fStartPosY);
	DDX_Text(pDX, Edit_StartPos_Z, m_fStartPosZ);
	DDX_Text(pDX, Edit_TextureName, m_strTextureName);
}

HRESULT CDlgTerrain::Load_Texture()
{
	wstring		wstrFilePath = L"../Client/Bin/Resource/Terrain/*.*";
	CFileFind	FileFind;
	BOOL		bContinue = FileFind.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		bContinue = FileFind.FindNextFile();
		if (FileFind.IsDots() || FileFind.IsDirectory() || FileFind.IsSystem())
			continue;
		else
			Add_TextureName(m_ListBoxTexture, FileFind.GetFileTitle(), FileFind.GetFilePath());
	}

	FileFind.Close();
	return S_OK;
}

HRESULT CDlgTerrain::Add_TextureName(CListBox& ListBox, CString TextureName, CString TextureFullPath)
{
	UpdateData(TRUE);

	if (0 < ListBox.FindString(0, TextureName))
		return S_OK;

	ListBox.AddString(TextureName);
	m_vecFileFullPath.emplace_back(TextureFullPath);

	UpdateData(FALSE);
	return S_OK;
}

HRESULT CDlgTerrain::Init_EditControl()
{
	UpdateData(TRUE);
	wcscpy_s(m_tObjInfo.szTextureName, m_vecFileFullPath[0]);
	wcscpy_s(m_tObjInfo.szShaderFilePath, Shader_Terrain);
	strcpy_s(m_tObjInfo.szTehcniqueName, DefaultTechnique);
	UpdateData(FALSE);

	wcscpy_s(m_tObjInfo.szObjName, L"Terrain");
	Change_Values();
	return S_OK;
}

HRESULT CDlgTerrain::Terrain_Render()
{
	
	
	return S_OK;
}

void CDlgTerrain::Free()
{
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pContextDevice);
	Safe_Release(m_pTerrain);
}


BEGIN_MESSAGE_MAP(CDlgTerrain, CDialogEx)
	ON_LBN_SELCHANGE(Terrain_List_Texture, &CDlgTerrain::TextureListSelChange)
	ON_BN_CLICKED(Button_Create, &CDlgTerrain::OnBnClickedCreate)
	ON_EN_CHANGE(Edit_Vertices_X, &CDlgTerrain::OnEnChangeVerticesX)
	ON_EN_CHANGE(Edit_Vertices_Z, &CDlgTerrain::OnEnChangeVerticesZ)
	ON_EN_CHANGE(Edit_Size_X, &CDlgTerrain::OnEnChangeSizeX)
	ON_EN_CHANGE(Edit_Size_Z, &CDlgTerrain::OnEnChangeSizeZ)
	ON_EN_CHANGE(Edit_Tex_U, &CDlgTerrain::OnEnChangeTexU)
	ON_EN_CHANGE(Edit_Tex_V, &CDlgTerrain::OnEnChangeTexV)
	ON_EN_CHANGE(Edit_StartPos_X, &CDlgTerrain::OnEnChangeStartposX)
	ON_EN_CHANGE(Edit_StartPos_Y, &CDlgTerrain::OnEnChangeStartposY)
	ON_EN_CHANGE(Edit_StartPos_Z, &CDlgTerrain::OnEnChangeStartposZ)
	ON_EN_CHANGE(Edit_TextureName, &CDlgTerrain::OnEnChangeTexturename)
END_MESSAGE_MAP()


// CDlgTerrain 메시지 처리기입니다.


BOOL CDlgTerrain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_pGraphicDevice = pView->Clone_GraphicDevice();
	m_pContextDevice = pView->Clone_ContextDevice();
	
	
	
	Load_Texture();
	Init_EditControl();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgTerrain::TextureListSelChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_ListBoxTexture.GetCurSel();
	if (iIndex < 0 || iIndex > m_vecFileFullPath.size())
		return;

	if(m_pTerrain != nullptr)
		m_pTerrain->Change_Texture(m_vecFileFullPath[iIndex]);
	wcscpy_s(m_tObjInfo.szTextureName, m_vecFileFullPath[iIndex]);
	Invalidate(FALSE);
}


void CDlgTerrain::OnBnClickedCreate()
{
	CMainFrame*	pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView*	pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	
	pView->Add_Objects(CToolTerrain::Create(m_pGraphicDevice, m_pContextDevice, m_tObjInfo));
}


void CDlgTerrain::OnEnChangeVerticesX()
{
	Change_Values();
}


void CDlgTerrain::OnEnChangeVerticesZ()
{
	Change_Values();
}


void CDlgTerrain::OnEnChangeSizeX()
{
	Change_Values();
}


void CDlgTerrain::OnEnChangeSizeZ()
{
	Change_Values();
}


void CDlgTerrain::OnEnChangeTexU()
{
	Change_Values();
}


void CDlgTerrain::OnEnChangeTexV()
{
	Change_Values();
}


void CDlgTerrain::OnEnChangeStartposX()
{
	Change_Values();
}


void CDlgTerrain::OnEnChangeStartposY()
{
	Change_Values();
}


void CDlgTerrain::OnEnChangeStartposZ()
{
	Change_Values();
}


void CDlgTerrain::OnEnChangeTexturename()
{
	UpdateData(TRUE);
	
	UpdateData(FALSE);
}
