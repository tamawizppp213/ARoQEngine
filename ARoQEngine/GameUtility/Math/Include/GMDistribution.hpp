//////////////////////////////////////////////////////////////////////////////////
///             @file   GMDistribution.hpp
///             @brief  Game Math Distribution (random, normal distribution..)
///             @author Toide Yutaro
///             @date   2021_02_15
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_DISTRIBUTION_HPP
#define GM_DISTRIBUTION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <random>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////


namespace gm
{

	/****************************************************************************
	*				  			Random 
	*************************************************************************//**
	*  @class     Random 
	*  @brief     Produces a random value of a real number.(double, long double..)
	*****************************************************************************/
	template<typename T>
	class Random 
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T GetRandomValue()
		{
			return _uniformDist(_engine);
		}

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetRange(T min, T max)
		{
			_uniformDist = std::uniform_real_distribution<T>(min, max);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Random()
		{
			std::mt19937 engine(_seed());
			_engine      = engine;
			_uniformDist = std::uniform_real_distribution<T>(0.0f, 1.0f);
		}

		Random(T min, T max)
		{
			std::mt19937 engine(_seed());
			_engine      = engine;
			_uniformDist = std::uniform_real_distribution<T>(min, max);
		}
		~Random() {};
		
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		std::random_device _seed;
		std::mt19937       _engine;
		std::uniform_real_distribution<T> _uniformDist;
	};

	/****************************************************************************
	*				  			RandomInt
	*************************************************************************//**
	*  @class     RandomInt
	*  @brief     Produces a random value of a integer number.
	*****************************************************************************/
	class RandomInt
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		int GetRandomValue()
		{
			return _uniformDist(_engine);
		}

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetRange(int min, int max)
		{
			_uniformDist = std::uniform_int_distribution<>(min, max);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RandomInt()
		{
			std::mt19937 engine(_seed());
			_engine = engine;
			_uniformDist = std::uniform_int_distribution<>(0, 1);
		}
		RandomInt(int min, int max)
		{
			std::mt19937 engine(_seed());
			_engine = engine;
			_uniformDist = std::uniform_int_distribution<int>(min, max);
		}
		~RandomInt() {};

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		std::random_device _seed;
		std::mt19937       _engine;
		std::uniform_int_distribution<> _uniformDist;
	};


	/****************************************************************************
	*				  			Distribution
	*************************************************************************//**
	*  @class     Distribution
	*  @brief     Produces a random value of a integer number.
	*****************************************************************************/
	template<typename T>
	class Distribution
	{
	public:

		Distribution()
		{
			std::mt19937 engine(_seed());
			this->_engine = engine;
		}

	protected:
		std::random_device _seed;
		std::mt19937       _engine;

	};

	/****************************************************************************
	*				  			Bernoulli Distribution
	*************************************************************************//**
	*  @class     Bernoulli_Distribution : public Distribution<double>
	*  @brief     BernoulliDistribution (p: true, 1-p: false)
	*  @param[in] double probability (0.0 ~ 1.0)
	*  @details   return true or false (situation: coin flipping)
	*****************************************************************************/
	class Bernoulli_Distribution : public Distribution<double>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool GetSampling() { return _distribution(this->_engine); }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(double probability)
		{
			if (0.0 > probability) { probability = 0.0; }
			if (1.0 < probability) { probability = 1.0; }
			_distribution = std::bernoulli_distribution(probability);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Bernoulli_Distribution() { _distribution = std::bernoulli_distribution(0.5); }
		Bernoulli_Distribution(double probability)
		{
			if (0.0 > probability) { probability = 0.0; }
			if (1.0 < probability) { probability = 1.0; }
			_distribution = std::bernoulli_distribution(probability);
		}
		~Bernoulli_Distribution() {};

	private:
		std::bernoulli_distribution _distribution;
	};


