#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CComputationPropertyPage dialog

class ComputationPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(ComputationPropertyPage)
public:
	ComputationPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPUTATIONPROPERTYPAGE };
#endif

private:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	afx_msg void OnTRBNThumbPosChangingSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	void ApplyValues();
	BOOL OnApply() override;
	BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedCheck();
	afx_msg void OnEnChangeEdit();
	afx_msg void OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadio();

	int m_Method;
	int m_CalculateEquipotentials;
	unsigned int m_nrLines;
	unsigned int m_nrThreads;
	CSpinButtonCtrl m_Spin1;
	CSpinButtonCtrl m_Spin2;

	CEdit m_Edit1;
	CEdit m_Edit2;
	CSliderCtrl m_Slider;
	CStatic m_Static;
};
