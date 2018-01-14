#pragma once
#include <map>
#include <fstream>
#include "Salesman.h"
#include <set>
#include <assert.h>
#include <algorithm>
#include <time.h>
#include "../../include/GeneticAlgorithm.hpp"

struct GlobalContext
{
	size_t N;
	size_t valueLength;
	double** matrix;
	double optimalValue;
};

std::pair<double, double> TestSalesman(GlobalContext* context);
std::pair<double, double> HillClimbing(GlobalContext* context);