	/****************************************************************************
	*				  			Binominal_Distribution
	*************************************************************************//**
	*  @class     Binominal_Distribution : public Distribution<int>
	*  @brief     BernoulliDistribution (p: true, 1-p: false)
	*  @param[in] int count (total)
	*  @param[in] double probability (0.0~1.0)
	*  @details   Perform the event with probability p of success n times and find the number of times it succeeded.
	*****************************************************************************/
	class Binominal_Distribution : public Distribution<int>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		int GetSampling() { return _distribution(this->_engine); }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(int count, double probability)
		{
			if (count < 0)         { count       = 0;  }
			if (0.0 > probability) { probability = 0.0;}
			if (1.0 < probability) { probability = 1.0;}
			_distribution = std::binomial_distribution(count, probability);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Binominal_Distribution() { _distribution = std::binomial_distribution(1, 0.5); }
		Binominal_Distribution(int count, double probability)
		{
			if (count < 0) { count = 0; }
			if (0.0 > probability) { probability = 0.0; }
			if (1.0 < probability) { probability = 1.0; }
			_distribution = std::binomial_distribution(count, probability);
		}
		~Binominal_Distribution() {};

	private:
		std::binomial_distribution<> _distribution;
	};

	/****************************************************************************
	*				  			Negative_Binominal_Distribution
	*************************************************************************//**
	*  @class     Negative_Binominal_Distribution : public Distribution<int>
	*  @brief     NegativeBernoulliDistribution (p: true, 1-p: false)
	*  @param[in] int successCount
	*  @param[in] double probability (0.0~1.0)
	*  @details   Get the number of times an event that succeeds with probability p fails before it succeeds k times.
	*****************************************************************************/
	class Negative_Binominal_Distribution : public Distribution<int>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		int GetSampling() { return _distribution(this->_engine); }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(int successCount, double probability)
		{
			if (successCount < 0) { successCount = 0; }
			if (0.0 > probability) { probability = 0.0; }
			if (1.0 < probability) { probability = 1.0; }
			_distribution = std::negative_binomial_distribution(successCount, probability);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Negative_Binominal_Distribution() { _distribution = std::negative_binomial_distribution(1, 0.5); }
		Negative_Binominal_Distribution(int count, double probability)
		{
			if (count < 0) { count = 0; }
			if (0.0 > probability) { probability = 0.0; }
			if (1.0 < probability) { probability = 1.0; }
			_distribution = std::negative_binomial_distribution(count, probability);
		}
		~Negative_Binominal_Distribution() {};

	private:
		std::negative_binomial_distribution<> _distribution;
	};

	/****************************************************************************
	*				  			Geometric_Distribution
	*************************************************************************//**
	*  @class     Geometric_Distribution : public Distribution<T>
	*  @brief     Geometric_Distribution
	*  @param[in] int count
	*  @param[in] double probability (0.0~1.0)
	*  @details p(x | É ) = exp(-É )É ^i / (i!) \n
	*           situation: Conduct a lottery and find the number of times it takes to draw a winning ticket.\n
	*           Ask for the number of units that failed to produce an unreliable part.
	*****************************************************************************/
	class Geometric_Distribution : public Distribution<int>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		double GetSampling() { return _distribution(this->_engine); }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(double probability)
		{
			if (0.0 > probability) { probability = 0.0; }
			if (1.0 < probability) { probability = 1.0; }
			_distribution = std::geometric_distribution(probability);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Geometric_Distribution() { _distribution = std::geometric_distribution(0.5); }
		Geometric_Distribution(double probability)
		{
			if (0.0 > probability) { probability = 0.0; }
			if (1.0 < probability) { probability = 1.0; }
			_distribution = std::geometric_distribution(probability);
		}
		~Geometric_Distribution() {};

	private:
		std::geometric_distribution<> _distribution;
	};

	/****************************************************************************
	*				  			Poisson_Distribution
	*************************************************************************//**
	*  @class     Poisson_Distribution : public Distribution<T>
	*  @brief     Poisson_Distribution
	*  @param[in] double mean
	*  @details p(x | É ) = exp(-É )É ^i / (i!)
	*           situation: Number of accidents per hour at a given intersection
	*****************************************************************************/
	class Poisson_Distribution : public Distribution<int>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		double GetSampling() { return _distribution(this->_engine); }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(double mean)
		{
			_distribution = std::poisson_distribution(mean);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Poisson_Distribution() { _distribution = std::poisson_distribution(0.0); }
		Poisson_Distribution(double mean)
		{
			_distribution = std::poisson_distribution(mean);
		}
		~Poisson_Distribution() {};

