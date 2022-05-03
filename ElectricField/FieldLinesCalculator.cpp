#include "stdafx.h"
#include "FieldLinesCalculator.h"

#include "PrecisionTimer.h"

#include "Singleton.h"


const double M_PI = 3.1415926535897932384626433832795028841971693993751;


FieldLinesCalculator::FieldLinesCalculator()
	: postedJobs(0), finishedJobs(0), potentialInterval(0), Terminate(false), calcMethod(Options::CalculationMethod::EulerMethod)
{
}



void FieldLinesCalculator::Clear()
{
	electricFieldLines.clear();
	potentialFieldLines.clear();
}


void FieldLinesCalculator::StartCalculating(const TheElectricField *theField)
{
	Clear();

	if (NULL == theField) return;

	potentialInterval = theApp.options.potentialInterval;
	calcMethod = theApp.options.calculationMethod;

	bool has_different_signs;
	const int total_charge = theField->GetTotalCharge(has_different_signs);
	const double r = theApp.options.chargeRadius / theApp.options.distanceUnitLength;
	
	Vector2D<double> point;

	double angle_start = 0;
	for (const auto &charge : theField->charges) {
		if (charge.charge == 0) continue;

		const double angle_step = 2.*M_PI / (fabs(charge.charge)*theApp.options.numLinesOnUnitCharge);

		angle_start = - angle_step / 2. - M_PI;
		if (sign(total_charge) != sign(charge.charge))	angle_start += M_PI + angle_step;

		for (double angle = angle_start; angle < 2.*M_PI + angle_start - angle_step / 4.; angle += angle_step) {
			if ((angle != angle_start || !theApp.options.calculateEquipotentials) && sign(total_charge) != sign(charge.charge)) break;

			point.X = charge.position.X + r * cos(angle);
			point.Y = charge.position.Y + r * sin(angle);

			CalcJob job{{ charge, total_charge, has_different_signs, angle, angle_start, point, false, 0 }};
			
			PostJob(job);
		}
	}
}

int FieldLinesCalculator::GetNumberOfElectricFieldLines(const TheElectricField* field, int total_charge)
{
	int result = 0;

	for (const auto &charge : field->charges) {
		if (charge.charge == 0) continue;

		if (sign(total_charge) == sign(charge.charge))
			result += charge.charge * theApp.options.numLinesOnUnitCharge;
		else ++result;
	}

	return result;
}


bool FieldLinesCalculator::CheckStatus()
{
	if (finishedJobs < postedJobs) return false;

	return true;
}



void FieldLinesCalculator::PostJob(const CalcJob& job)
{
	std::shared_ptr<CalcJob> theJob;

	switch (calcMethod)
	{
	case Options::CalculationMethod::EulerMethod:
		theJob = std::make_shared<CalcJobWithMethod<RungeKutta::Euler<Vector2D<double>>>>(job, this, &field, Singleton<RungeKutta::Euler<Vector2D<double>>>::getInstance());
		break;
	case Options::CalculationMethod::MidpointMethod:
		theJob = std::make_shared<CalcJobWithMethod<RungeKutta::Midpoint<Vector2D<double>>>>(job, this, &field, Singleton<RungeKutta::Midpoint<Vector2D<double>>>::getInstance());
		break;
	case Options::CalculationMethod::RalstonMethod:
		theJob = std::make_shared<CalcJobWithMethod<RungeKutta::Ralston<Vector2D<double>>>>(job, this, &field, Singleton<RungeKutta::Ralston<Vector2D<double>>>::getInstance());
		break;
	case Options::CalculationMethod::HeunMethod:
		theJob = std::make_shared<CalcJobWithMethod<RungeKutta::Heun<Vector2D<double>>>>(job, this, &field, Singleton<RungeKutta::Heun<Vector2D<double>>>::getInstance());
		break;
	case Options::CalculationMethod::RK4Method:
		theJob = std::make_shared<CalcJobWithMethod<RungeKutta::RK4<Vector2D<double>>>>(job, this, &field, Singleton<RungeKutta::RK4<Vector2D<double>>>::getInstance());
		break;
	case Options::CalculationMethod::RK3per8Method:
		theJob = std::make_shared<CalcJobWithMethod<RungeKutta::RK3per8<Vector2D<double>>>>(job, this, &field, Singleton<RungeKutta::RK3per8<Vector2D<double>>>::getInstance());
		break;
	case Options::CalculationMethod::AdaptiveHeunEulerMethod:
		theJob = std::make_shared<CalcJobWithMethod<RungeKutta::AdaptiveHeunEuler<Vector2D<double>>>>(job, this, &field, Singleton<RungeKutta::AdaptiveHeunEuler<Vector2D<double>>>::getInstance());
		break;
	case Options::CalculationMethod::AdaptiveBogackiShampineMethod:
		theJob = std::make_shared<CalcJobWithMethod<RungeKutta::AdaptiveBogackiShampine<Vector2D<double>>>>(job, this, &field, Singleton<RungeKutta::AdaptiveBogackiShampine<Vector2D<double>>>::getInstance());
		break;
	case Options::CalculationMethod::AdaptiveCashKarpMethod:
		theJob = std::make_shared<CalcJobWithMethod<RungeKutta::AdaptiveCashKarp<Vector2D<double>>>>(job, this, &field, Singleton<RungeKutta::AdaptiveCashKarp<Vector2D<double>>>::getInstance());
		break;
	case Options::CalculationMethod::AdaptiveFehlbergMethod:
		theJob = std::make_shared<CalcJobWithMethod<RungeKutta::AdaptiveFehlberg<Vector2D<double>>>>(job, this, &field, Singleton<RungeKutta::AdaptiveFehlberg<Vector2D<double>>>::getInstance());
		break;
	case Options::CalculationMethod::AdaptiveDormandPrinceMethod:
		theJob = std::make_shared<CalcJobWithMethod<RungeKutta::AdaptiveDormandPrince<Vector2D<double>>>>(job, this, &field, Singleton<RungeKutta::AdaptiveDormandPrince<Vector2D<double>>>::getInstance());
		break;
	}

	if (theJob)
	{
		++postedJobs;
		theApp.m_WorkerThreads.AddJob(theJob);
	}
}
