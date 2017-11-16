#include "stdafx.h"

#include "Charge.h"
#include "FieldLine.h"

#include "TheElectricField.h"


TheElectricField::TheElectricField()
{
}


TheElectricField::~TheElectricField()
{
}


void TheElectricField::Draw(CHwndRenderTarget* renderTarget, CRect& rect)
{	
	// draw electric field lines
	for (const auto &line : electricFieldLines)
		line.Draw(renderTarget, rect);

	// draw potential lines
	for (const auto &line : potentialFieldLines)
		line.Draw(renderTarget, rect, true);

	// draw charges
	for (const auto &charge : charges)
		charge.Draw(renderTarget, rect);
}

void TheElectricField::Draw(CDC* pDC, float zoom)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, static_cast<int>(theApp.options.electricFieldLineThickness), theApp.options.electricFieldLineColor);
	CPen *oldpen = pDC->SelectObject(&pen);

	// draw electric field lines
	for (const auto &line : electricFieldLines)
		line.Draw(pDC, zoom);

	CPen newpen;
	newpen.CreatePen(PS_SOLID, static_cast<int>(theApp.options.potentialFieldLineThickness), theApp.options.potentialFieldLineColor);
	pDC->SelectObject(&newpen);

	// draw potential lines
	for (const auto &line : potentialFieldLines)
		line.Draw(pDC, zoom);

	pDC->SelectObject(oldpen);

	// draw charges
	for (const auto &charge : charges)
		charge.Draw(pDC, zoom);
}

void TheElectricField::Clear()
{
	electricFieldLines.clear();
	potentialFieldLines.clear();
}

int TheElectricField::GetTotalCharge(bool& hasDifferentSigns) const
{
	int s = 1;
	int total_charge = 0;

	auto it = charges.begin();
	if (it->charge < 0) s = -1;

	hasDifferentSigns = false;
	for (; it != charges.end(); ++it) {
		total_charge += it->charge;
		if (s != (it->charge> 0 ? 1 : -1)) hasDifferentSigns = true;
	}

	return total_charge;
}


