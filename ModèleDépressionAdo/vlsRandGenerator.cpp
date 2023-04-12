#include "vlsRandGenerator.h"



vlsRandGenerator::vlsRandGenerator() : _k(-1)
{
}

void vlsRandGenerator::init(VSLStreamStatePtr stream) 
{
	_stream = stream;
}

void vlsRandGenerator::init()
{
}

void vlsRandGenerator::free()
{
}

vlsRandGenerator::~vlsRandGenerator()
{
}

double vlsRandGenerator::operator()()
{
	double _r[1];
	vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, _stream, 1, _r, 0, 1);
	return _r[0];
}


double vlsRandGenerator::weibull(double alpha, double beta) const
{
	double _r[1];
	vdRngWeibull(VSL_RNG_METHOD_WEIBULL_ICDF, _stream, 1, _r, alpha, 0, beta);
	return _r[0];

}
double vlsRandGenerator::weibull(double alpha, double beta, double hr) const
{
	double beta0(-log(beta)*alpha);
	double beta1(log(hr));
	beta = exp((beta0 + beta1) / -alpha);
	return weibull(alpha, beta);


}
double vlsRandGenerator::weibull_rnd(double alpha, double beta, double pr) const
{
	return beta * pow(-log(1-pr),1/alpha);
}

double vlsRandGenerator::gamma(double alpha, double beta) const
{
	double _r[1];
	vdRngGamma(VSL_RNG_METHOD_GAMMA_GNORM, _stream, 1, _r, alpha, 0, beta);
	return _r[0];
}

double vlsRandGenerator::weibull_rnd(double alpha, double beta, double pr, double hr) const
{
	double beta0(-log(beta) * alpha);
	double beta1(log(hr));
	beta = exp((beta0 + beta1) / -alpha);
	return weibull_rnd(alpha, beta, pr);
}

double vlsRandGenerator::piecewizeHazardFunction(double h0, std::vector<double> survivals, std::vector<double> ages) const
{
	// Draw S = 1 - F
	double _r[1];
	vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, _stream, 1, _r, 0, 1);

	// Remember that survivals array needs to start with 1 !!!!
	for (size_t i = 1; i < survivals.size(); i++)
	{
		double diff(_r[0] - survivals[i]);
		if (diff > 0)
		{
			double remainder(survivals[i - 1] - survivals[i]);
			double intervals(ages[i - 1] - ages[i]);
			
			return (diff / remainder) * intervals + ages[i - 1];
		}
	}

	return DBL_MAX;

}

int vlsRandGenerator::poisson(double lambda) const
{
	int _r[1];
	viRngPoisson(VSL_RNG_METHOD_POISSON_PTPE, _stream, 1, _r, lambda);
	return _r[0];
}
double vlsRandGenerator::normal(double alpha, double sigma) const
{
	double _r[1];
	vdRngGaussian(VSL_RNG_METHOD_UNIFORM_STD, _stream, 1, _r, alpha, sigma);
	return _r[0];
}

double vlsRandGenerator::lognormal(double alpha, double sigma) const
{
	double _r[1];
	vdRngLognormal(VSL_RNG_METHOD_LOGNORMAL_ICDF, _stream, 1, _r, alpha, sigma, 0, 1);
	return _r[0];
}

double vlsRandGenerator::betad(double alpha, double beta) const
{
	double _r[1];
	vdRngBeta(VSL_RNG_METHOD_BETA_CJA, _stream, 1, _r, alpha, beta, 0, 1);
	return _r[0];
}

int vlsRandGenerator::uniform()
{
	int _r[1];
	viRngUniform(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, _stream, 1, _r, 0, INT_MAX);
	return _r[0];
}

int vlsRandGenerator::uniform(int a, int b)
{
	int _r[1];
	viRngUniform(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER, _stream, 1, _r, a, b);
	return _r[0];
}

double vlsRandGenerator::gompertz(double alpha, double beta, double hr)
{
	// Generate a random uniform value
	double _r[1];
	vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, _stream, 1, _r, 0, 1);

	//calculate Gompertz
	return (log(-alpha * log(1 - pow(_r[0],hr)) / beta) + 1) / alpha;
}

double vlsRandGenerator::gompertz(double alpha, double beta)
{
	return gompertz(alpha, beta, 1);
}

void vlsRandGenerator::Many_uniform(double _r[], int n)
{
	vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, _stream, n, _r, 0, 1);

}

void vlsRandGenerator::correlated_rnd(double r[], double corr[], int n)
{
	double result[50];

	// Generate n random normal
	vdRngGaussian(VSL_RNG_METHOD_UNIFORM_STD, _stream, n, r, 0.0, 1.0);

	// Apply correlation matrix
	mmult(r, corr, n, result);

	// Transform back to uniform
	for (int i = 0; i < n; i++) r[i] = 0.5*erfc(-*(result+i) * sqr1_2);

	//delete(result);
}

void mmult(double matrix_a[], double matrix_b[], int n, double matrix_c[])
{
	// initiate the result matrix
	for (int i = 0; i < n; i++)
	{
		*(matrix_c + i) = 0;
		for (int j = 0; j < n; j++)
		{
			*(matrix_c + i) += *(matrix_a + j) * *(matrix_b + j * 3 + i);
		}
	}
}
