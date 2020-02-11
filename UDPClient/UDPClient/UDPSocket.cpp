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
	// TODO: �ڴ����ר�ô����/����û���
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
		//WSAEWOULDBLOCK��������ģʽ�£�����Ĳ�������������ȴ��ٴε���
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			//WSAECONNRESET����ʾ���ӱ�reset��UDP�ǲ���Ҫ�������ӵģ�����ֱ�Ӻ����������
			//��ʵ������Windows socket��һ��bug����UDP�׽�����ĳ�η��ͺ��յ�һ�����ɵ����ICMP��ʱ
			//�����������һ�ε���OnReceive()�󷵻أ�����ɼٶ������������в��ɴ�������������
			if (GetLastError() == WSAECONNRESET)
			{
				return;
			}
			//CUDPClientDlg *pDlg = (CUDPClientDlg*)AfxGetApp()->GetMainWnd();
			pDlg->KillTimer(1);	  //�رն�ʱ��
			CString error;
			int errorcode = GetLastError();
			error.Format(L"Socket failed to receive: %d", errorcode);
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
			CString request;
			pDlg->m_request.GetWindowTextW(request);
			CString strTime;
			CTime tm;
			tm = CTime::GetCurrentTime();
			strTime = tm.Format("%Y-%m-%d %H:%M:%S");
			CString temp, errorip;
			pDlg->m_ip.GetWindowTextW(errorip);
			int errorport = pDlg->m_portnum;
			temp.Format(strTime + L"���������IP=" + errorip + L"��������Port=%d����������%s������ʧ�ܣ������룺%d", errorport, request, errorcode);
			pDlg->m_loglist.InsertString(pDlg->m_loglist.GetCount(), temp);	 
			pDlg->m_response.SetWindowTextW(L"");
			AfxMessageBox(error);
			Close();
		}
	}
	else
	{
		//CUDPClientDlg *pDlg = (CUDPClientDlg*)AfxGetApp()->GetMainWnd();
		pDlg->KillTimer(1);	 //�رն�ʱ��
		CString request;
		pDlg->m_request.GetWindowTextW(request);
		//ʵ�ʵ��ַ���Ӧ�Ƿ��ͻ������ռ��һ�루һ���ַ�ռ2���ֽڣ�
		buff[nRead / 2] = _T('\0'); //terminate the string
		CString receiveText;
		receiveText.Format(L"%s", buff);
		pDlg->m_response.SetWindowTextW(receiveText);
		CString strTime;
		CTime tm;
		tm = CTime::GetCurrentTime();
		strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString temp;
		temp.Format(strTime + L"���������IP=" + ip + L"��������Port=%d����������%s����������Ӧ��%s", port, request, receiveText);
		pDlg->m_loglist.InsertString(pDlg->m_loglist.GetCount(), temp);
		MessageBox(NULL, receiveText, L"��������Ӧ", MB_OK | MB_ICONINFORMATION);
		//ShutDown();
		Close();  //��������Ӧ֮��ر��׽���
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
