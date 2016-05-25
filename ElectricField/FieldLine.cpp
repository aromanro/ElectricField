#include "stdafx.h"
#include "FieldLine.h"


#include "ElectricField.h"

FieldLine::FieldLine()
{
}


FieldLine::~FieldLine()
{
}


void FieldLine::Draw(CHwndRenderTarget* renderTarget, CRect& rect, bool isPotential) const
{
	if (points.empty()) return;

	// enlarge the drawing area a little
	CRect irect = rect;
	irect.InflateRect(10, 10);

	CD2DPathGeometry geometry(renderTarget);
	geometry.Create(renderTarget);
	
	ID2D1GeometrySink* sink = geometry.Open();
	if (sink)
	{
		sink->SetFillMode(D2D1_FILL_MODE_WINDING);

		D2D1_POINT_2F pt1, pt2, pt3;
		pt1.x = pt1.y = pt2.x = pt2.y = 0;
		
		bool morePoints = false;
		bool pt2Valid = false;

		bool opened = false;
		bool broken = false;
		
		for (auto it = points.begin(); it != points.end(); ++it)
		{
			pt1.x = (float)(theApp.options.distanceUnitLength * it->X);
			pt1.y = (float)(theApp.options.distanceUnitLength * it->Y);

			if (irect.PtInRect(CPoint((int)pt1.x, (int)pt1.y)))
			{
				if (!opened)
				{
					opened = true;
					sink->BeginFigure(pt1, D2D1_FIGURE_BEGIN_FILLED);
					continue;
				}
			}
			else
			{
				if (opened) {
					opened = false;
					sink->EndFigure(D2D1_FIGURE_END_OPEN);
				}
				broken = true;
				continue;
			}

			if (++it == points.end()) {
				morePoints = true;
				break;
			}

			pt2.x = (float)(theApp.options.distanceUnitLength * it->X);
			pt2.y = (float)(theApp.options.distanceUnitLength * it->Y);

			if (++it == points.end()) {
				morePoints = true;
				pt2Valid = true;
				break;
			}

			pt3.x = (float)(theApp.options.distanceUnitLength * it->X);
			pt3.y = (float)(theApp.options.distanceUnitLength * it->Y);

			sink->AddBezier(D2D1::BezierSegment(pt1, pt2, pt3));
		}

		
		if (morePoints)
		{
			sink->AddLine(pt1);
			if (pt2Valid) sink->AddLine(pt2);
		}
		

		if (opened)
		{
			if (isPotential && points.front() == points.back() && !broken) sink->EndFigure(D2D1_FIGURE_END_CLOSED);
			else sink->EndFigure(D2D1_FIGURE_END_OPEN);
		}

		sink->Close();
		sink->Release();

		CD2DSolidColorBrush *pBrush = new CD2DSolidColorBrush(renderTarget, isPotential ? theApp.options.potentialFieldLineColor : theApp.options.electricFieldLineColor);

		renderTarget->DrawGeometry(&geometry, pBrush, (float)(isPotential ? theApp.options.potentialFieldLineThickness : theApp.options.electricFieldLineThickness));

		delete pBrush;
	}
}




void FieldLine::Draw(CDC* pDC, float zoom) const
{
	if (points.empty()) return;
		
	CRect rect;
	pDC->GetBoundsRect(rect,0);
	rect.InflateRect(100, 100);

	std::vector<CPoint> gdiPoints;
	gdiPoints.reserve(points.size());
	

	for (auto& point : points)
	{
		if (!rect.PtInRect(CPoint((int)point.X, (int)point.Y)))
		{
			DrawBezierAndLine(pDC, gdiPoints);
			gdiPoints.clear();
			continue;
		}
		
		gdiPoints.push_back(CPoint((int)(zoom*point.X* theApp.options.distanceUnitLength), (int)(zoom*point.Y* theApp.options.distanceUnitLength)));
		
		if (gdiPoints.size() == 1) pDC->MoveTo(gdiPoints.back());
		else if (gdiPoints.size() == 16)
		{
			pDC->PolyBezier(gdiPoints.data(), (int)gdiPoints.size());
			gdiPoints.clear();
			gdiPoints.push_back(CPoint((int)(zoom*point.X* theApp.options.distanceUnitLength), (int)(zoom*point.Y* theApp.options.distanceUnitLength)));
		}
	}


	DrawBezierAndLine(pDC, gdiPoints);
}


