#pragma once

#include <atlcrack.h>
#include "TabProcessConst.h"
#include "TabManager.h"

class CTabIPCWindow : public CWindowImpl<CTabIPCWindow>
{
public:
	DECLARE_WND_CLASS(g_szIPCWindowClassName)
public:
	BEGIN_MSG_MAP(CTabIPCWindow)
		MESSAGE_RANGE_HANDLER(WM_IPC_F2T_BEGIN, WM_IPC_F2T_END, OnIPCFrameToTabMsg)
		MESSAGE_HANDLER(WM_COPYDATA, OnIPCFrameToTabMsg)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_CREATE(OnCreate)
	END_MSG_MAP()
public:
	CTabIPCWindow(CTabManager* pTabManager) : m_pTabManager(pTabManager){}
	~CTabIPCWindow(void);

	LRESULT OnIPCFrameToTabMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnDestroy();
	LRESULT OnCreate(LPCREATESTRUCT lpStruct);
	LRESULT OnTimer(UINT_PTR nIDEvent);

private:
	
	CTabManager* m_pTabManager;
};

