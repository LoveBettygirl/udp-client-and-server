#pragma once


// CCmdList 对话框

class CCmdList : public CDialogEx
{
	DECLARE_DYNAMIC(CCmdList)

public:
	CCmdList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCmdList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMDLIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
