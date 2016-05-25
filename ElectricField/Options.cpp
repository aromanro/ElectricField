#include "stdafx.h"

#include "TheElectricField.h"

#include "Options.h"


Options::Options()
	: distanceUnitLength(100),
	chargeRadius(25.0f),
	//drawing options
	positiveChargeColor(RGB(0, 0, 255)),
	negativeChargeColor(RGB(255, 0, 0)),
	electricFieldLineColor(RGB(0, 0, 0)),
	potentialFieldLineColor(RGB(192, 192, 192)),
	electricFieldLineThickness(1),
	potentialFieldLineThickness(1),
	//computing options
	numLinesOnUnitCharge(16),
	calculateEquipotentials(true),
	potentialInterval(0.1),
	numThreads(4),
	calculationMethod(CalculationMethod::EulerMethod)
{
}


Options::~Options()
{
}


void Options::Load()
{
	// computing options
	int res = (int)theApp.GetProfileInt(L"options", L"calculateEquipotentials", 1);
	calculateEquipotentials = (res ? true : false);

	numLinesOnUnitCharge = theApp.GetProfileInt(L"options", L"numLines", 16);
	numThreads = theApp.GetProfileInt(L"options", L"numThreads", 4);

	res = (int)theApp.GetProfileInt(L"options", L"potentialInterval", 100);
	potentialInterval = ((double)res) / 1000.;

	calculationMethod = (CalculationMethod) theApp.GetProfileInt(L"options", L"method", CalculationMethod::EulerMethod);

	//drawing options
	electricFieldLineThickness = theApp.GetProfileInt(L"options", L"elFieldLineThick", 1);
	potentialFieldLineThickness = theApp.GetProfileInt(L"options", L"potFieldLineThick", 1);

	UINT sz = 0;
	LPBYTE buf = NULL;

	if (theApp.GetProfileBinary(L"options", L"positiveChargeColor", &buf, &sz))
	{
		if (sizeof(COLORREF) == sz)	positiveChargeColor = *((COLORREF*)buf);
		delete[] buf;
	}

	if (theApp.GetProfileBinary(L"options", L"negativeChargeColor", &buf, &sz))
	{
		if (sizeof(COLORREF) == sz)	negativeChargeColor = *((COLORREF*)buf);
		delete[] buf;
	}

	if (theApp.GetProfileBinary(L"options", L"electricFieldLineColor", &buf, &sz))
	{
		if (sizeof(COLORREF) == sz)	electricFieldLineColor = *((COLORREF*)buf);
		delete[] buf;
	}

	if (theApp.GetProfileBinary(L"options", L"potentialFieldLineColor", &buf, &sz))
	{
		if (sizeof(COLORREF) == sz)	potentialFieldLineColor = *((COLORREF*)buf);
		delete[] buf;
	}
}


void Options::Save()
{
	// computing options
	theApp.WriteProfileInt(L"options", L"calculateEquipotentials", calculateEquipotentials ? 1 : 0);
	theApp.WriteProfileInt(L"options", L"numLines", (int)numLinesOnUnitCharge);
	theApp.WriteProfileInt(L"options", L"numThreads", (int)numThreads);
	theApp.WriteProfileInt(L"options", L"potentialInterval", (int)(1000 * potentialInterval));
	theApp.WriteProfileInt(L"options", L"method", calculationMethod);

	//drawing options
	theApp.WriteProfileInt(L"options", L"elFieldLineThick", (int)electricFieldLineThickness);
	theApp.WriteProfileInt(L"options", L"potFieldLineThick", (int)potentialFieldLineThickness);

	theApp.WriteProfileBinary(L"options", L"positiveChargeColor", (LPBYTE)&positiveChargeColor, sizeof(COLORREF));
	theApp.WriteProfileBinary(L"options", L"negativeChargeColor", (LPBYTE)&negativeChargeColor, sizeof(COLORREF));
	theApp.WriteProfileBinary(L"options", L"electricFieldLineColor", (LPBYTE)&electricFieldLineColor, sizeof(COLORREF));
	theApp.WriteProfileBinary(L"options", L"potentialFieldLineColor", (LPBYTE)&potentialFieldLineColor, sizeof(COLORREF));
}

