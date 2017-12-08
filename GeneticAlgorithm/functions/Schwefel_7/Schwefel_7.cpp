#include "stdafx.hpp"
#include "Schwefel_7.hpp"


double to_float(uint64_t val, SchwefelContext* global)
{
	return global->minValue + val*(global->maxValue - global->minValue) / ((1ULL << global->valueSizeBits) - 1);
}

// Returns the actual value of the function on
// a given Chromossome
double Schwafel(ChromossomePtr ch, void* globalContext)
{
	SchwefelContext* global = (SchwefelContext*)(globalContext);
	double *x = new double[global->N];
	CodeIterator it = ch->begin();
	for (size_t i = 0; i < global->N; i++)
	{
		x[i] = to_float(it.GetBitsAsInt(global->valueSizeBits), global);
		it += global->valueSizeBits;
	}

	double s = 0;
	for (size_t i = 0; i < global->N; i++)
	{
		s += (-x[i] * sin(sqrt(fabs(x[i]))));
	}
	delete[] x;
	return s;
}

// Fitness function for the Genetic Algorithm that
// Uses the actual value of the function
double SchwafelEval(const ChromossomePtr& ch, void* globalContext)
{
	return (-Schwafel(ch, globalContext));
}