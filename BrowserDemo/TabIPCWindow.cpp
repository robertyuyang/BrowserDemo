#include "StdAfx.h"
#include "TabIPCWindow.h"

static const ULONG g_ulCheckFrameWndTimerID = 0x500;
static const ULONG g_ulCheckFrameWndInterval = 1000;

CTabIPCWindow::~CTabIPCWindow(void)
{
}

LRESULT CTabIPCWindow::OnIPCFrameToTabMsg( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch(uMsg)
	{
	case WM_COPYDATA:
		{
			if((UINT)wParam ==  WM_IPC_F2T_CREATE_WEB_PAGE)
			{
				LPCTSTR lpszUrl = (LPCTSTR)((COPYDATASTRUCT*)lParam)->lpData;
				int nIndexInTab = 0;
				HWND hWnd = m_pTabManager->CreateWebPage(lpszUrl, &nIndexInTab);

				return (LRESULT)hWnd;			

			}
		break;
		}
		break;
	
	case WM_IPC_F2T_SHOW_WEB_PAGE:
		{
			return (LRESULT) m_pTabManager->ShowWebPage((HWND)wParam, (BOOL)lParam);
			
		}
		break;
	case WM_IPC_F2T_CMD:
		{
			HWND hWnd = (HWND)wParam;
			auto Iter = m_pTabManager->m_pWebPageList.find(hWnd);
			if((Iter == m_pTabManager->m_pWebPageList.end()) || (NULL == Iter->second))
			{
				break;
			}

			switch(lParam)
			{
			case CMD_F2T_GOBACK:
				{
					Iter->second->SendMessage(WM_WEB_PAGE_CMD, CMD_WB_GOBACK, 0);
				}
				break;
			case CMD_F2T_GOFORWARD:
				{
					Iter->second->SendMessage(WM_WEB_PAGE_CMD, CMD_WB_GOFORWARD, 0);
				}
				break;
			case CMD_F2T_REFRESH:
				{
					Iter->second->SendMessage(WM_WEB_PAGE_CMD, CMD_WB_REFRESH, 0);
				}
				break;
			default:
				break;
			}

		}
		break;
	case WM_IPC_F2T_CHANGE_WINDOW_SIZE:
		{
			HWND hWnd = (HWND)wParam;
			::SetWindowPos(hWnd, NULL, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), SWP_NOACTIVATE | SWP_NOMOVE);
			RECT TabRect;
			m_pTabManager->GetTabRect(&TabRect);
			TabRect.right = TabRect.left + GET_X_LPARAM(lParam);
			TabRect.bottom = TabRect.top + GET_Y_LPARAM(lParam);
			m_pTabManager->SetTabRect(TabRect);
		//	m_pTabManager->m_pWebPageList[hWnd]->SetWindowPos(NULL, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), SWP_NOACTIVATE | SWP_NOMOVE);
		}
		break;
	default:
		break;
	}
	
	bHandled = TRUE;
	return 1;
}

LRESULT CTabIPCWindow::OnDestroy()
{
	PostQuitMessage(0);
	return 0;

}

LRESULT CTabIPCWindow::OnCreate( LPCREATESTRUCT lpStruct )
{
	SetTimer(g_ulCheckFrameWndTimerID, g_ulCheckFrameWndInterval);
	return 0;
}

LRESULT CTabIPCWindow::OnTimer( UINT_PTR nIDEvent )
{
	switch(nIDEvent)
	{
	case g_ulCheckFrameWndTimerID:
		{
			HWND hFrameWnd = m_pTabManager->GetFrameWnd();
			if(!::IsWindow(hFrameWnd))
			{
				SendMessage(WM_CLOSE);
			}
		}
		break;
	default:
		break;
	}

	return 0;
}
