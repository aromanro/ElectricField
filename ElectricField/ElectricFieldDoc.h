
// ElectricFieldDoc.h : interface of the CElectricFieldDoc class
//


#pragma once

#include "TheElectricField.h"
#include "FieldLinesCalculator.h"

class CElectricFieldDoc : public CDocument
{
protected: // create from serialization only
	CElectricFieldDoc();
	DECLARE_DYNCREATE(CElectricFieldDoc)

	FieldLinesCalculator *calculator;

// Attributes
public:
// Operations
	bool GetData();
	bool CheckStatus();
	BOOL OnNewDocument() override;

private:
// Overrides
	void OnCloseDocument() override;
	void Serialize(CArchive& ar) override;
#ifdef SHARED_HANDLERS
	void InitializeSearchContent() override;
	void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds) override;
#endif // SHARED_HANDLERS

// Implementation
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif


// Generated message map functions
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
	void GetDataFromThreads();
};


