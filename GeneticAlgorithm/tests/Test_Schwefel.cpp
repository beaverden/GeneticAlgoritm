/*
	This source describes a use case of a algorithm
	The function we want to minimize is {Schwefel 7}
	We will use Tournament selection to obtain the desider result
*/

#include "stdafx.hpp"
#include <time.h>
#include "functions/Schwefel_7/Schwefel_7.hpp"
#include "Test_Schwefel.hpp"
#include "GeneticAlgorithm.hpp"
#include <string>
#include <fstream>

#define PRECISION 2
/* 
	One may describe a global context, that will be passed to
	the evaluation function and compare function.
	This structure may contain anything
*/


std::pair<double, double> TestSchwefel(
	size_t N, 
	size_t popSize, 
	size_t iterations,
	std::ofstream* file)
{
	SchwefelContext context = { -500.0, 500.0, N, PRECISION };
	context.valueSizeBits = GetLength(context.minValue, context.maxValue, context.precision);

	IV iv;
	iv.populationSize = popSize;
	iv.codeLength = context.N * context.valueSizeBits;
	iv.fitnessEval = SchwafelEval;
	iv.globalContext = &context;
	iv.mutationRate = 0.008;
	iv.crossoverRate = 0.7;
	iv.tournamentSize = 30;//s (size_t)(iv.populationSize / 10.0);
	iv.selectionAlgo = SelectionAlgorithm::TOURNAMENT;
	iv.crossoverAlgo = CrossOverAlgorithm::UNIFORM_CROSS;
	iv.mutationAlgo = MutationAlgorithm::RANDOM_ITERATION;

	const clock_t begin_time = clock();

	GeneticAlgorithm gen(&iv);
	//gen.Update(iterations);
	
	for (size_t i = 0; i < iterations; i++)
	{
		gen.Update();
		EvaluatedChromossome bestCh = gen.GetKthBest(0);
		if (file != nullptr)
		{
			(*file).precision(5);
			(*file) << std::fixed << Schwafel(bestCh.first, &context) << std::endl;
		}
		//printf("%f\n", Schwafel(bestCh.first, &context));
	}
	EvaluatedChromossome bestCh = gen.GetKthBest(0);

	const clock_t end_time = clock();
	double value = Schwafel(bestCh.first, &context);
	double time = (double)(end_time - begin_time) / CLOCKS_PER_SEC;
	
	return std::make_pair(value, time);
}
