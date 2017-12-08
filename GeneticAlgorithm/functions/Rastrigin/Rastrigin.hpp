#pragma once
#include "GeneticAlgorithm.hpp"

struct RastriginContext
{
	double minValue;
	double maxValue;
	size_t N;
	size_t precision;
	size_t valueSizeBits;
};

double to_float(uint64_t val, RastriginContext* global);

double Rastrigin(ChromossomePtr ch, void* globalContext);

double RastriginEval(const ChromossomePtr& ch, void* globalContext);