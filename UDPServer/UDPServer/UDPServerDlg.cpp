
// UDPServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UDPServer.h"
#include "UDPServerDlg.h"
#include "afxdialogex.h"
#include "CmdList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUDPServerDlg 对话框



CUDPServerDlg::CUDPServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UDPSERVER_DIALOG, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_SMILE);
	m_pSocket = new CUDPSocket;
}

CUDPServerDlg::~CUDPServerDlg()
{
	delete m_pSocket;
}

void CUDPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGLIST, m_loglist);
}

BEGIN_MESSAGE_MAP(CUDPServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CUDPServerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CMDLIST, &CUDPServerDlg::OnBnClickedCmdlist)
END_MESSAGE_MAP()


// CUDPServerDlg 消息处理程序

BOOL CUDPServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//m_pSocket->m_pDlg = this;
	//m_loglist.SetHorizontalExtent(1000);
	//CRect rectTemp;
	//m_loglist.GetWindowRect(rectTemp);
	CString ip = L"127.0.0.1";
	int port = 2000;
	CString temp1, temp2, temp3;
	temp3.Format(L"%d", port);
	((CStatic*)GetDlgItem(IDC_IP))->GetWindowTextW(temp1);
	((CStatic*)GetDlgItem(IDC_PORT))->GetWindowTextW(temp2);
	((CStatic*)GetDlgItem(IDC_IP))->SetWindowTextW(temp1 + ip);
	((CStatic*)GetDlgItem(IDC_PORT))->SetWindowTextW(temp2 + temp3);
	if (!m_pSocket->Create(port, SOCK_DGRAM, FD_READ, ip))
	{
		MessageBox(L"创建套接字错误！", L"UDPServer", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	m_pSocket->Bind(port, ip);
	//服务器应该总在线并在固定的ip和端口等候

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUDPServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUDPServerDlg::OnPaint()
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
HCURSOR CUDPServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUDPServerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pSocket->Close();
	CDialogEx::OnClose();
}


void CUDPServerDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_loglist.ResetContent();
	//CDialogEx::OnOK();
}


void CUDPServerDlg::OnBnClickedCmdlist()
{
	// TODO: 在此添加控件通知处理程序代码
	CCmdList dlg;
	dlg.DoModal();
}