	private:
		std::poisson_distribution<> _distribution;
	};

	/****************************************************************************
	*				  			Exponential_Distribution
	*************************************************************************//**
	*  @class     Exponential_Distribution : public Distribution<T>
	*  @brief     Exponential_Distribution
	*  @param[in] T lambda
	*  @details p(x | É…) = É… * exp(- É…x)
	*           situation: Time interval for customers to arrive at the bank counter
	*****************************************************************************/
	template <typename T>
	class Exponential_Distribution : public Distribution<T>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T GetSampling() { return _distribution(this->_engine); }
		T Min()   { return _distribution.min;    }
		T Max()   { return _distribution.max;    }
		T Lambda(){ return _distribution.lambda; }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(T lambda)
		{
			_distribution = std::exponential_distribution<T>(lambda);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Exponential_Distribution() { _distribution = std::exponential_distribution<T>(0.0); }
		Exponential_Distribution(T lambda)
		{
			_distribution = std::exponential_distribution<T>(lambda);
		}
		~Exponential_Distribution() {};

	private:
		std::exponential_distribution<T> _distribution;
	};

	/****************************************************************************
	*				  			Gamma_Distribution
	*************************************************************************//**
	*  @class     Gamma_Distribution : public Distribution<T>
	*  @brief     Gamma_Distribution
	*  @param[in] T alpha
	*  @param[in] T beta
	*  @details p(x | alpha,beta) = x^(-x/beta) / (beta^alpha * É°(alpha)) ^ x(alpha - 1) 
	*           situation: Prediction of maximum temperature in N years
	*****************************************************************************/
	template <typename T>
	class Gamma_Distribution : public Distribution<T>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T GetSampling() { return _distribution(this->_engine); }
		T Min()   { return _distribution.min; }
		T Max()   { return _distribution.max; }
		T Alpha() { return _distribution.alpha; }
		T Beta()  { return _distribution.beta; }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(T alpha, T beta)
		{
			_distribution = std::gamma_distribution<T>(alpha, beta);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Gamma_Distribution() { _distribution = std::gamma_distribution<T>(1.0, 1.0); }
		Gamma_Distribution(T alpha, T beta)
		{
			_distribution = std::gamma_distribution<T>(alpha, beta);
		}
		~Gamma_Distribution() {};

	private:
		std::gamma_distribution<T> _distribution;
	};

	/****************************************************************************
	*				  			Weibull_Distribution
	*************************************************************************//**
	*  @class     Weibull_Distribution : public Distribution<T>
	*  @brief     Weibull_Distribution
	*  @param[in] T a
	*  @param[in] T b
	*  @details p(x | a,b) = a / b * (x / b)^(a-1) *  exp(-(x/b)^a)
	*           A distribution method designed to describe the deterioration, life, strength, and destruction of objects.
	*****************************************************************************/
	template <typename T>
	class Weibull_Distribution : public Distribution<T>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T GetSampling() { return _distribution(this->_engine); }
		T Min()         { return _distribution.min; }
		T Max()         { return _distribution.max; }
		T Param_A()     { return _distribution.a; }
		T Param_B()     { return _distribution.b; }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(T a, T b)
		{
			_distribution = std::weibull_distribution<T>(a, b);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Weibull_Distribution() { _distribution = std::weibull_distribution(1.0, 1.0); }
		Weibull_Distribution(T a, T b)
		{
			_distribution = std::weibull_distribution<T>(a,b);
		}
		~Weibull_Distribution() {};

	private:
		std::weibull_distribution<T> _distribution;
	};

