#pragma once

#include <vector>
#include <algorithm>

#include "Vector2D.h"

#include "ElectricField.h"
#include "Options.h"

class FieldLine
{
public:
	std::vector<Vector2D<double>> points;
	
	FieldLine();
	~FieldLine();

	inline void AddPoint(Vector2D<double>& pt)
	{
		if (points.empty())	points.push_back(pt);
		else {
			Vector2D<double>& old = points.back();
			double dif = (old - pt).Length() * (double)theApp.options.distanceUnitLength;

			// do not add points that are very far away
			if ((dif >= 256. && pt.Length() >= 2000 && pt.Length() < 3000) || (dif >= 64. && pt.Length() > 1000 && pt.Length() < 2000) || (dif >= 8. && pt.Length() <= 1000))
				points.push_back(pt);
		}
	}

	void Draw(CHwndRenderTarget* renderTarget, CRect& rect, bool isPotential = false) const;
	void Draw(CDC* pDC, float zoom) const;
private:
	void DrawBezierAndLine(CDC* pDC, std::vector<CPoint>& gdiPoints) const;
protected:
	static void AdjustForBezier(const Vector2D<double>& pt0, const Vector2D<double>& pt1, const Vector2D<double>& pt2, const Vector2D<double>& pt3, double& Xo1, double& Yo1, double& Xo2, double& Yo2);
public:
	void AdjustForBezier();
};


class PotentialLine : public FieldLine {
public:
	double potential;
	Vector2D<double> weightCenter;

	PotentialLine() : potential(0), weightCenter(0,0) {}
};