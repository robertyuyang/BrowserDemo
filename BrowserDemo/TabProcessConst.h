#pragma once
#ifndef __TAB_PROCESS_CONST_H_
#define __TAB_PROCESS_CONST_H_

static const TCHAR* g_szIPCWindowClassName = _T("ClassName_Wisdom_TabProcessIPCWindow");

#define WM_IPC_F2T_BEGIN					WM_USER + 0x200
#define WM_IPC_F2T_CREATE_WEB_PAGE			WM_USER + 0x200
#define WM_IPC_F2T_SHOW_WEB_PAGE			WM_USER + 0x201
#define WM_IPC_F2T_CMD						WM_USER + 0x202
#define WM_IPC_F2T_CHANGE_WINDOW_SIZE			WM_USER + 0x203



#define WM_IPC_F2T_END						WM_USER + 0x2ff

#define CMD_F2T_GOBACK						0x1
#define CMD_F2T_GOFORWARD					0x2
#define CMD_F2T_REFRESH						0x3


#define WM_IPC_T2F_BEGIN					WM_USER + 0x300
//#define WM_IPC_T2F_CREATE_WEB_PAGE_RESULT	WM_USER + 0x300
//#define WM_IPC_T2F_SHOW_WEB_PAGE_RESULT		WM_USER + 0x301
#define WM_IPC_T2F_SET_TAB_CTRL_TITLE		WM_USER + 0x310
#define WM_IPC_T2F_END WM_USER + 0x3ff

#define ARG_FRAMEWND			_T("-FrameWnd=")
#define ARG_IPCWNDNAME			_T("-IPCWindowName=")
#define ARG_RECT_TOP			_T("-RectTop=")
#define ARG_RECT_LEFT			_T("-RectLeft=")
#define ARG_RECT_BOTTOM			_T("-RectBottom=")
#define ARG_RECT_RIGHT			_T("-RectRight=")

#endif



