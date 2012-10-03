/**
 * Bool
 */

struct True;
struct False;

template<typename B> struct Not;

template<> struct Not<True>
{
	typedef False result;
};

template<> struct Not<False>
{
	typedef True result;
};

template<typename A, typename B> struct AndOp
{
	typedef False result;
};

template<> struct AndOp<True,True>
{
	typedef True result;
};

template<typename A, typename B> struct OrOp
{
	typedef True result;
};

template<> struct OrOp<False,False>
{
	typedef False result;
};

template<typename A, typename B> struct Eq;

/**
 * Arithmetic
 */

struct Zero;
template<typename N> struct Dec;
template<typename N> struct Inc;

template<typename A, typename B> struct Add;
template<typename A, typename B> struct Sub;
template<typename A, typename B> struct Mul;
template<typename A, typename B> struct Div;
template<typename A, typename B> struct Mod;

/**
 * Integer
 */

template<int N> struct Int
{
	enum { value = N };
};

template<int N, int M> struct Add<Int<N>,Int<M>>
{
	typedef Int<N+M> result;
};

template<int N, int M> struct Sub<Int<N>,Int<M>>
{
	typedef Int<N-M> result;
};

template<int N, int M> struct Mul<Int<N>,Int<M>>
{
	typedef Int<N*M> result;
};

template<int N, int M> struct Div<Int<N>,Int<M>>
{
	typedef Int<N/M> result;
};

template<int N, int M> struct Mod<Int<N>,Int<M>>
{
	typedef Int<N%M> result;
};

template<> struct Dec<Int<1>>
{
	typedef Zero result;
};

template<int N> struct Dec<Int<N>>
{
	typedef Int<N-1> result;
};

template<> struct Inc<Zero>
{
	typedef Int<1> result;
};

template<int N> struct Inc<Int<N>>
{
	typedef Int<N+1> result;
};

template<int N> struct Eq<Int<N>,Int<N>>
{
	typedef True result;
};

template<int N, int M> struct Eq<Int<N>,Int<M>>
{
	typedef False result;
};

template<typename N> struct Even
{
	typedef typename Eq<typename Mod<N,Int<2>>::result,Int<0>>::result result;
};

/**
 * Id
 */

template<typename A> struct Id
{
	typedef A result;
};

/**
 * Tuples
 */

template<typename A, typename B> struct Tuple;

template<typename T> struct Fst;
template<typename A, typename B> struct Fst<Tuple<A,B>>
{
	typedef A result;
};

template<typename T> struct Snd;
template<typename A, typename B> struct Snd<Tuple<A,B>>
{
	typedef B result;
};

template<template<typename,typename> class UCF> struct UncurryF
{
	template<typename T> struct F;
	template<typename A, typename B> struct F<Tuple<A,B>>
	{
		typedef typename UCF<A,B>::result result;
	};
};

/**
 * List
 */

struct Empty;
template<typename X, typename XS> struct List;

template<typename XS> struct Head;

template<> struct Head<Empty>
{
	typedef Empty result;
};

template<typename X, typename XS> struct Head<List<X,XS>>
{
	typedef X result;
};

template<typename XS> struct Tail;

template<> struct Tail<Empty>
{
	typedef Empty result;
};

template<typename X, typename XS> struct Tail<List<X,XS>>
{
	typedef XS result;
};

/**
 * Add lists
 */

template<> struct Add<Empty,Empty>
{
	typedef Empty result;
};

template<typename YS> struct Add<Empty,YS>
{
	typedef YS result;
};

template<typename XS> struct Add<XS,Empty>
{
	typedef XS result;
};

template<typename X, typename XS, typename Y, typename YS>
struct Add<List<X,XS>,List<Y,YS>>
{
	typedef List<X,typename Add<XS,List<Y,YS>>::result> result;
};

/**
 * Length
 */

template<typename XS> struct Length;

template<> struct Length<Empty>
{
	typedef Zero result;
};

template<typename X, typename XS> struct Length<List<X,XS>>
{
	typedef typename Inc<typename Length<XS>::result>::result result;
};

/**
 * IterateN
 */

template<typename N, template<typename> class F, typename A> struct IterateN;

