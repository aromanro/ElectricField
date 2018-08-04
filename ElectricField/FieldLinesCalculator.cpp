#include "stdafx.h"
#include "FieldLinesCalculator.h"

#include "PrecisionTimer.h"


const double M_PI = 3.1415926535897932384626433832795028841971693993751;

template<class T> FieldLinesCalculator::CalcThread<T>::CalcThread(FieldLinesCalculator* calculator, const TheElectricField *field, T *solver)
	: m_pCalculator(calculator), m_Solver(solver),
	calculateEquipotentials(theApp.options.calculateEquipotentials), potentialInterval(theApp.options.potentialInterval), distanceUnitLength(theApp.options.distanceUnitLength)
{
	functorE.theField = field;
	functorV.theField = field;

	Start();
}


template<class T> FieldLinesCalculator::CalcThread<T>::~CalcThread()
{
	delete m_Solver;
}

template<class T> inline void FieldLinesCalculator::CalcThread<T>::PostCalculateEquipotential()
{
	if (m_Job.angle != m_Job.angle_start || !calculateEquipotentials) return;

	const Vector2D<double> startPoint = m_Job.point;

	const double potential = functorV.theField->Potential(startPoint);

	if (sign(m_Job.charge.charge)*sign(potential) > 0 && abs(m_Job.old_potential - potential) >= potentialInterval)
	{
		if (m_Job.old_potential == 0) m_Job.old_potential = floor(potential / potentialInterval) * potentialInterval;
		else m_Job.old_potential += sign(potential - m_Job.old_potential) * potentialInterval;

		std::lock_guard<std::mutex> lock(m_pCalculator->m_jobsSection);
		FieldLineJob job(m_Job);
		job.isEquipotential = true;
		m_pCalculator->m_jobs.push_back(job);

		// some threads finished although there are still jobs posted
		// restart one
		if (m_pCalculator->finishedThreads > 0)
		{
			--m_pCalculator->finishedThreads;
			m_pCalculator->StartComputingThread(&m_pCalculator->field);
		}
	}
}

template<class T> inline void FieldLinesCalculator::CalcThread<T>::CalculateElectricFieldLine()
{
	unsigned int steps = 40000;
	
	//dummy values
	double precision = 0.01;
	double max_step = 1.;
	
	// if the charges have different signs, the code tries to end all starting lines on some charge
	// it might need a lot of steps for that - for net charge 0 try even more, all lines should ideally end

	if (m_Job.has_different_signs && m_Job.total_charge == 0) steps = 5000000;
	else if (m_Job.has_different_signs) steps = 1000000;

	double t = 0;  // this is dummy
	double step = 0.001;
	double next_step = step;


	// start electric field lines only from charges that have the charge with the same sign as the total charge
	// on the other ones the lines will end (hopefully all of them if the total charge is zero)
	if (sign(m_Job.total_charge) == sign(m_Job.charge.charge))
		fieldLine.AddPoint(m_Job.point);
		
	for (unsigned int i = 0; i < steps; ++i)
	{
		const double len = m_Job.point.Length() * distanceUnitLength;

		// precision is needed for parts of lines close to the charge
		const bool needs_precision = (len < 2000);

		if (m_Solver->IsAdaptive())
		{
			precision = (needs_precision ? 0.001 : 0.5);
			max_step = (needs_precision ? 0.001 : 1.);
		}
		else step = (needs_precision ? 0.001 : 0.5);


		m_Job.point = m_Solver->SolveStep(functorE, m_Job.point, t, step, next_step, precision, max_step);
		
		// add points only for this, the other case is for equipotentials, no need for the actual electric field line
		if (sign(m_Job.total_charge) == sign(m_Job.charge.charge))
			fieldLine.AddPoint(m_Job.point);

		// line ended on a charge, bail out
		if (functorE.theField->HitCharge(m_Job.point)) break;

		t += step;
		if (m_Solver->IsAdaptive()) step = next_step;


		//****************************** Equipotential lines *****************************************************

		// post a job for the equipotential line
		PostCalculateEquipotential();

		//*********************************************************************************************************
	}
}


template<class T> inline void FieldLinesCalculator::CalcThread<T>::CalculateEquipotential()
{
	Vector2D<double> startPoint = m_Job.point;
	Vector2D<double> point = startPoint;

	double dist = 0;
	double t = 0;
	const unsigned int num_steps = (m_Solver->IsAdaptive() ? 800000 : 1500000);
	double step = (m_Solver->IsAdaptive() ? 0.001 : 0.0001);
	double next_step = step;

	fieldLine.AddPoint(startPoint);
	fieldLine.weightCenter = Vector2D<double>(startPoint);
	
	for (unsigned int i = 0; i < num_steps; ++i)
	{
		point = m_Solver->SolveStep(functorV, point, t, step, next_step, 1E-3, 0.01);

		fieldLine.AddPoint(point);
		
		// 'step' plays the role of the portion of the curve 'weight'
		fieldLine.weightCenter += point * step;

		t += step;
		if (m_Solver->IsAdaptive()) step = next_step;

		// if the distance is smaller than 6 logical units but the line length is bigger than
		// double the distance from the start point
		// the code assumes that the field line closes
		dist = (startPoint - point).Length();
		if (dist * distanceUnitLength < 6. && t > 2.*dist)
		{
			fieldLine.points.push_back(startPoint);  // close the loop
			fieldLine.weightCenter /= t; // divide by the whole 'weight' of the curve
			break;
		}
	}
}


