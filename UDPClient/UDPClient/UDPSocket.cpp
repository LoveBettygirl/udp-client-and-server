#include "stdafx.h"
#include "UDPSocket.h"
#include "UDPClientDlg.h"


CUDPSocket::CUDPSocket()
{
}


CUDPSocket::~CUDPSocket()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		Close();
	}
}


void CUDPSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CUDPClientDlg *pDlg = (CUDPClientDlg*)AfxGetApp()->GetMainWnd();
	TCHAR buff[4096];
	CString ip;
	UINT port;
	//int port = m_pDlg->m_portnum;
	//m_pDlg->m_ip.GetWindowTextW
	int nRead = ReceiveFrom(buff, sizeof(buff), ip, port);
	
	if (!nRead)
	{
		Close();
	}
	else if (nRead == SOCKET_ERROR)
	{
		//WSAEWOULDBLOCK：非阻塞模式下，请求的操作被阻塞，需等待再次调用
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			//WSAECONNRESET：表示连接被reset，UDP是不需要建立连接的，这里直接忽略这个错误
			//事实上这是Windows socket的一个bug，当UDP套接字在某次发送后收到一个不可到达的ICMP包时
			//这个错误将在下一次调用OnReceive()后返回，会造成假读的现象，网络中不可达的主机报这个错
			if (GetLastError() == WSAECONNRESET)
			{
				return;
			}
			//CUDPClientDlg *pDlg = (CUDPClientDlg*)AfxGetApp()->GetMainWnd();
			pDlg->KillTimer(1);	  //关闭定时器
			CString error;
			int errorcode = GetLastError();
			error.Format(L"Socket failed to receive: %d", errorcode);
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
			CString request;
			pDlg->m_request.GetWindowTextW(request);
			CString strTime;
			CTime tm;
			tm = CTime::GetCurrentTime();
			strTime = tm.Format("%Y-%m-%d %H:%M:%S");
			CString temp, errorip;
			pDlg->m_ip.GetWindowTextW(errorip);
			int errorport = pDlg->m_portnum;
			temp.Format(strTime + L"，向服务器IP=" + errorip + L"，服务器Port=%d，发送请求：%s，发送失败，错误码：%d", errorport, request, errorcode);
			pDlg->m_loglist.InsertString(pDlg->m_loglist.GetCount(), temp);	 
			pDlg->m_response.SetWindowTextW(L"");
			AfxMessageBox(error);
			Close();
		}
	}
	else
	{
		//CUDPClientDlg *pDlg = (CUDPClientDlg*)AfxGetApp()->GetMainWnd();
		pDlg->KillTimer(1);	 //关闭定时器
		CString request;
		pDlg->m_request.GetWindowTextW(request);
		//实际的字符数应是发送缓冲区空间的一半（一个字符占2个字节）
		buff[nRead / 2] = _T('\0'); //terminate the string
		CString receiveText;
		receiveText.Format(L"%s", buff);
		pDlg->m_response.SetWindowTextW(receiveText);
		CString strTime;
		CTime tm;
		tm = CTime::GetCurrentTime();
		strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString temp;
		temp.Format(strTime + L"，向服务器IP=" + ip + L"，服务器Port=%d，发送请求：%s，服务器响应：%s", port, request, receiveText);
		pDlg->m_loglist.InsertString(pDlg->m_loglist.GetCount(), temp);
		MessageBox(NULL, receiveText, L"服务器响应", MB_OK | MB_ICONINFORMATION);
		//ShutDown();
		Close();  //服务器响应之后关闭套接字
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
