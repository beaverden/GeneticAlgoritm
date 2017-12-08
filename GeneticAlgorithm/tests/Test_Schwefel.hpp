#pragma once
#include <algorithm>


std::pair<double, double> TestSchwefel(
	size_t N,
	size_t popSize,
	size_t iterations,
	std::ofstream* file
);