	/****************************************************************************
	*				  			Extream_Value_Distribution
	*************************************************************************//**
	*  @class     Extreme_Value_Distribution : public Distribution<T>
	*  @brief     Extreme_Value_Distribution
	*  @param[in] T a
	*  @param[in] T b
	*  @details p(x | a,b) = 1 / b * exp((a-x)/b - exp((a-x)/b))
	*           situation: Prediction of maximum temperature in N years
	*****************************************************************************/
	template <typename T>
	class Extreme_Value_Distribution : public Distribution<T>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T GetSampling() { return _distribution(this->_engine); }
		T Min()     { return _distribution.min;}
		T Max()     { return _distribution.max;}
		T Param_A() { return _distribution.a;}
		T Param_B() { return _distribution.b;}


		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(T a, T b)
		{
			_distribution = std::extreme_value_distribution<T>(a, b);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Extreme_Value_Distribution() { _distribution = std::extreme_value_distribution<T>(1.0, 1.0); }
		Extreme_Value_Distribution(T a, T b)
		{
			_distribution = std::extreme_value_distribution<T>(a, b);
		}
		~Extreme_Value_Distribution() {};

	private:
		std::extreme_value_distribution<T> _distribution;
	};

	/****************************************************************************
	*				  			LogNormal_Distribution
	*************************************************************************//**
	*  @class     Normal_Distribution : public Distribution<T>
	*  @brief     Normal_Distribution
	*  @param[in] T mean
	*  @param[in] T stdev
	*  @details p(x | m,s) = 1 / (sigma * Å„2ÉŒ) * exp((-x - mean)^2 / 2 * stdev^2)
	*           situation: Create height data near the mean from the parameters of mean height 170 cm and standard deviation 5 cm.
	*****************************************************************************/
	template <typename T>
	class Normal_Distribution : public Distribution<T>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T GetSampling() { return _distribution(this->_engine); }
		T Min()   { return _distribution.min; }
		T Max()   { return _distribution.max; }
		T Mean()  { return _distribution.mean; }
		T Stdev() { return _distribution.stddev; }
		T Sigma() { return _distribution.sigma; }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(T mean, T stdev)
		{
			_distribution = std::normal_distribution<T>(mean, stdev);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Normal_Distribution() { _distribution = std::normal_distribution<T>(0.0,1.0); }
		Normal_Distribution(T mean, T stdev)
		{
			_distribution = std::normal_distribution<T>(mean, stdev);
		}
		~Normal_Distribution() {};

	private:
		std::normal_distribution<T> _distribution;
	};

	/****************************************************************************
	*				  			LogNormal_Distribution
	*************************************************************************//**
	*  @class     LogNormal_Distribution : public Distribution<T>
	*  @brief     LogNormal_Distribution
	*  @param[in] T mean
	*  @param[in] T stdev
	*  @details p(x | m,s) = 1 / (sigma*xÅ„2ÉŒ) * exp((-(lnx) - mean)^2 / 2 * stdev^2)
	*           situation: Stock prices
	*****************************************************************************/
	template <typename T>
	class LogNormal_Distribution : public Distribution<T>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T GetSampling() { return _distribution(this->_engine); }
		T Min()   { return _distribution.min; }
		T Max()   { return _distribution.max; }
		T Mean()  { return _distribution.m; }
		T Stdev() { return _distribution.s; }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(T mean, T stdev)
		{
			_distribution = std::lognormal_distribution<T>(mean, stdev);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		LogNormal_Distribution() { _distribution = std::lognormal_distribution<T>(0.0, 1.0); }
		LogNormal_Distribution(T mean, T stdev)
		{
			_distribution = std::lognormal_distribution<T>(mean, stdev);
		}
		~LogNormal_Distribution() {};

	private:
		std::lognormal_distribution<T> _distribution;
	};

	/****************************************************************************
	*				  			Chi_Squared_Distribution
	*************************************************************************//**
	*  @class     Chi_Squared_Distribution : public Distribution<T>
	*  @brief     Chi_Squared_Distribution
	*  @param[in] T degree_of_freedom
	*  @details p(x | n) = x^(n/2 - 1) * exp(-x/2) / (É°(n/2) * 2^(n/2))
	*           situation: It is classified as a scientific test such as the chi-square test or the Friedman test.
	*****************************************************************************/
	template <typename T>
	class Chi_Squared_Distribution : public Distribution<T>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T GetSampling() { return _distribution(this->_engine); }
		T Min() { return _distribution.min; }
		T Max() { return _distribution.max; }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(T degree_of_freedom)
		{
			_distribution = std::chi_squared_distribution<T>(degree_of_freedom);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Chi_Squared_Distribution() { _distribution = std::chi_squared_distribution<T>(1.0); }
		Chi_Squared_Distribution(T degree_of_freedom)
		{
			_distribution = std::chi_squared_distribution<T>(degree_of_freedom);
		}
		~Chi_Squared_Distribution() {};

	private:
		std::chi_squared_distribution<T> _distribution;
	};

