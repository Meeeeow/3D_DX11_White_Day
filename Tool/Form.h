#pragma once

#include "DlgTerrain.h"
#include "DlgMesh.h"
#include "DlgEffect.h"
#include "Dlg_NavMesh.h"

#include "afxcmn.h"
#include "afxwin.h"

#include "ToolModel.h"

// CForm 폼 뷰입니다.

class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
public:
	virtual ~CForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
	void					Set_Device(_pGraphicDevice* pGraphicDevice, _pContextDevice* pContextDevice);
	void					Set_AnimationList(CToolModel* pGameObject);
	void					Set_NavMeshPoint(_float3 vPos, _float fDistance);
	void					Set_Pick();

public:
	void					Add_String(const _tchar* szObjectName);
	void					Add_StringNav(const _tchar* szObjectName);

public:
	

public:
	_int					Tick(_double dTimeDelta);
	_int					LateTick(_double dTimeDelta);

private:
	_pGraphicDevice*		m_pGraphicDevcie = nullptr;
	_pContextDevice*		m_pContextDevice = nullptr;
	CDlgTerrain*			m_pDlg1 = nullptr;
	CDlgMesh*				m_pDlg2 = nullptr;
	CDlgEffect*				m_pDlg3 = nullptr;
	CDlg_NavMesh*			m_pDlg4 = nullptr;

public:
	void					Free();

public:
	void					Update_CameraInfo(_float3 fPos, _float fNear, _float fFar, _float fFOV);
	OBJ_INFO				Change_NavToObject(OBJ_INFO tObjInfo, NAV_INFO tNavInfo);

public:
	_bool					Get_Pick() { return m_pDlg4->Get_Pick(); }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	float					m_fCamPosX		 = 0.f;
	float					m_fCamPosY		 = 0.f;
	float					m_fCamPosZ		 = 0.f;
	float					m_fNear			 = 0.f;
	float					m_fFar			 = 0.f;
	float					m_fFOV			 = 0.f;
	float					m_fPositionX	 = 0.f;
	float					m_fPositionY	 = 0.f;
	float					m_fPositionZ	 = 0.f;
	float					m_fScaleX		 = 1.f;
	float					m_fScaleY		 = 1.f;
	float					m_fScaleZ		 = 1.f;
	float					m_fRotX			 = 0.f;
	float					m_fRotY			 = 0.f;
	float					m_fRotZ			 = 0.f;
	CTabCtrl				m_TabCtrl;
	CListBox				m_ListObjects;
	CListBox				m_ListAnimations;
	OBJ_INFO::COLLIDERTYPE	m_eColliderType;

	afx_msg void			OnSelchangeTabFormMain(NMHDR *pNMHDR, LRESULT *pResult); /* Tab 변경 */
	virtual void			OnInitialUpdate();
	afx_msg void			OnLbnSelchangePrototype(); /* 프로토타입 */
	afx_msg void			OnLbnSelchangeFormAnimations(); /* 애니메이션 변경 */
	
	afx_msg void			OnEnChangePoisitonX();
	afx_msg void			OnEnChangePositionY();
	afx_msg void			OnEnChangePositionZ();
	afx_msg void			OnBnClickedSaveScene(); /* 씬 저장 */
	afx_msg void			OnBnClickedLoadScene(); /* 씬 오브젝트 로드 */

	afx_msg void			OnEnChangeScaleX();
	afx_msg void			OnEnChangeScaleY();
	afx_msg void			OnEnChangeScaleZ();
	afx_msg void			OnEnChangeRotationX();
	afx_msg void			OnEnChangeRotationY();
	afx_msg void			OnEnChangeRotationZ();
	CString					m_strScene = L"";
	afx_msg void			OnEnChangeScenename();
	afx_msg void			OnBnClickedDelete();
	afx_msg void			OnClose();
	afx_msg void			OnBnClickedNavmeshLoad(); /* 씬 네비메시 로드 */
	afx_msg void			OnBnClickedNavmeshSave(); /* 씬 네비메시 저장*/
	CString					m_strObjectName = L"";
	afx_msg void			OnEnChangeObjectname();
	float					m_fCullRadius = 0.f;
	afx_msg void			OnEnChangeCullingradius();
	CString m_strFileName;
	afx_msg void			OnBnClickedAabb();
	afx_msg void			OnBnClickedObb();
	afx_msg void			OnBnClickedSphere();
	afx_msg void			OnBnClickedNone();
	afx_msg void			OnEnChangeColliderCenterx();
	afx_msg void			OnEnChangeColliderCentery();
	afx_msg void			OnEnChangeColliderCenterz();
	afx_msg void			OnEnChangeColliderExtx();
	afx_msg void			OnEnChangeColliderExty();
	afx_msg void			OnEnChangeColliderExtz();
	afx_msg void			OnEnChangeColliderRotx();
	afx_msg void			OnEnChangeColliderRoty();
	afx_msg void			OnEnChangeColliderRotz();
	float					m_fCenterX;
	float					m_fCenterY;
	float					m_fCenterZ;
	float					m_fExtX;
	float					m_fExtY;
	float					m_fExtZ;
	float					m_fColRotX;
	float					m_fColRotY;
	float					m_fColRotZ;
	afx_msg void			OnBnClickedCollideractive();
	
	bool					m_bRadio[4];
	CListBox m_ListNav;
	afx_msg void OnBnClickedNavdelete();
	float m_fColliderRadius;
	afx_msg void OnEnChangeColliderRadius();
	float m_fGetOnePointX;
	float m_fGetOnePointY;
	float m_fGetOnePointZ;
	afx_msg void OnEnChangeOnepointX();
	afx_msg void OnEnChangeOnepointY();
	afx_msg void OnEnChangeOnepointZ();
	float m_fNav1X;
	float m_fNav1Y;
	float m_fNav1Z;
	float m_fNav2X;
	float m_fNav2Y;
	float m_fNav2Z;
	float m_fNav3X;
	float m_fNav3Y;
	float m_fNav3Z;
	afx_msg void OnLbnSelchangeList1();
};


