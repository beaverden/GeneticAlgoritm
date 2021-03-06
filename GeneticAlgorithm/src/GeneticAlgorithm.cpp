#include "stdafx.hpp"
#include "utility.hpp"
#include "GeneticAlgorithm.hpp"
#include <functional>
#include <assert.h>
#include <numeric>
#include <thread>
#include <vector>

GeneticAlgorithm::GeneticAlgorithm(IV* data) :
	populationSize(data->populationSize),
	codeLength(data->codeLength),
	selectionAlgorithm(data->selectionAlgo),
	mutationAlgorithm(data->mutationAlgo),
	mutationRate(data->mutationRate),
	crossoverAlgorithm(data->crossoverAlgo),
	crossoverRate(data->crossoverRate),
	fitnessEval(data->fitnessEval),
	tournamentSize(data->tournamentSize),
	globalContext(data->globalContext),
	threaded(data->threaded),
	maxThreads(data->maxThreads),
	customGenerate(data->customGenerate),
	customMutation(data->customMutation),
	customCrossover(data->customCrossover),
	iteration(0)
{
	population.resize(populationSize);
	
	if (this->threaded)
	{
		std::vector<std::thread> threads;
		size_t part = ((populationSize % maxThreads == 0) ? (populationSize/maxThreads) : (1 + populationSize/maxThreads));
		size_t start = 0;
		while (start < data->populationSize)
		{
			if (start + part >= data->populationSize)
			{
				part = (data->populationSize - start);
			}
			std::thread t([this, start, part, data] 
			{ 
				for (size_t i = start; i < start + part; i++)
				{
					ChromossomePtr ptr;
					if (this->customGenerate != nullptr)
					{
						ptr = customGenerate(data);
					}
					else
					{
						ptr = new Chromossome(this->codeLength);
					}
					this->GetPopulation()[i] = this->Evaluate(ptr);
				}
				
			});
			threads.push_back(std::move(t));
			start += part;
		}
		for (auto& t : threads)
		{
			t.join();
		}
	}
	else
	{
		for (size_t i = 0; i < data->populationSize; i++)
		{
			ChromossomePtr ptr;
			if (customGenerate != nullptr)
			{
				ptr = customGenerate(data);
			}
			else
			{
				ptr = new Chromossome(codeLength);
			}
			population[i] = Evaluate(ptr);
		}

	}
	

	if (this->selectionAlgorithm == SelectionAlgorithm::ROULETTE_WHEEL)
	{
		evalArr.resize(population.size() + 1);
		individual.resize(population.size() + 1);
		cumulative.resize(population.size() + 1);
	}
	
}

GeneticAlgorithm::~GeneticAlgorithm()
{
	for (size_t i = 0; i < population.size(); i++)
	{
		delete population[i].first;
	}
}

void GeneticAlgorithm::Sort()
{
	std::sort(
		this->population.begin(), 
		this->population.end(),
		[](const EvaluatedChromossome& ch1, const EvaluatedChromossome& ch2)
		{
			return (ch1.second > ch2.second);
		}
	);
}

EvaluatedChromossome GeneticAlgorithm::Evaluate(ChromossomePtr ch)
{
	return std::make_pair(ch, this->fitnessEval(ch, globalContext));
}

std::vector<EvaluatedChromossome>& GeneticAlgorithm::GetPopulation()
{
	return this->population;
}

EvaluatedChromossome GeneticAlgorithm::GetKthBest(size_t k)
{
	std::nth_element(
		population.begin(), 
		population.begin() + k, 
		population.end(),
		[](const EvaluatedChromossome& ch1, const EvaluatedChromossome& ch2)
		{
			return (ch1.second > ch2.second);
		}
	);
	return this->population[k];
}

ChromossomePair GeneticAlgorithm::HalfSplitCrossover(const ChromossomePtr& ch1, const ChromossomePtr& ch2)
{
	uint64_t length = ch1->getLength();
	uint64_t half = (length >> 1);
	CodeIterator ch1h1 = ch1->position(0);
	CodeIterator ch1h2 = ch1->position(half);
	CodeIterator ch2h1 = ch2->position(0);
	CodeIterator ch2h2 = ch2->position(half);

	ChromossomePtr newCh1 = new Chromossome(
		&ch1h1, half, &ch2h2, length - half
	);
	ChromossomePtr newCh2 = new Chromossome(
		&ch2h1, half, &ch1h2, length - half
	);
	assert(newCh1->getLength() == ch1->getLength());
	assert(newCh2->getLength() == ch1->getLength());
	return std::make_pair(std::move(newCh1), std::move(newCh2));
}

