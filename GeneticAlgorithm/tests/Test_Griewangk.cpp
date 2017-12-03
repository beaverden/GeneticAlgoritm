#include "stdafx.hpp"
#include <time.h>
#include "Test_Griewangk.hpp"
#include "GeneticAlgorithm.hpp"
#include <string>
#include <fstream>


static double pow10[7] = { 1.0, 10.0, 100.0, 1000.0, 10000.0, 100000.0, 1000000.0 };
static double pow2[63] = { 1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,4294967296,8589934592,17179869184,34359738368,68719476736,137438953472,274877906944,549755813888,1099511627776,2199023255552,4398046511104,8796093022208,17592186044416,35184372088832,70368744177664,140737488355328,281474976710656,562949953421312,1125899906842624,2251799813685248,4503599627370496,9007199254740992,18014398509481984,36028797018963968,72057594037927936,144115188075855872,288230376151711744,576460752303423488,1152921504606846976,2305843009213693952,4611686018427387904 };
#define PRECISION 2
/*
	One may describe a global context, that will be passed to
	the evaluation function and compare function.
	This structure may contain anything
*/

struct GlobalContext
{
	double minValue;
	double maxValue;
	size_t N;
	size_t precision;
	size_t valueSizeBits;
};


static double to_float(uint64_t val, GlobalContext* global)
{
	return global->minValue + val*(global->maxValue - global->minValue) / (pow2[global->valueSizeBits] - 1);
}

static double Griewangk(ChromossomePtr ch, void* globalContext)
{
	GlobalContext* global = (GlobalContext*)(globalContext);
	double *x = new double[global->N];
	CodeIterator it = ch->begin();

	double sum = 0;
	double prod = 1;

	for (size_t i = 0; i < global->N; i++)
	{
		x[i] = to_float(it.GetBitsAsInt(global->valueSizeBits), global);
		sum += (x[i] * x[i]) / 4000.0;
		prod *= cos((x[i] / sqrt((double)(i+1)))*(PI/180));
		it += global->valueSizeBits;
	}
	delete[] x;
	return sum - prod + 1;
}

static double GriewangkEval(const ChromossomePtr& ch, void* globalContext)
{
	return 1/fabs(Griewangk(ch, globalContext));
}

std::pair<double, double> TestGriewangk(
	size_t N, 
	size_t popSize, 
	size_t iterations,
	std::ofstream* file)
{
	GlobalContext context = { -600, 600, N, PRECISION };
	context.valueSizeBits = (size_t)ceil(log2((context.maxValue - context.minValue)*pow10[context.precision]));
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
