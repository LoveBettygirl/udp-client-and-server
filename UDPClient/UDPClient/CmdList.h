#pragma once


// CCmdList �Ի���

class CCmdList : public CDialogEx
{
	DECLARE_DYNAMIC(CCmdList)

public:
	CCmdList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCmdList();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMDLIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
