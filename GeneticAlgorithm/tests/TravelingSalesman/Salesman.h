#pragma once

struct GlobalContext
{
	size_t N;
	size_t valueLength;
	double** matrix;
	double optimalValue;
};

std::pair<double, double> TestSalesman(GlobalContext* context);