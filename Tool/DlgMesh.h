#pragma once
#include "afxwin.h"


// CDlgMesh 대화 상자입니다.

class CDlgMesh : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMesh)

public:
	CDlgMesh(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMesh();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = TAB_MESH };
#endif

private:
	_pGraphicDevice*			m_pGraphicDevice = nullptr;
	_pContextDevice*			m_pContextDevice = nullptr;

public:
	OBJ_INFO					m_tObjInfo;

public:
	OBJ_INFO					Get_ObjInfo() { return m_tObjInfo; }

public:
	void						Set_ObjInfo(OBJ_INFO tObjInfo) { m_tObjInfo = tObjInfo; }

private:
	HRESULT						Init_List(CString strFolder);
	HRESULT						Add_TextureName(CListBox& ListBox, CString TextureName, CString TextureFullPath);
	HRESULT						Add_AnimationNames();

public:
	void						Free();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL				OnInitDialog();
	afx_msg void				OnBnClickedMeshCreate(); /* Create 버튼 */
	afx_msg void				OnLbnSelchangeModelList(); /* List 클릭 */

public:
	CListBox					m_ListModel;
	CListBox					m_ListAnimation;

	vector<CString>				m_vecModelPath;
	vector<CString>				m_vecModelName;
	

	CString						m_strObjectName;
	afx_msg void				OnEnChangeObjectname();
	afx_msg void OnBnClickedButton1();
};
