
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

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CElectricFieldView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	UINT_PTR timer;

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnDrawWithD2D(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

#ifndef _DEBUG  // debug version in ElectricFieldView.cpp
inline CElectricFieldDoc* CElectricFieldView::GetDocument() const
   { return reinterpret_cast<CElectricFieldDoc*>(m_pDocument); }
#endif

