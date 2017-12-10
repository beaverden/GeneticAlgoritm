#include "stdafx.hpp"
#include "Test_Genetic_Supervisor.hpp"
#include <numeric>


#define PRECISION 2

struct IncomingContext
{
	size_t N;

	size_t nrValues;
	std::vector<size_t> valueSizes;
	FITNESS_EVALUATION func;
};


double GeneticEval(const ChromossomePtr& ch, void* globalContext)
{
	IncomingContext* incomingContext = (IncomingContext*)(globalContext);
	std::vector<uint64_t> values;

	CodeIterator it = ch->begin();
	size_t currentValue = 0;
	while (it != ch->end())
	{
		uint64_t value = it.GetBitsAsInt(incomingContext->valueSizes[currentValue]);
		values.push_back(value);
		it += incomingContext->valueSizes[currentValue];
		currentValue++;
		
	}
	// 0-1 Selection Algorithm (bit 0)
	// 0-1 Crossover Algorithm (bit 1)
	// 0-2 Mutation Algorithm (bit 2-3)
	// 0.0-1.0 Mutation rate
	// 0.0-1.0 Crossover rate
	// 10-30 Tournament size
	SelectionAlgorithm selectionAlgo = (SelectionAlgorithm)(values[0]);
	CrossOverAlgorithm crossoverAlgo = (CrossOverAlgorithm)(values[1]);
	MutationAlgorithm  mutationAlgo = (MutationAlgorithm)(values[2]);
	double mutationRate = to_float(values[3], 0.0, 1.0, incomingContext->valueSizes[3]);
	double crossoverRate = to_float(values[4], 0.0, 1.0, incomingContext->valueSizes[4]);
	size_t tournamentSize = (size_t)(values[5]);

	/* ======================== SCHWEFEL RELATED ======================== */
	RastriginContext context = { -5.12, 5.12, incomingContext->N, PRECISION };
	context.valueSizeBits = GetLength(context.minValue, context.maxValue, context.precision);
	IV iv;
	iv.codeLength = incomingContext->N * context.valueSizeBits;
	iv.crossoverAlgo = crossoverAlgo;
	iv.crossoverRate = crossoverRate;
	iv.fitnessEval = incomingContext->func;
	iv.globalContext = &context;
	iv.mutationAlgo = mutationAlgo;
	iv.mutationRate = mutationRate;
	iv.populationSize = 200;
	iv.selectionAlgo = selectionAlgo;
	iv.tournamentSize = tournamentSize;
	iv.threaded = true;

	GeneticAlgorithm testAlgo(&iv);
	for (size_t i = 0; i < 200; i++)
	{
		testAlgo.Update();
	}
	
	EvaluatedChromossome best = testAlgo.GetKthBest(0);
	return best.second;
}

std::pair<EvaluatedChromossome, double> SuperviseRastrigin(size_t N)
{
	// 0-1 Selection Algorithm (bit 0)
	// 0-1 Crossover Algorithm (bit 1)
	// 0-2 Mutation Algorithm (bit 2-3)
	// 0.0-1.0 Mutation rate
	// 0.0-1.0 Crossover rate
	// 10-30 Tournament size

	IncomingContext context;
	context.N = N;
	context.valueSizes = {
		GetLength(0, 1, 0),			// Selection
		GetLength(0, 1, 0),			// Crossover
		GetLength(0, 2, 0),			// Mutation
		GetLength(0.0, 1.0, 4),		// Mutation rate
		GetLength(0.0, 1.0, 2),		// Crossover rate
		GetLength(10, 30, 0)		// Tournament size
	};
	context.nrValues = context.valueSizes.size();
	context.func = RastriginEval;

	IV iv;
	iv.codeLength = std::accumulate(context.valueSizes.begin(), context.valueSizes.end(), 0);
	iv.crossoverAlgo = CrossOverAlgorithm::UNIFORM_CROSS;
	iv.crossoverRate = 0.7;
	iv.fitnessEval = GeneticEval;
	iv.globalContext = &context;
	iv.mutationAlgo = MutationAlgorithm::RANDOM_ITERATION;
	iv.mutationRate = 0.008;
	iv.populationSize = 200;
	iv.selectionAlgo = SelectionAlgorithm::TOURNAMENT;
	iv.tournamentSize = 30;
	iv.threaded = true;

	GeneticAlgorithm gen(&iv);
	for (size_t i = 0; i < 100; i++)
	{
		gen.Update();
		printf("%d\n", i);
	}

	EvaluatedChromossome ch = gen.GetKthBest(0);
	std::vector<uint64_t> values;
	CodeIterator it = ch.first->begin();
	size_t currentValue = 0;
	while (it != ch.first->end())
	{
		uint64_t value = it.GetBitsAsInt(context.valueSizes[currentValue]);
		values.push_back(value);
		it += context.valueSizes[currentValue];
		currentValue++;

	}
	// 0-1 Selection Algorithm (bit 0)
	// 0-1 Crossover Algorithm (bit 1)
	// 0-2 Mutation Algorithm (bit 2-3)
	// 0.0-1.0 Mutation rate
	// 0.0-1.0 Crossover rate
	// 10-30 Tournament size
	SelectionAlgorithm selectionAlgo = (SelectionAlgorithm)(values[0]);
	CrossOverAlgorithm crossoverAlgo = (CrossOverAlgorithm)(values[1]);
	MutationAlgorithm  mutationAlgo = (MutationAlgorithm)(values[2]);
	double mutationRate = to_float(values[3], 0.0, 1.0, context.valueSizes[3]);
	double crossoverRate = to_float(values[4], 0.0, 1.0, context.valueSizes[4]);
	size_t tournamentSize = (size_t)(values[5]);

	printf("[Rastrigin]:\nSelection: %u\nCross: %u\nMutation: %u\nMutation rate: %f\nCrossover rate: %f\nTournament size: %u\n",
		   selectionAlgo, crossoverAlgo, mutationAlgo, mutationRate, crossoverRate, tournamentSize);

	return std::make_pair(std::make_pair(nullptr, 0), 0);
}


