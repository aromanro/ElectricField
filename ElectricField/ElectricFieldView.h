
// ElectricFieldView.h : interface of the CElectricFieldView class
//

#pragma once


class CElectricFieldView : public CView
{
protected: // create from serialization only
	CElectricFieldView();
	DECLARE_DYNCREATE(CElectricFieldView)

// Attributes
public:
	CElectricFieldDoc* GetDocument() const;

private:
// Operations
// Overrides
	void OnDraw(CDC* pDC) override;  // overridden to draw this view
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;


	BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

// Implementation
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

	UINT_PTR timer;

// Generated message map functions
	afx_msg void OnFilePrintPreview();

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnDrawWithD2D(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void OnPrint(CDC* pDC, CPrintInfo* pInfo) override;
	void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = nullptr) override;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void OnInitialUpdate() override;
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

#ifndef _DEBUG  // debug version in ElectricFieldView.cpp
inline CElectricFieldDoc* CElectricFieldView::GetDocument() const
   { return reinterpret_cast<CElectricFieldDoc*>(m_pDocument); }
#endif

