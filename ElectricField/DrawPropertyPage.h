#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxcolorbutton.h"


// CDrawPropertyPage dialog

class DrawPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(DrawPropertyPage)

public:
	DrawPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAWPROPERTYPAGE };
#endif

private:
	unsigned int electricFieldLineThickness;
	unsigned int potentialFieldLineThickness;

	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void ApplyValues();
	BOOL OnApply() override;
	BOOL OnInitDialog() override;
	afx_msg void OnEnChangeEdit();
	afx_msg void OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMfccolorbutton();

	CEdit m_Edit1;
	CEdit m_Edit2;
	CSpinButtonCtrl m_Spin1;
	CSpinButtonCtrl m_Spin2;
	CMFCColorButton m_Color1;
	CMFCColorButton m_Color2;
	CMFCColorButton m_Color3;
	CMFCColorButton m_Color4;
};
