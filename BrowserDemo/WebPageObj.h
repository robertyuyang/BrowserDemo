#pragma once
//class CTabProcessObj
//{
//public:
//	CTabProcessObj(void);
//	~CTabProcessObj(void);
//
//	BOOL Create(HWND hFrameWnd, const RECT& rect, HWND* phIPCTabWnd);
//	BOOL NotifyCreateWebPage(LPCTSTR lpszUrl);
//
//private:
//	HWND m_hIPCWindow;
//	HANDLE m_hProcess;
//};

typedef HWND WebPageObjId;

class IWebPageObj
{
	
public:
	virtual BOOL Create(LPCTSTR lpszUrl) = 0;
	virtual BOOL Show(BOOL bShow) = 0;
	virtual BOOL Remove() = 0;
	virtual VOID GoBack() = 0;
	virtual VOID GoForward() = 0;
	virtual VOID Refresh() = 0;
	virtual VOID ChangeWindowSize(int nWidth, int nHeight) = 0;

	virtual WebPageObjId GetId() const = 0;

	static IWebPageObj* FindObj(HWND hWnd);


};



#include <atlmisc.h>
#include <list>
class IWebPageObjCreater
{
public:
	virtual IWebPageObj* CreateObj(LPCTSTR lpszUrl) = 0;
	virtual VOID Init(HWND hFrameWnd, const RECT& TabRect) = 0;
	virtual BOOL IsInited() const = 0;
};

class CWebPageObjCreater : IWebPageObjCreater
{
public:
	CWebPageObjCreater():m_hFrameWnd(NULL)
	{

	}
	IWebPageObj* CreateObj(LPCTSTR lpszUrl);
	VOID Init(HWND hFrameWnd, const RECT& TabRect)
	{
		m_hFrameWnd = hFrameWnd;
		m_TabRect = TabRect;
	}
	BOOL IsInited() const
	{
		return m_hFrameWnd != NULL;
	}
private:
	BOOL CreateTabProcess(HWND hFrameWnd, const RECT& rect, HWND* phIPCTabWnd);
	HWND GetAvailableProcIPCWnd() const;
private:
	typedef struct _TAB_PROC_INFO
	{
		HANDLE hProcess;
		HWND hIPCTabWnd;
	}TAB_PROC_INFO, *PTAB_PROC_INFO;
	std::list<TAB_PROC_INFO> m_TabProcInfoList;
	HWND m_hFrameWnd;
	WTL::CRect m_TabRect;
};