void FieldLine::DrawBezierAndLine(CDC* pDC, std::vector<CPoint>& gdiPoints) const
{
	if (gdiPoints.empty()) return;

	std::vector<CPoint> gdiPointsLine;

	unsigned int l = gdiPoints.size() % 3;

	CPoint endPoint;
	if (0 == l)
	{
		CPoint point1 = gdiPoints.back();
		gdiPoints.pop_back();
		CPoint point2 = gdiPoints.back();
		gdiPoints.pop_back();

		gdiPointsLine.push_back(point2);
		gdiPointsLine.push_back(point1);
	}
	else if (2 == l)
	{
		gdiPointsLine.push_back(gdiPoints.back());
		gdiPoints.pop_back();
	}

	if (gdiPoints.size() >= 4)
		pDC->PolyBezier(gdiPoints.data(), (int)gdiPoints.size());

	if (gdiPointsLine.size())
		pDC->MoveTo(gdiPoints.back());

	for (CPoint & pt : gdiPointsLine)
		pDC->LineTo(pt);
}


void FieldLine::AdjustForBezier()
{
	for (auto it = points.begin(); it != points.end();)
	{
		Vector2D<double> &pt0 = *it;
		++it;
		if (it == points.end()) break;
		Vector2D<double> &pt1 = *it;
		++it;
		if (it == points.end()) break;
		Vector2D<double> &pt2 = *it;
		++it;
		if (it == points.end()) break;
		Vector2D<double> &pt3 = *it;

		double Xo1, Yo1, Xo2, Yo2;
		
		AdjustForBezier(pt0, pt1, pt2, pt3, Xo1, Yo1, Xo2, Yo2);

		pt1.X = Xo1;
		pt1.Y = Yo1;

		pt2.X = Xo2;
		pt2.Y = Yo2;
	}
}


void FieldLine::AdjustForBezier(const Vector2D<double>& pt0, const Vector2D<double>& pt1, const Vector2D<double>& pt2, const Vector2D<double>& pt3, double& Xo1, double& Yo1, double& Xo2, double& Yo2)
{
	double t1 = (pt1 - pt0).Length();
	double t2 = t1 + (pt2 - pt1).Length();
	double t3 = t1 + t2 + (pt3 - pt2).Length();

	t1 /= t3;
	t2 /= t3;

	double divi = 3. * t1 * t2 * (1. - t1) * (1. - t2) * (t2 - t1);

	ASSERT(abs(divi) > DBL_MIN);

	double a = t2 * (1. - t2) * (pt1.X - (1. - t1) * (1. - t1) * (1. - t1) * pt0.X - t1 * t1 * t1 * pt3.X);
	double b = t1 * (1. - t1) * (pt2.X - (1. - t2) * (1. - t2) * (1. - t2) * pt0.X - t2 * t2 * t2 * pt3.X);
	
	Xo1 = (t2 * a - t1 * b) / divi;
	Xo2 = ((1. - t1) * b - (1. - t2) * a) / divi;


	a = t2 * (1. - t2) * (pt1.Y - (1. - t1) * (1. - t1) * (1. - t1) * pt0.Y - t1 * t1 * t1 * pt3.Y);
	b = t1 * (1. - t1) * (pt2.Y - (1. - t2) * (1. - t2) * (1. - t2) * pt0.Y - t2 * t2 * t2 * pt3.Y);

	Yo1 = (t2 * a - t1 * b) / divi;
	Yo2 = ((1. - t1) * b - (1. - t2) * a) / divi;
}

