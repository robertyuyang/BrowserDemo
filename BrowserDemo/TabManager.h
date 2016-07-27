#pragma once

#include <map>

#include "WebPage.h"


class CTabManager
{
private:
	CTabManager(void);
	~CTabManager(void);

public:
	static CTabManager* GetInstancePtr()
	{
		static CTabManager* pInstance = NULL;
		if(NULL == pInstance)
		{
			pInstance = new CTabManager();
		}
		return pInstance;
	}

	VOID SetFrameWnd(HWND hFrameWnd)
	{
		m_hFrameWnd = hFrameWnd;
	}

	HWND GetFrameWnd() const
	{
		return m_hFrameWnd;
	}

	VOID SetTabRect(const RECT& rc)
	{
		::CopyRect(&m_TabRect, &rc);
	}

	VOID GetTabRect(RECT* pRect)
	{
		if(pRect)
		{
			::CopyRect(pRect, &m_TabRect);
		}
	}

	static unsigned int __stdcall WebPageThreadProc(LPVOID lpParam);
	HWND CreateWebPage(IN LPCTSTR lpszUrl, OUT int* pnIndexInTab);
	BOOL ShowWebPage(IN HWND hWnd, IN BOOL bShow);

public:
	std::map<HWND, CWebPage*> m_pWebPageList;
private:
	HWND m_hFrameWnd;
	RECT m_TabRect;

	CWebPage* m_pCurShowedPage;
};