	/****************************************************************************
	*				  			Cauchy_Distribution 
	*************************************************************************//**
	*  @class     Cauchy_Distribution : public Distribution<T>
	*  @brief     Cauchy_Distribution 
	*  @param[in] T a
	*  @param[in] T b
	*  @details p(x | a, b) = (ÉŒb(1 + ((x-a)/b)^2))^(-1)
	*           situation: A model of the distribution when outliers are much more common than in a normal distribution.
	*****************************************************************************/
	template <typename T>
	class Cauchy_Distribution : public Distribution<T>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T GetSampling() { return _distribution(this->_engine); }
		T Min()     { return _distribution.min; }
		T Max()     { return _distribution.max; }
		T Param_A() { return _distribution.a; }
		T Param_B() { return _distribution.b; }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(T a, T b)
		{
			_distribution = std::cauchy_distribution<T>(a, b);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Cauchy_Distribution() { _distribution = std::cauchy_distribution<T>(0.0, 1.0); }
		Cauchy_Distribution(T a, T b)
		{
			_distribution = std::cauchy_distribution<T>(a, b);
		}
		~Cauchy_Distribution() {};

	private:
		std::cauchy_distribution<T> _distribution;
	};

	/****************************************************************************
	*				  			Fisher_F_Distribution
	*************************************************************************//**
	*  @class     Fisher_F_Distribution : public Distribution<T>
	*  @brief     Fisher_F_Distribution
	*  @param[in] T m
	*  @param[in] T n
	*  @details p(x | m, n) = É°((m+n)/2)/(É°(m/2)É°(n/2)) * (m/n)^(m/2) * x^(m/2 - 1) * (1 + mx/n)^(-(m+n)/2) 
	*           situation: Find the variance ratio of the two samples.
	*****************************************************************************/
	template <typename T>
	class Fisher_F_Distribution : public Distribution<T>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T GetSampling() { return _distribution(this->_engine); }
		T Min()     { return _distribution.min; }
		T Max()     { return _distribution.max; }
		T Param_M() { return _distribution.m; }
		T Param_N() { return _distribution.n; }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(T m, T n)
		{
			_distribution = std::fisher_f_distribution<T>(m, n);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Fisher_F_Distribution() { _distribution = std::fisher_f_distribution<T>(0.0, 1.0); }
		Fisher_F_Distribution(T m, T n)
		{
			_distribution = std::fisher_f_distribution<T>(m, n);
		}
		~Fisher_F_Distribution() {};

	private:
		std::fisher_f_distribution<T> _distribution;
	};

	/****************************************************************************
	*				  			Student_T_Distribution
	*************************************************************************//**
	*  @class     Student_T_Distribution : public Distribution<T>
	*  @brief     Student_T_Distribution 
	*  @param[in] T mean
	*  @param[in] T stdev
	*  @details p(x | n) = 1 /(nÉŒ) * É°((n + 1) / 2)/ É°(n/2) * (1 + x^2/n)^(-(n+1)/2)
	*           situation: Estimate the properties of the population (mean, variance)
	*****************************************************************************/
	template <typename T>
	class Student_T_Distribution : public Distribution<T>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T GetSampling() { return _distribution(this->_engine); }
		T Min()     { return _distribution.min; }
		T Max()     { return _distribution.max; }
		T Param_N() { return _distribution.n; }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetDistribution(T n)
		{
			_distribution = std::student_t_distribution<T>(n);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Student_T_Distribution() { _distribution = std::student_t_distribution<T>(1.0); }
		Student_T_Distribution(T n)
		{
			_distribution = std::student_t_distribution<T>(n);
		}
		~Student_T_Distribution() {};

	private:
		std::student_t_distribution<T> _distribution;
	};


}


#endif