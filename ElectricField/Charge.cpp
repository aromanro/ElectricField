#include "stdafx.h"


#include "Charge.h"


Charge::Charge(int value)
	: charge(value)
{	
}


Charge::~Charge()
{
}




void Charge::Draw(CHwndRenderTarget* renderTarget, CRect& /*rect*/) const
{
	// draw the charge
	D2D1_POINT_2F point;
	point.x = (float)(position.X * theApp.options.distanceUnitLength);
	point.y = (float)(position.Y * theApp.options.distanceUnitLength);

	CD2DSolidColorBrush *pBrush = new CD2DSolidColorBrush(renderTarget, charge > 0 ? theApp.options.positiveChargeColor : theApp.options.negativeChargeColor);

	CD2DSizeF size;
	size.height = theApp.options.chargeRadius;
	size.width = theApp.options.chargeRadius;
	renderTarget->FillEllipse(CD2DEllipse(point, size), pBrush);

	delete pBrush;
}



void Charge::Draw(CDC* pDC, float zoom) const
{
	CBrush brush;
	brush.CreateSolidBrush(charge > 0 ? theApp.options.positiveChargeColor : theApp.options.negativeChargeColor);
	CBrush *oldBrush = pDC->SelectObject(&brush);

	CRect rect;
	rect.top = (long)(zoom*position.Y * theApp.options.distanceUnitLength - zoom*theApp.options.chargeRadius);
	rect.left = (long)(zoom*position.X * theApp.options.distanceUnitLength - zoom*theApp.options.chargeRadius);
	rect.right = (long)(rect.left + 2.*zoom*theApp.options.chargeRadius+2.);
	rect.bottom = (long)(rect.top + 2.*zoom*theApp.options.chargeRadius+2.);

	pDC->Ellipse(rect);

	pDC->SelectObject(oldBrush);
}