template<template<typename> class F, typename A> struct IterateN<Zero,F,A>
{
	typedef Empty result;
};

template<typename N, template<typename> class F, typename A> struct IterateN
{
	typedef List<A,typename IterateN<
		typename Dec<N>::result,F,typename F<A>::result>::result> result;
};

/**
 * Replicate
 */

template<typename N, typename A> struct Replicate
{
	typedef typename IterateN<N,Id,A>::result result;
};

/**
 * Zip
 */

template<typename XS, typename YS> struct Zip;

template<> struct Zip<Empty,Empty>
{
	typedef Empty result;
};

template<typename XS> struct Zip<XS,Empty>
{
	typedef Empty result;
};

template<typename YS> struct Zip<Empty,YS>
{
	typedef Empty result;
};

template<typename X, typename XS, typename Y, typename YS>
struct Zip<List<X,XS>,List<Y,YS>>
{
	typedef List<Tuple<X,Y>,typename Zip<XS,YS>::result> result;
};

/**
 * Select
 */

template<typename B, typename T, typename F> struct Select;

template<typename T, typename F> struct Select<True,T,F>
{
	typedef T result;
};

template<typename T, typename F> struct Select<False,T,F>
{
	typedef F result;
};

/**
 * Foldl
 */

template<template<typename,typename> class F, typename Z, typename XS>
struct Foldl;

template<template<typename,typename> class F, typename Z>
struct Foldl<F,Z,Empty>
{
	typedef Z result;
};

template<
template<typename,typename> class F, typename Z, typename X, typename XS>
struct Foldl<F,Z,List<X,XS>>
{
	typedef typename Foldl<F,typename F<Z,X>::result,XS>::result result;
};

/**
 * Map
 */

template<template<typename> class F, typename XS> struct Map;

template<template<typename> class F> struct Map<F, Empty>
{
	typedef Empty result;
};

template<template<typename> class F, typename X, typename XS>
struct Map<F,List<X,XS>>
{
	typedef List<typename F<X>::result,typename Map<F,XS>::result> result;
};

/**
 * Filter
 */

template<template<typename> class P, typename XS> struct Filter;

template<template<typename> class P> struct Filter<P,Empty>
{
	typedef Empty result;
};

template<template<typename> class P, typename X, typename XS>
struct Filter<P,List<X,XS>>
{
	typedef typename P<X>::result b;
	typedef typename Filter<P,XS>::result rest;
	typedef typename Select<b,List<X,rest>,rest>::result result;
};

/**
 * Or
 */

template<typename BS> struct Or
{
	typedef typename Foldl<OrOp,False,BS>::result result;
};

/**
 * Any
 */

template<template<typename> class P, typename XS> struct Any
{
	typedef typename Or<typename Map<P,XS>::result>::result result;
};

/**
 * And
 */

template<typename BS> struct And
{
	typedef typename Foldl<AndOp,True,BS>::result result;
};

/**
 * All
 */

template<template<typename> class P, typename XS> struct All
{
	typedef typename And<typename Map<P,XS>::result>::result result;
};

/**
 * Sum
 */

template<typename XS> struct Sum
{
	typedef typename Foldl<Add,Int<0>,XS>::result result;
};

/**
 * Graph
 */

template<typename ES, typename VS> struct Graph;

template<typename G> struct Edges;
template<typename ES, typename VS> struct Edges<Graph<ES,VS>>
{
	typedef ES result;
};

template<typename G> struct Vertices;
template<typename ES, typename VS> struct Vertices<Graph<ES,VS>>
{
	typedef VS result;
};

template<typename VS> struct CompleteGraph;

template<> struct CompleteGraph<Empty>
{
	typedef Graph<Empty,Empty> result;
};

template<typename V> struct CompleteGraph<List<V,Empty>>
{
	typedef Graph<Empty,V> result;
};

template<typename V, typename VS> struct CompleteGraph<List<V,VS>>
{
	typedef Graph<
		typename Add<
			typename Zip<
				typename Replicate<typename Length<VS>::result,V>::result,
				VS
			>::result,
			typename Edges<typename CompleteGraph<VS>::result>::result
		>::result,
		List<V,VS>
	> result;
};
