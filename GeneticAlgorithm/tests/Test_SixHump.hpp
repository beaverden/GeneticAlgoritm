#pragma once
#include <algorithm>
#include "functions/SixHump/SixHump.hpp"

std::pair<double, double> TestSixHump(
	size_t N,
	size_t popSize,
	size_t iterations,
	std::ofstream* file
);