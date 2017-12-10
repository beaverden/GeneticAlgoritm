#pragma once
#include <algorithm>
#include "GeneticAlgorithm.hpp"
#include "functions/Rastrigin/Rastrigin.hpp"
#include "functions/Schwefel_7/Schwefel_7.hpp"

std::pair<EvaluatedChromossome, double> SuperviseRastrigin(size_t N);
std::pair<EvaluatedChromossome, double> SuperviseSchwefel(size_t N);