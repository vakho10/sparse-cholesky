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

	double deltaNew(Functions::fastVectorByVectorProd(r, r, jnz->n));

	int count = 0;
	while (deltaNew >= eps * eps)
	{
		jnz->fastMatrixByVector(jnz->A, jnz->Ind, d, q);

		double alpha = deltaNew / Functions::fastVectorByVectorProd(d, q, jnz->n);
		for (int i = 0; i < jnz->n; i++)
			x[i] += alpha * d[i];

		for (int i = 0; i < jnz->n; i++)
			r[i] -= alpha * q[i];

		double deltaOld(deltaNew);
		deltaNew = Functions::fastVectorByVectorProd(r, r, jnz->n);
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