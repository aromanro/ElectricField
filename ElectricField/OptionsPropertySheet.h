#pragma once



// OptionsPropertySheet

class OptionsPropertySheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(OptionsPropertySheet)

public:
	OptionsPropertySheet(UINT nIDCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	OptionsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	~OptionsPropertySheet() override;

private:
	HICON hIcon;

	DECLARE_MESSAGE_MAP()

	BOOL OnInitDialog() override;
};


