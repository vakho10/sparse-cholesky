#pragma once

class Functions 
{
public:
	static void arrayCopy(double* x, double* y, const int n);

	static void intArrayCopy(int* x, int* y, const int n);

	static int iterativeSearch(int* arr, int l, int r, int x);

	static int binarySearch(int* arr, int l, int r, int x);

	static double fastVectorByVectorProd(double* x, double* y, const int n);
};
