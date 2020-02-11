
// UDPServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "UDPSocket.h"
#include "MyListBox.h"


// CUDPServerDlg �Ի���
class CUDPServerDlg : public CDialogEx
{
// ����
public:
	CUDPServerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CUDPServerDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UDPSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMyListBox m_loglist;
	CUDPSocket *m_pSocket;
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCmdlist();
};
