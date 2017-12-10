#include "stdafx.hpp"
#include <random>
#include "utility.hpp"

static std::mt19937 gen(std::random_device{}());
static std::mt19937_64 gen_64(std::random_device{}());
static std::uniform_real_distribution<double> dis(0.0, 1.0);
static double pow10[7] = { 1.0, 10.0, 100.0, 1000.0, 10000.0, 100000.0, 1000000.0 };

double to_float(uint64_t intValue, double min, double max, size_t valueSizeBits)
{
	return min + intValue*(max - min) / ((1ULL << valueSizeBits) - 1);
}

double randd() {
	return dis(gen);
}

uint64_t randi64()
{
	return gen_64();
}

size_t GetLength(double min, double max, size_t precision)
{
	size_t result = (size_t)ceil(log2((max - min)*pow10[precision]));
	return std::max(1u, result);
}



