#include "StdAfx.h"
#include "WebPageObj.h"

#include <map>

#include <comdef.h>
#include <comutil.h>

#include "TabProcessConst.h"
//
//CTabProcessObj::CTabProcessObj(void)
//{
//}
//
//
//CTabProcessObj::~CTabProcessObj(void)
//{
//}
//
//BOOL CTabProcessObj::Create( HWND hFrameWnd, const RECT& rect, HWND* phIPCTabWnd )
//{
//	TCHAR szExeFilePath[MAX_PATH] = {0};
//
//	if(::GetModuleFileName(NULL, szExeFilePath, sizeof(szExeFilePath) / sizeof(szExeFilePath[0])) == 0)
//	{
//		return FALSE;
//	}
//
//
//	TCHAR szIPCWindowName[MAX_PATH] = {0};
//	GUID uuid; 
//	if (SUCCEEDED(::CoCreateGuid(&uuid))){
//		BSTR lclsid = NULL;
//		::StringFromCLSID(uuid, &lclsid);
//		if (lclsid){
//			_bstr_t bb = lclsid;
//			_stprintf(szIPCWindowName, _T("IPCWondowName_%s"), (LPCTSTR)bb);
//			::CoTaskMemFree(lclsid);
//		}
//	}
//
//
//	TCHAR szCmd[MAX_PATH * 2] = {0};
//	TCHAR szFrameWnd[MAX_PATH] = {0};
//	_stprintf(szCmd, _T("\"%s\" -Type=Tab %s%s %s%s %s%d %s%d %s%d %s%d"), 
//		szExeFilePath, 
//		ARG_FRAMEWND, _itot((DWORD)hFrameWnd, szFrameWnd, 16),  
//		ARG_IPCWNDNAME, szIPCWindowName, 
//		ARG_RECT_TOP, rect.top,
//		ARG_RECT_LEFT, rect.left,
//		ARG_RECT_BOTTOM, rect.bottom,
//		ARG_RECT_RIGHT, rect.right
//		);
//
//	STARTUPINFO si;
//	PROCESS_INFORMATION pi;
//
//	ZeroMemory(&si, sizeof(si));
//	si.cb = sizeof(si);
//	si.wShowWindow = SW_SHOWNORMAL;
//
//
//	BOOL bRet = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
//	if(bRet)
//	{
//		::CloseHandle(pi.hThread);
//		m_hProcess = pi.hProcess;
//
//		HWND hIPCWnd = NULL;
//
//		DWORD dwStart = ::GetTickCount();
//		DWORD dwTimeOut = 0;
//#ifdef _DEBUG
//		dwTimeOut = 60000;
//#else
//		dwTimeOut = 5000;
//#endif
//		while(::GetTickCount() - dwStart < dwTimeOut)
//		{
//			hIPCWnd = ::FindWindow(g_szIPCWindowClassName, szIPCWindowName);
//			if(hIPCWnd)
//			{
//				m_hIPCWindow = hIPCWnd;
//				if(phIPCTabWnd)
//				{
//					*phIPCTabWnd = hIPCWnd;
//				}
//				return TRUE;
//			}
//			else
//			{
//				::Sleep(1000);
//			}
//		}
//
//		
//	}
//
//	return FALSE;
//}
//
//BOOL CTabProcessObj::NotifyCreateWebPage( LPCTSTR lpszUrl )
//{
//	::SendMessage(m_hIPCWindow, WM_IPC_F2T_CREATE_WEB_PAGE, (WPARAM)lpszUrl, 0);
//	return TRUE;
//}


std::map<HWND, IWebPageObj*> g_FrameWebPageObjMap;

class CWebPageObj : public IWebPageObj
{
public:
	CWebPageObj(HWND hIPCTabWnd):m_hIPCTabWnd(hIPCTabWnd)
	{

	}
	virtual BOOL Create(LPCTSTR lpszUrl)
	{
		COPYDATASTRUCT lpcs;
		lpcs.dwData = (ULONG_PTR)0;
		lpcs.cbData = (_tcslen(lpszUrl) + 1) * sizeof(lpszUrl[0]);
		lpcs.lpData = (PVOID)lpszUrl;
		LRESULT lRet = ::SendMessage(m_hIPCTabWnd, WM_COPYDATA, WM_IPC_F2T_CREATE_WEB_PAGE, (LPARAM)&lpcs);
		if((HWND)lRet == 0)
		{
			return FALSE;
		}
		else
		{
			m_hWebPageWnd = (HWND)lRet;
			g_FrameWebPageObjMap[m_hWebPageWnd] = (IWebPageObj*)this;
			return TRUE;
		}
	}
	virtual BOOL Show(BOOL bShow)
	{
		LRESULT lRet = ::SendMessage(m_hIPCTabWnd, WM_IPC_F2T_SHOW_WEB_PAGE, (WPARAM)m_hWebPageWnd, (LPARAM)TRUE);
		return lRet;
		
	}
	virtual BOOL Remove()
	{
		//LRESULT lRet = ::SendMessage(m_hIPCTabWnd, WM_IPC_F2T_SHOW_WEB_PAGE, (WPARAM)m_hWnd, (LPARAM)TRUE);
		g_FrameWebPageObjMap.erase(m_hWebPageWnd);
		return FALSE;
	}
	virtual WebPageObjId GetId() const
	{
		return m_hWebPageWnd;
	}
	virtual VOID GoBack()
	{
		::SendMessage(m_hIPCTabWnd, WM_IPC_F2T_CMD, (WPARAM)m_hWebPageWnd, (LPARAM)CMD_F2T_GOBACK);
	}
	virtual VOID GoForward()
	{
		::SendMessage(m_hIPCTabWnd, WM_IPC_F2T_CMD, (WPARAM)m_hWebPageWnd, (LPARAM)CMD_F2T_GOFORWARD);
	}
	virtual VOID Refresh()
	{
		::SendMessage(m_hIPCTabWnd, WM_IPC_F2T_CMD, (WPARAM)m_hWebPageWnd, (LPARAM)CMD_F2T_REFRESH);
	}
	virtual VOID ChangeWindowSize(int nWidth, int nHeight)
	{
		::PostMessage(m_hIPCTabWnd, WM_IPC_F2T_CHANGE_WINDOW_SIZE, (WPARAM)m_hWebPageWnd, MAKELPARAM(nWidth, nHeight));
	}
private:
	HWND m_hWebPageWnd;
	HWND m_hIPCTabWnd;
	//int m_nIndexInTab;
};




