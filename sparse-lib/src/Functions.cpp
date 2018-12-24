#include "Functions.h"

void Functions::arrayCopy(double* x, double* y, const int n)
{
	int i, n5;
	if (n <= 0) return;
	n5 = n % 5;
	for (i = 0; i < n5; i++)  y[i] = x[i];
	for (; i < n; i += 5)
	{
		y[i] = x[i]; y[i + 1] = x[i + 1]; y[i + 2] = x[i + 2];  y[i + 3] = x[i + 3]; y[i + 4] = x[i + 4];
	}
}

void Functions::intArrayCopy(int* x, int* y, const int n)
{
	int i, n5;
	if (n <= 0) return;
	n5 = n % 5;
	for (i = 0; i < n5; i++)  y[i] = x[i];
	for (; i < n; i += 5)
	{
		y[i] = x[i]; y[i + 1] = x[i + 1]; y[i + 2] = x[i + 2];  y[i + 3] = x[i + 3]; y[i + 4] = x[i + 4];
	}
}

int Functions::iterativeSearch(int* arr, int l, int r, int x)
{
	for (int i = l; i <= r; i++) {
		if (arr[i] == x) {
			return i;
		}
	}
	return -1;
}

int Functions::binarySearch(int* arr, int l, int r, int x)
{
	if (r >= l)
	{
		int mid = l + (r - l) / 2;

		// If the element is present at the middle 
		// itself
		if (arr[mid] == x)
			return mid;

		// If element is smaller than mid, then 
		// it can only be present in left subarray
		if (arr[mid] > x)
			return binarySearch(arr, l, mid - 1, x);

		// Else the element can only be present
		// in right subarray
		return binarySearch(arr, mid + 1, r, x);
	}

	// We reach here when element is not 
	// present in array
	return -1;
}

double Functions::fastVectorByVectorProd(double* x, double* y, const int n)
{
	double sum = .0;
	if (n <= 0) return sum;

	int i, n5 = n % 5;

	// Multiply what is left
	for (i = 0; i < n5; i++) {
		sum += x[i] * y[i];
	}

	// Multiply by fives
	for (; i < n; i += 5) {
		sum += x[i] * y[i] + x[i + 1] * y[i + 1] + x[i + 2] * y[i + 2]
			+ x[i + 3] * y[i + 3] + x[i + 4] * y[i + 4];
	}
	return sum;
}