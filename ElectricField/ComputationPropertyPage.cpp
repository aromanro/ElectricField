// ComputationPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "ElectricField.h"
#include "ComputationPropertyPage.h"
#include "afxdialogex.h"


// CComputationPropertyPage dialog

IMPLEMENT_DYNAMIC(ComputationPropertyPage, CMFCPropertyPage)

ComputationPropertyPage::ComputationPropertyPage()
	: CMFCPropertyPage(IDD_COMPUTATIONPROPERTYPAGE)	
{
	m_Method = theApp.options.calculationMethod;
	m_CalculateEquipotentials = (theApp.options.calculateEquipotentials ? BST_CHECKED : BST_UNCHECKED);
	m_nrLines = theApp.options.numLinesOnUnitCharge;
	m_nrThreads = theApp.options.numThreads;
}

ComputationPropertyPage::~ComputationPropertyPage()
{
}

void ComputationPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK1, m_CalculateEquipotentials);
	DDX_Radio(pDX, IDC_RADIO1, m_Method);
	DDX_Text(pDX, IDC_EDIT1, m_nrLines);
	DDX_Text(pDX, IDC_EDIT2, m_nrThreads);
	DDX_Control(pDX, IDC_SPIN1, m_Spin1);
	DDX_Control(pDX, IDC_SPIN2, m_Spin2);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);

	DDV_MinMaxUInt(pDX, m_nrLines, 4, 128);
	DDV_MinMaxUInt(pDX, m_nrThreads, 1, 128);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
	DDX_Control(pDX, IDC_STATIC1, m_Static);
}


BEGIN_MESSAGE_MAP(ComputationPropertyPage, CMFCPropertyPage)
	ON_BN_CLICKED(IDC_CHECK1, &ComputationPropertyPage::OnBnClickedCheck)
	ON_EN_CHANGE(IDC_EDIT1, &ComputationPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT2, &ComputationPropertyPage::OnEnChangeEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &ComputationPropertyPage::OnDeltaposSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &ComputationPropertyPage::OnDeltaposSpin)
	ON_BN_CLICKED(IDC_RADIO1, &ComputationPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO2, &ComputationPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO3, &ComputationPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO4, &ComputationPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO5, &ComputationPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO6, &ComputationPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO7, &ComputationPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO8, &ComputationPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO9, &ComputationPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO10, &ComputationPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO11, &ComputationPropertyPage::OnBnClickedRadio)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER1, &ComputationPropertyPage::OnTRBNThumbPosChangingSlider1)
END_MESSAGE_MAP()


// CComputationPropertyPage message handlers


void ComputationPropertyPage::ApplyValues()
{
	theApp.options.calculationMethod = (Options::CalculationMethod)m_Method;
	theApp.options.calculateEquipotentials = (m_CalculateEquipotentials == BST_CHECKED ? true : false);
	theApp.options.numLinesOnUnitCharge = m_nrLines;
	theApp.options.numThreads = m_nrThreads;

	theApp.options.potentialInterval = ((double)m_Slider.GetPos())/1000.;

	theApp.options.Save();
}


BOOL ComputationPropertyPage::OnApply()
{
	UpdateData();

	ApplyValues();

	return CMFCPropertyPage::OnApply();
}


void ComputationPropertyPage::OnBnClickedCheck()
{
	SetModified();
}


void ComputationPropertyPage::OnEnChangeEdit()
{
	SetModified();
}

void ComputationPropertyPage::OnDeltaposSpin(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
	*pResult = 0;

	SetModified();
}

void ComputationPropertyPage::OnBnClickedRadio()
{
	SetModified();
}

BOOL ComputationPropertyPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Spin1.SetRange(4, 128);
	m_Spin2.SetRange(1, 128);
	m_Edit1.SetLimitText(3);
	m_Edit2.SetLimitText(3);

	//0.025
	m_Slider.SetRange(10,500);
	m_Slider.SetPos((int)(theApp.options.potentialInterval * 1000.));

	CString str;
	str.Format(L"%.2f", theApp.options.potentialInterval);
	m_Static.SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}



void ComputationPropertyPage::OnTRBNThumbPosChangingSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CString str;
	str.Format(L"%.2f", ((double)pNMTPC->dwPos)/1000.);
	m_Static.SetWindowText(str);

	SetModified();
}
