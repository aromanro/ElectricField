// DrawPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "ElectricField.h"
#include "DrawPropertyPage.h"
#include "afxdialogex.h"

#include "MainFrm.h"


// CDrawPropertyPage dialog

IMPLEMENT_DYNAMIC(DrawPropertyPage, CMFCPropertyPage)

DrawPropertyPage::DrawPropertyPage()
	: CMFCPropertyPage(IDD_DRAWPROPERTYPAGE)
{
	electricFieldLineThickness = theApp.options.electricFieldLineThickness;
	potentialFieldLineThickness = theApp.options.potentialFieldLineThickness;
}

DrawPropertyPage::~DrawPropertyPage()
{
}

void DrawPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_SPIN1, m_Spin1);
	DDX_Control(pDX, IDC_SPIN2, m_Spin2);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_Color1);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, m_Color2);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON3, m_Color3);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON4, m_Color4);
	DDX_Text(pDX, IDC_EDIT1, electricFieldLineThickness);
	DDX_Text(pDX, IDC_EDIT2, potentialFieldLineThickness);

	DDV_MinMaxUInt(pDX, electricFieldLineThickness, 1, 64);
	DDV_MinMaxUInt(pDX, electricFieldLineThickness, 1, 64);
}


BEGIN_MESSAGE_MAP(DrawPropertyPage, CMFCPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &DrawPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT2, &DrawPropertyPage::OnEnChangeEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &DrawPropertyPage::OnDeltaposSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &DrawPropertyPage::OnDeltaposSpin)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &DrawPropertyPage::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON2, &DrawPropertyPage::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON3, &DrawPropertyPage::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON4, &DrawPropertyPage::OnBnClickedMfccolorbutton)
END_MESSAGE_MAP()


// CDrawPropertyPage message handlers


void DrawPropertyPage::ApplyValues()
{
	theApp.options.electricFieldLineThickness = electricFieldLineThickness;
	theApp.options.potentialFieldLineThickness = potentialFieldLineThickness;

	theApp.options.positiveChargeColor = m_Color1.GetColor();
	theApp.options.negativeChargeColor = m_Color2.GetColor();
	theApp.options.electricFieldLineColor = m_Color3.GetColor();
	theApp.options.potentialFieldLineColor = m_Color4.GetColor();

	theApp.options.Save();

	POSITION posDocTemplate = theApp.GetFirstDocTemplatePosition();
	while (posDocTemplate)
	{
		CDocTemplate* pDocTemplate = theApp.GetNextDocTemplate(posDocTemplate);

		// get each document open in given document template
		POSITION posDoc = pDocTemplate->GetFirstDocPosition();
		while (posDoc)
		{
			CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);
			
			if (pDoc) pDoc->UpdateAllViews(NULL);
		}
	}

}


BOOL DrawPropertyPage::OnApply()
{
	UpdateData();

	ApplyValues();

	return CMFCPropertyPage::OnApply();
}


BOOL DrawPropertyPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	m_Spin1.SetRange(1, 64);
	m_Spin2.SetRange(1, 64);
	m_Edit1.SetLimitText(2);
	m_Edit2.SetLimitText(2);


	m_Color1.EnableOtherButton(_T("More Colors..."));
	m_Color1.SetColor(theApp.options.positiveChargeColor);

	m_Color2.EnableOtherButton(_T("More Colors..."));
	m_Color2.SetColor(theApp.options.negativeChargeColor);

	m_Color3.EnableOtherButton(_T("More Colors..."));
	m_Color3.SetColor(theApp.options.electricFieldLineColor);

	m_Color4.EnableOtherButton(_T("More Colors..."));
	m_Color4.SetColor(theApp.options.potentialFieldLineColor);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void DrawPropertyPage::OnEnChangeEdit()
{
	SetModified();
}


void DrawPropertyPage::OnDeltaposSpin(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
	*pResult = 0;

	SetModified();
}



void DrawPropertyPage::OnBnClickedMfccolorbutton()
{
	SetModified();
}

