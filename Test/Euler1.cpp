#include <iostream>

#include "FunTs.h"

template<typename L, typename N> struct Num
{
	typedef
		typename Div<
			typename Sub<
				L,
				Int<1>
			>::result,
			N
		>::result
	result;
};

template<typename N> struct SumN
{
	typedef
		typename Div<
			typename Mul<
				N,
				typename Add<
					N,
					Int<1>
				>::result
			>::result,
			Int<2>
		>::result
	result;
};

template<typename L, typename N> struct SumLN
{
	typedef
		typename Mul<
			typename SumN<typename Num<L,N>::result>::result,
			N
		>::result
	result;
};

template<typename L> struct SumLNF
{
	template<typename N> struct F
	{
		typedef typename SumLN<L,N>::result result;
	};
};

template<typename N, typename M> struct Divisible
{
	typedef typename Eq<typename Mod<N,M>::result,Int<0>>::result result;
};

template<typename DS> struct DivisibleAnyF
{
	template<typename N> struct F
	{
		template<typename D> struct DivisibleN
		{
			typedef typename Divisible<N,D>::result result;
		};
		typedef typename Any<DivisibleN,DS>::result result;
	};
};

int main(int argc, char** argv)
{
	// input
	#define LIMIT Int<1000>
	#define DIVS List<Int<3>,List<Int<5>,Empty>>

	/**
	 * Solution 1, only works for n=2 but it's pretty so I leave it.
	 */

	// add up sums
	#define SUMS Map<SumLNF<LIMIT>::F,DIVS>::result
	#define SUM Sum<SUMS>::result

	// find numbers generating duplicates
	#define PAIRS Edges<CompleteGraph<DIVS>::result>::result
	#define DUPES Map<UncurryF<Mul>::F,PAIRS>::result

	// add up sums from numbers generating duplicates
	#define SUB_SUMS Map<SumLNF<LIMIT>::F,DUPES>::result
	#define SUB_SUM Sum<SUB_SUMS>::result

	#define RESULT1 Sub<SUM,SUB_SUM>::result

	/**
	 * Solution 2, O(n*m) where n = length of list and m = limit.
	 */

	#define NUMS IterateN<Dec<LIMIT>::result,Inc,Int<1>>::result
	#define NUMS_FILTERED Filter<DivisibleAnyF<DIVS>::F,NUMS>::result
	#define RESULT2 Sum<NUMS_FILTERED>::result

	std::cout << RESULT1::value << std::endl;
	std::cout << RESULT2::value << std::endl;
	return 0;
}
