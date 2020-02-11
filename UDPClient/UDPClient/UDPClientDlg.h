
// UDPClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "UDPSocket.h"
#include "MyListBox.h"


// CUDPClientDlg 对话框
class CUDPClientDlg : public CDialogEx
{
// 构造
public:
	CUDPClientDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CUDPClientDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UDPCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CUDPSocket * m_pSocket;
	CIPAddressCtrl m_ip;
	CEdit m_port;
	CEdit m_request;
	CEdit m_response;
	int m_portnum;
	afx_msg void OnClose();
	CMyListBox m_loglist;
	afx_msg void OnBnClickedCmdlist();
	afx_msg void OnBnClickedClearlog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString ip;
	CString msg;
};
