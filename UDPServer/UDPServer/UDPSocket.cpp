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
	// TODO: �ڴ����ר�ô����/����û���
	TCHAR buff[4096];
	CString ip;
	UINT port;
	int nRead = ReceiveFrom(buff, sizeof(buff), ip, port);

	if (!nRead)
	{
		//Close(); //ʲô��û�յ���ʲô������
	}
	else if (nRead == SOCKET_ERROR)
	{
		//WSAEWOULDBLOCK��������ģʽ�£�����Ĳ�������������ȴ��ٴε���
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
		//ʵ�ʵ��ַ���Ӧ�Ƿ��ͻ������ռ��һ�루һ���ַ�ռ2���ֽڣ�
		buff[nRead / 2] = _T('\0'); //terminate the string
		CString receiveText;
		receiveText.Format(L"%s", buff);
		CString strTime;
		CTime tm;
		tm = CTime::GetCurrentTime();
		strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString temp;
		temp.Format(strTime + L"���յ�IP=" + ip + L"��Port=%d������%s����Ӧ��", port, receiveText);
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
