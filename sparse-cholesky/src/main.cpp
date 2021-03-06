#include <iostream>

#include "JNZ.h"
#include "JNZNew.h"
#include "CGSolver.h"
#include "CholeskySolver.h"

using namespace std;

void printDenseMatrix(double** A, const int size) {
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			cout << A[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}



int main()
{
	cout << "sizeof(int) = " << sizeof(int) << endl;
	cout << "sizeof(double) = " << sizeof(double) << endl;
	cout << "sizeof(unsigned char) = " << sizeof(unsigned char) << endl;

	int* intArr = new int[5] { 1, 2, 3, 4, 5 };
	cout << "sizeof(intArr[10]) = " << sizeof(intArr) << endl;

	double** A = (double **)malloc(sizeof(double *) * 10);
	cout << "sizeof(A) = " << sizeof(A) << endl;

	//// Read sparse matrix into JNZ format
	//JNZ* jnz = new JNZ();
	//jnz->fromMTXAsSymetricAndPositive("resources/LFAT5.mtx");

	//cout << "JNZ: " << endl;
	//printDenseMatrix(jnz->toDense(), jnz->n);
	//	
	//// (2) Read sparse matrix into the new JNZ format
	//JNZNew* jnzNew = new JNZNew();
	//jnzNew->fromMTXAsSymetricAndPositive("resources/LFAT5.mtx");

	//cout << "JNZNew: " << endl;
	//printDenseMatrix(jnzNew->toDense(), jnzNew->n);

	//// Generate Ys
	//double* y = (double*)malloc(sizeof(double)*jnz->n);
	//for (size_t i = 0; i < jnz->n; i++) 
	//	y[i] = i + 1;

	//// Cholesky solver for JNZ (and normal matrix)
	//CholeskySolver* choleskySolver = new CholeskySolver();
	//JNZ* L = choleskySolver->decompose(jnz);
	//double* b1 = choleskySolver->forwardSubstitution(L, y);
	//double* x1_1 = choleskySolver->backwardSubstitution(L, b1, false);

	//// TODO this cholesky gives different results! Check why!
	//double** dense = jnz->toDense();
	//double* inlineMatrix = (double*)calloc(jnz->n * jnz->n, sizeof(double));
	//for (size_t i = 0; i < jnz->n; i++)
	//	for (size_t j = 0; j < jnz->n; j++)
	//		inlineMatrix[j + i * jnz->n] = dense[i][j];
	//double* inline_L = choleskySolver->decompose(inlineMatrix, jnz->n);
	//double* b2 = choleskySolver->forwardSubstitution(inline_L, y, jnz->n);
	//double* x1_2 = choleskySolver->backwardSubstitution(inline_L, b2, false);

	//// JNZ CG solver
	//CGSolver* solver = new CGSolver();
	//double* x2 = solver->solve(jnz, y);

	//// (2) JNZNew CG solver
	//double* x3 = solver->solve(jnzNew, y);
	//
	//// Compare results
	//for (size_t i = 0; i < jnz->n; i++) {
	//	std::cout << x2[i] << " -> " << x3[i] << " = " << (x2[i] == x3[i]) << std::endl;
	//}
}