
// UDPClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UDPClient.h"
#include "UDPClientDlg.h"
#include "UDPSocket.h"
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


// CUDPClientDlg 对话框



CUDPClientDlg::CUDPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UDPCLIENT_DIALOG, pParent)
	, m_portnum(0)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_SMILE);
	m_pSocket = new CUDPSocket;
}

CUDPClientDlg::~CUDPClientDlg()
{
	delete m_pSocket;
}

void CUDPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVERIP, m_ip);
	DDX_Control(pDX, IDC_SERVERPORT, m_port);
	DDX_Control(pDX, IDC_REQUEST, m_request);
	DDX_Control(pDX, IDC_RESPONSE, m_response);
	DDX_Text(pDX, IDC_SERVERPORT, m_portnum);
	DDX_Control(pDX, IDC_LOGLIST, m_loglist);
}

BEGIN_MESSAGE_MAP(CUDPClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CUDPClientDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CMDLIST, &CUDPClientDlg::OnBnClickedCmdlist)
	ON_BN_CLICKED(IDC_CLEARLOG, &CUDPClientDlg::OnBnClickedClearlog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CUDPClientDlg 消息处理程序

BOOL CUDPClientDlg::OnInitDialog()
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
	m_port.SetWindowTextW(L"");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUDPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUDPClientDlg::OnPaint()
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
HCURSOR CUDPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUDPClientDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString portstr;
	m_request.GetWindowTextW(msg);
	m_port.GetWindowTextW(portstr);
	int msglength = msg.GetLength();
	if (!msglength)
	{
		MessageBox(L"命令不能为空！", L"UDPClient", MB_OK | MB_ICONWARNING);
		return;
	}
	if (m_ip.IsBlank())
	{
		MessageBox(L"服务器IP不能为空！", L"UDPClient", MB_OK | MB_ICONWARNING);
		return;
	}
	if (!portstr.GetLength())
	{
		MessageBox(L"服务器端口号不能为空！", L"UDPClient", MB_OK | MB_ICONWARNING);
		return;
	}
	BYTE nf1, nf2, nf3, nf4;
	m_ip.GetAddress(nf1, nf2, nf3, nf4);
	ip.Format(L"%d.%d.%d.%d", nf1, nf2, nf3, nf4);
	//UpdateData(true);
	//由操作系统随机分配空闲端口
	//IP为NULL表示本地地址
	if (!m_pSocket->Create(0, SOCK_DGRAM, FD_READ))
	{
		MessageBox(L"创建套接字错误！", L"UDPClient", MB_OK | MB_ICONERROR);
		return;
	}
	UpdateData(true);
	TCHAR *msgbuf = msg.GetBuffer(msg.GetLength());
	//Unicode模式下一个字符占两个字节
	if (m_pSocket->SendTo(msgbuf, msglength * 2, m_portnum, ip) == SOCKET_ERROR)
	{
		SetTimer(1, 6000, NULL);  //设置定时器，定时6s
		//WSAEWOULDBLOCK：非阻塞模式下，请求的操作被阻塞，需等待再次调用
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			KillTimer(1);
			CString error;
			int errorcode = GetLastError();
			error.Format(L"Socket failed to send: %d", errorcode);
			LPVOID lpMsgBuf;
			//错误详细信息
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				errorcode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //Default language
				(LPTSTR)&lpMsgBuf,
				0,
				NULL
				);
			error = error + L"\n" + (LPCTSTR)lpMsgBuf;
			CString strTime;
			CTime tm;
			tm = CTime::GetCurrentTime();
			strTime = tm.Format("%Y-%m-%d %H:%M:%S");
			CString temp;
			temp.Format(strTime + L"，向服务器IP=" + ip + L"，服务器Port=%d，发送请求：%s，发送失败，错误码：%d", m_portnum, msg, errorcode);
			m_loglist.InsertString(m_loglist.GetCount(), temp);	 
			m_response.SetWindowTextW(L"");
			AfxMessageBox(error);
			m_pSocket->Close();
		}
	}
	else
	{
		SetTimer(1, 6000, NULL);  //设置定时器，定时6s
	}
	//m_pSocket->Close();
	//CDialogEx::OnOK();
}


void CUDPClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pSocket->Close();
	CDialogEx::OnClose();
}


void CUDPClientDlg::OnBnClickedCmdlist()
{
	// TODO: 在此添加控件通知处理程序代码
	CCmdList dlg;
	dlg.DoModal();
}


void CUDPClientDlg::OnBnClickedClearlog()
{
	// TODO: 在此添加控件通知处理程序代码
	m_loglist.ResetContent();
}


void CUDPClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(1);
	CString strTime;
	CTime tm;
	tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y-%m-%d %H:%M:%S");
	CString temp;
	temp.Format(strTime + L"，向服务器IP=" + ip + L"，服务器Port=%d，发送请求：%s，发送失败，请求超时", m_portnum, msg);
	m_loglist.InsertString(m_loglist.GetCount(), temp);
	m_response.SetWindowTextW(L"");
	AfxMessageBox(L"请求超时！");
	m_pSocket->Close();
	CDialogEx::OnTimer(nIDEvent);
}
