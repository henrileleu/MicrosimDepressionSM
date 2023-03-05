#pragma once

#include <mkl.h>
#include <vector>
#include <iostream>

class vlsRandGenerator
{
public:
	vlsRandGenerator();
	void init(VSLStreamStatePtr stream);
	void init();
	void free();

	~vlsRandGenerator();

	// Distributions
	double operator() ();
	double gamma(double alpha, double beta) const;
	double weibull(double alpha, double beta) const;
	double weibull(double alpha, double beta, double hr) const;
	double weibull_rnd(double alpha, double beta, double pr) const;
	double weibull_rnd(double alpha, double beta, double pr, double hr) const;
	double piecewizeHazardFunction(double h0, std::vector<double> survivals, std::vector<double> ages) const;
	int poisson(double lambda) const;
	double normal(double alpha, double sigma) const;
	double lognormal(double alpha, double sigma) const;

	double betad(double alpha, double beta) const;
	int uniform();
	int uniform(int a, int b);
	double gompertz(double alpha, double beta, double hr);
	double gompertz(double alpha, double beta);
	void Many_uniform(double r[], int n);

	const static unsigned size = 2000;
private:

	VSLStreamStatePtr _stream;
	int _k;

};

