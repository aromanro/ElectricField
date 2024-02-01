
// ElectricFieldView.cpp : implementation of the CElectricFieldView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ElectricField.h"
#endif

#include "ElectricFieldDoc.h"
#include "ElectricFieldView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CElectricFieldView

IMPLEMENT_DYNCREATE(CElectricFieldView, CView)

BEGIN_MESSAGE_MAP(CElectricFieldView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CElectricFieldView::OnFilePrintPreview)	
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &CElectricFieldView::OnDrawWithD2D)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CElectricFieldView construction/destruction

CElectricFieldView::CElectricFieldView()
	: timer(0)
{	
	EnableD2DSupport();
}



BOOL CElectricFieldView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}




// CElectricFieldView printing


void CElectricFieldView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CElectricFieldView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CElectricFieldView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CElectricFieldView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CElectricFieldView diagnostics

#ifdef _DEBUG
void CElectricFieldView::AssertValid() const
{
	CView::AssertValid();
}

void CElectricFieldView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CElectricFieldDoc* CElectricFieldView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CElectricFieldDoc)));
	return dynamic_cast<CElectricFieldDoc*>(m_pDocument);
}
#endif //_DEBUG


// CElectricFieldView message handlers


// CElectricFieldView drawing


void CElectricFieldView::OnDraw(CDC* pDC)
{
	CView::OnDraw(pDC);
}

LRESULT CElectricFieldView::OnDrawWithD2D(WPARAM /*wParam*/, LPARAM lParam)
{
	CElectricFieldDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)	return FALSE;

	CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
	ASSERT_VALID(pRenderTarget);

	pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	CRect rect;
	GetClientRect(rect);

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(1., -1.)*D2D1::Matrix3x2F::Translation(rect.Width() / 2.0f, rect.Height() / 2.0f));

	rect.left -= rect.Width() / 2;
	rect.top -= rect.Height() / 2;
	pDoc->calculator->field.Draw(pRenderTarget,rect);

	return TRUE;
}

BOOL CElectricFieldView::OnEraseBkgnd(CDC* pDC)
{
	if (pDC->IsPrinting())
		return CView::OnEraseBkgnd(pDC);

	return TRUE;
}




void CElectricFieldView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: Add your specialized code here and/or call the base class

	//CView::OnPrint(pDC, pInfo);

	CElectricFieldDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)	return;

	int save = pDC->SaveDC();	

	pDoc->calculator->field.Draw(pDC, static_cast<float>(pDC->GetDeviceCaps(PHYSICALWIDTH) / (GetSystemMetrics(SM_CXSCREEN)/2.)));

	pDC->RestoreDC(save);
}


void CElectricFieldView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CView::OnPrepareDC(pDC, pInfo);

	if (pDC->IsPrinting())
	{
		pDC->SetMapMode(MM_ISOTROPIC);
		
		int horz = GetSystemMetrics(SM_CXSCREEN)/2;
		pDC->SetWindowExt(horz, -horz);
		pDC->SetViewportExt(horz, horz);

		pDC->SetViewportOrg(pDC->GetDeviceCaps(PHYSICALWIDTH)/2, pDC->GetDeviceCaps(PHYSICALHEIGHT) /2);
	}
}


void CElectricFieldView::OnTimer(UINT_PTR nIDEvent)
{
	CView::OnTimer(nIDEvent);

	CElectricFieldDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)	return;

	if (pDoc->GetData())
	{
		KillTimer(timer);
		timer = 0;
		
		EndWaitCursor();
		Invalidate();
	}
}


void CElectricFieldView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	timer = SetTimer(1, 100, nullptr);
	BeginWaitCursor();
}


void CElectricFieldView::OnDestroy()
{
	CView::OnDestroy();
	
	if (timer) KillTimer(timer);
}


BOOL CElectricFieldView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CElectricFieldDoc* pDoc = GetDocument();

	if (pDoc && !pDoc->CheckStatus())
	{
		RestoreWaitCursor();

		return TRUE;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}
