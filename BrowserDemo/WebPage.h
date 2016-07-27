#pragma once

#include <atlcrack.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlmisc.h>
#include <comdef.h>


#include <DispEx.h>
#include <ExDispid.h>

#define IDC_WB    0

#define WM_WEB_PAGE_CMD			WM_USER + 0x500
#define CMD_WB_GOBACK			0x1
#define CMD_WB_GOFORWARD		0x2
#define CMD_WB_REFRESH			0x3

class CWebPage : public CWindowImpl<CWebPage, CAxWindow>
	, public IDispEventImpl<IDC_WB, CWebPage, &DIID_DWebBrowserEvents2 ,&LIBID_SHDocVw, 1, 0> 
{
public:
	DECLARE_WND_SUPERCLASS(_T("ActiveMode_WebPage"), CAxWindow::GetWndClassName())

	BEGIN_MSG_MAP(CWebPage)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_SIZE(OnSize)
		MESSAGE_HANDLER(WM_WEB_PAGE_CMD, OnWebPageCmdMsg)
	END_MSG_MAP()

	BEGIN_SINK_MAP(CWebPage)
		SINK_ENTRY_EX(IDC_WB, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
	END_SINK_MAP()
public:
	CWebPage(IN LPCTSTR lpszUrl) : m_bstrUrl(lpszUrl){}
	~CWebPage(void);

	

	LRESULT OnCreate(LPCREATESTRUCT lpStruct);
	LRESULT OnDestroy();
	LRESULT OnWebPageCmdMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize( UINT wParam, _WTYPES_NS::CSize NewSize )
	{
		m_wndWebBrowser.SetWindowPos(NULL, 0, 0, NewSize.cx, NewSize.cy, SWP_NOACTIVATE | SWP_NOMOVE);
		return 0;
	}

	void __stdcall OnDocumentComplete(IDispatch* pDisp, VARIANT* URL);

private:
	CComBSTR m_bstrUrl;
	IWebBrowser2Ptr m_pWB2;
	CAxWindow m_wndWebBrowser;

};

