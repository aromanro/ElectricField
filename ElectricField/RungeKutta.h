#pragma once

#include <vector>
#include <valarray>
#include <array>

#define sign(x) ((x) >= 0 ? 1 : -1)

namespace RungeKutta {

	template<typename T, unsigned int Stages> class RungeKutta
	{
	protected:
		std::array<double, Stages> m_weights;
		std::array<double, Stages> m_nodes;
		std::vector<std::valarray<double>> m_coefficients;
		
	public:
		RungeKutta(const double weights[], const double nodes[], const double *coefficients[]);

		template<typename Func> inline T SolveStep(Func& Function, const T& curVal, double t, double h) const {
				T thesum(0); 
				std::array<T, Stages> K;

				for (unsigned int stage = 0; stage < Stages; ++stage)
				{                                                
					T accum(0);                                     
					for (unsigned int j = 0; j < stage; ++j) accum += m_coefficients[stage - 1][j] * K[j];  

					K[stage] = Function(t + m_nodes[stage] * h, curVal + h * accum);
					
					thesum += m_weights[stage] * K[stage];
				} 
				
				return curVal + h * thesum;
			}

		template<typename Func> inline T SolveStep(Func& Function, const T& curVal, double t, double& h, double& /*next_h*/, double /*tolerance*/, double /*max_step*/ = DBL_MAX, double /*min_step*/ = DBL_MIN) const {
			//next_h = h;
			return SolveStep(Function, curVal, t, h);
		}

		bool IsAdaptive() const { return false; }
	};



	template<typename T, unsigned int Stages, unsigned int Order> class AdaptiveRungeKutta : public RungeKutta<T, Stages> {
	protected:
		std::array<double, Stages> m_low_order_weights;

	public:
		AdaptiveRungeKutta(const double weights[], const double low_order_weights[], const double nodes[], const double *coefficients[]);
	
		template<typename Func> inline T SolveStep(Func& Function, const T& curVal, double t, double& h, double& next_h, double tolerance, double max_step = DBL_MAX, double min_step = DBL_MIN) const {			
			std::array<T, Stages> K;			
			
			for (unsigned int loop = 0;; ++loop) {
				T thesumHigh(0), thesumLow(0);  

				for (unsigned int stage = 0; stage < Stages; ++stage)
				{                                                
					T accum(0);                                     					
					for (unsigned int j = 0; j < stage; ++j) accum += RungeKutta<T, Stages>::m_coefficients[stage - 1][j] * K[j];
					
					K[stage] = Function(t + h * RungeKutta<T, Stages>::m_nodes[stage], curVal + h * accum);

					thesumHigh += RungeKutta<T, Stages>::m_weights[stage] * K[stage];   
					thesumLow  += m_low_order_weights[stage] * K[stage];  
				}

				const double error = abs((thesumHigh - thesumLow) * h);

				if (error <= tolerance || loop > 1000 || abs(h) <= min_step) {
					T result = curVal + h * thesumHigh;
					
					if (error <= DBL_MIN) next_h = h * 2.;
					else if (error < tolerance)
						next_h = h * 0.9 * pow(tolerance / error, 1. / (Order+1));
					else next_h = h;

					if (abs(next_h) < min_step) next_h = min_step*sign(next_h);
					else if (abs(next_h) > max_step) next_h = max_step*sign(next_h);

					return result;
				}

				// decrease the step size
				h *= 0.9 * pow(tolerance / error, 1. / Order);
			} 
		}

		bool IsAdaptive() const { return true; }
	};


	template<typename T> class Euler : public RungeKutta<T, 1>
	{
	public:
		Euler();
	};

	template<typename T> class Midpoint : public RungeKutta<T, 2>
	{
	public:
		Midpoint();
	};

	template<typename T> class Heun : public RungeKutta<T, 2>
	{
	public:
		Heun();
	};

	template<typename T> class Ralston : public RungeKutta<T, 2>
	{
	public:
		Ralston();
	};

	template<typename T> class RK4 : public RungeKutta<T, 4>
	{
	public:
		RK4();
	};

	template<typename T> class RK3per8 : public RungeKutta<T, 4>
	{
	public:
		RK3per8();
	};


	template<typename T> class AdaptiveHeunEuler : public AdaptiveRungeKutta<T, 2, 2>
	{
	public:
		AdaptiveHeunEuler();
	};

	template<typename T> class AdaptiveBogackiShampine : public AdaptiveRungeKutta<T, 4, 3>
	{
	public:
		AdaptiveBogackiShampine();
	};

	template<typename T> class AdaptiveCashKarp : public AdaptiveRungeKutta<T,6,5>
	{
	public:
		AdaptiveCashKarp();
	};

	template<typename T> class AdaptiveFehlberg : public AdaptiveRungeKutta<T,6,5>
	{
	public:
		AdaptiveFehlberg();
	};

	
	template<typename T> class AdaptiveDormandPrince : public AdaptiveRungeKutta<T,7,5>
	{
	public:
		AdaptiveDormandPrince();
	};
	
}


#ifndef  __RK_IMPL
#include "RungeKutta.inl"
#endif // ! __RK_IMPL
