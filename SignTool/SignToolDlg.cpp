
// SignToolDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "../DRAutoPackageTool/SignHelper.h"
#include <string>
#include "SignTool.h"
#include "SignToolDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSignToolDlg 对话框

CSignToolDlg::CSignToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSignToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSignToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSignToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSignToolDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CSignToolDlg::OnEnChangeMfceditbrowse1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSignToolDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CSignToolDlg 消息处理程序

BOOL CSignToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSignToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSignToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSignToolDlg::OnBnClickedButton1()
{
	//TODO:  在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_MFCEDITBROWSE1, str);
	std::wstring path(str);
	if (path.length() > 0)
	{
		wchar_t buf[1000];
		int i = 1000;
		GetCurrentDirectory(1000, buf);  //得到当前工作路径
		SignHelper heper(std::wstring(buf) + L"\\signtool\\");
		if (heper.SignSinaFile(path))
		{
			SetDlgItemText(IDC_STATIC, L"成功");
		}
		else
		{
			SetDlgItemText(IDC_STATIC, L"失败");
		}
		UpdateData(FALSE);
	}
}


void CSignToolDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CSignToolDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	CString str;
	GetDlgItemText(IDC_MFCEDITBROWSE2, str);
	std::wstring path(str);
	if (path.length() > 0)
	{
		path += L"\\";
		wchar_t buf[1000];
		int i = 1000;
		GetCurrentDirectory(1000, buf);  //得到当前工作路径
		SignHelper heper(std::wstring(buf) + L"\\signtool\\");
		if (heper.SignSinaFolder(path, L"*.exe") && heper.SignSinaFolder(path, L"*.dll"))
		{
			SetDlgItemText(IDC_STATIC, L"成功");
		}
		else
		{
			SetDlgItemText(IDC_STATIC, L"失败");
		}
		UpdateData(FALSE);
	}
}
