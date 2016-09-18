#pragma once

#include <vector>

#include "Charge.h"
#include "FieldLine.h"

class TheElectricField
{
protected:

public:
	std::vector<Charge> charges;
	
	std::vector<FieldLine> electricFieldLines;
	std::vector<PotentialLine> potentialFieldLines;

	TheElectricField();
	~TheElectricField();

	void Draw(CHwndRenderTarget* renderTarget, CRect& rect);
	void Draw(CDC* pDC, float zoom);
	
	inline bool HitCharge(const Vector2D<double>& pos) const
	{
		for (const auto &charge : charges)
			if (charge.HitCharge(pos))	return true;

		return false;
	}


	inline double Potential(const Vector2D<double>& pos) const
	{
		double P = 0;

		for (const auto &charge : charges)
			P += charge.Potential(pos);

		return P;
	}


	inline Vector2D<double> E(const Vector2D<double>& pos) const
	{
		Vector2D<double> result;

		for (const auto &charge : charges)
			result += charge.E(pos);

		return result;
	}

	inline Vector2D<double> ENormalized(const Vector2D<double>& pos) const { return E(pos).Normalize(); };

	void Clear();
	int GetTotalCharge(bool& hasDifferentSigns) const;
};

