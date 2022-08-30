#pragma once


// CDlgEffect 대화 상자입니다.

class CDlgEffect : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEffect)

public:
	CDlgEffect(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgEffect();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = TAB_EFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
