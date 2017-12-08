#include "stdafx.hpp"

// The only include needed
#include "GeneticAlgorithm.hpp"

#include "Test_Schwefel.hpp"
#include "Test_SixHump.hpp"
#include "Test_Rastrigin.hpp"
#include "Test_Griewangk.hpp"
#include "Test_Genetic_Supervisor.hpp"

#include <functional>
#include <iostream>
#include <string>
#include <fstream>
#include <future>
#include <vector>
#define NO_ITERATIONS 15

void dump_func(
	std::function<std::pair<double, double>(size_t, size_t, size_t, std::ofstream*)> func,
	std::string functionName,
	size_t N,
	size_t popSize,
	size_t iterations
)
{
	std::cout << functionName << " " << std::to_string(N) << std::endl;
	std::string valuesFileName = "Values\\Test_" + functionName + "_" + std::to_string(N) + "_values.txt";
	std::ofstream valuesFile(valuesFileName);

	std::vector<std::future<std::pair<double, double>>> values;
	values.resize(NO_ITERATIONS);
	// Run once for the values
	func(N, popSize, iterations, &valuesFile);
	valuesFile.close();
	
	double avgTime = 0;
	double avgValue = 0;
	for (size_t i = 0; i < NO_ITERATIONS; i++)
	{
		values[i] = std::async(func, N, popSize, iterations, nullptr);
	}
	for (size_t i = 0; i < NO_ITERATIONS; i++)
	{
		std::pair<double, double> res = values[i].get();
		avgTime += res.second;
		avgValue += res.first;
		printf("%f\n", avgValue);
	}
	
	avgTime /= NO_ITERATIONS;
	avgValue /= NO_ITERATIONS;

	std::string outFileName = "Raport\\Raport_" + functionName + ".txt";
	std::ofstream outFile(outFileName, std::ios_base::app);
	outFile << functionName << " [" << std::to_string(N) << "] " << avgValue << " " << avgTime << " s" << std::endl;
	outFile.close();
}

int main(int argc, char* argv[])
{	
	//SuperviseSchwefel(30);
	dump_func(TestSixHump, "SixHump", 2, 200, 500);

	dump_func(TestSchwefel, "Schwefel", 5, 300, 500);
	dump_func(TestSchwefel, "Schwefel", 10, 1000, 500);
	dump_func(TestSchwefel, "Schwefel", 30, 2000, 500);

	dump_func(TestGriewangk, "Griewangk", 5, 100, 200);
	dump_func(TestGriewangk, "Griewangk", 10, 300, 300);
	dump_func(TestGriewangk, "Griewangk", 30, 500, 500);

	dump_func(TestRastrigin, "Rastrigin", 5, 100, 500);
	dump_func(TestRastrigin, "Rastrigin", 10, 300, 500);
	dump_func(TestRastrigin, "Rastrigin", 30, 500, 500);

	return 0;
}