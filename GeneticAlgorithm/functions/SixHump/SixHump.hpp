#pragma once
#include "GeneticAlgorithm.hpp"

struct SixHumpContext
{
	double minValue;
	double maxValue;
	size_t N;
	size_t precision;
	size_t valueSizeBits;
};


double to_float(uint64_t val, SixHumpContext* global);

double SixHump(ChromossomePtr ch, void* globalContext);

double SixHumpEval(const ChromossomePtr& ch, void* globalContext);