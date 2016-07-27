#include "StdAfx.h"
#include "FrameManager.h"

#include "TabProcessConst.h"

static const int g_nWebPageTop = 200;

CFrameManager::~CFrameManager(void)
{
	/*for(auto it = m_pTabProcessObjList.begin(); 
		it != m_pTabProcessObjList.end();
		it++)
	{
		delete *it;
	}



	m_pTabProcessObjList.clear();*/

	for(auto it = m_pWebPageObjList.begin(); 
		it != m_pWebPageObjList.end();
		it++)
	{
		delete *it;
	}

	m_pWebPageObjList.clear();
}



BOOL CFrameManager::CreateTab( LPCTSTR lpszUrl )
{

	if(!m_WebPageObjCreater.IsInited())
	{
		RECT rect;
		::GetClientRect(m_hFrameWnd, &rect);

		rect.top += g_nWebPageTop;

		m_WebPageObjCreater.Init(m_hFrameWnd, rect);
	}

	IWebPageObj* pNewObj = m_WebPageObjCreater.CreateObj(lpszUrl);

	if(pNewObj)
	{
		m_pWebPageObjList.push_back(pNewObj);
		m_pCurWebPageObj = pNewObj;

	}
	
	return TRUE;


	/*

	HANDLE hThread = (HANDLE)_beginthreadex(NULL, NULL, TabProcessCreateThreadProc, this, 0, NULL);
	if(hThread > 0)
	{
	::CloseHandle(hThread);
	return TRUE;
	}
	else
	{
	return FALSE;
	}*/

}
//
//unsigned int __stdcall CFrameManager::TabProcessCreateThreadProc( LPVOID lpParam )
//{
//	CFrameManager* pThis = static_cast<CFrameManager*>(lpParam);
//	if(!::IsWindow(pThis->m_hFrameWnd))
//	{
//		return FALSE;
//	}
//
//	CTabProcessObj* pObj = new CTabProcessObj();
//
//	RECT rect;
//	::GetClientRect(pThis->m_hFrameWnd, &rect);
//
//	rect.top += g_nWebPageTop;
//
//	HWND hIPCTabWnd = NULL;
//	if(pObj->Create(pThis->m_hFrameWnd, rect, &hIPCTabWnd))
//	{
//		::SendMessage(hIPCTabWnd, WM_IPC_F2T_CREATE_WEB_PAGE, (WPARAM)_T("www.baidu.com"), 0);
//		::SendMessage(hIPCTabWnd, WM_IPC_F2T_CREATE_WEB_PAGE, (WPARAM)_T("www.qq.com"), 0);
//		::SendMessage(hIPCTabWnd, WM_IPC_F2T_CREATE_WEB_PAGE, (WPARAM)_T("www.sogou.com"), 0);
//		
//		
//	//	pObj->NotifyCreateWebPage(_T("www.baidu.com"));
//	//	pObj->NotifyCreateWebPage(_T("www.qq.com"));
//	//	pThis->m_pTabProcessObjList.push_back(pObj);
//		
//		
//	}
//	else
//	{
//		delete pObj;
//	}
//
//	
//
//	return 0;
//}


//
//VOID CFrameManager::OnTabCreated( HWND hIPCTabWnd, int nIndexInTab )
//{
//	CWebPageObj* pObj = new CWebPageObj();
//	pObj->m_hIPCTabWnd = hIPCTabWnd;
//	pObj->m_nIndexInTab = nIndexInTab;
//	m_pWebPageObjList.push_back(pObj);
//
//}

BOOL CFrameManager::ShowTab( int nIndex , BOOL bShow/* = TRUE*/)
{
	if(nIndex >= m_pWebPageObjList.size())
	{
		return FALSE;
	}
	if(m_pWebPageObjList[nIndex] == NULL)
	{
		return FALSE;
	}

	m_pWebPageObjList[nIndex]->Show(bShow);
	m_pCurWebPageObj = m_pWebPageObjList[nIndex];
	
	return TRUE;
}

int CFrameManager::FindIndex( WebPageObjId hWebPageWnd )
{
	int nIndex = 0;
	IWebPageObj* pObj = IWebPageObj::FindObj(hWebPageWnd);
	for(auto it = m_pWebPageObjList.begin();
		it != m_pWebPageObjList.end();
		it++, nIndex++)
	{
		if((*it)->GetId() == hWebPageWnd)
		{
			return nIndex;
		}
	}
	return -1;
}

