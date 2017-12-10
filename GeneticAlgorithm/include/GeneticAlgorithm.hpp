#pragma once
#include "GeneticCode.hpp"
#include "utility.hpp"
#include <algorithm>
#include <queue>
#include <memory>
#include <functional>

typedef Chromossome* ChromossomePtr;
typedef std::pair<ChromossomePtr, double> EvaluatedChromossome;
typedef std::pair<ChromossomePtr, ChromossomePtr> ChromossomePair;
typedef double(*FITNESS_EVALUATION)(const ChromossomePtr& ch, void* globalContext);

enum SelectionAlgorithm
{
	ROULETTE_WHEEL	= 0,
	TOURNAMENT		= 1
};

enum CrossOverAlgorithm
{
	/*
		Parent1: aaaabbbb
		Parent2: ccccdddd
		Child1:  aaaadddd
		Child2:  ccccbbbb
	*/
	HALF_SPLIT		= 0,

	/*
		Same as half split, but a random position
	*/
	RANDOM_SPLIT	= 1,

	/*
		Child1 and Child2 get different genes from 
		parents (based on 50% probability)
	*/
	UNIFORM_CROSS	= 2
};

enum MutationAlgorithm
{
	RANDOM_ITERATION	= 0,
	FLIP_ONE			= 1
};

/*
	This structure is always passed to the algorithm constructor

	- All the relevant members to the current problem should be 
	filled in, or the defaults will be used otherwise
	- Irrelevant members can be left as default
*/
struct IV
{
	size_t populationSize = 100;
	size_t codeLength = 32;

	// Probabilities
	double mutationRate = 0.015;
	double crossoverRate = 0.7;

	// Algorithms
	SelectionAlgorithm selectionAlgo = TOURNAMENT;
	CrossOverAlgorithm crossoverAlgo = RANDOM_SPLIT;

	// Mutation
	MutationAlgorithm mutationAlgo = RANDOM_ITERATION;

	// Functions
	FITNESS_EVALUATION fitnessEval = nullptr;

	// Tournament related value
	size_t tournamentSize = 10;

	// Global Context
	void* globalContext = nullptr;

	// Max thread count
	bool threaded = false;
	size_t maxThreads = 4;
};

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(IV* data);
	~GeneticAlgorithm();

	/*
		Sort the population according to the comparison function
		Works in O(nlogn)
	*/
	void Sort(); 
	
	/* 
		Get the k-th best individual (without sorting)
		Works in O(n) on avearge
	*/
	EvaluatedChromossome GetKthBest(size_t k);

	std::vector<EvaluatedChromossome>& GetPopulation();

	/*
		Performs one generation change with all the relevant operations
	*/
	void GeneticAlgorithm::Update();
	void GeneticAlgorithm::Update(size_t generations);

	EvaluatedChromossome Evaluate(ChromossomePtr ch);
protected:
	/*
		Returns a pair of a ChromossomePtr and a double, representing the Chromossome and it's fitness value
	*/
	

	//Selection methods
	void GeneticAlgorithm::RouletteWheelSelection();
	void GeneticAlgorithm::TournamentSelection();
	EvaluatedChromossome GeneticAlgorithm::SelectByTournament();

	// Genetic ChromossomeCrossover (the called method depends on CrossoverAlgorithm
	ChromossomePair GeneticAlgorithm::ChromossomeCrossover(const ChromossomePtr& ch1, const ChromossomePtr& ch2);

	// Crossover methods
	ChromossomePair GeneticAlgorithm::HalfSplitCrossover(const ChromossomePtr& ch1, const ChromossomePtr& ch2);
	ChromossomePair GeneticAlgorithm::RandomSplitCrossover(const ChromossomePtr& ch1, const ChromossomePtr& ch2);
	ChromossomePair GeneticAlgorithm::UniformCrossover(const ChromossomePtr& ch1, const ChromossomePtr& ch2);

	// Mutation methods
	void Mutate(ChromossomePtr ch);
	void RandomIteration(ChromossomePtr ch);
	void FlipOne(ChromossomePtr ch);

	// Variables from IV and locals
	void* globalContext;
	size_t iteration;
	size_t populationSize;
	size_t codeLength;
	SelectionAlgorithm selectionAlgorithm;

	CrossOverAlgorithm crossoverAlgorithm;
	double crossoverRate;

	MutationAlgorithm mutationAlgorithm;
	double mutationRate;

	FITNESS_EVALUATION fitnessEval;

	std::vector<EvaluatedChromossome> population;

	// Roulette wheel related
	std::vector<double> evalArr;
	std::vector<double> individual;
	std::vector<double> cumulative;

	//Tournament related
	size_t tournamentSize;

	//Threads
	bool threaded;
	size_t maxThreads;
};

struct ThreadedParamEval
{
	ChromossomePtr ptr;
	EvaluatedChromossome* toEval;
};