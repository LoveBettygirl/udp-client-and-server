// CmdList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDPServer.h"
#include "CmdList.h"
#include "afxdialogex.h"


// CCmdList �Ի���

IMPLEMENT_DYNAMIC(CCmdList, CDialogEx)

CCmdList::CCmdList(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CMDLIST, pParent)
{

}

CCmdList::~CCmdList()
{
}

void CCmdList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCmdList, CDialogEx)
END_MESSAGE_MAP()


// CCmdList ��Ϣ�������
