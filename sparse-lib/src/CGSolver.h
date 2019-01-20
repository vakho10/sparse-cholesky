#pragma once

#include <stdlib.h>

#include "JNZ.h"
#include "JNZNew.h"

class CGSolver {
private:
	double eps;
public:
	CGSolver(double eps = 1E-5);
	~CGSolver();

	double* solve(JNZ* jnz, double* y);
	double* solve(JNZNew* jnzNew, double* y);

	inline double fastVectorByVectorProd(double *x, double *y, const int n);
};