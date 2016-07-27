#pragma once

#include <list>
#include <vector>

#include "WebPageObj.h"


class CFrameManager
{
public:
	CFrameManager(HWND hFameWnd) : m_hFrameWnd(hFameWnd), m_pCurWebPageObj(NULL){}
	~CFrameManager(void);

	
//	static unsigned int __stdcall TabProcessCreateThreadProc(LPVOID lpParam);

	BOOL CreateTab(LPCTSTR lpszUrl);
	//VOID OnTabCreated(HWND hIPCTabWnd, int nIndexInTab);
	BOOL ShowTab(int nIndex, BOOL bShow = TRUE);
	int FindIndex(WebPageObjId hWebPageWnd); 
	
public:
	std::vector<IWebPageObj*> m_pWebPageObjList;
	IWebPageObj* m_pCurWebPageObj;
	
private:
	HWND m_hFrameWnd;
	CWebPageObjCreater m_WebPageObjCreater;
};

