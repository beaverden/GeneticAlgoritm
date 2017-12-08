#include "stdafx.hpp"
#include "Rastrigin.hpp"

double to_float(uint64_t val, RastriginContext* global)
{
	return global->minValue + val*(global->maxValue - global->minValue) / ((1 << global->valueSizeBits) - 1);
}

double Rastrigin(ChromossomePtr ch, void* globalContext)
{
	RastriginContext* global = (RastriginContext*)(globalContext);
	double *x = new double[global->N];
	CodeIterator it = ch->begin();
	for (size_t i = 0; i < global->N; i++)
	{
		x[i] = to_float(it.GetBitsAsInt(global->valueSizeBits), global);
		it += global->valueSizeBits;
	}

	double s = 10 * global->N;
	for (size_t i = 0; i < global->N; i++)
	{
		s += (x[i] * x[i] - 10 * cos((2 * PI * x[i])*(PI / 180.0)));
	}
	delete[] x;
	return s;
}

double RastriginEval(const ChromossomePtr& ch, void* globalContext)
{
	double res = Rastrigin(ch, globalContext);
	if (fabs(0.0 - res) <= 0.00000001) res = ((res < 0) ? -0.00000001 : 0.00000001);
	return 1.0 / fabs(res);
}
