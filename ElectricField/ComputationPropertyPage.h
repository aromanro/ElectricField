#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CComputationPropertyPage dialog

class ComputationPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(ComputationPropertyPage)

public:
	ComputationPropertyPage();
	virtual ~ComputationPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPUTATIONPROPERTYPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void ApplyValues();
	virtual BOOL OnApply();
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
	virtual BOOL OnInitDialog();
	CEdit m_Edit1;
	CEdit m_Edit2;
public:
	CSliderCtrl m_Slider;
	CStatic m_Static;
	afx_msg void OnTRBNThumbPosChangingSlider1(NMHDR *pNMHDR, LRESULT *pResult);
};
