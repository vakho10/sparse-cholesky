#include "CppUnitTest.h"
#include "Functions.h"

#define SIZE 5

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(FunctionTests)
	{
	public:

		TEST_METHOD(test_arrayCopy_function)
		{
			double* x = new double[SIZE] { .1, .2, .3, .4, .5 };
			double* y = (double*)calloc(SIZE, sizeof(double));
			Functions::arrayCopy(x, y, SIZE);

			for (size_t i = 0; i < SIZE; i++) {
				Assert::AreEqual(x[i], y[i]);
			}
		}

		TEST_METHOD(test_intArrayCopy_function)
		{
			int* x = new int[SIZE] { 1, 2, 3, 4, 5 };
			int* y = (int*)calloc(SIZE, sizeof(double));
			Functions::intArrayCopy(x, y, SIZE);

			for (size_t i = 0; i < SIZE; i++) {
				Assert::AreEqual(x[i], y[i]);
			}
		}

		TEST_METHOD(test_fastVectorByVectorProd_function)
		{
			double* x = new double[SIZE] { 1, 2, 3, 4, 5 };
			double* y = new double[SIZE] { 2, 4, 6, 8, 10 };

			// Should be 110
			Assert::AreEqual(110.0, Functions::fastVectorByVectorProd(x, y, SIZE));
		}

		TEST_METHOD(test_iterativeSearch_function)
		{
			int* x = new int[SIZE] { 12, 2, 34, 4, 55 };

			// Should return index 1
			Assert::AreEqual(1, Functions::iterativeSearch(x, 0, SIZE, 2));

			// Should return -1
			Assert::AreEqual(-1, Functions::iterativeSearch(x, 0, SIZE, 100)); // Non-existent element
			Assert::AreEqual(-1, Functions::iterativeSearch(x, 2, SIZE - 10, -100)); // Wrong range
		}

		TEST_METHOD(test_binarySearch_function)
		{
			int* x = new int[SIZE] { 1, 2, 3, 4, 5 };

			// Should return index 3
			Assert::AreEqual(3, Functions::binarySearch(x, 0, SIZE, 4));

			// Should return -1
			Assert::AreEqual(-1, Functions::binarySearch(x, 0, SIZE, 100)); // Non-existent element
			Assert::AreEqual(-1, Functions::binarySearch(x, 2, SIZE - 10, -100)); // Wrong range
		}


	};
}