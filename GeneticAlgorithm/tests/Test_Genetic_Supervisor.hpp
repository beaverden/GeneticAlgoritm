#pragma once
#include <algorithm>
#include "GeneticAlgorithm.hpp"
#include "functions/Schwefel_7/Schwefel_7.hpp"

std::pair<EvaluatedChromossome, double> SuperviseSchwefel(size_t N);