#pragma once

#include <string>
#include <exception>

#include "mmio.h"
#include "Functions.h"

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
	void fromMTXAsSymetricAndPositive(std::string fileName);

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

	// აბრუნებს გამოყენებული ბაიტების რაოდენობას
	int getSize();
};