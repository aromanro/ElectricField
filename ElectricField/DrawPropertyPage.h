#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxcolorbutton.h"


// CDrawPropertyPage dialog

class DrawPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(DrawPropertyPage)

public:
	DrawPropertyPage();
	virtual ~DrawPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAWPROPERTYPAGE };
#endif

protected:
	unsigned int electricFieldLineThickness;
	unsigned int potentialFieldLineThickness;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void ApplyValues();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
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
