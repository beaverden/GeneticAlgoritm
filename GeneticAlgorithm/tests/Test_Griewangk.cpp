#include "stdafx.hpp"
#include "Test_Griewangk.hpp"
#include "GeneticAlgorithm.hpp"
#include <string>
#include <fstream>
#include <time.h>

#define PRECISION 2



std::pair<double, double> TestGriewangk(
	size_t N, 
	size_t popSize, 
	size_t iterations,
	std::ofstream* file)
{
	GriewangkContext context = { -600, 600, N, PRECISION };
	context.valueSizeBits = GetLength(context.minValue, context.maxValue, context.precision);
	IV iv;
	iv.populationSize = popSize;
	iv.codeLength = context.N * context.valueSizeBits;
	iv.fitnessEval = GriewangkEval;
	iv.globalContext = &context;
	iv.mutationRate = 0.001;
	iv.crossoverRate = 0.7;
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
			(*file) << std::fixed << Griewangk(bestCh.first, &context) << std::endl;
		}
		

		//printf("%f\n", Griewangk(bestCh.first, &context));
	}
	

	const clock_t end_time = clock();
	EvaluatedChromossome bestCh = gen.GetKthBest(0);
	double value = Griewangk(bestCh.first, &context);
	double time = (double)(end_time - begin_time) / CLOCKS_PER_SEC;

	return std::make_pair(value, time);
}
