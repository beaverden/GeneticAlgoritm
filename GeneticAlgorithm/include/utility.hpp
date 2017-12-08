#pragma once

enum BIT_VALUE
{
	ZERO = 0,
	ONE = 1
};

double randd();
uint64_t randi64();

size_t GetLength(double min, double max, size_t precision);

// supposed to be a power of two
#define SIZE_BITS sizeof(uint64_t) * CHAR_BIT
#define SIZE_POW 6 // 2 ^ 6 = 64
#define GET(x, n) ((x) & (1ULL << (sizeof(x) * CHAR_BIT - (n) - 1)))
#define SET(x, n) (((x) |= (1ULL << (sizeof(x) * CHAR_BIT - (n) - 1))))
#define UNSET(x, n) (((x) &= ~(1ULL << (sizeof(x) * CHAR_BIT - (n) - 1))))
#define FLIP(x, n) (((x) ^= (1ULL << (sizeof(x) * CHAR_BIT - (n) - 1))))


#define GET_FROM_ARRAY(arr, n)\
		(GET((arr)[(n) >> SIZE_POW], ((n) & (SIZE_BITS - 1))))
#define SET_IN_ARRAY(arr, n)\
		(SET((arr)[(n) >> SIZE_POW], ((n) & (SIZE_BITS - 1))))
#define UNSET_IN_ARRAY(arr, n)\
		(UNSET((arr)[(n) >> SIZE_POW], ((n) & (SIZE_BITS - 1))))
#define FLIP_IN_ARRAY(arr, n)\
		(FLIP((arr)[(n) >> SIZE_POW], ((n) & (SIZE_BITS - 1))))

#define SET_FROM_LOWEST(x, n)\
		((x) |= (1ULL << (n)))

#define PI 3.14159265


