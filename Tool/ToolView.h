
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

#include "Form.h"
#include "ToolDoc.h"
#include "ToolCamera.h"

class CToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

public:
	typedef unordered_map<CString, vector<CGameObject*>>	SCENE;
	typedef vector<CGameObject*>							LIST;
// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:
	_pGraphicDevice*		Clone_GraphicDevice() { Safe_AddRef(m_pGraphicDevice); return m_pGraphicDevice; }
	_pContextDevice*		Clone_ContextDevice() { Safe_AddRef(m_pContextDevice); return m_pContextDevice; }

public:
	HRESULT					Add_Objects(CGameObject* pObject);
	HRESULT					Add_Navi(CGameObject* pObject);

public:
	OBJ_INFO				Get_ObjInfo();
	_fvector				Get_Position();
	_fvector				Get_Scale();
	LIST					Get_ObjectList() { return m_vecObjects; }
	LIST					Get_NaviList() { return m_vecNavi; }
	_float					Get_ObjectCullRadius();
	CString					Get_ObjName();
	CString					Get_FileName();
	_fvector				Get_OnePoint();

public:
	void					Set_ObjectIndex(_uint iIndex);
	void					Set_AnimationIndex(_uint iIndex) { m_iAnimationIndex = iIndex; }
	void					Set_Animation();
	void					Set_ObjectColliderType(OBJ_INFO::COLLIDERTYPE eType);
	void					Set_ColliderInfo(OBJ_INFO::COLLIDERTYPE eType, _float3 vCenter, _float3 vExt, _float4 vRot = _float4(0.f, 0.f, 0.f, 0.f), _float fRadius = 0.f);
	void					Set_ObjectTransform(_fvector vPos);
	void					Set_ObjectScale(_fvector vScale);
	void					Set_ObjectRotation(_float fRadian, _fvector vAxis);
	void					Set_NavMeshPoint(_float3 vPos, _float fDistance);
	void					Set_ObjectName(CString wstrObjectName);
	void					Set_ObjectCullRadius(_float fRadius);
	void					Delete_SelObject(_uint iIndex);
	void					Delete_SelNav(_uint iIndex);
	void					Clear_Object();


public:
	_float					Get_NavPointX(_uint iCurSel, _uint iIndex);
	_float					Get_NavPointY(_uint iCurSel, _uint iIndex);
	_float					Get_NavPointZ(_uint iCurSel, _uint iIndex);
	void					Set_TargetOn(_uint iCurSel);
	

public:
	void					Load_Object(OBJ_INFO* pObjInfo);
	void					Load_NaviMesh(OBJ_INFO* pObjInfo);

public:
	static _bool			Compare(CGameObject* pSour, CGameObject* pDest);
	void					Sort_Object();

public:
	_int					Tick(_double dTimeDelta);
	_int					LateTick(_double dTimeDelta);

public:
	void					Free();
	void					Render();
	void					Camera_Update();
	void					Update();
	void					LeftButton();

private:
	_pGraphicDevice*		m_pGraphicDevice = nullptr;
	_pContextDevice*		m_pContextDevice = nullptr;
	CRenderer*				m_pRenderer = nullptr;

	CToolCamera*			m_pCamera = nullptr;

	LIST					m_vecObjects;
	LIST					m_vecNavi;

	_uint					m_iObjectIndex = 0;
	_uint					m_iAnimationIndex = 0;	
private:
	CGameInstance*			m_pGameInstance = nullptr;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

