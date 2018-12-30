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

	// Cholesky solver for JNZ (and normal matrix)
	CholeskySolver* choleskySolver = new CholeskySolver();
	JNZ* L = choleskySolver->decompose(jnz);
	double* b1 = choleskySolver->forwardSubstitution(L, y);
	double* x1_1 = choleskySolver->backwardSubstitution(L, b1, false);

	// TODO this cholesky gives different results! Check why!
	double** dense = jnz->toDense();
	double* inlineMatrix = (double*)calloc(jnz->n * jnz->n, sizeof(double));
	for (size_t i = 0; i < jnz->n; i++)
		for (size_t j = 0; j < jnz->n; j++)
			inlineMatrix[j + i * jnz->n] = dense[i][j];
	double* inline_L = choleskySolver->decompose(inlineMatrix, jnz->n);
	double* b2 = choleskySolver->forwardSubstitution(inline_L, y, jnz->n);
	double* x1_2 = choleskySolver->backwardSubstitution(inline_L, b2, false);

	// JNZ CG solver
	CGSolver* solver = new CGSolver();
	double* x2 = solver->solve(jnz, y);
	
	// Compare results
	for (size_t i = 0; i < jnz->n; i++) {
		std::cout << x1_1[i] << " && " << x1_2[i] << " -> " << x2[i] << " = " << (x1_1[i] == x1_2[i] == x2[i]) << std::endl;
	}
}