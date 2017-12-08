#pragma once
#include <algorithm>
#include "functions/Griewangk/Griewangk.hpp"

std::pair<double, double> TestGriewangk(
	size_t N, 
	size_t popSize, 
	size_t iterations,
	std::ofstream* file
);