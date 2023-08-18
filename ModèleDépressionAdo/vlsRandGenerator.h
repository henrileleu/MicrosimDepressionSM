#pragma once

#include <mkl.h>
#include <mkl_vml.h>
#include <vector>
#include <iostream>
#include <math.h>

// Matrix product of two arrays of size 1, n and n,n
// outputs a 1, n matrix
void mmult(double matrix_a[], double matrix_b[], int n, double matrix_c[]);

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
	void normal(int n, double * target, double alpha, double sigma) const;

	double lognormal(double alpha, double sigma) const;

	double betad(double alpha, double beta) const;
	int uniform();
	int uniform(int a, int b);
	double gompertz(double alpha, double beta, double hr);
	double gompertz(double alpha, double beta);
	void Many_uniform(double r[], int n);

	// Transformatio
	void normalDist(int n, double* source, double* target);


	void correlated_rnd(double r[], double corr[], int n);
	void correlated_rnd(double r[], double corr[], int n, double value[], int m);

	const static unsigned size = 2000;
private:

	VSLStreamStatePtr _stream;
	int _k;

	const double sqr1_2 = 0.707107;
	const double inverseSqrt2Pi = 0.3989422804014;

};

extern vlsRandGenerator rnd;