#include "stdafx.hpp"
#include "Salesman.h"
#define MAX_ITERATIONS 1000
#define MAX_EVAL 5000
typedef std::vector<uint64_t> solution;


solution generateSolution(GlobalContext* context)
{
	std::vector<uint64_t> values;
	for (size_t i = 0; i < context->N; i++)
	{
		values.push_back(i);
	}
	std::mt19937 gen(std::random_device{}());
	std::shuffle(values.begin(), values.end(), gen);
	return values;
}

double Evaluate(solution& values, GlobalContext* context)
{
	double sum = 0;
	for (size_t i = 0; i < context->N; i++)
	{
		if (i > 0)
		{
			sum += context->matrix[values[i - 1]][values[i]];
		}
	}
	sum += context->matrix[values[context->N - 1]][values[0]];
	assert(elements.size() == context->N);
	return sum;
}

std::vector<solution> generateSwaps(solution& values, GlobalContext* context)
{
	std::vector<solution> result;
	for (size_t i = 0; i < context->N; i++)
	{
		for (size_t j = i+1; j < context->N; j++)
		{
			if (i != j)
			{
				std::swap(values[i], values[j]);
				result.push_back(values);
				std::swap(values[i], values[j]);
			}
		}
	}
	return result;
}

double Objective(solution& values, GlobalContext* context)
{
	return -Evaluate(values, context);
}

std::pair<double, double> HillClimbing(GlobalContext* context)
{
	const clock_t begin_time = clock();

	size_t count = 0;
	solution solutionBest;
	double solutionValue;
	bool first = true;
	while (count < MAX_ITERATIONS)
	{
		solution currentBest = generateSolution(context);
		double bestValue = Objective(currentBest, context);
		size_t evaluations = 0;
		while (evaluations < MAX_EVAL)
		{
			bool found = false;
			for (solution& s : generateSwaps(currentBest, context))
			{
				double newValue = Objective(s, context);
				if (newValue > bestValue)
				{
					bestValue = newValue;
					currentBest = s;
					found = true;
					break;
				}
			}
			
			if (!found)
			{
				break;
			}
			evaluations++;
		}
		if (first || bestValue > solutionValue)
		{
			printf("<<< %f\n", bestValue);
			solutionBest = currentBest;
			solutionValue = bestValue;
			first = false;
		}
		printf("%f\n", solutionValue);
		count++;
	}
	const clock_t end_time = clock();
	double time = (double)(end_time - begin_time) / CLOCKS_PER_SEC;
	return std::make_pair(Evaluate(solutionBest, context), time);
}