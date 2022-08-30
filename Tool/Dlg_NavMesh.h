#pragma once
#include "afxwin.h"
#include "ToolNavMesh.h"

// CDlg_NavMesh 대화 상자입니다.

class CDlg_NavMesh : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_NavMesh)

public:
	CDlg_NavMesh(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_NavMesh();

public:

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = TAB_NavMesh };
#endif

public:
	_int			Tick(_double dTimeDelta);
	_int			LateTick(_double dTimeDelta);

public:
	void			LeftButton(_float3 vPos, _float fDistnace);
	void			Make_Points();
	void			Free();
	void			Update_Points();
	void			Pick();
	_bool			Get_Pick() { return m_bPicking; }

	static _bool	PointDistanceCompare(PDDESC t1, PDDESC t2)	{
		return t1.fDistance < t2.fDistance;
	};

public:
	_uint						m_iCurrent = 0;
	_float3						m_vPoints[3];
	_bool						m_bPicking = false;

	list<PDDESC>				m_listDesc;
	OBJ_INFO					m_tObjInfo;
private:
	_pGraphicDevice*			m_pGraphicDevice = nullptr;
	_pContextDevice*			m_pContextDevice = nullptr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListNav;
	virtual BOOL OnInitDialog();
	float m_fPoint1_X = 0.f;
	float m_fPoint1_Y = 0.f;
	float m_fPoint1_Z = 0.f;
	float m_fPoint2_X = 0.f;
	float m_fPoint2_Y = 0.f;
	float m_fPoint2_Z = 0.f;
	float m_fPoint3_X = 0.f;
	float m_fPoint3_Y = 0.f;
	float m_fPoint3_Z = 0.f;
	afx_msg void OnEnChangePoint1X();
	afx_msg void OnEnChangePoint1Y();
	afx_msg void OnEnChangePoint1Z();
	afx_msg void OnEnChangePoint2X();
	afx_msg void OnEnChangePoint2Y();
	afx_msg void OnEnChangePoint2Z();
	afx_msg void OnEnChangePoint3X();
	afx_msg void OnEnChangePoint3Y();
	afx_msg void OnEnChangePoint3Z();
	CButton m_CheckPicking;
	afx_msg void OnBnClickedCheckPicking();
};