std::pair<EvaluatedChromossome, double> SuperviseSchwefel(size_t N)
{
	// 0-1 Selection Algorithm (bit 0)
	// 0-1 Crossover Algorithm (bit 1)
	// 0-2 Mutation Algorithm (bit 2-3)
	// 0.0-1.0 Mutation rate
	// 0.0-1.0 Crossover rate
	// 10-30 Tournament size

	IncomingContext context;
	context.N = N;
	context.valueSizes = {
		GetLength(0, 1, 0),			// Selection
		GetLength(0, 1, 0),			// Crossover
		GetLength(0, 2, 0),			// Mutation
		GetLength(0.0, 1.0, 4),		// Mutation rate
		GetLength(0.0, 1.0, 2),		// Crossover rate
		GetLength(10, 30, 0)		// Tournament size
	};
	context.nrValues = context.valueSizes.size();
	context.func = SchwefelEval;

	IV iv;
	iv.codeLength = std::accumulate(context.valueSizes.begin(), context.valueSizes.end(), 0);
	iv.crossoverAlgo = CrossOverAlgorithm::UNIFORM_CROSS;
	iv.crossoverRate = 0.7;
	iv.fitnessEval = GeneticEval;
	iv.globalContext = &context;
	iv.mutationAlgo = MutationAlgorithm::RANDOM_ITERATION;
	iv.mutationRate = 0.008;
	iv.populationSize = 200;
	iv.selectionAlgo = SelectionAlgorithm::TOURNAMENT;
	iv.tournamentSize = 30;
	iv.threaded = true;

	GeneticAlgorithm gen(&iv);
	for (size_t i = 0; i < 100; i++)
	{
		gen.Update();
		printf("%d\n", i);
	}

	EvaluatedChromossome ch = gen.GetKthBest(0);
	std::vector<uint64_t> values;
	CodeIterator it = ch.first->begin();
	size_t currentValue = 0;
	while (it != ch.first->end())
	{
		uint64_t value = it.GetBitsAsInt(context.valueSizes[currentValue]);
		values.push_back(value);
		it += context.valueSizes[currentValue];
		currentValue++;

	}
	// 0-1 Selection Algorithm (bit 0)
	// 0-1 Crossover Algorithm (bit 1)
	// 0-2 Mutation Algorithm (bit 2-3)
	// 0.0-1.0 Mutation rate
	// 0.0-1.0 Crossover rate
	// 10-30 Tournament size
	SelectionAlgorithm selectionAlgo = (SelectionAlgorithm)(values[0]);
	CrossOverAlgorithm crossoverAlgo = (CrossOverAlgorithm)(values[1]);
	MutationAlgorithm  mutationAlgo = (MutationAlgorithm)(values[2]);
	double mutationRate = to_float(values[3], 0.0, 1.0, context.valueSizes[3]);
	double crossoverRate = to_float(values[4], 0.0, 1.0, context.valueSizes[4]);
	size_t tournamentSize = (size_t)(values[5]);

	printf("[Schwefel]:\nSelection: %u\nCross: %u\nMutation: %u\nMutation rate: %f\nCrossover rate: %f\nTournament size: %u\n",
		selectionAlgo, crossoverAlgo, mutationAlgo, mutationRate, crossoverRate, tournamentSize);

	return std::make_pair(std::make_pair(nullptr, 0), 0);
}