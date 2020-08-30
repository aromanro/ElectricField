#pragma once

#include <vector>

#include "Vector2D.h"

#include "ElectricField.h"
#include "Options.h"

class Charge
{
public:
	int charge;

	Vector2D<double> position;

	Charge(int value = 1);
	Charge(const Charge& c)
		: charge(c.charge), position(c.position)
	{
	}

	~Charge();

	Charge& operator=(const Charge& c)
	{
		charge = c.charge;
		position = c.position;

		return *this;
	}
	
	void Draw(CHwndRenderTarget* renderTarget, CRect& rect) const;
	void Draw(CDC* pDC, float zoom) const;

	inline bool HitCharge(const Vector2D<double>& pos) const
	{
		if ((position - pos).Length() < theApp.options.chargeRadius / 1.5 / static_cast<double>(theApp.options.distanceUnitLength)) return true;

		return false;
	}


	inline double Potential(const Vector2D<double>& pos) const
	{
		return charge / (pos - position).Length();
	}


	inline Vector2D<double> E(const Vector2D<double>& pos) const
	{
		const Vector2D<double> toPos = pos - position;
		const double len2 = toPos * toPos;
		Vector2D<double> result = toPos / (len2 * sqrt(len2));

		result *= charge;

		return result;
	}
};

