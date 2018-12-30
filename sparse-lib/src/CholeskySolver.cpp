#include "CholeskySolver.h"

CholeskySolver::CholeskySolver() {}

CholeskySolver::~CholeskySolver() {}

JNZ* CholeskySolver::decompose(JNZ* jnz)
{
	// Construct L matrix's value and index arrays
	JNZ* L = new JNZ();
	L->n = jnz->n;
	L->A = (double **)malloc(sizeof(double *) * L->n);
	L->Ind = (int**)malloc((L->n + 1) * sizeof(int*));
	L->Ind[0] = (int*)malloc((L->n + 1) * sizeof(int));
	L->Ind[0][0] = L->n;

	for (int i = 0; i < L->n; i++)
	{
		// create temporary row 
		double* tmpRow = (double*)calloc(L->n, sizeof(double));

		int nnzsInTmpRow = 0;
		for (int j = 0; j <= i; j++)
		{
			double sum = 0;

			if (j == i) // summation for diagnols	
			{
				for (int k = 0; k < j; k++)
					sum += pow(tmpRow[k], 2); // TODO may be sped up

				// Find column index and element in A matrix
				int colIndex = Functions::iterativeSearch(jnz->Ind[j + 1], 0, jnz->Ind[0][j + 1], j);
				tmpRow[j] = sqrt(((colIndex == -1) ? 0 : jnz->A[j][colIndex]) - sum);
			}
			else
			{
				// Evaluating L(i, j) using L(j, j)
				for (int k = 0; k < j; k++) {
					int colIndex = Functions::binarySearch(L->Ind[j + 1], 0, L->Ind[0][j + 1], k); // [j, k]
					sum += (tmpRow[k] * (colIndex == -1 ? 0 : L->A[j][colIndex]));
				}

				// Find column index and element in A matrix
				// We only save upper triangular matrix in sparse memory (so i is always less that j)!
				int findI = i <= j ? i : j;
				int findJ = i <= j ? j : i;
				int colIndexA = Functions::iterativeSearch(jnz->Ind[findI + 1], 0, jnz->Ind[0][findI + 1], findJ);
				int colIndexL_A = Functions::binarySearch(L->Ind[j + 1], 0, L->Ind[0][j + 1], j);
				tmpRow[j] = ((colIndexA == -1 ? 0 : jnz->A[findI][colIndexA]) - sum) / L->A[j][colIndexL_A]; // denominator must be present!
			}

			if (tmpRow[j] != 0) {
				nnzsInTmpRow++;
			}
		}

		// Store tmp row in real matrix 
		L->Ind[i + 1] = (int*)malloc(nnzsInTmpRow * sizeof(int));
		L->Ind[0][i + 1] = nnzsInTmpRow;
		L->A[i] = (double*)calloc(nnzsInTmpRow, sizeof(double));
		for (int j = 0, ind = 0; j < L->n; j++) {
			if (tmpRow[j] != 0) {
				L->A[i][ind] = tmpRow[j];
				L->Ind[i + 1][ind] = j;
				++ind;
			}
		}

		delete tmpRow;
	}

	return L;
}

double* CholeskySolver::forwardSubstitution(JNZ* L, double* y)
{
	double *result = (double*)calloc(L->n, sizeof(double));

	for (int i = 0; i < L->n; i++) {
		result[i] = y[i];

		for (int j = 0, colIndex; j <= i - 1; j++) {
			colIndex = Functions::binarySearch(L->Ind[i + 1], 0, L->Ind[0][i + 1], j);
			result[i] -= (colIndex == -1 ? 0 : L->A[i][colIndex]) * result[j];
		}

		// Diagonal entries are real and positive!
		result[i] /= L->A[i][
			Functions::binarySearch(L->Ind[i + 1], 0, L->Ind[0][i + 1], i)
		];
	}
	return result;
}

double* CholeskySolver::backwardSubstitution(JNZ* U, double* b, bool isTransposed)
{
	double *result = (double*)calloc(U->n, sizeof(double));
	
	// Same codes but with i and j swapped for matrix U
	if (isTransposed) {
		for (int i = U->n - 1; i >= 0; i--) {
			result[i] = b[i];

			for (int j = i + 1, colIndex; j < U->n; j++) {
				colIndex = Functions::binarySearch(U->Ind[i + 1], 0, U->Ind[0][i + 1], j);
				result[i] -= (colIndex == -1 ? 0 : U->A[i][colIndex]) * result[j];
			}

			// Diagonal entries are real and positive!
			result[i] /= U->A[i][
				Functions::binarySearch(U->Ind[i + 1], 0, U->Ind[0][i + 1], i)
			];
		}
	}
	else {
		for (int i = U->n - 1; i >= 0; i--) {
			result[i] = b[i];

			for (int j = i + 1, colIndex; j < U->n; j++) {
				colIndex = Functions::binarySearch(U->Ind[j + 1], 0, U->Ind[0][j + 1], i);
				result[i] -= (colIndex == -1 ? 0 : U->A[j][colIndex]) * result[j];
				//x[i] -= U->first[i][j] * x[j];
			}

			// Diagonal entries are real and positive!
			result[i] /= U->A[i][
				Functions::binarySearch(U->Ind[i + 1], 0, U->Ind[0][i + 1], i)
			];
		}
	}
	return result;
}
