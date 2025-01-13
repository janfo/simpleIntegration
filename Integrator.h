


#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#include <initializer_list>
#include <functional>
#include <tuple>
#include <assert.h>

#ifndef _H_INTEGRATOR
#define  _H_INTEGRATOR

/*Uncomment to see the traces*/
//#define ENABLE_TRACES

template <typename L>
struct Logger
{


	static void  print(std::string&& msg, L&& var) {

		std::cout << std::forward<decltype(msg)>(msg) << ": " << std::forward<decltype(var)>(var) << std::endl;

	}

};

template <class T, class ...Args>
class Integrator {

public:

	Integrator(std::function<T(Args...)> expressionFunMulDimen, long int nrOfDiscPts, std::initializer_list<T> intScopes);

	Integrator& operator=(Integrator& integrator) = delete;
	Integrator& operator=(Integrator&& integrator) = delete;


	//integrate number of dimensions you want
	T integrate();

private:
	T loopDimensions2(T sum, std::vector <T> vec, std::vector <T> h_vec, const int currArg);
	T doSum(T x, T cum);

	T getFunValue(T x);
	T calcInteg();



	std::function<T(Args...)> expressionFunMulDimen;

	std::vector<T> m_varParamValues;
	std::vector<T> m_intScopes;


	T lb = { 0 };
	T ub = { 0 };
	long int nrOfDiscPts;

};



template <typename T, std::size_t... Indices>
auto vectorToTupleHelper(const std::vector<T>& v, std::index_sequence<Indices...>) {
	return std::make_tuple(v[Indices]...);
}

template <std::size_t N, typename T>
auto vectorToTuple(const std::vector<T>& v) {
	assert(v.size() >= N);
	return vectorToTupleHelper(v, std::make_index_sequence<N>());
}

template<int ...> struct seq {};
template<int N, int ...S> struct gens : gens<N - 1, N - 1, S...> { };

template<int ...S> struct gens<0, S...> { typedef seq<S...> type; };


template <typename X, typename ...TupArgs>
struct save_it_for_later
{
	std::tuple<TupArgs...> params;
	//double(*func)(Args...) = &foo;

	std::function<X(TupArgs...)> expressionFunMulDimen;


	X delayed_dispatch(std::function<X(TupArgs...)> expressionFunMulDimen)
	{
		this->expressionFunMulDimen = expressionFunMulDimen;

		return callFunc(typename gens<sizeof...(TupArgs)>::type()); // Item #1
	}

	template<int ...S>
	X callFunc(seq<S...>)
	{
		return expressionFunMulDimen(std::get<S>(params) ...);
	}
};




template  <class T, class ...Args> T Integrator <T, Args... >::loopDimensions2
(T sum, std::vector <T> vec, std::vector <T> h_vec, const int currArg) {


	T tmp = 0.0;

	// including 0 element plus the last N so <=
	for (long int i = 0; i <= this->nrOfDiscPts; i++)
	{


		if (currArg == 1 || currArg < vec.size()) {

			tmp = loopDimensions2(0, vec, h_vec, currArg + 1);

			if (i != 0 && i != this->nrOfDiscPts)
				sum += static_cast<T>(2) * tmp;
			vec[currArg - 1] += h_vec[currArg - 1];
		}
		else {

			std::tuple<Args...> t1 = vectorToTuple<sizeof...(Args), T>(vec);
			save_it_for_later<T, Args...> saved_sum_part1 = { t1 };
			//std::cout << "Wywlanie funkcji podcalkowej: " << saved_sum_part1.delayed_dispatch(this->expressionFunMulDimen) << std::endl;
			sum += saved_sum_part1.delayed_dispatch(this->expressionFunMulDimen);

			std::vector<T> tmp_vec = vec;
			tmp_vec[currArg - 1] = tmp_vec[currArg - 1] + h_vec[currArg - 1];
			std::tuple<Args...> t2 = vectorToTuple<sizeof...(Args), T>(tmp_vec);
			save_it_for_later<T, Args...> saved_sum_part2 = { t2 };
			//std::cout << "Wywlanie funkcji podcalkowej: " << saved_sum_part2.delayed_dispatch(this->expressionFunMulDimen) << std::endl;
			sum += saved_sum_part2.delayed_dispatch(this->expressionFunMulDimen);

			vec[currArg - 1] += h_vec[currArg - 1];


		}



	}
	return sum;

}




