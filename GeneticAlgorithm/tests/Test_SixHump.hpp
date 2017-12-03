#pragma once
#include <algorithm>

std::pair<double, double> TestSixHump(
	size_t N,
	size_t popSize,
	size_t iterations,
	std::ofstream* file
);