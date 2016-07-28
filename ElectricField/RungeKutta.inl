#include "stdafx.h"

#define __RK_IMPL 1


#include "RungeKutta.h"

namespace RungeKutta {

	template<typename T, unsigned int Stages> RungeKutta<T, Stages>::RungeKutta(const double weights[], const double nodes[], const double *coefficients[])
	{
		m_coefficients.reserve(Stages - 1);

		for (unsigned int stage = 0; stage < Stages; ++stage)
		{
			m_weights[stage] = weights[stage];
			m_nodes[stage] = nodes[stage];

			if (stage > 0)
			{
				m_coefficients.push_back(std::vector<double>());
				
				unsigned int coef_i = stage - 1;
				m_coefficients[coef_i].reserve(stage);

				for (unsigned int j = 0; j < stage; ++j)
				{
					m_coefficients[coef_i].push_back(coefficients[coef_i][j]);
				}
			}
		}
	}

	template<typename T, unsigned int Stages, unsigned int Order> AdaptiveRungeKutta<T, Stages, Order>::AdaptiveRungeKutta(const double weights[], const double low_order_weights[], const double nodes[], const double *coefficients[])
		: RungeKutta<T, Stages>(weights, nodes, coefficients)
	{
		for (unsigned int stage = 0; stage < Stages; ++stage)
		{
			m_low_order_weights[stage] = low_order_weights[stage];
		}
	}


	static const double EulerWeights[] = { 1 };
	static const double EulerNodes[] = { 0 };

	template<typename T> Euler<T>::Euler(void)
		: RungeKutta(EulerWeights, EulerNodes, NULL)
	{
	}




	static const double MidpointWeights[] = { 0, 1 };
	static const double MidpointNodes[] = { 0, 1. / 2. };

	static const double row[] = { 1. / 2. };
	static const double *MidpointCoeff[] = { row };

	template<typename T> Midpoint<T>::Midpoint(void)
		: RungeKutta(MidpointWeights, MidpointNodes, MidpointCoeff)
	{
	}




	static const double RalstonWeights[] = { 1. / 4., 3. / 4. };
	static const double RalstonNodes[] = { 0, 2. / 3. };

	static const double rrow[] = { 2. / 3. };
	static const double *RalstonCoeff[] = { rrow };

	template<typename T> Ralston<T>::Ralston(void)
		: RungeKutta(RalstonWeights, RalstonNodes, RalstonCoeff)
	{
	}



	static const double HeunWeights[] = { 1. / 2., 1. / 2. };
	static const double HeunNodes[] = { 0, 1 };

	static const double hrow[] = { 1 };
	static const double *HeunCoeff[] = { hrow };

	template<typename T> Heun<T>::Heun(void)
		: RungeKutta(HeunWeights, HeunNodes, HeunCoeff)
	{
	}





	static const double RK4weights[] = { 1. / 6., 1. / 3., 1. / 3., 1. / 6. };
	static const double RK4nodes[] = { 0, 1. / 2., 1. / 2., 1. };

	static const double row1[] = { 1. / 2. };
	static const double row2[] = { 0, 1. / 2. };
	static const double row3[] = { 0, 0, 1 };
	static const double *RK4coeff[] = { row1, row2, row3 };

	template<typename T> RK4<T>::RK4(void)
		: RungeKutta(RK4weights, RK4nodes, RK4coeff)
	{
	}





	static const double RK3per8weights[] = { 1. / 8., 3. / 8., 3. / 8., 1. / 8. };
	static const double RK3per8nodes[] = { 0, 1. / 3., 2. / 3., 1. };

	static const double RK3per8row1[] = { 1. / 3. };
	static const double RK3per8row2[] = { -1. / 3., 1. };
	static const double RK3per8row3[] = { 1, -1, 1 };
	static const double *RK3per8coeff[] = { RK3per8row1, RK3per8row2, RK3per8row3 };

	template<typename T> RK3per8<T>::RK3per8(void)
		: RungeKutta(RK3per8weights, RK3per8nodes, RK3per8coeff)
	{
	}






	static const double EulerWeightsExt[] = { 1, 0 };

	template<typename T> AdaptiveHeunEuler<T>::AdaptiveHeunEuler(void)
		:   AdaptiveRungeKutta(HeunWeights, EulerWeightsExt, HeunNodes, HeunCoeff)
	{
	}



