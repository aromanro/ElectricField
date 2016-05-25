
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
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CElectricFieldDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
private:
	void GetDataFromThreads();
public:
	bool GetData();
	bool CheckStatus();
	virtual void OnCloseDocument();
};


