// OptionsPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "ElectricField.h"
#include "OptionsPropertySheet.h"


// OptionsPropertySheet

IMPLEMENT_DYNAMIC(OptionsPropertySheet, CPropertySheet)

OptionsPropertySheet::OptionsPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage), hIcon(0)
{

}

OptionsPropertySheet::OptionsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage), hIcon(0)
{

}

OptionsPropertySheet::~OptionsPropertySheet()
{
	if (hIcon) DestroyIcon(hIcon);
}


BEGIN_MESSAGE_MAP(OptionsPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// OptionsPropertySheet message handlers


BOOL OptionsPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	hIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);

	SetIcon(hIcon, FALSE);

	return bResult;
}
