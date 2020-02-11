
// UDPClientDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CUDPClientDlg �Ի���



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


// CUDPClientDlg ��Ϣ�������

BOOL CUDPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//m_pSocket->m_pDlg = this;
	m_port.SetWindowTextW(L"");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUDPClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUDPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUDPClientDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString portstr;
	m_request.GetWindowTextW(msg);
	m_port.GetWindowTextW(portstr);
	int msglength = msg.GetLength();
	if (!msglength)
	{
		MessageBox(L"�����Ϊ�գ�", L"UDPClient", MB_OK | MB_ICONWARNING);
		return;
	}
	if (m_ip.IsBlank())
	{
		MessageBox(L"������IP����Ϊ�գ�", L"UDPClient", MB_OK | MB_ICONWARNING);
		return;
	}
	if (!portstr.GetLength())
	{
		MessageBox(L"�������˿ںŲ���Ϊ�գ�", L"UDPClient", MB_OK | MB_ICONWARNING);
		return;
	}
	BYTE nf1, nf2, nf3, nf4;
	m_ip.GetAddress(nf1, nf2, nf3, nf4);
	ip.Format(L"%d.%d.%d.%d", nf1, nf2, nf3, nf4);
	//UpdateData(true);
	//�ɲ���ϵͳ���������ж˿�
	//IPΪNULL��ʾ���ص�ַ
	if (!m_pSocket->Create(0, SOCK_DGRAM, FD_READ))
	{
		MessageBox(L"�����׽��ִ���", L"UDPClient", MB_OK | MB_ICONERROR);
		return;
	}
	UpdateData(true);
	TCHAR *msgbuf = msg.GetBuffer(msg.GetLength());
	//Unicodeģʽ��һ���ַ�ռ�����ֽ�
	if (m_pSocket->SendTo(msgbuf, msglength * 2, m_portnum, ip) == SOCKET_ERROR)
	{
		SetTimer(1, 6000, NULL);  //���ö�ʱ������ʱ6s
		//WSAEWOULDBLOCK��������ģʽ�£�����Ĳ�������������ȴ��ٴε���
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			KillTimer(1);
			CString error;
			int errorcode = GetLastError();
			error.Format(L"Socket failed to send: %d", errorcode);
			LPVOID lpMsgBuf;
			//������ϸ��Ϣ
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
			temp.Format(strTime + L"���������IP=" + ip + L"��������Port=%d����������%s������ʧ�ܣ������룺%d", m_portnum, msg, errorcode);
			m_loglist.InsertString(m_loglist.GetCount(), temp);	 
			m_response.SetWindowTextW(L"");
			AfxMessageBox(error);
			m_pSocket->Close();
		}
	}
	else
	{
		SetTimer(1, 6000, NULL);  //���ö�ʱ������ʱ6s
	}
	//m_pSocket->Close();
	//CDialogEx::OnOK();
}


void CUDPClientDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pSocket->Close();
	CDialogEx::OnClose();
}


void CUDPClientDlg::OnBnClickedCmdlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCmdList dlg;
	dlg.DoModal();
}


void CUDPClientDlg::OnBnClickedClearlog()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_loglist.ResetContent();
}


void CUDPClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(1);
	CString strTime;
	CTime tm;
	tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y-%m-%d %H:%M:%S");
	CString temp;
	temp.Format(strTime + L"���������IP=" + ip + L"��������Port=%d����������%s������ʧ�ܣ�����ʱ", m_portnum, msg);
	m_loglist.InsertString(m_loglist.GetCount(), temp);
	m_response.SetWindowTextW(L"");
	AfxMessageBox(L"����ʱ��");
	m_pSocket->Close();
	CDialogEx::OnTimer(nIDEvent);
}
