
// ChildFrm.h : interface of the CChildFrame class
//

#pragma once

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
// Attributes
// Operations
// Overrides
	
// Implementation
private:
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

// Generated message map functions
	DECLARE_MESSAGE_MAP()
};
