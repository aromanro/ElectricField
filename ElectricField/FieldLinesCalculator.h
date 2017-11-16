#pragma once

#include <deque>

#include <atomic>
#include <mutex>

#include "TheElectricField.h"
#include "RungeKutta.h"
#include "ComputationThread.h"
#include "Options.h"

class FieldLinesCalculator
{
protected:
	Options::CalculationMethod calcMethod;

	// some info needed for a field line job
	typedef struct FieldLineJob {
		Charge charge;
		int total_charge;
		bool has_different_signs;
		double angle_start;
		double angle;

		Vector2D<double> point;

		bool isEquipotential;
		double old_potential;
		
	} FieldLineJob;


	std::mutex m_jobsSection;
	std::deque<FieldLineJob> m_jobs;

	template<class T> class CalcThread : public ComputationThread {
	protected:
		FieldLinesCalculator* m_pCalculator;
		T* m_Solver;

		class FunctorForCalc {
		public:
			const TheElectricField *theField;

			FunctorForCalc(const TheElectricField *field = NULL) : theField(field) {}
		};

		class FunctorForE : public FunctorForCalc {
		public:
			int charge_sign;

			FunctorForE(const TheElectricField *field = NULL) : FunctorForCalc(field), charge_sign(1) {};

			inline Vector2D<double> operator()(double /*t*/, const Vector2D<double>& pos) {
				const Vector2D<double> v = theField->ENormalized(pos);

				return charge_sign > 0 ? v : -v;
			};
		};


		class FunctorForV : public FunctorForCalc {
		public:

			FunctorForV(const TheElectricField *field = NULL) : FunctorForCalc(field) {};

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

		FieldLineJob m_Job;
		PotentialLine fieldLine;

		// some values copied from options to be easily accesible
		// besides, they could change during calculation in the main thread
		// that means strange things happening :)
		bool calculateEquipotentials;
		double potentialInterval;
		unsigned int distanceUnitLength;


		inline void CalculateElectricFieldLine();
		inline void CalculateEquipotential();
		inline void PostCalculateEquipotential();

		virtual void Calculate();

	public:
		CalcThread(FieldLinesCalculator* calculator, const TheElectricField *field, T *solver);
		~CalcThread();
	};

	unsigned int startedThreads;
	
	std::atomic_uint finishedThreads;

	double potentialInterval;
public:
	TheElectricField field;

	std::mutex m_electricLinesSection;
	std::vector<FieldLine> electricFieldLines;

	std::mutex m_potentialLinesSection;
	std::vector<PotentialLine> potentialFieldLines;

	std::atomic_bool Terminate;

	FieldLinesCalculator();
	~FieldLinesCalculator();

	void Clear();

	double getPotentialInterval() { return potentialInterval; }
	
	void StartCalculating(const TheElectricField *theField);
	static int GetNumberOfElectricFieldLines(const TheElectricField* field, int total_charge);

	bool CheckStatus();
	void StartComputingThread(const TheElectricField *theField);
};

