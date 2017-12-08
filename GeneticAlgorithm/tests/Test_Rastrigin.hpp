#pragma once
#include <algorithm>
#include "functions/Rastrigin/Rastrigin.hpp"

std::pair<double, double> TestRastrigin(
	size_t N,
	size_t popSize,
	size_t iterations,
	std::ofstream* file
);