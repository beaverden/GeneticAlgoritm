#pragma once
#include "GeneticAlgorithm.hpp"

struct GriewangkContext
{
	double minValue;
	double maxValue;
	size_t N;
	size_t precision;
	size_t valueSizeBits;
};

double to_float(uint64_t val, GriewangkContext* global);

double Griewangk(ChromossomePtr ch, void* globalContext);

double GriewangkEval(const ChromossomePtr& ch, void* globalContext);