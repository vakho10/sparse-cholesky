#pragma once

#include <string>
#include <exception>
#include <chrono>

#include "mmio.h"

class JNZ
{
public:
	int** Ind;
	double** A;
	int n, nnz;

	JNZ();
	JNZ(double** A, int** Ind, int n, int nnz);
	~JNZ();	

	// (სწრაფი) წაკითხვა *.mtx ფაილიდან
	// *სიმეტრიული და დადებითად განსაზღვრული უნდა იყოს!
	// წაიკითხავს, როგორც დიაგონალსა და მის ზედა მხარეს.
	std::pair<long long, long long> fromMTXAsSymetricAndPositive(std::string fileName);

	// წაკითხვა *.mtx ფაილიდან
	void fromMTXFile(std::string fileName);

	// მეჩხერი ფორმატიდან წაკითხვა
	void fromDense(double** matrix, const int n);
	void fromDense(double** matrix, const int m, const int n);

	// მეჩხერ ფორმატში გადაყვანა
	double** toDense(bool makeSymmetric = false);
		
	// TODO move these functions elsewhere!
	// დამხმარე ფუნქცია მეჩხერი მატრიცის სტრიქონის ვექტორზე სწრაფი გამრავლებისთვის
	inline double fastMatrixRowByVectorProd(double* v, int* ind, double *y);

	// დამხმარე ფუნქცია სიმეტრიული მატრიცის და მასივის სწრაფი გამრავლებისთვის
	void fastMatrixByVector(double** m, int** index, double* x, double* res);

	void arrayCopy(double* x, double* y, const int n);

	void intArrayCopy(int* x, int* y, const int n);

	// აბრუნებს გამოყენებული ბაიტების რაოდენობას
	int getSize();
};