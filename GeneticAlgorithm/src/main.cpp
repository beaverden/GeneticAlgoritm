#include "stdafx.hpp"

// The only include needed
#include "GeneticAlgorithm.hpp"

#include "Test_Schwefel.hpp"
#include "Test_SixHump.hpp"
#include "Test_Rastrigin.hpp"
#include "Test_Griewangk.hpp"
#include "Test_Genetic_Supervisor.hpp"
#include "TravelingSalesman/Salesman.h"

#include <functional>
#include <iostream>
#include <string>
#include <fstream>
#include <future>
#include <vector>
#include <experimental/filesystem>
#define NO_ITERATIONS 1

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

void dump_salesman(
	std::string testcase
)
{
	std::cout << testcase << std::endl;
	std::experimental::filesystem::path pPath = std::experimental::filesystem::current_path();
	std::string path = pPath.generic_string();
	std::string valuesFileName = "test_data/" + testcase + ".txt";
	std::string optimalFileName = "test_data/" + testcase + ".optimal.txt";
	std::cout << valuesFileName << std::endl;
	GlobalContext context;
	std::ifstream input(valuesFileName);
	size_t N = 0;
	input >> N;
	std::cout << N;
	double** matrix = new double*[N];
	for (size_t i = 0; i < N; i++)
	{
		matrix[i] = new double[N];
	}
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			input >> matrix[i][j];
		}
	}
	std::ifstream optim(optimalFileName);
	size_t optSize;
	optim >> optSize;
	double optResult = 0;
	std::vector<uint64_t> optimalValues;
	for (size_t i = 0; i < optSize; i++)
	{
		uint64_t temp;
		optim >> temp;
		optimalValues.push_back(temp);
		if (i > 0)
		{
			optResult += matrix[optimalValues[i - 1] - 1][optimalValues[i] - 1];
		}
	}
	optResult += matrix[optimalValues.back() - 1][optimalValues[0] - 1];

	context.optimalValue = optResult;
	context.matrix = matrix;
	context.N = N;
	context.valueLength = (size_t)(GetLength(0, N - 1, 0));

	std::vector<std::future<std::pair<double, double>>> values;
	values.resize(NO_ITERATIONS);
	double avgTime = 0;
	double avgValue = 0;
	for (size_t i = 0; i < NO_ITERATIONS; i++)
	{
		values[i] = std::async(TestSalesman, &context);
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

	std::string outFileName = "Raport\\Raport_" + testcase + ".txt";
	std::ofstream outFile(outFileName, std::ios_base::app);
	outFile << testcase << " " << avgValue << " " << avgTime << " s" << std::endl;
	outFile.close();

	for (size_t i = 0; i < N; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

int main(int argc, char* argv[])
{
	/*
	TestSalesman(
		"D:/Google Drive/Study/Univ/Anul 2 Sem 1/AG/Tema2/GeneticAlgorithm/GeneticAlgorithm/test_data/bays29.txt", 
		"D://Google Drive/Study//Univ//Anul 2 Sem 1//AG//Tema2//GeneticAlgorithm//GeneticAlgorithm//test_data//bays29.optimal.txt"
	);
	*/
	dump_salesman("bayg29");
	/*
	TestSalesman(
		"D:/Google Drive/Study/Univ/Anul 2 Sem 1/AG/Tema2/GeneticAlgorithm/GeneticAlgorithm/test_data/att48.txt",
		"D:/Google Drive/Study/Univ/Anul 2 Sem 1/AG/Tema2/GeneticAlgorithm/GeneticAlgorithm/test_data/att48.optimal.txt"
	);
	*/
	/*
	//SuperviseRastrigin(5);
	//SuperviseSchwefel(5);
	
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
	*/
	return 0;
}