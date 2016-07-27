#include "StdAfx.h"
#include "WebPage.h"

#include "TabProcessConst.h"


#include <atlconv.h>



CWebPage::~CWebPage(void)
{
}

LRESULT CWebPage::OnCreate( LPCREATESTRUCT lpStruct )
{
	
	IUnknownPtr punkCtrl;
	

	RECT rect;
	GetClientRect(&rect);
	m_wndWebBrowser.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN);

	m_wndWebBrowser.CreateControlEx ( L"{8856F961-340A-11D0-A96B-00C04FD705A2}", NULL,
		NULL, &punkCtrl );

	m_pWB2 = punkCtrl;

	if(m_pWB2)
	{
		m_pWB2->put_Silent(VARIANT_TRUE);
		CComVariant vEmpty;
		m_pWB2->Navigate(m_bstrUrl, &vEmpty, &vEmpty, &vEmpty, &vEmpty);

		if(FAILED(DispEventAdvise(m_pWB2, &DIID_DWebBrowserEvents2)))  {
			ATLASSERT(FALSE);  
			return -1L;
		}
	}

	return 0;
}

void __stdcall CWebPage::OnDocumentComplete( IDispatch* pDisp, VARIANT* URL )
{
	OutputDebugString(_T("[XXX]CWebPage::OnDocumentComplete"));
	if(m_pWB2 == NULL)
	{
		return;
	}

	READYSTATE State;
	HRESULT hr = m_pWB2->get_ReadyState(&State);
	if(SUCCEEDED(hr) && ( READYSTATE_COMPLETE == State) && (pDisp == m_pWB2))
	{
		CComBSTR bstrTitle;
		hr = m_pWB2->get_LocationName(&bstrTitle);
		if(SUCCEEDED(hr))
		{
			ATL::CWindow wndParent = GetParent();
			
			USES_CONVERSION;
			
			COPYDATASTRUCT cs;
			cs.dwData = (ULONG_PTR)m_hWnd;
			cs.lpData = OLE2T((BSTR)bstrTitle);
			cs.cbData = (bstrTitle.Length() + 1 ) * sizeof(TCHAR);OutputDebugString(OLE2T((BSTR)bstrTitle));
			//wndParent.SendMessage(WM_IPC_T2F_SET_TAB_CTRL_TITLE, (WPARAM)m_hWnd, (LPARAM)lp/*OLE2T((BSTR)bstrTitle)*/);
			wndParent.SendMessage(WM_COPYDATA, WM_IPC_T2F_SET_TAB_CTRL_TITLE, (LPARAM)&cs);
		}
	}
}

LRESULT CWebPage::OnDestroy()
{
	if( m_pWB2 ){
		DispEventUnadvise( m_pWB2 , &DIID_DWebBrowserEvents2);  
	}
	m_pWB2 = NULL; 

	return 0;
}

LRESULT CWebPage::OnWebPageCmdMsg( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if(!m_pWB2)
	{
		return 0;
	}
	switch(wParam)
	{
	case CMD_WB_GOBACK:
		{
			m_pWB2->GoBack();
		}
		break;
	case CMD_WB_GOFORWARD:
		{
			m_pWB2->GoForward();
		}
		break;
	case CMD_WB_REFRESH:
		{
			m_pWB2->Refresh();
		}
	default:
		break;
	}

	return 0;
}
