// BrowserDemo.cpp : main source file for BrowserDemo.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "MainFrm.h"

#include "TabIPCWindow.h"
#include "TabManager.h"

CAppModule _Module;


BOOL GetArg(IN LPCTSTR lpszCmdLine, IN LPCTSTR lpszArgString, OUT LPTSTR lpszContent, IN int nLen)
{
	//-FrameWnd=%s -IPCWindowName=%s
	LPCTSTR lpCurArg = _tcsstr(lpszCmdLine, lpszArgString);
	if(NULL == lpCurArg)
	{
		return FALSE;
	}

	lpCurArg += _tcslen(lpszArgString);
	LPCTSTR lpNextSpace = _tcschr(lpCurArg, ' ');
	if(lpNextSpace)
	{
		if(lpNextSpace - lpCurArg > nLen)
		{
			return FALSE;
		}
		_tcsncpy(lpszContent, lpCurArg, lpNextSpace - lpCurArg);
		return TRUE;
	}
	else
	{
		_tcsncpy(lpszContent, lpCurArg, nLen);
		return TRUE;
	}
}

int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	if(_tcsstr(lpstrCmdLine, _T("-Type=Tab")) != 0)
	{
		//MessageBox(0, 0, 0, 0);
		//-Type=Tab -FrameWnd=1b4d5eff -IPCWindowName=IPCWND{dfdf}
		TCHAR szFrameWnd[MAX_PATH] = {0};
		if(!GetArg(lpstrCmdLine, ARG_FRAMEWND, szFrameWnd, MAX_PATH))
		{
			return 0;
		}

		TCHAR szIPCWindowName[MAX_PATH] = {0};
		if(!GetArg(lpstrCmdLine, ARG_IPCWNDNAME, szIPCWindowName, MAX_PATH))
		{
			return 0;
		}

		TCHAR szRectTop[20] = {0};
		GetArg(lpstrCmdLine, ARG_RECT_TOP, szRectTop, 20);
		TCHAR szRectLeft[20] = {0};
		GetArg(lpstrCmdLine, ARG_RECT_LEFT, szRectLeft, 20);
		TCHAR szRectBottom[20] = {0};
		GetArg(lpstrCmdLine, ARG_RECT_BOTTOM, szRectBottom, 20);
		TCHAR szRectRight[20] = {0};
		GetArg(lpstrCmdLine, ARG_RECT_RIGHT, szRectRight, 20);
		

		CMessageLoop theLoop;
		_Module.AddMessageLoop(&theLoop);

		CTabManager* pTabManager = CTabManager::GetInstancePtr();

		pTabManager->SetFrameWnd((HWND)_tcstol(szFrameWnd, 0, 16));
		RECT rect = {_ttoi(szRectLeft), _ttoi(szRectTop),  _ttoi(szRectRight), _ttoi(szRectBottom)};
		pTabManager->SetTabRect(rect);

		CTabIPCWindow IPCWnd(pTabManager);
		HWND hWnd = IPCWnd.Create(NULL, 0, szIPCWindowName, WS_POPUP);
		
		int nRet = theLoop.Run();

		_Module.RemoveMessageLoop();
		return nRet;
		return 0;
	}
	else
	{
		CMessageLoop theLoop;
		_Module.AddMessageLoop(&theLoop);

		CMainFrame wndMain;

		RECT rect = {0, 0, 760, 512};
		if(wndMain.Create(NULL, rect, _T("BrowserFrameWindow")) == NULL)
		{
			ATLTRACE(_T("Main window creation failed!\n"));
			return 0;
		}

		wndMain.ShowWindow(nCmdShow);

		int nRet = theLoop.Run();

		_Module.RemoveMessageLoop();
		return nRet;
	}


	
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));



	
	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
