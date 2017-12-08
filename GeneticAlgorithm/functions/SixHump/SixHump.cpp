#include "stdafx.hpp"
#include "SixHump.hpp"


double to_float(uint64_t val, SixHumpContext* global)
{
	return global->minValue + val*(global->maxValue - global->minValue) / ((1 << global->valueSizeBits) - 1);
}

double SixHump(ChromossomePtr ch, void* globalContext)
{
	SixHumpContext* global = (SixHumpContext*)(globalContext);
	double *x = new double[global->N];
	CodeIterator it = ch->begin();
	for (size_t i = 0; i < global->N; i++)
	{
		x[i] = to_float(it.GetBitsAsInt(global->valueSizeBits), global);
		it += global->valueSizeBits;
	}

	double x1 = x[0];
	double x2 = x[1];
	delete[] x;
	return (4 * x1*x1) - (2.1*x1*x1*x1*x1) + 0.33333333*(x1*x1*x1*x1*x1*x1) + x1*x2 - 4 * x2*x2 + 4 * x2*x2*x2*x2;
}

double SixHumpEval(const ChromossomePtr& ch, void* globalContext)
{
	return (-SixHump(ch, globalContext));
}