#pragma once
#include "GeneticCode.hpp"
#include "utility.hpp"
#include <algorithm>
#include <queue>
#include <memory>


typedef Chromossome* ChromossomePtr;
typedef std::pair<ChromossomePtr, double> EvaluatedChromossome;
typedef std::pair<ChromossomePtr, ChromossomePtr> ChromossomePair;
typedef double(*FITNESS_EVALUATION)(const ChromossomePtr& ch, void* globalContext);

enum SelectionAlgorithm
{
	ROULETTE_WHEEL,
	TOURNAMENT
};

enum CrossOverAlgorithm
{
	/*
		Parent1: aaaabbbb
		Parent2: ccccdddd
		Child1:  aaaadddd
		Child2:  ccccbbbb
	*/
	HALF_SPLIT,

	/*
		Same as half split, but a random position
	*/
	RANDOM_SPLIT,

	/*
		Child1 and Child2 get different genes from 
		parents (based on 50% probability)
	*/
	UNIFORM_CROSS
};

enum MutationAlgorithm
{
	RANDOM_ITERATION,
	FLIP_ONE
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

protected:
	/*
		Returns a pair of a ChromossomePtr and a double, representing the Chromossome and it's fitness value
	*/
	EvaluatedChromossome Evaluate(ChromossomePtr ch);

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
};