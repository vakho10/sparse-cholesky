#include "CGSolver.h"

CGSolver::CGSolver(double eps) {
	this->eps = eps;
}

CGSolver::~CGSolver() {}

double* CGSolver::solve(JNZ* jnz, double* y)
{
	double *x = (double*)calloc(jnz->n, sizeof(double));

	double *r, *d, *q;
	r = (double *)malloc(sizeof(double)*jnz->n);
	d = (double *)malloc(sizeof(double)*jnz->n);
	q = (double *)malloc(sizeof(double)*jnz->n);

	for (int i = 0; i < jnz->n; i++)
		r[i] = d[i] = q[i] = 0.;

	for (int i = 0; i < jnz->n; i++)
		d[i] = r[i] = y[i];

	double deltaNew(fastVectorByVectorProd(r, r, jnz->n));

	int count = 0;
	while (deltaNew >= eps * eps)
	{
		jnz->fastMatrixByVector(jnz->A, jnz->Ind, d, q);

		double alpha = deltaNew / fastVectorByVectorProd(d, q, jnz->n);
		for (int i = 0; i < jnz->n; i++)
			x[i] += alpha * d[i];

		for (int i = 0; i < jnz->n; i++)
			r[i] -= alpha * q[i];

		double deltaOld(deltaNew);
		deltaNew = fastVectorByVectorProd(r, r, jnz->n);
		double beta(deltaNew / deltaOld);
		for (int i = 0; i < jnz->n; i++)
			d[i] = r[i] + (beta*d[i]);
		count++;
	}

	free(r); r = NULL;
	free(d); d = NULL;
	free(q); q = NULL;

	return x;
}

double* CGSolver::solve(JNZNew* jnzNew, double* y)
{
	double *x = (double*)calloc(jnzNew->n, sizeof(double));

	double *r, *d, *q;
	r = (double *)malloc(sizeof(double)*jnzNew->n);
	d = (double *)malloc(sizeof(double)*jnzNew->n);
	q = (double *)malloc(sizeof(double)*jnzNew->n);

	for (int i = 0; i < jnzNew->n; i++)
		r[i] = d[i] = q[i] = 0.;

	for (int i = 0; i < jnzNew->n; i++)
		d[i] = r[i] = y[i];

	double deltaNew(fastVectorByVectorProd(r, r, jnzNew->n));

	int count = 0;
	while (deltaNew >= eps*eps)
	{
		jnzNew->fastMatrixByVector(jnzNew->A, jnzNew->Ind, d, q);

		double alpha = deltaNew / fastVectorByVectorProd(d, q, jnzNew->n);
		for (int i = 0; i < jnzNew->n; i++)
			x[i] += alpha * d[i];

		for (int i = 0; i < jnzNew->n; i++)
			r[i] -= alpha * q[i];

		double deltaOld(deltaNew);
		deltaNew = fastVectorByVectorProd(r, r, jnzNew->n);
		double beta(deltaNew / deltaOld);
		for (int i = 0; i < jnzNew->n; i++)
			d[i] = r[i] + (beta*d[i]);
		count++;
	}

	free(r); r = NULL;
	free(d); d = NULL;
	free(q); q = NULL;

	return x;
}

inline double CGSolver::fastVectorByVectorProd(double * x, double * y, const int n)
{
	int i, n5;
	double sum(0.0);
	if (n <= 0) return sum;
	n5 = n % 5;
	for (i = 0; i < n5; i++) sum += x[i] * y[i];
	for (; i < n; i += 5)
	{
		sum += x[i] * y[i] + x[i + 1] * y[i + 1] + x[i + 2] * y[i + 2]
			+ x[i + 3] * y[i + 3] + x[i + 4] * y[i + 4];
	}
	return sum;
}
