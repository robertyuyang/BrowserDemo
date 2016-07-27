#include "stdafx.h"
#include "MainFrm.h"

#include "FrameManager.h"

#define FUNC_BTN_WIDTH 60
#define FUNC_BTN_DISTANCE 2
#define ADDRESS_EDIT_WIDTH 260



CFrameManager* g_pFrameManager = NULL;

LRESULT CMainFrame::OnCreate( LPCREATESTRUCT lpStruct )
{
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	CenterWindow();
	lpStruct->cx = 760;
	lpStruct->cy = 512;


	RECT rect = {0, 150, 760, 200};
	m_wndTabCtrl.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE, 0, CTRL_ID_TABCTRL );
	RECT BtnRect = {0, 0, FUNC_BTN_WIDTH, 30};
	m_wndBackBtn.Create(m_hWnd, BtnRect, _T("Back"), WS_CHILD | WS_VISIBLE, 0, CTRL_ID_BTN_GOBACK );
	BtnRect.left += FUNC_BTN_WIDTH + FUNC_BTN_DISTANCE;
	BtnRect.right += FUNC_BTN_WIDTH + FUNC_BTN_DISTANCE;
	m_wndForwardBtn.Create(m_hWnd, BtnRect, _T("Forward"), WS_CHILD | WS_VISIBLE, 0, CTRL_ID_BTN_GOFORWARD );
	BtnRect.left += FUNC_BTN_WIDTH + FUNC_BTN_DISTANCE;
	BtnRect.right += FUNC_BTN_WIDTH + FUNC_BTN_DISTANCE;
	m_wndRefreshBtn.Create(m_hWnd, BtnRect, _T("Refresh"), WS_CHILD | WS_VISIBLE, 0, CTRL_ID_BTN_REFRESTH );

	BtnRect.left += FUNC_BTN_WIDTH + FUNC_BTN_DISTANCE;
	BtnRect.right += BtnRect.left + ADDRESS_EDIT_WIDTH ;
	m_wndAddressEdit.Create(m_hWnd, BtnRect, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 0, CTRL_ID_EDIT_ADDRESS );


	g_pFrameManager = new CFrameManager(*this);
	g_pFrameManager->CreateTab(_T("www.sogou.com"));

	int nIndex = m_wndTabCtrl.AddItem(_T("加载中"));
	m_wndTabCtrl.SetCurSel(nIndex);

	g_pFrameManager->CreateTab(_T("www.baidu.com"));

	nIndex = m_wndTabCtrl.AddItem(_T("加载中"));
	m_wndTabCtrl.SetCurSel(nIndex);

	
	return 0;
}

LRESULT CMainFrame::OnIPCTabToFrameMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch(uMsg)
	{
	case WM_COPYDATA:
		{
			if((ULONG)wParam == WM_IPC_T2F_SET_TAB_CTRL_TITLE)
			{
				COPYDATASTRUCT* pCs = (COPYDATASTRUCT*)lParam;
				HWND hWnd = (HWND)pCs->dwData;
				LPCTSTR lpszTitle = (LPCTSTR)pCs->lpData;

				int nIndex = g_pFrameManager->FindIndex(hWnd);
				if(nIndex >= 0)
				{

					TCITEM tcItem = {0};
					tcItem.mask = TCIF_TEXT;
					tcItem.pszText = (LPWSTR)lpszTitle;
					m_wndTabCtrl.SetItem(nIndex, &tcItem);

				}
			}
		}
		break;
	/*case WM_IPC_T2F_CREATE_WEB_PAGE_RESULT:
		{
			HWND hIPCTabWnd = (HWND)wParam;
			int nIndexInTab = (int)lParam;
			g_pFrameManager->OnTabCreated(hIPCTabWnd, nIndexInTab);
			int nIndex = m_wndTabCtrl.AddItem(_T("加载中"));
			m_wndTabCtrl.SetCurSel(nIndex);
		}
		break;
	case WM_IPC_T2F_SHOW_WEB_PAGE_RESULT:
		{

		}
		break*/
	default:
		break;
	}
	return 0;
}


LRESULT CMainFrame::OnTabCtrlSelChanged(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	int nItem = m_wndTabCtrl.GetCurSel();

	g_pFrameManager->ShowTab(nItem);

	return 0;
}

LRESULT CMainFrame::OnFuncBtnsCmd( WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/ )
{
	if(!g_pFrameManager->m_pCurWebPageObj)
	{
		return 0;
	}
	switch(wID)
	{
	case CTRL_ID_BTN_GOBACK:
		{
			g_pFrameManager->m_pCurWebPageObj->GoBack();
		}
		break;
	case CTRL_ID_BTN_GOFORWARD:
		{
			g_pFrameManager->m_pCurWebPageObj->GoForward();
		}
		break;
	case CTRL_ID_BTN_REFRESTH:
		{
			g_pFrameManager->m_pCurWebPageObj->Refresh();
		}
		break;
	default:
		break;
	}
	return 0;
}

BOOL CMainFrame::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->hwnd == m_wndAddressEdit)
	{
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			OnAddressGo();
			return TRUE;
		}
	}
	return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

VOID CMainFrame::OnAddressGo()
{
	TCHAR szUrl[1024] = {0};
	m_wndAddressEdit.GetWindowText(szUrl, 1024);
	if(g_pFrameManager->CreateTab(szUrl))
	{
		int nIndex = m_wndTabCtrl.AddItem(_T("加载中"));
		m_wndTabCtrl.SetCurSel(nIndex);
	}
}

LRESULT CMainFrame::OnDestroy()
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	SetMsgHandled(FALSE);

	return 1;
}

LRESULT CMainFrame::OnSize( UINT wParam, _WTYPES_NS::CSize NewSize )
{
	for(auto Iter = g_pFrameManager->m_pWebPageObjList.begin();
		Iter != g_pFrameManager->m_pWebPageObjList.end();
		Iter++)
	{
		if(*Iter)
		{
			(*Iter)->ChangeWindowSize(NewSize.cx, NewSize.cy);
		}
		
	}
	
	return 0;
}
