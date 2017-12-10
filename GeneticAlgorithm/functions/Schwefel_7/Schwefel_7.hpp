#pragma once
#include <stdint.h>
#include "GeneticAlgorithm.hpp"

struct SchwefelContext
{
	double minValue;
	double maxValue;
	size_t N;
	size_t precision;
	size_t valueSizeBits;
};

double to_float(uint64_t val, SchwefelContext* global);

double Schwefel(ChromossomePtr ch, void* globalContext);

double SchwefelEval(const ChromossomePtr& ch, void* globalContext);
