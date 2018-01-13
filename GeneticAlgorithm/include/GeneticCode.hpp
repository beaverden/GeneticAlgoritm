#pragma once
#include "stdafx.hpp"
#include "utility.hpp"

class Chromossome;

class CodeIterator
{
public:
	CodeIterator(Chromossome* code, size_t position);
	CodeIterator(const CodeIterator& it);
	CodeIterator& operator=(const CodeIterator& it);
	CodeIterator& operator++();
	CodeIterator& operator++(int i);
	CodeIterator& operator--();
	CodeIterator& operator+=(const int value);
	CodeIterator& operator-=(const int value);
	BIT_VALUE operator*();
	bool operator==(const CodeIterator& it);
	bool operator!=(const CodeIterator& it);

	template <typename T> 
	T GetInt()
	{
		T ret = 0;
		if (this->bitPosition + sizeof(T) * CHAR_BIT - 1 < this->code->getLength())
		{
			for (size_t i = 0; i < sizeof(T) * CHAR_BIT; i++)
			{
				if (GET_FROM_ARRAY(code->getCode(), this->bitPosition + i))
				{
					SET(ret, i);
				}
			}
		}
		else
		{
			throw std::invalid_argument("Out of bounds");
		}
		return ret;
	}

	uint64_t GetBitsAsInt(size_t count);
	void CodeIterator::SetIntAsBits(uint64_t value, size_t count);
	void SetBit(BIT_VALUE value);
	void FlipBit();
private:
	uint64_t		bitPosition;
	Chromossome*	code;
};

class Chromossome
{
public:
	Chromossome(size_t codeLength);
	Chromossome(uint64_t* existingCode, size_t codeLength);
	Chromossome(
		CodeIterator* fromIt1, size_t codeLength1,
		CodeIterator* fromIt2, size_t codeLength2);
	Chromossome(Chromossome* ch);
	~Chromossome();
	const size_t getLength();
	uint64_t* getCode();
	
	CodeIterator position(uint64_t position);
	CodeIterator begin();
	CodeIterator end();
	void Free();
private:
	uint64_t*		code;
	size_t		codeLength;
	const size_t  arrayLength;
};