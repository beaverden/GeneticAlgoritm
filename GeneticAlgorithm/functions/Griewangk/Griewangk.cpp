#include "stdafx.hpp"
#include "Griewangk.hpp"
#include <math.h>

double to_float(uint64_t val, GriewangkContext* global)
{
	return global->minValue + val*(global->maxValue - global->minValue) / ((1 << global->valueSizeBits) - 1);
}

double Griewangk(ChromossomePtr ch, void* globalContext)
{
	GriewangkContext* global = (GriewangkContext*)(globalContext);
	double *x = new double[global->N];
	CodeIterator it = ch->begin();

	double sum = 0;
	double prod = 1;

	for (size_t i = 0; i < global->N; i++)
	{
		x[i] = to_float(it.GetBitsAsInt(global->valueSizeBits), global);
		sum += (x[i] * x[i]) / 4000.0;
		prod *= cos((x[i] / sqrt((double)(i + 1)))*(PI / 180));
		it += global->valueSizeBits;
	}
	delete[] x;
	return sum - prod + 1;
}

double GriewangkEval(const ChromossomePtr& ch, void* globalContext)
{
	return 1 / fabs(Griewangk(ch, globalContext));
}