	static const double BSweightsHigh[] = { 2./9., 1./3., 4./9., 0};
	static const double BSweightsLow[] = { 7. / 24., 1. / 4., 1. / 3., 1. / 8. };
	static const double BSnodes[] = { 0, 1./2., 3./4., 1 };

	static const double BSrow1[] = {1./2.};
	static const double BSrow2[] = { 0, 3./4. };
	static const double BSrow3[] = { 2./9., 1./3., 4./9. };
	static const double *BSCoeff[] = { BSrow1, BSrow2, BSrow3 };

	template<typename T> AdaptiveBogackiShampine<T>::AdaptiveBogackiShampine(void)
		: AdaptiveRungeKutta(BSweightsHigh, BSweightsLow, BSnodes, BSCoeff)
	{
	}



	static const double CKweightsHigh[] = { 37./378., 0, 250./621., 125./594., 0, 512./1771. };
	static const double CKweightsLow[] = { 2825./27648., 0, 18575./48384., 13525./55296., 277./14336., 1./4. };

	static const double CKnodes[] = { 0, 1./5., 3./10., 3./5., 1., 7./8. };

	static const double CKrow1[] = { 1. / 5. };
	static const double CKrow2[] = { 3./40, 9./40.};
	static const double CKrow3[] = { 3./10., -9./10., 6./5. };
	static const double CKrow4[] = { -11./54., 5./2., -70./27., 35./27. };
	static const double CKrow5[] = { 1631./55296., 175./512., 575./13824., 44275./110592., 253./4096.};
	static const double *CKCoeff[] = { CKrow1, CKrow2, CKrow3, CKrow4, CKrow5 };


	template<typename T> AdaptiveCashKarp<T>::AdaptiveCashKarp(void)
		: AdaptiveRungeKutta(CKweightsHigh, CKweightsLow, CKnodes, CKCoeff)
	{
	}




	static const double RKFweightsHigh[] = { 16./135., 0, 6656./12825., 28561./56430., -9./50., 2./55. };
	static const double RKFweightsLow[] = { 25./216., 0, 1408./2565., 2197./4104., -1./5., 0 };
	//static const double RKFweightsLow[] = { -1./360., 0, 128./4275., 2197./75240.,-1./50.,-2./55.};
	static const double RKFnodes[] = { 0, 1./4., 3./8., 12./13., 1, 1./2. };

	static const double RKFrow1[] = { 1./4. };
	static const double RKFrow2[] = { 3./32., 9./32. };
	static const double RKFrow3[] = { 1932./2197., -7200./2197., 7296./2197. };
	static const double RKFrow4[] = { 439./216., -8, 3680./513., -845./4104. };
	static const double RKFrow5[] = { -8./27., 2, -3544./2565., 1859./4104., -11./40. };
	static const double *RKFCoeff[] = { RKFrow1, RKFrow2, RKFrow3, RKFrow4, RKFrow5 };

	template<typename T> AdaptiveFehlberg<T>::AdaptiveFehlberg(void)
		: AdaptiveRungeKutta(RKFweightsHigh, RKFweightsLow, RKFnodes, RKFCoeff)
	{
	}


	
	static const double DPweightsHigh[] = { 35./384., 0, 500./1113., 125./192., -2187./6784., 11./84., 0 };
	static const double DPweightsLow[] = { 71. / 57600., 0, -71. / 16695., 71./ 1920., -17253. / 339200., 22. / 525., -1. / 40. };
	static const double DPnodes[] = { 0, 1./5., 3./10., 4./5., 8./9., 1, 1 };

	static const double DProw1[] = { 1./5. };
	static const double DProw2[] = { 3./40., 9./40. };
	static const double DProw3[] = { 44./45., -56./15., 32./9. };
	static const double DProw4[] = { 19372./6561., -25360./2187., 64448./6561., -212./729. };
	static const double DProw5[] = { 9017./3168., -355./33., 46732./5247., 49./176., -5103./18656. };
	static const double DProw6[] = { 35. / 384., 0, 500. / 1113., 125. / 192., -2187. / 6784., 11. / 84. };
	static const double *DPCoeff[] = { DProw1, DProw2, DProw3, DProw4, DProw5, DProw6 };

	template<typename T> AdaptiveDormandPrince<T>::AdaptiveDormandPrince(void)
		: AdaptiveRungeKutta(DPweightsHigh, DPweightsLow, DPnodes, DPCoeff)
	{
	}
	
}