ChromossomePair GeneticAlgorithm::RandomSplitCrossover(const ChromossomePtr& ch1, const ChromossomePtr& ch2)
{
	uint64_t length = ch1->getLength();
	uint64_t position = (randi64() % (length - 2)) + 1;
	CodeIterator ch1h1 = ch1->position(0);
	CodeIterator ch1h2 = ch1->position(position);
	CodeIterator ch2h1 = ch2->position(0);
	CodeIterator ch2h2 = ch2->position(position);

	ChromossomePtr newCh1 = new Chromossome(
		&ch1h1, position, &ch2h2, length - position
	);
	ChromossomePtr newCh2 = new Chromossome(
		&ch2h1, position, &ch1h2, length - position
	);
	assert(newCh1->getLength() == ch1->getLength());
	assert(newCh2->getLength() == ch1->getLength());
	return std::make_pair(std::move(newCh1), std::move(newCh2));
}

ChromossomePair GeneticAlgorithm::UniformCrossover(const ChromossomePtr& ch1, const ChromossomePtr& ch2)
{
	CodeIterator it1 = ch1->begin();
	CodeIterator it2 = ch2->begin();
	ChromossomePtr childOne = new Chromossome(ch1->getLength());
	CodeIterator childOneIT = childOne->begin();
	ChromossomePtr childTwo = new Chromossome(ch1->getLength());
	CodeIterator childTwoIT = childTwo->begin();
	while (childOneIT != childOne->end())
	{
		BIT_VALUE v1, v2;
		v1 = (*it1);
		v2 = (*it2);
		if (randd() <= 0.5)
		{
			std::swap(v1, v2);
		}
		childOneIT.SetBit(v1);
		childTwoIT.SetBit(v2);
		++childOneIT;
		++childTwoIT;
		++it1;
		++it2;
	}
	return std::make_pair(childOne, childTwo);
}

ChromossomePair GeneticAlgorithm::ChromossomeCrossover(const ChromossomePtr& ch1, const ChromossomePtr& ch2)
{
	if (randd() <= this->crossoverRate)
	{
		switch (this->crossoverAlgorithm)
		{
		case CrossOverAlgorithm::HALF_SPLIT:
			return HalfSplitCrossover(ch1, ch2);
		case CrossOverAlgorithm::RANDOM_SPLIT:
			return RandomSplitCrossover(ch1, ch1);
		case CrossOverAlgorithm::UNIFORM_CROSS:
			return UniformCrossover(ch1, ch2);
		case CrossOverAlgorithm::CUSTOM_CROSS:
			return customCrossover(ch1, ch2, globalContext);
		default:
			return std::make_pair(nullptr, nullptr);
		}
	}
	else
	{
		return std::make_pair(
			new Chromossome(
				ch1->getCode(), ch1->getLength()
			),
			new Chromossome(
				ch2->getCode(), ch2->getLength()
			)
		);
	}
}



void GeneticAlgorithm::Mutate(ChromossomePtr ch1)
{
	if (this->mutationAlgorithm == MutationAlgorithm::FLIP_ONE)
	{
		FlipOne(ch1);
	}
	else if (this->mutationAlgorithm == MutationAlgorithm::RANDOM_ITERATION)
	{
		RandomIteration(ch1);
	}
	else if (this->mutationAlgorithm == MutationAlgorithm::CUSTOM_MUTATION)
	{
		customMutation(ch1, globalContext);
	}
}

void GeneticAlgorithm::RandomIteration(ChromossomePtr ch1)
{
	CodeIterator it = ch1->begin();
	while (it != ch1->end())
	{
		if (randd() <= this->mutationRate)
		{
			it.FlipBit();
		}
		++it;
	}
}

void GeneticAlgorithm::FlipOne(ChromossomePtr ch1)
{
	if (randd() <= this->mutationRate)
	{
		size_t pos = (randi64() % ch1->getLength());
		CodeIterator it = ch1->position(pos);
		it.FlipBit();
	}
}


