#include "stdafx.hpp"
#include "Test_Genetic_Supervisor.hpp"
#include <numeric>


#define PRECISION 2

struct IncomingContext
{
	size_t N;

	size_t nrValues;
	std::vector<size_t> valueSizes;
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
		currentValue++;
	}
	SelectionAlgorithm selectionAlgo = (SelectionAlgorithm)values[0];
	CrossOverAlgorithm crossoverAlgo = (CrossOverAlgorithm)values[1];
	MutationAlgorithm  mutationAlgo = (MutationAlgorithm)values[2];
	uint64_t populationSize = values[3];

	/* ======================== SCHWEFEL RELATED ======================== */
	SchwefelContext context = { -500.0, 500.0, incomingContext->N, PRECISION };
	context.valueSizeBits = GetLength(context.maxValue, context.minValue, context.precision);
	IV iv;
	iv.globalContext = &context;
	iv.codeLength = context.valueSizeBits;
	iv.crossoverAlgo = crossoverAlgo;
	iv.crossoverRate = 0.7;
	iv.fitnessEval = SchwafelEval;
	iv.globalContext = &context;
	iv.mutationAlgo = mutationAlgo;
	iv.mutationRate = 0.008;
	iv.populationSize = populationSize;
	iv.selectionAlgo = selectionAlgo;
	iv.tournamentSize = 30;

	GeneticAlgorithm testAlgo(&iv);
	testAlgo.Update(1000);
	EvaluatedChromossome best = testAlgo.GetKthBest(0);
	return best.second;
}

std::pair<EvaluatedChromossome, double> SuperviseSchwefel(size_t N)
{
	// 0-1 Algorithm (bit 0)
	// 0-1 Mutation Type (bit 1)
	// 0-2 Crossover Algorithm (bit 2-3)
	// 100-1000 population size (bit 3-13)

	IncomingContext context;
	context.N = N;
	context.nrValues = 4;
	context.valueSizes = { GetLength(0, 1, 0), GetLength(0, 1, 0), GetLength(0, 2, 0), GetLength(100, 1000, 0) };

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

	GeneticAlgorithm gen(&iv);
	gen.Update(100);

	return std::make_pair(std::make_pair(nullptr, 0), 0);
}