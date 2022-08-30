#pragma once
#include "afxwin.h"
#include "ToolTerrain.h"

// CDlgTerrain 대화 상자입니다.

class CDlgTerrain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTerrain)

public:
	CDlgTerrain(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTerrain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = TAB_TERRAIN };
#endif
public:
	_int						Tick(_double dTimeDelta);
	_int						LateTick(_double dTimeDelta);

public:
	HRESULT						Change_Values();

public: /* Inline */

public: /* Inline */

private:
	_pGraphicDevice*			m_pGraphicDevice = nullptr;
	_pContextDevice*			m_pContextDevice = nullptr;
	CToolTerrain*				m_pTerrain = nullptr;
	OBJ_INFO					m_tObjInfo;
private:
	HRESULT						Load_Texture();
	HRESULT						Add_TextureName(CListBox& ListBox, CString TextureName, CString TextureFullPath);

	HRESULT						Init_EditControl();
	HRESULT						Terrain_Render();

	

public:
	void						Free();
		

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CListBox					m_ListBoxTexture;
	CStatic						m_PicControl;
	vector<CString>				m_vecFileFullPath;
	int							m_iNumVerticesX = 2;
	int							m_iNumVerticesZ = 2;
	float						m_fSizeX = 1.f;
	float						m_fSizeZ = 1.f;
	float						m_fTexU = 1.f;
	float						m_fTexV = 1.f;
	float						m_fStartPosX = 0.f;
	float						m_fStartPosY = 0.f;
	float						m_fStartPosZ = 0.f;


	afx_msg void TextureListSelChange();
	afx_msg void OnBnClickedCreate();

	
	afx_msg void OnEnChangeVerticesX();
	afx_msg void OnEnChangeVerticesZ();
	afx_msg void OnEnChangeSizeX();
	afx_msg void OnEnChangeSizeZ();
	afx_msg void OnEnChangeTexU();
	afx_msg void OnEnChangeTexV();
	afx_msg void OnEnChangeStartposX();
	afx_msg void OnEnChangeStartposY();
	afx_msg void OnEnChangeStartposZ();
	
	CString m_strTextureName;
	afx_msg void OnEnChangeTexturename();
};
