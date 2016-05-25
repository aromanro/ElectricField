
// ElectricField.h : main header file for the ElectricField application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include <gdiplus.h>

#pragma comment (lib, "d2d1.lib")


#include "Options.h"

#include <vector>

// CElectricFieldApp:
// See ElectricField.cpp for the implementation of this class
//

class FieldLinesCalculator;

class CElectricFieldApp : public CWinAppEx
{
public:
	CElectricFieldApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	Options options;

	// here stay the ones that are not finished before closing the document
	std::vector<FieldLinesCalculator*> calculators;

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};


extern CElectricFieldApp theApp;
