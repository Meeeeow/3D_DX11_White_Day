#pragma once


// CDlgEffect ��ȭ �����Դϴ�.

class CDlgEffect : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEffect)

public:
	CDlgEffect(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgEffect();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = TAB_EFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
