
// ElectricFieldDoc.cpp : implementation of the CElectricFieldDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ElectricField.h"
#endif

#include "ElectricFieldDoc.h"

#include <propkey.h>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CElectricFieldDoc

IMPLEMENT_DYNCREATE(CElectricFieldDoc, CDocument)

BEGIN_MESSAGE_MAP(CElectricFieldDoc, CDocument)
END_MESSAGE_MAP()


// CElectricFieldDoc construction/destruction

CElectricFieldDoc::CElectricFieldDoc()
{
	calculator = new FieldLinesCalculator();
}

CElectricFieldDoc::~CElectricFieldDoc()
{
}

BOOL CElectricFieldDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}




// CElectricFieldDoc serialization

void CElectricFieldDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CElectricFieldDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CElectricFieldDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CElectricFieldDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CElectricFieldDoc diagnostics

#ifdef _DEBUG
void CElectricFieldDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CElectricFieldDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CElectricFieldDoc commands


bool CElectricFieldDoc::GetData()
{
	if (!calculator->CheckStatus()) return false;

	GetDataFromThreads();

	std::thread thread1 = std::thread([calc = calculator] {	for (auto& line : calc->field.electricFieldLines) line.AdjustForBezier(); });
	std::thread thread2 = std::thread([calc = calculator] {	for (auto& line : calc->field.potentialFieldLines) line.AdjustForBezier(); });

	thread1.join();
	thread2.join();

	return true;
}


void CElectricFieldDoc::GetDataFromThreads()
{
	calculator->field.electricFieldLines.swap(calculator->electricFieldLines);

	if (calculator->potentialFieldLines.empty()) return;

	bool has_different_signs;
	calculator->field.GetTotalCharge(has_different_signs);

	// in this case there cannot be duplicates
	if (calculator->field.charges.size() == 2 && has_different_signs)
	{
		calculator->field.potentialFieldLines.swap(calculator->potentialFieldLines);
		return;
	}

	// sort lines by potential
	std::sort(calculator->potentialFieldLines.begin(), calculator->potentialFieldLines.end(),
		[](const auto &a, const auto &b) -> bool {
		return a.potential < b.potential;
	}
	);

	// a very ugly method of getting rid of duplicates
	// seems to work, anyway, at least in most cases

	double potential = 0;
	std::vector<PotentialLine> equipotentials;

	// pick a point with no symmetric arrangement of charges around it
	Vector2D<double> ref(-7., 5.5);

	for (const auto &line : calculator->potentialFieldLines)
	{
		// all lines that are in an interval smaller than a third of the potential interval
		// are considered to be at the same potential
		if (abs(potential - line.potential) < calculator->getPotentialInterval() / 3. || 0 == potential)
		{
			potential = line.potential;
			equipotentials.push_back(line);
		}
		else
		{
			// now that all lines at a certain potential are in 'equipotentials', sort them by the distance to the reference point
			std::sort(equipotentials.begin(), equipotentials.end(),
				[&ref](const auto &a, const auto &b) -> bool {
				return (ref - a.weightCenter).Length() < (ref - b.weightCenter).Length();
			}
			);

			for (const auto &equiline : equipotentials)
			{
				if (calculator->field.potentialFieldLines.size())
				{
					// if their weight centers are more than 2*chargeRadius far apart, they are considered different field lines, not duplicate of the same one
					if ((calculator->field.potentialFieldLines.back().weightCenter - equiline.weightCenter).Length() * theApp.options.distanceUnitLength > 2. * theApp.options.chargeRadius
						|| abs(potential - calculator->field.potentialFieldLines.back().potential) > calculator->getPotentialInterval() * 2. / 3.)
						calculator->field.potentialFieldLines.push_back(equiline);
				}
				else
					calculator->field.potentialFieldLines.push_back(equiline);
			}

			equipotentials.clear();

			potential = line.potential;
			equipotentials.push_back(line);
		}
	}

	calculator->field.potentialFieldLines.insert(calculator->field.potentialFieldLines.end(), equipotentials.begin(), equipotentials.end());

	calculator->potentialFieldLines.clear();
}


bool CElectricFieldDoc::CheckStatus()
{
	return calculator->CheckStatus();
}


void CElectricFieldDoc::OnCloseDocument()
{
	if (calculator->CheckStatus())
	{
		delete calculator;
		calculator = NULL;
	}
	else
	{
		calculator->Terminate = true;
		theApp.calculators.push_back(calculator);
	}

	CDocument::OnCloseDocument();
}
