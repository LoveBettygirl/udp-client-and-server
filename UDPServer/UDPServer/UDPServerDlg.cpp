
// UDPServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDPServer.h"
#include "UDPServerDlg.h"
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


// CUDPServerDlg �Ի���



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


// CUDPServerDlg ��Ϣ�������

BOOL CUDPServerDlg::OnInitDialog()
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
		MessageBox(L"�����׽��ִ���", L"UDPServer", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	m_pSocket->Bind(port, ip);
	//������Ӧ�������߲��ڹ̶���ip�Ͷ˿ڵȺ�

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUDPServerDlg::OnPaint()
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
HCURSOR CUDPServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUDPServerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pSocket->Close();
	CDialogEx::OnClose();
}


void CUDPServerDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_loglist.ResetContent();
	//CDialogEx::OnOK();
}


void CUDPServerDlg::OnBnClickedCmdlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CCmdList dlg;
	dlg.DoModal();
}
