#pragma once

#include <stdlib.h>

#include "Functions.h"
#include "JNZ.h"

class CGSolver {
private:
	double eps;
public:
	CGSolver(double eps = 1E-5);
	~CGSolver();

	double* solve(JNZ* jnz, double* y);
};