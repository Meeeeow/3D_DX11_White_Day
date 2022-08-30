// DlgEffect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "DlgEffect.h"
#include "afxdialogex.h"


// CDlgEffect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgEffect, CDialogEx)

CDlgEffect::CDlgEffect(CWnd* pParent /*=NULL*/)
	: CDialogEx(TAB_EFFECT, pParent)
{

}

CDlgEffect::~CDlgEffect()
{
}

void CDlgEffect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffect, CDialogEx)
END_MESSAGE_MAP()


// CDlgEffect 메시지 처리기입니다.
