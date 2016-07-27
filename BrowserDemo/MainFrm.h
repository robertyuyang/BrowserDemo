// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlframe.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlmisc.h>

#include "resource.h"
#include "AboutDlg.h"

#include "TabProcessConst.h"

#define CTRL_ID_TABCTRL				0x200
#define CTRL_ID_BTN_GOBACK			0x201
#define CTRL_ID_BTN_GOFORWARD		0x202
#define CTRL_ID_BTN_REFRESTH		0x203
#define CTRL_ID_EDIT_ADDRESS		0x204



class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(_T("BrowserFrameWindowClass"), IDR_MAINFRAME)

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_SIZE(OnSize)
		COMMAND_RANGE_HANDLER(CTRL_ID_BTN_GOBACK, CTRL_ID_BTN_REFRESTH, OnFuncBtnsCmd)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		MESSAGE_RANGE_HANDLER(WM_IPC_T2F_BEGIN, WM_IPC_T2F_END, OnIPCTabToFrameMsg)
		MESSAGE_HANDLER(WM_COPYDATA, OnIPCTabToFrameMsg)
		NOTIFY_HANDLER(CTRL_ID_TABCTRL, TCN_SELCHANGE, OnTabCtrlSelChanged)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(LPCREATESTRUCT lpStruct);
	LRESULT OnDestroy();
	LRESULT OnSize(UINT wParam, _WTYPES_NS::CSize NewSize);

	LRESULT OnIPCTabToFrameMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnTabCtrlSelChanged(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);



	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: add code to initialize document

		return 0;
	}

	LRESULT OnFuncBtnsCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

protected:
	VOID OnAddressGo();




private:
	WTL::CTabCtrl m_wndTabCtrl;
	WTL::CButton m_wndBackBtn;
	WTL::CButton m_wndForwardBtn;
	WTL::CButton m_wndRefreshBtn;
	WTL::CEdit m_wndAddressEdit;
};
