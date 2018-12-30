#include <iostream>

#include "JNZ.h"
#include "CGSolver.h"
#include "CholeskySolver.h"

using namespace std;

int main()
{
	// Read sparse matrix into JNZ format
	JNZ* jnz = new JNZ();
	jnz->fromMTXAsSymetricAndPositive("resources/LFAT5.mtx");

	// Generate Ys
	double* y = (double*)malloc(sizeof(double)*jnz->n);
	for (size_t i = 0; i < jnz->n; i++) 
		y[i] = i + 1;

	// JNZ Cholesky solver
	CholeskySolver* choleskySolver = new CholeskySolver();
	JNZ* L = choleskySolver->decompose(jnz);
	double* b = choleskySolver->forwardSubstitution(L, y);
	double* x1 = choleskySolver->backwardSubstitution(L, b, false);

	// JNZ CG solver
	CGSolver* solver = new CGSolver();
	double* x2 = solver->solve(jnz, y);
	
	// Compare results
	for (size_t i = 0; i < jnz->n; i++) {
		std::cout << x1[i] << " -> " << x2[i] << " = " << (x1[i] == x2[i]) << std::endl;
	}
}