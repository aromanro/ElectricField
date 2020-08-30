#pragma once

#include <deque>

#include <atomic>
#include <mutex>

#include "TheElectricField.h"
#include "RungeKutta.h"
#include "Options.h"

#include "CalcJob.h"

class FieldLinesCalculator
{
protected:
	Options::CalculationMethod calcMethod;

	double potentialInterval;
public:
	unsigned int postedJobs;

	std::atomic_uint finishedJobs;


	TheElectricField field;

	std::mutex m_electricLinesSection;
	std::vector<FieldLine> electricFieldLines;

	std::mutex m_potentialLinesSection;
	std::vector<PotentialLine> potentialFieldLines;

	std::atomic_bool Terminate;

	FieldLinesCalculator();

	void Clear();

	double getPotentialInterval() { return potentialInterval; }
	
	void StartCalculating(const TheElectricField *theField);
	static int GetNumberOfElectricFieldLines(const TheElectricField* field, int total_charge);

	bool CheckStatus();

	void PostJob(const CalcJob& job);
};

