#pragma once

class Options
{
public:
	enum class CalculationMethod {
		EulerMethod = 0,
		MidpointMethod,
		RalstonMethod,
		HeunMethod,
		RK4Method,
		RK3per8Method,
		AdaptiveHeunEulerMethod,
		AdaptiveBogackiShampineMethod,
		AdaptiveCashKarpMethod,
		AdaptiveFehlbergMethod,
		AdaptiveDormandPrinceMethod
	};

	// those are internal options

	unsigned int distanceUnitLength;
	float chargeRadius;

	// computation options

	unsigned int numLinesOnUnitCharge;

	bool calculateEquipotentials;
	double potentialInterval;

	unsigned int numThreads;
	CalculationMethod calculationMethod;

	// drawing options

	COLORREF positiveChargeColor;
	COLORREF negativeChargeColor;
	COLORREF electricFieldLineColor;
	COLORREF potentialFieldLineColor;

	unsigned int electricFieldLineThickness;
	unsigned int potentialFieldLineThickness;


	Options();
	~Options();
	void Load();
	void Save();
};

