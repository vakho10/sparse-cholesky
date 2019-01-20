#pragma once

#include <string>

#include "Functions.h"

class JNZNew
{
public:
	void** Ind;
	double** A;
	int n, nnz;

	JNZNew();
	JNZNew(double** A, void** Ind, int n, int nnz);
	~JNZNew();

	// (სწრაფი) წაკითხვა *.mtx ფაილიდან
	// *სიმეტრიული და დადებითად განსაზღვრული უნდა იყოს!
	// წაიკითხავს, როგორც დიაგონალსა და მის ზედა მხარეს.
	void fromMTXAsSymetricAndPositive(std::string fileName);

	// მეჩხერ ფორმატში გადაყვანა
	double** toDense(bool makeSymmetric = false);

	// დამხმარე ფუნქცია მეჩხერი მატრიცის სტრიქონის ვექტორზე სწრაფი გამრავლებისთვის
	inline double fastMatrixRowByVectorProd(double* v, int* ind, double *y);

	// დამხმარე ფუნქცია სიმეტრიული მატრიცის და მასივის სწრაფი გამრავლებისთვის
	void fastMatrixByVector(double** m, void** index, double* x, double* res);

	template<typename T>
	void intArrayCopy(int *x, T *y, const int n)
	{
		int i, n5;
		if (n <= 0) return;
		n5 = n % 5;
		for (i = 0; i < n5; i++)  y[i] = (T)x[i];
		for (; i < n; i += 5)
		{
			y[i] = (T)x[i]; y[i + 1] = (T)x[i + 1]; y[i + 2] = (T)x[i + 2]; y[i + 3] = (T)x[i + 3]; y[i + 4] = (T)x[i + 4];
		}
	}
};