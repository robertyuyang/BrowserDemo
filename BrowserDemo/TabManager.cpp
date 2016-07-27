#include "StdAfx.h"
#include "TabManager.h"


CTabManager::CTabManager(void) : m_pCurShowedPage(NULL)
{
	
}


CTabManager::~CTabManager(void)
{
	for(auto it = m_pWebPageList.begin();
		it != m_pWebPageList.end();
		it++)
	{
		delete it->second;
	}

	m_pWebPageList.clear();
}

class CWebPageCreateThreadParam
{
public:
	CWebPageCreateThreadParam(CWebPage* pPage, CTabManager* pThis) : m_pPage(pPage), m_pThis(pThis)
	{
		m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	~CWebPageCreateThreadParam()
	{
		if(m_hEvent)
		{
			::CloseHandle(m_hEvent);
		}
	}

	CWebPage* m_pPage;
	HANDLE m_hEvent;
	CTabManager* m_pThis;
};

HWND CTabManager::CreateWebPage( LPCTSTR lpszUrl , int* pnIndexInTab)
{
	OutputDebugString(_T("[XXX]CTabManager::CreateWebPage"));


	CWebPage* pNewPage = new CWebPage(lpszUrl);

	
	if((m_TabRect.top == 0) && (m_TabRect.bottom == 0))
	{
		::GetClientRect(m_hFrameWnd, &m_TabRect);
		m_TabRect.top += 200;
	}
	
	CWebPageCreateThreadParam PageCreateParam(pNewPage, this);
	HANDLE hThread = (HANDLE)::_beginthreadex(0, 0, WebPageThreadProc, &PageCreateParam, 0, NULL);
	if(NULL == hThread)
	{
		return NULL;
	}

	DWORD dwRet = ::WaitForSingleObject(PageCreateParam.m_hEvent, 5000);

	/*HWND hWnd = pNewPage->Create(m_hFrameWnd, m_TabRect, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	*/


	HWND hWnd = (HWND)*pNewPage;
	if(NULL == hWnd)
	{
		delete pNewPage;
		return NULL;
	}

	

	m_pWebPageList[hWnd] = pNewPage;
	if(m_pCurShowedPage)
	{
		m_pCurShowedPage->ShowWindow(SW_HIDE);
	}
	m_pCurShowedPage = pNewPage;
	if(pnIndexInTab)
	{
		*pnIndexInTab = m_pWebPageList.size() - 1;
	}


	return hWnd;
}

BOOL CTabManager::ShowWebPage( IN HWND hWnd, IN BOOL bShow )
{
	if(m_pWebPageList.find(hWnd) == m_pWebPageList.end())
	{
		return FALSE;
	}

	if(m_pCurShowedPage != m_pWebPageList[hWnd])
	{
		if(m_pCurShowedPage)
		{
			m_pCurShowedPage->ShowWindow(SW_HIDE);
		}
		m_pCurShowedPage = m_pWebPageList[hWnd];
		m_pWebPageList[hWnd]->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
	
	return TRUE;
}

unsigned int __stdcall CTabManager::WebPageThreadProc( LPVOID lpParam )
{
	CWebPageCreateThreadParam* pPageCreateParam = static_cast<CWebPageCreateThreadParam*>(lpParam);
	if(!pPageCreateParam)
	{
		return 0;
	}
	if(!pPageCreateParam->m_pPage || !pPageCreateParam->m_pThis)
	{
		if(pPageCreateParam->m_hEvent)
		{
			::SetEvent(pPageCreateParam->m_hEvent);
		}
	}

	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	HWND hWnd = pPageCreateParam->m_pPage->Create(pPageCreateParam->m_pThis->m_hFrameWnd, pPageCreateParam->m_pThis->m_TabRect, 
		NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	
	if(pPageCreateParam->m_hEvent)
	{
		::SetEvent(pPageCreateParam->m_hEvent);
	}

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();

	return nRet;
}
