#include "stdafx.h"
#include "UDPSocket.h"
#include "UDPServerDlg.h"


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
	TCHAR buff[4096];
	CString ip;
	UINT port;
	int nRead = ReceiveFrom(buff, sizeof(buff), ip, port);

	if (!nRead)
	{
		//Close(); //什么都没收到则什么都不做
	}
	else if (nRead == SOCKET_ERROR)
	{
		//WSAEWOULDBLOCK：非阻塞模式下，请求的操作被阻塞，需等待再次调用
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			CString error;
			int errorcode = GetLastError();
			error.Format(L"Socket failed to receive: %d", errorcode);
			LPVOID lpMsgBuf;
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
			AfxMessageBox(error);
			//Close();
		}
	}
	else
	{
		//实际的字符数应是发送缓冲区空间的一半（一个字符占2个字节）
		buff[nRead / 2] = _T('\0'); //terminate the string
		CString receiveText;
		receiveText.Format(L"%s", buff);
		CString strTime;
		CTime tm;
		tm = CTime::GetCurrentTime();
		strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString temp;
		temp.Format(strTime + L"，收到IP=" + ip + L"，Port=%d，请求：%s，响应：", port, receiveText);
		CString response;
		if (receiveText.CompareNoCase(L"date") == 0)
		{
			response = tm.Format("%Y-%m-%d");
		}
		else if (receiveText.CompareNoCase(L"time") == 0)
		{
			response = tm.Format("%H:%M:%S");
		}
		else
		{
			response = L"Bad Request";
		}
		temp += response;
		CUDPServerDlg *pDlg = (CUDPServerDlg*)AfxGetApp()->GetMainWnd();
		pDlg->m_loglist.InsertString(pDlg->m_loglist.GetCount(), temp);
		TCHAR *resbuf = response.GetBuffer(response.GetLength());
		if (SendTo(resbuf, response.GetLength() * 2, port, ip) == SOCKET_ERROR)
		{
			if (GetLastError() != WSAEWOULDBLOCK)
			{
				CString error;
				int errorcode = GetLastError();
				error.Format(L"Socket failed to send: %d", errorcode);
				LPVOID lpMsgBuf;
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
				AfxMessageBox(error);
				//Close();
			}
		}
		//m_pDlg->m_response.SetWindowTextW(res);
		//MessageBox(NULL, res, L"UDPClient", MB_OK | MB_ICONINFORMATION);
		//ShutDown();
		//s_eventDone.SetEvent();
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
