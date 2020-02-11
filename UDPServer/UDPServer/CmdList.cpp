// CmdList.cpp : 实现文件
//

#include "stdafx.h"
#include "UDPServer.h"
#include "CmdList.h"
#include "afxdialogex.h"


// CCmdList 对话框

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


// CCmdList 消息处理程序
