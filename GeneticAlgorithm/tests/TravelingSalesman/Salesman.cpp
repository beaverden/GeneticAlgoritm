#include "stdafx.hpp"
#include <map>
#include <fstream>
#include "Salesman.h"
#include <set>
#include <assert.h>
#include <algorithm>
#include "../../include/GeneticAlgorithm.hpp"



void SetAtPosition(ChromossomePtr ch, size_t valueSize, int position, uint64_t value)
{
	CodeIterator it = ch->begin();
	it += (valueSize * position);
	it.SetIntAsBits(value, valueSize);
}

uint64_t GetAtPosition(ChromossomePtr ch, size_t valueSize, int position)
{
	CodeIterator it = ch->begin();
	it += valueSize * position;
	return it.GetBitsAsInt(valueSize);
}

std::vector<uint64_t> GetValues(ChromossomePtr ch, GlobalContext* context)
{
	std::vector<uint64_t> values;
	for (size_t i = 0; i < context->N; i++)
	{
		values.push_back(GetAtPosition(ch, context->valueLength, i));
	}
	return values;
}

ChromossomePtr customGenerate(IV* data)
{
	GlobalContext* context = (GlobalContext*)(data->globalContext);
	std::vector<uint64_t> v;
	v.resize(context->N);
	for (size_t i = 0; i < context->N; i++)
	{
		v[i] = i;
	}
	std::mt19937 gen(std::random_device{}());
	std::shuffle(v.begin(), v.end(), gen);
	ChromossomePtr ch = new Chromossome(context->valueLength * context->N);
	for (size_t i = 0; i < context->N; i++)
	{
		SetAtPosition(ch, context->valueLength, v[i], i);
	}
	return ch;
}

double Evaluate(const ChromossomePtr& ch, void* globalContext)
{
	GlobalContext* context = (GlobalContext*)(globalContext);
	uint64_t* elems = new uint64_t[context->N]();
	CodeIterator it = ch->begin();
	std::set<uint64_t> elements;
	double sum = 0;
	for (size_t i = 0; i < context->N; i++)
	{
		elems[i] = it.GetBitsAsInt(context->valueLength);
		elements.insert(elems[i]);
		it += context->valueLength;
		if (i > 0)
		{
			sum += context->matrix[elems[i - 1]][elems[i]];
		}
	}
	sum += context->matrix[elems[context->N - 1]][elems[0]];
	assert(elements.size() == context->N);
	delete elems;
	return sum;
}

double Fitness(const ChromossomePtr& ch, void* globalContext)
{
	return -Evaluate(ch, globalContext);
}


void customMutation(ChromossomePtr ch, void* globalContext) 
{
	//printf("Mutation\n");
	GlobalContext* context = (GlobalContext*)(globalContext);
	uint64_t* elems = new uint64_t[context->N]();
	CodeIterator it = ch->begin();
	for (size_t i = 0; i < context->N; i++)
	{
		elems[i] = it.GetBitsAsInt(context->valueLength);
		it += context->valueLength;
	}
	size_t first = (randi64() % context->N);
	size_t second = (randi64() % context->N);
	while (second == first) second = (randi64() % context->N);
	uint64_t elem1 = GetAtPosition(ch, context->valueLength, first);
	uint64_t elem2 = GetAtPosition(ch, context->valueLength, second);
	SetAtPosition(ch, context->valueLength, second, elem1);
	SetAtPosition(ch, context->valueLength, first, elem2);
}

ChromossomePtr probeFrom(const ChromossomePtr& ch1, const ChromossomePtr& ch2, void* globalContext)
{
	GlobalContext* context = (GlobalContext*)(globalContext);
	size_t first, second;
	while (true)
	{
		first = (randi64() % context->N);
		second = (randi64() % context->N);
		if (first > second) std::swap(first, second);
		if (first != second) break;
	}
	ChromossomePtr child1 = new Chromossome(ch1->getLength());
	std::set<uint64_t> addedValues;
	for (size_t i = first; i <= second; i++)
	{
		uint64_t value = GetAtPosition(ch1, context->valueLength, i);
		assert(value < context->N);
		SetAtPosition(child1, context->valueLength, i, value);
		addedValues.insert(value);
	}
	size_t current = 0;
	for (size_t i = 0; i < context->N; i++)
	{
		if (i >= first && i <= second) continue;
		bool found = false;
		while (!found)
		{
			uint64_t value = GetAtPosition(ch2, context->valueLength, current);
			assert(value < context->N);
			if (addedValues.find(value) == addedValues.end())
			{
				found = true;
				SetAtPosition(child1, context->valueLength, i, value);
			}
			current++;
		}
	}
	return child1;
}

ChromossomePair customCrossover(const ChromossomePtr& ch1, const ChromossomePtr& ch2, void* globalContext)
{
	return std::make_pair(
		probeFrom(ch1, ch2, globalContext), 
		probeFrom(ch2, ch1, globalContext)
	);
}


std::pair<double, double> TestSalesman(GlobalContext* context)
{
	IV data;
	data.mutationRate = 0.05;
	data.populationSize = 2000;
	data.codeLength = context->valueLength;
	data.crossoverRate = 0.7;
	data.customCrossover = customCrossover;
	data.customGenerate = customGenerate;
	data.customMutation = customMutation;
	data.fitnessEval = Fitness;
	data.globalContext = context;
	data.maxThreads = 1;
	data.crossoverAlgo = CUSTOM_CROSS;
	data.mutationAlgo = CUSTOM_MUTATION;
	data.selectionAlgo = TOURNAMENT;
	data.tournamentSize = 70;
	data.threaded = false;

	GeneticAlgorithm algo(&data);
	double bestValue = -1;
	for (size_t i = 0; i < 1000; i++)
	{
		algo.Update();
		EvaluatedChromossome p = algo.GetKthBest(0);
		double value = Evaluate(p.first, context);
		//printf("%f %f\n", value, context->optimalValue);
		if (bestValue == -1) bestValue = value;
		else bestValue = std::min(bestValue, value);
	}
	return std::make_pair(bestValue, 0);
}