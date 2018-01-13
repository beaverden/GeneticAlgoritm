#include "stdafx.hpp"
#include "GeneticCode.hpp"
#include <random>
#include <assert.h>


Chromossome::Chromossome(size_t codeLength) :
	codeLength(codeLength), arrayLength((codeLength >> SIZE_POW) + 1)
{
	this->code = new uint64_t[arrayLength]();
	for (uint64_t i = 0; i < arrayLength; i++)
	{
		code[i] = randi64();
	}
}

Chromossome::Chromossome(uint64_t* existingCode, size_t codeLength) :
	codeLength(codeLength), arrayLength((codeLength >> SIZE_POW) + 1)
{
	this->code = new uint64_t[arrayLength]();
	for (size_t i = 0; i < arrayLength; i++)
	{
		code[i] = existingCode[i];
	}
}

Chromossome::Chromossome(Chromossome* ch):
	codeLength(ch->getLength()),
	arrayLength((ch->codeLength >> SIZE_POW) + 1)
{
	this->code = new uint64_t[arrayLength];
	uint64_t* existingCode = ch->getCode();
	for (uint64_t i = 0; i < arrayLength; i++)
	{
		code[i] = existingCode[i];
	}
}

Chromossome::Chromossome(
	CodeIterator* pFromIt1, size_t codeLength1,
	CodeIterator* pFromIt2, size_t codeLength2):
	codeLength(codeLength1 + codeLength2),
	arrayLength((codeLength >> SIZE_POW) + 1)
{
	CodeIterator fromIt1 = (*pFromIt1);
	CodeIterator fromIt2 = (*pFromIt2);
	this->code = new uint64_t[arrayLength];
	uint64_t currentPosition = 0;
	while (codeLength1--)
	{
		if (*fromIt1)
		{
			SET_IN_ARRAY(code, currentPosition);
		}
		++fromIt1;
		currentPosition++;
	}
	while (codeLength2--)
	{
		if (*fromIt2)
		{
			SET_IN_ARRAY(code, currentPosition);
		}
		++fromIt2;
		currentPosition++;
	}
}

Chromossome::~Chromossome()
{
	if (this->code != nullptr)
	{
		delete[] code;
	}
}

const size_t Chromossome::getLength()
{
	return this->codeLength;
}

uint64_t* Chromossome::getCode()
{
	return this->code;
}


CodeIterator Chromossome::position(uint64_t position)
{
	if (position > this->codeLength)
	{
		throw std::invalid_argument("Invalid position");
	}
	return CodeIterator(this, position);
}

CodeIterator Chromossome::begin()
{
	return CodeIterator(this, 0);
}

CodeIterator Chromossome::end()
{
	return CodeIterator(this, codeLength);
}

void Chromossome::Free()
{
	delete[] this->code;
}

/* ======================================================== ITERATOR ====================================================*/
CodeIterator::CodeIterator(Chromossome* code, size_t position) :
	code(code)
{
	this->bitPosition = position;
}

CodeIterator::CodeIterator(const CodeIterator& it):
	code(it.code)
{
	this->bitPosition = it.bitPosition;
}

CodeIterator& CodeIterator::operator=(const CodeIterator& it)
{
	this->code = it.code;
	this->bitPosition = it.bitPosition;
	return (*this);
}

CodeIterator& CodeIterator::operator++()
{
	if (this->bitPosition < this->code->getLength())
	{
		this->bitPosition++;
	}
	return (*this);
}

CodeIterator& CodeIterator::operator++(int i)
{
	if (this->bitPosition < this->code->getLength())
	{
		this->bitPosition++;
	}
	return (*this);
}
CodeIterator& CodeIterator::operator--()
{
	if (this->bitPosition > 0)
	{
		this->bitPosition--;
	}
	return (*this);
}
CodeIterator& CodeIterator::operator+=(const int value)
{
	if (this->bitPosition + value <= this->code->getLength())
	{
		this->bitPosition += value;
	}
	else
	{
		throw std::invalid_argument("Iterator out of bounds");
	}
	return (*this);
}
CodeIterator& CodeIterator::operator-=(const int value)
{
	if (this->bitPosition >= value)
	{
		this->bitPosition -= value;
	}
	else
	{
		throw std::invalid_argument("Iterator out of bounds");
	}
	return (*this);
}

BIT_VALUE CodeIterator::operator*()
{
	if (GET_FROM_ARRAY(code->getCode(), this->bitPosition) > 0)
	{
		return BIT_VALUE::ONE;
	}
	return BIT_VALUE::ZERO;
}

bool CodeIterator::operator==(const CodeIterator& it)
{
	return (this->bitPosition == it.bitPosition && this->code == it.code);
}

bool CodeIterator::operator!=(const CodeIterator& it)
{
	return !(*this == it);
}

uint64_t CodeIterator::GetBitsAsInt(size_t count)
{
	uint64_t ret = 0;
	if (this->bitPosition + count - 1 < this->code->getLength())
	{
		for (uint64_t i = 0; i < count; i++)
		{
			if (GET_FROM_ARRAY(code->getCode(), this->bitPosition + i))
			{
				SET_FROM_LOWEST(ret, i);
			}
		}
	}
	return ret;
}

void CodeIterator::SetIntAsBits(uint64_t value, size_t count)
{
	uint64_t pos = this->bitPosition;
	int v = 0;
	for (size_t i = 0; i < count; i++)
	{		
		if (GET_FROM_LOWEST(value, i) != 0)
		{
			SET_IN_ARRAY(code->getCode(), pos);
		}
		else
		{
			UNSET_IN_ARRAY(code->getCode(), pos);
		}
		pos++;
	}
}

void CodeIterator::SetBit(BIT_VALUE value)
{
	if (value == BIT_VALUE::ONE)
	{
		SET_IN_ARRAY(code->getCode(), this->bitPosition);
	}
	else
	{
		UNSET_IN_ARRAY(code->getCode(), this->bitPosition);
	}
}

void CodeIterator::FlipBit()
{
	FLIP_IN_ARRAY(code->getCode(), this->bitPosition);
}