BOOL CWebPageObjCreater::CreateTabProcess( HWND hFrameWnd, const RECT& rect, HWND* phIPCTabWnd )
{
	TCHAR szExeFilePath[MAX_PATH] = {0};

	if(::GetModuleFileName(NULL, szExeFilePath, sizeof(szExeFilePath) / sizeof(szExeFilePath[0])) == 0)
	{
		return FALSE;
	}


	TCHAR szIPCWindowName[MAX_PATH] = {0};
	GUID uuid; 
	if (SUCCEEDED(::CoCreateGuid(&uuid))){
		BSTR lclsid = NULL;
		::StringFromCLSID(uuid, &lclsid);
		if (lclsid){
			_bstr_t bb = lclsid;
			_stprintf(szIPCWindowName, _T("IPCWondowName_%s"), (LPCTSTR)bb);
			::CoTaskMemFree(lclsid);
		}
	}


	TCHAR szCmd[MAX_PATH * 2] = {0};
	TCHAR szFrameWnd[MAX_PATH] = {0};
	_stprintf(szCmd, _T("\"%s\" -Type=Tab %s%s %s%s %s%d %s%d %s%d %s%d"), 
		szExeFilePath, 
		ARG_FRAMEWND, _itot((DWORD)hFrameWnd, szFrameWnd, 16),  
		ARG_IPCWNDNAME, szIPCWindowName, 
		ARG_RECT_TOP, rect.top,
		ARG_RECT_LEFT, rect.left,
		ARG_RECT_BOTTOM, rect.bottom,
		ARG_RECT_RIGHT, rect.right
		);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_SHOWNORMAL;


	TAB_PROC_INFO NewInfo;
	
	BOOL bRet = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bRet)
	{
		::CloseHandle(pi.hThread);
		NewInfo.hProcess = pi.hProcess;

		HWND hIPCWnd = NULL;

		DWORD dwStart = ::GetTickCount();
		DWORD dwTimeOut = 0;
#ifdef _DEBUG
		dwTimeOut = 60000;
#else
		dwTimeOut = 5000;
#endif
		while(::GetTickCount() - dwStart < dwTimeOut)
		{
			hIPCWnd = ::FindWindow(g_szIPCWindowClassName, szIPCWindowName);
			if(hIPCWnd)
			{
				NewInfo.hIPCTabWnd = hIPCWnd;
				if(phIPCTabWnd)
				{
					*phIPCTabWnd = hIPCWnd;
				}
				m_TabProcInfoList.push_back(NewInfo);
				return TRUE;
			}
			else
			{
				::Sleep(1000);
			}
		}


	}

	return FALSE;
}


IWebPageObj* CWebPageObjCreater::CreateObj( LPCTSTR lpszUrl )
{
	HWND hAvailableProcIPCWnd = NULL;
	
	hAvailableProcIPCWnd = GetAvailableProcIPCWnd();
	if(!hAvailableProcIPCWnd)
	{
		CreateTabProcess(m_hFrameWnd, m_TabRect, &hAvailableProcIPCWnd);
	}
	if(!hAvailableProcIPCWnd)
	{
		return NULL;
	}

	CWebPageObj* pNewObj = new CWebPageObj(hAvailableProcIPCWnd);
	if(pNewObj->Create(lpszUrl))
	{
		return pNewObj;
	}
	else
	{
		delete pNewObj;
		return NULL;
	}
	
}

HWND CWebPageObjCreater::GetAvailableProcIPCWnd() const
{
	if(m_TabProcInfoList.size() > 0)
	{
		return m_TabProcInfoList.begin()->hIPCTabWnd;
	}
	else
	{
		return NULL;
	}
}


IWebPageObj* IWebPageObj::FindObj( HWND hWnd )
{
	auto it = g_FrameWebPageObjMap.find(hWnd);
	if(it == g_FrameWebPageObjMap.end())
	{
		return NULL;
	}
	else
	{
		return it->second;
	}
}
