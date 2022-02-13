#pragma once

#include "Charge.h"
#include "Vector2D.h"


// some info needed for a field line job
struct FieldLineJob 
{
	Charge charge;
	int total_charge = 0;
	bool has_different_signs = false;
	double angle_start = 0;
	double angle = 0;

	Vector2D<double> point;

	bool isEquipotential = false;
	double old_potential = 0;
};

class CalcJob : public FieldLineJob
{
public:
	CalcJob(const FieldLineJob& fieldLineJob)
		: FieldLineJob(fieldLineJob)
	{};

	virtual ~CalcJob() {}

	virtual void Calculate() {};
};



template<class T> class CalcJobWithMethod : public CalcJob
{
protected:
	FieldLinesCalculator* m_pCalculator;
	const T& m_Solver;

	class FunctorForCalc {
	public:
		const TheElectricField* theField;

		FunctorForCalc(const TheElectricField* field = NULL) : theField(field) {}
	};

	class FunctorForE : public FunctorForCalc {
	public:
		int charge_sign;

		FunctorForE(const TheElectricField* field = NULL) : FunctorForCalc(field), charge_sign(1) {};

		inline Vector2D<double> operator()(double /*t*/, const Vector2D<double>& pos) {
			const Vector2D<double> v = theField->ENormalized(pos);

			return charge_sign > 0 ? v : -v;
		};
	};


	class FunctorForV : public FunctorForCalc {
	public:
		FunctorForV(const TheElectricField* field = NULL) : FunctorForCalc(field) {};

		// just returns a perpendicular vector on E
		inline Vector2D<double> operator()(double /*t*/, const Vector2D<double>& pos) {
			Vector2D<double> v = theField->E(pos);
			double temp = v.X;
			v.X = -v.Y;
			v.Y = temp;

			return v.Normalize();
		};
	};

	FunctorForE functorE;
	FunctorForV functorV;

	PotentialLine fieldLine;

	// some values copied from options to be easily accessible
	// besides, they could change during calculation in the main thread
	// that means strange things happening :)
	bool calculateEquipotentials;
	double potentialInterval;
	unsigned int distanceUnitLength;


	inline void CalculateElectricFieldLine()
	{
		unsigned int steps = 40000;

		//dummy values
		double precision = 0.01;
		double max_step = 0.1;

		// if the charges have different signs, the code tries to end all starting lines on some charge
		// it might need a lot of steps for that - for net charge 0 try even more, all lines should ideally end

		if (has_different_signs && total_charge == 0) steps = 5000000;
		else if (has_different_signs) steps = 1000000;

		double t = 0;  // this is dummy
		double step = 0.001;
		double next_step = step;


		// start electric field lines only from charges that have the charge with the same sign as the total charge
		// on the other ones the lines will end (hopefully all of them if the total charge is zero)
		if (sign(total_charge) == sign(charge.charge))
			fieldLine.AddPoint(point);

		for (unsigned int i = 0; i < steps; ++i)
		{
			const double len = point.Length() * distanceUnitLength;

			// precision is needed for parts of lines close to the charge
			const bool needs_precision = (len < 3000);

			if (m_Solver.IsAdaptive())
			{
				precision = (needs_precision ? 0.001 : 0.1);
				max_step = (needs_precision ? 0.001 : 0.1);
			}
			else step = (needs_precision ? 0.001 : 0.5);


			point = m_Solver.SolveStep(functorE, point, t, step, next_step, precision, max_step);

			// add points only for this, the other case is for equipotentials, no need for the actual electric field line
			if (sign(total_charge) == sign(charge.charge))
				fieldLine.AddPoint(point);

			// line ended on a charge, bail out
			if (functorE.theField->HitCharge(point)) break;

			t += step;
			if (m_Solver.IsAdaptive()) step = next_step;


			//****************************** Equipotential lines *****************************************************

			// post a job for the equipotential line
			PostCalculateEquipotential();

			//*********************************************************************************************************
		}
	}


	inline void CalculateEquipotential()
	{
		Vector2D<double> startPoint = point;
		Vector2D<double> curPoint = startPoint;

		double dist = 0;
		double t = 0;
		const unsigned int num_steps = (m_Solver.IsAdaptive() ? 800000 : 1500000);
		double step = (m_Solver.IsAdaptive() ? 0.001 : 0.0001);
		double next_step = step;

		fieldLine.AddPoint(startPoint);
		fieldLine.weightCenter = Vector2D<double>(startPoint);

		for (unsigned int i = 0; i < num_steps; ++i)
		{
			curPoint = m_Solver.SolveStep(functorV, curPoint, t, step, next_step, 1E-3, 0.01);

			fieldLine.AddPoint(curPoint);

			// 'step' plays the role of the portion of the curve 'weight'
			fieldLine.weightCenter += curPoint * step;

			t += step;
			if (m_Solver.IsAdaptive()) step = next_step;

			// if the distance is smaller than 6 logical units but the line length is bigger than
			// double the distance from the start point
			// the code assumes that the field line closes
			dist = (startPoint - curPoint).Length();
			if (dist * distanceUnitLength < 6. && t > 2. * dist)
			{
				fieldLine.points.push_back(startPoint);  // close the loop
				fieldLine.weightCenter /= t; // divide by the whole 'weight' of the curve
				break;
			}
		}
	}

	inline void PostCalculateEquipotential()
	{
		if (angle != angle_start || !calculateEquipotentials) return;

		const Vector2D<double> startPoint = point;

		const double potential = functorV.theField->Potential(startPoint);

		if (sign(charge.charge) * sign(potential) > 0 && abs(old_potential - potential) >= potentialInterval)
		{
			if (old_potential == 0) old_potential = floor(potential / potentialInterval) * potentialInterval;
			else old_potential += sign(potential - old_potential) * potentialInterval;

			isEquipotential = true;

			m_pCalculator->PostJob(*this);
		}
	}

	void Calculate() override
	{
		ProcessJob();
		++m_pCalculator->finishedJobs;
	}

	void ProcessJob()
	{
		if (isEquipotential)
		{
			CalculateEquipotential();

			if (m_pCalculator->Terminate) return;

			std::lock_guard<std::mutex> lock(m_pCalculator->m_potentialLinesSection);
			m_pCalculator->potentialFieldLines.push_back(PotentialLine());
			m_pCalculator->potentialFieldLines.back().potential = old_potential;
			m_pCalculator->potentialFieldLines.back().weightCenter = fieldLine.weightCenter;
			m_pCalculator->potentialFieldLines.back().points.swap(fieldLine.points);
		}
		else
		{
			functorE.charge_sign = sign(charge.charge);

			CalculateElectricFieldLine();

			if (m_pCalculator->Terminate) return;

			std::lock_guard<std::mutex> lock(m_pCalculator->m_electricLinesSection);
			m_pCalculator->electricFieldLines.push_back(FieldLine());
			m_pCalculator->electricFieldLines.back().points.swap(fieldLine.points);
		}
	}
public:
	CalcJobWithMethod(const CalcJob& job, FieldLinesCalculator* calculator, const TheElectricField* field, const T& solver)
		: CalcJob(job), m_pCalculator(calculator), m_Solver(solver),
		functorE(field), functorV(field),
		calculateEquipotentials(theApp.options.calculateEquipotentials), potentialInterval(theApp.options.potentialInterval), distanceUnitLength(theApp.options.distanceUnitLength)
	{
	};

	virtual ~CalcJobWithMethod()
	{
	}
private:
	// prevent copy
	CalcJobWithMethod(const CalcJobWithMethod&) = delete;
	CalcJobWithMethod& operator=(const CalcJobWithMethod&) = delete;
};