void GeneticAlgorithm::Update()
{
	if (this->selectionAlgorithm == SelectionAlgorithm::ROULETTE_WHEEL)
	{
		RouletteWheelSelection();
	}
	else if (this->selectionAlgorithm == SelectionAlgorithm::TOURNAMENT)
	{
		TournamentSelection();
	}
}

void GeneticAlgorithm::Update(size_t generations)
{
	for (size_t i = 0; i < generations; i++)
	{
		this->Update();
	}
}

void GeneticAlgorithm::RouletteWheelSelection()
{

	std::vector<EvaluatedChromossome> newPop;

	double minNegValue = 0;
	double totalFitness = 0;
	for (size_t i = 0; i < population.size(); i++)
	{
		evalArr[i] = this->fitnessEval(population[i].first, this->globalContext);
		if (evalArr[i] < 0)
		{
			minNegValue = std::min(evalArr[i], minNegValue);
		}

	}

	for (size_t i = 0; i < population.size(); i++)
	{
		evalArr[i] += -minNegValue;
		totalFitness += evalArr[i];
	}

	for (size_t i = 0; i < population.size(); i++)
	{
		double individualProb = evalArr[i] / totalFitness;
		cumulative[i] = ((i == 0) ? 0.0 : (cumulative[i-1] + individualProb));
	}
	cumulative[population.size()] = 1.1;
	

	for (uint64_t i = 0; i < population.size() && newPop.size() < population.size(); i++)
	{
		double r1 = randd();
		auto it = std::upper_bound(
			cumulative.begin(),
			cumulative.end() - 1,
			r1,
			std::less<double>()
		);
		size_t index = std::distance(cumulative.begin(), it);
		if (index > 0) index--;
		ChromossomePtr ch1 = population[index].first;
		
		double r2 = randd();
		it = std::upper_bound(
			cumulative.begin(),
			cumulative.end() - 1,
			r2,
			std::less<double>()
		);
		index = std::distance(cumulative.begin(), it);
		if (index > 0) index--;
		ChromossomePtr ch2 = population[index].first;

		ChromossomePair children = ChromossomeCrossover(ch1, ch2);
		if (children.first != nullptr)
		{
			if (newPop.size() < population.size())
			{
				this->Mutate(children.first);
				newPop.push_back(Evaluate(children.first));
			}
		}
		if (children.second != nullptr)
		{
			if (newPop.size() < population.size())
			{
				this->Mutate(children.second);
				newPop.push_back(Evaluate(children.second));
			}
		}
	}

	for (size_t i = 0; i < population.size(); i++)
	{
		delete population[i].first;
	}
	this->population.clear();
	std::move(newPop.begin(), newPop.end(), std::back_inserter(population));
	iteration++;
}

EvaluatedChromossome GeneticAlgorithm::SelectByTournament()
{
	size_t currentBestIndex = (randi64() % population.size());
	EvaluatedChromossome currentBest = population[currentBestIndex];
	for (size_t j = 0; j < tournamentSize; j++)
	{
		size_t random = (randi64() % population.size());
		const EvaluatedChromossome& newSelected = population[random];
		if (newSelected.second > currentBest.second)
		{
			currentBestIndex = random;
			currentBest = std::move(newSelected);
		}
	}
	return currentBest;
}

void GeneticAlgorithm::TournamentSelection()
{
	std::vector<EvaluatedChromossome> newPop;
	for (uint64_t i = 0; i < population.size() && newPop.size() < population.size(); i++)
	{
		const EvaluatedChromossome& ch1 = SelectByTournament();
		const EvaluatedChromossome& ch2 = SelectByTournament();
		ChromossomePair children =
			ChromossomeCrossover(ch1.first, ch2.first);
		if (children.first != nullptr)
		{
			if (newPop.size() < population.size())
			{
				this->Mutate(children.first);
				newPop.push_back(Evaluate(children.first));
			}
		}
		if (children.second != nullptr)
		{
			if (newPop.size() < population.size())
			{
				this->Mutate(children.second);
				newPop.push_back(Evaluate(children.second));
			}
		}
	}
	for (size_t i = 0; i < population.size(); i++)
	{
		delete population[i].first;
	}
	this->population.clear();
	std::move(newPop.begin(), newPop.end(), std::back_inserter(population));
	iteration++;
}