template<class T> void FieldLinesCalculator::CalcThread<T>::Calculate()
{
	for (;;)
	{
		// grab a job from the job list
		{
			std::lock_guard<std::mutex> lock(m_pCalculator->m_jobsSection);

			if (m_pCalculator->Terminate || m_pCalculator->m_jobs.empty()) break; // no more jobs or asked to finish

			m_Job = m_pCalculator->m_jobs.front();
			m_pCalculator->m_jobs.pop_front();
		}

		if (m_Job.isEquipotential) {
			CalculateEquipotential();

			if (m_pCalculator->Terminate) break;

			std::lock_guard<std::mutex> lock(m_pCalculator->m_potentialLinesSection);
			m_pCalculator->potentialFieldLines.push_back(PotentialLine());
			m_pCalculator->potentialFieldLines.back().potential = m_Job.old_potential;
			m_pCalculator->potentialFieldLines.back().weightCenter = fieldLine.weightCenter;
			m_pCalculator->potentialFieldLines.back().points.swap(fieldLine.points);
		}
		else {
			functorE.charge_sign = sign(m_Job.charge.charge);

			CalculateElectricFieldLine();

			if (m_pCalculator->Terminate) break;

			std::lock_guard<std::mutex> lock(m_pCalculator->m_electricLinesSection);
			m_pCalculator->electricFieldLines.push_back(FieldLine());
			m_pCalculator->electricFieldLines.back().points.swap(fieldLine.points);
		}
	}

	FieldLinesCalculator* calc = m_pCalculator;

	delete this;

	++calc->finishedThreads;
}





FieldLinesCalculator::FieldLinesCalculator()
	: startedThreads(0), finishedThreads(0), potentialInterval(0), Terminate(false), calcMethod(Options::CalculationMethod::EulerMethod)
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

			point.X = charge.position.X + r*cos(angle);
			point.Y = charge.position.Y + r*sin(angle);

			m_jobs.push_back( { charge, total_charge, has_different_signs, angle, angle_start, point, false, 0 } );
		}
	}

	potentialInterval = theApp.options.potentialInterval;
	calcMethod = theApp.options.calculationMethod;

	startedThreads = theApp.options.numThreads;
	for (unsigned int i = 0; i < startedThreads; ++i) StartComputingThread(theField);
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
	if (finishedThreads < startedThreads) return false;

	return true;
}


void FieldLinesCalculator::StartComputingThread(const TheElectricField *theField)
{
	switch (calcMethod)
	{
	case Options::CalculationMethod::EulerMethod:
		new CalcThread<RungeKutta::Euler<Vector2D<double>>>(this, theField, new RungeKutta::Euler<Vector2D<double>>());
		break;
	case Options::CalculationMethod::MidpointMethod:
		new CalcThread<RungeKutta::Midpoint<Vector2D<double>>>(this, theField, new RungeKutta::Midpoint<Vector2D<double>>());
		break;
	case Options::CalculationMethod::RalstonMethod:
		new CalcThread<RungeKutta::Ralston<Vector2D<double>>>(this, theField, new RungeKutta::Ralston<Vector2D<double>>());
		break;
	case Options::CalculationMethod::HeunMethod:
		new CalcThread<RungeKutta::Heun<Vector2D<double>>>(this, theField, new RungeKutta::Heun<Vector2D<double>>());
		break;
	case Options::CalculationMethod::RK4Method:
		new CalcThread<RungeKutta::RK4<Vector2D<double>>>(this, theField, new RungeKutta::RK4<Vector2D<double>>());
		break;
	case Options::CalculationMethod::RK3per8Method:
		new CalcThread<RungeKutta::RK3per8<Vector2D<double>>>(this, theField, new RungeKutta::RK3per8<Vector2D<double>>());
		break;
	case Options::CalculationMethod::AdaptiveHeunEulerMethod:
		new CalcThread<RungeKutta::AdaptiveHeunEuler<Vector2D<double>>>(this, theField, new RungeKutta::AdaptiveHeunEuler<Vector2D<double>>());
		break;
	case Options::CalculationMethod::AdaptiveBogackiShampineMethod:
		new CalcThread<RungeKutta::AdaptiveBogackiShampine<Vector2D<double>>>(this, theField, new RungeKutta::AdaptiveBogackiShampine<Vector2D<double>>());
		break;
	case Options::CalculationMethod::AdaptiveCashKarpMethod:
		new CalcThread<RungeKutta::AdaptiveCashKarp<Vector2D<double>>>(this, theField, new RungeKutta::AdaptiveCashKarp<Vector2D<double>>());
		break;
	case Options::CalculationMethod::AdaptiveFehlbergMethod:
		new CalcThread<RungeKutta::AdaptiveFehlberg<Vector2D<double>>>(this, theField, new RungeKutta::AdaptiveFehlberg<Vector2D<double>>());
		break;
	case Options::CalculationMethod::AdaptiveDormandPrinceMethod:
		new CalcThread<RungeKutta::AdaptiveDormandPrince<Vector2D<double>>>(this, theField, new RungeKutta::AdaptiveDormandPrince<Vector2D<double>>());
		break;
	}
}
