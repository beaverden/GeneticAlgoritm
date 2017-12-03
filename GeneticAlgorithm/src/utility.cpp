#include "stdafx.hpp"
#include <random>
#include "utility.hpp"

static std::mt19937 gen(std::random_device{}());
static std::mt19937_64 gen_64(std::random_device{}());
static std::uniform_real_distribution<double> dis(0.0, 1.0);

double randd() {
	return dis(gen);
}

uint64_t randi64()
{
	return gen_64();
}