template  <class T, class ...Args>   T  Integrator <T, Args... >::integrate() {

	std::vector<T> startingPoints;


	int sizeArgs = sizeof ...(Args);
#ifdef ENABLE_TRACES
	Logger<int&> logInt;
	logInt.print("Nr of arguments ", sizeArgs);
#endif

	std::vector <T> h_vec = {};

	for (int i = 0; i < this->m_intScopes.size(); i += 2)
	{
		T step = (this->m_intScopes[i + 1] - this->m_intScopes[i]) / static_cast<T>(this->nrOfDiscPts);
#ifdef ENABLE_TRACES
		Logger<T&> logT;
		logT.print("Integrals step ", step);
#endif
		h_vec.push_back(step);
		startingPoints.push_back(this->m_intScopes[i]);
	}
#ifdef ENABLE_TRACES
	Logger<T&> logT;
#endif
	//working exmaple uncomment to see
	//auto sum = loopDimensions(0.0, 1, { 3.0,0.0 }, h, 1, arg1, par...);
	//std::tuple<Args...> ppar = vectorToTuple<(sizeof...(Args)+1), double>(startingPoints);

	T result;

	if (sizeof...(Args) == 1) {
		this->lb = this->m_intScopes[0];
		this->ub = this->m_intScopes[1];
		result = calcInteg();
#ifdef ENABLE_TRACES
		logT.print("1D integral value ", result);
#endif
		;
	}
	else {
		T sum = loopDimensions2(0.0, startingPoints, h_vec, 1);

		T h_multot = 1.0;
		T k = 1.0;
		for (auto& it : h_vec) {
			h_multot *= it;
			k *= 0.5;
		}

		T compute_result = static_cast<T>(k * h_multot * sum);

		result = compute_result;
#ifdef ENABLE_TRACES
		logT.print("Computed Integral value ", compute_result);
#endif

	}

	return result;
}



template  <class T, class ...Args>  Integrator <T, Args...>::Integrator(
	std::function<T(Args...)> expressionFunMulDimen, long int nrOfDiscPts,
	std::initializer_list<T> intScopes)

	:expressionFunMulDimen(expressionFunMulDimen),
	nrOfDiscPts(nrOfDiscPts),
	m_intScopes(intScopes)

{
	//std::cout << sizeof...(Args) << std::endl;
#ifdef ENABLE_TRACES

	for (auto& scope : this->m_intScopes)
	{
		using ScopeType = decltype(scope);
		Logger<ScopeType> logScope;
		logScope.print("Intergral's scope", scope);
	}
	using PtsType = decltype(this->nrOfDiscPts);
	Logger<PtsType&> logPts;
	logPts.print("Nr of discrete points per grid", this->nrOfDiscPts);
#endif

}





template  <class T, class ...Args>T Integrator<T, Args...>::getFunValue(T x) {
	return this->expressionFun(x);
}

template <class T, class ...Args> T Integrator<T, Args...>::doSum(T x, T cum) {

	T h = (this->ub - this->lb) / static_cast<T>(this->nrOfDiscPts);

	for (int i = 0; i <= this->nrOfDiscPts; i++)
	{

		std::vector <T > vec1 = { x };
		std::vector <T > vec2 = { x + h };

		std::tuple<Args...> part1 = vectorToTuple<sizeof...(Args), T>(vec1);
		save_it_for_later<T, Args...> saved_sum_part1 = { part1 };
		cum += saved_sum_part1.delayed_dispatch(this->expressionFunMulDimen);

		std::tuple<Args...> part2 = vectorToTuple<sizeof...(Args), T>(vec2);
		save_it_for_later<T, Args...> saved_sum_part2 = { part2 };
		cum += saved_sum_part2.delayed_dispatch(this->expressionFunMulDimen);


		//cum += this->expressionFunMulDimen(x) + this->expressionFunMulDimen(x + h);
		x += h;
		//std::cout << "X: " << x << "Y:" << expressionFun(x) << "Cumm: " << cum << std::endl;
	}
	return cum;
}
template <class T, class ...Args> T Integrator<T, Args...>::calcInteg() {


	T sum = static_cast<T>(0.5 * (this->ub - this->lb) / static_cast<T>(this->nrOfDiscPts)) * doSum(this->lb, 0.0);

	return sum;

}
#endif