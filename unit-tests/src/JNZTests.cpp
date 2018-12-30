#include "CppUnitTest.h"
#include "JNZ.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Solution to include resource files in the deployment:
// https://stackoverflow.com/questions/5779703/automatic-copy-files-to-output-during-application-building
namespace UnitTests
{
	TEST_CLASS(JNZTests)
	{
	public:

		TEST_METHOD(test_fromMTXFile_method)
		{
			JNZ* jnz = new JNZ();
			jnz->fromMTXFile("resources/LFAT5.mtx");

			Assert::AreEqual(14, jnz->Ind[0][0]); // Check number of rows

			// Check number of non-zero elements in rows
			Assert::AreEqual(1, jnz->Ind[0][1]);
			Assert::AreEqual(1, jnz->Ind[0][2]);
			Assert::AreEqual(1, jnz->Ind[0][3]);
			Assert::AreEqual(2, jnz->Ind[0][4]);
			Assert::AreEqual(2, jnz->Ind[0][5]);
			Assert::AreEqual(2, jnz->Ind[0][6]);
			Assert::AreEqual(2, jnz->Ind[0][7]);
			Assert::AreEqual(3, jnz->Ind[0][8]);
			Assert::AreEqual(3, jnz->Ind[0][9]);
			Assert::AreEqual(2, jnz->Ind[0][10]);
			Assert::AreEqual(2, jnz->Ind[0][11]);
			Assert::AreEqual(3, jnz->Ind[0][12]);
			Assert::AreEqual(3, jnz->Ind[0][13]);
			Assert::AreEqual(3, jnz->Ind[0][14]);

			// Check indices
			Assert::AreEqual(0, jnz->Ind[1][0]);	// 1 1 1.57088
			Assert::AreEqual(1, jnz->Ind[2][0]);	// 2 2 1.25664e7
			Assert::AreEqual(2, jnz->Ind[3][0]);	// 3 3 .6088062015503876
			Assert::AreEqual(0, jnz->Ind[4][0]);	// 4 1 -94.2528
			Assert::AreEqual(3, jnz->Ind[4][1]);	// 4 4 15080.447999999997
			Assert::AreEqual(0, jnz->Ind[5][0]);	// 5 1 .78544
			Assert::AreEqual(4, jnz->Ind[5][1]);	// 5 5 3.14176
			Assert::AreEqual(1, jnz->Ind[6][0]);	// 6 2 -6.2832e6
			Assert::AreEqual(5, jnz->Ind[6][1]);	// 6 6 1.25664e7
			Assert::AreEqual(2, jnz->Ind[7][0]);	// 7 3 -.3044031007751938
			Assert::AreEqual(6, jnz->Ind[7][1]);	// 7 7 .6088062015503876
			Assert::AreEqual(3, jnz->Ind[8][0]);	// 8 4 -7540.223999999998
			Assert::AreEqual(4, jnz->Ind[8][1]);	// 8 5 -94.2528
			Assert::AreEqual(7, jnz->Ind[8][2]);	// 8 8 15080.447999999997
			Assert::AreEqual(3, jnz->Ind[9][0]);	// 9 4 94.2528
			Assert::AreEqual(4, jnz->Ind[9][1]);	// 9 5 .78544
			Assert::AreEqual(8, jnz->Ind[9][2]);	// 9 9 3.14176
			Assert::AreEqual(5, jnz->Ind[10][0]);	// 10 6 -6.2832e6
			Assert::AreEqual(9, jnz->Ind[10][1]);	// 10 10 1.25664e7
			Assert::AreEqual(6, jnz->Ind[11][0]);	// 11 7 -.3044031007751938
			Assert::AreEqual(10, jnz->Ind[11][1]);	// 11 11 .6088062015503876
			Assert::AreEqual(7, jnz->Ind[12][0]);	// 12 8 -7540.223999999998
			Assert::AreEqual(8, jnz->Ind[12][1]);	// 12 9 -94.2528
			Assert::AreEqual(11, jnz->Ind[12][2]);	// 12 12 15080.447999999997
			Assert::AreEqual(7, jnz->Ind[13][0]);	// 13 8 94.2528
			Assert::AreEqual(8, jnz->Ind[13][1]);	// 13 9 .78544
			Assert::AreEqual(12, jnz->Ind[13][2]);	// 13 13 3.14176
			Assert::AreEqual(11, jnz->Ind[14][0]);	// 14 12 94.2528
			Assert::AreEqual(12, jnz->Ind[14][1]);	// 14 13 .78544
			Assert::AreEqual(13, jnz->Ind[14][2]);	// 14 14 1.57088

			// Check values
			Assert::AreEqual(1.57088, jnz->A[0][0]);		// 1 1 1.57088
			Assert::AreEqual(1.25664e7, jnz->A[1][0]);		// 2 2 1.25664e7
			Assert::AreEqual(.6088062015503876, jnz->A[2][0]);		// 3 3 .6088062015503876
			Assert::AreEqual(-94.2528, jnz->A[3][0]);		// 4 1 -94.2528
			Assert::AreEqual(15080.447999999997, jnz->A[3][1]);		// 4 4 15080.447999999997
			Assert::AreEqual(.78544, jnz->A[4][0]);		// 5 1 .78544
			Assert::AreEqual(3.14176, jnz->A[4][1]);		// 5 5 3.14176
			Assert::AreEqual(-6.2832e6, jnz->A[5][0]);		// 6 2 -6.2832e6
			Assert::AreEqual(1.25664e7, jnz->A[5][1]);		// 6 6 1.25664e7
			Assert::AreEqual(-.3044031007751938, jnz->A[6][0]);		// 7 3 -.3044031007751938
			Assert::AreEqual(.6088062015503876, jnz->A[6][1]);		// 7 7 .6088062015503876
			Assert::AreEqual(-7540.223999999998, jnz->A[7][0]);		// 8 4 -7540.223999999998
			Assert::AreEqual(-94.2528, jnz->A[7][1]);		// 8 5 -94.2528
			Assert::AreEqual(15080.447999999997, jnz->A[7][2]);		// 8 8 15080.447999999997
			Assert::AreEqual(94.2528, jnz->A[8][0]);		// 9 4 94.2528
			Assert::AreEqual(.78544, jnz->A[8][1]);		// 9 5 .78544
			Assert::AreEqual(3.14176, jnz->A[8][2]);		// 9 9 3.14176
			Assert::AreEqual(-6.2832e6, jnz->A[9][0]);		// 10 6 -6.2832e6
			Assert::AreEqual(1.25664e7, jnz->A[9][1]);		// 10 10 1.25664e7
			Assert::AreEqual(-.3044031007751938, jnz->A[10][0]);		// 11 7 -.3044031007751938
			Assert::AreEqual(.6088062015503876, jnz->A[10][1]);		// 11 11 .6088062015503876
			Assert::AreEqual(-7540.223999999998, jnz->A[11][0]);		// 12 8 -7540.223999999998
			Assert::AreEqual(-94.2528, jnz->A[11][1]);		// 12 9 -94.2528
			Assert::AreEqual(15080.447999999997, jnz->A[11][2]);		// 12 12 15080.447999999997
			Assert::AreEqual(94.2528, jnz->A[12][0]);		// 13 8 94.2528
			Assert::AreEqual(.78544, jnz->A[12][1]);		// 13 9 .78544
			Assert::AreEqual(3.14176, jnz->A[12][2]);		// 13 13 3.14176
			Assert::AreEqual(94.2528, jnz->A[13][0]);		// 14 12 94.2528
			Assert::AreEqual(.78544, jnz->A[13][1]);		// 14 13 .78544
			Assert::AreEqual(1.57088, jnz->A[13][2]);		// 14 14 1.57088
		}

		TEST_METHOD(test_toDense_method)
		{
			JNZ* jnz = new JNZ();

			jnz->fromMTXFile("resources/LFAT5.mtx");
			double** dense = jnz->toDense();

			Assert::AreEqual(1.57088, dense[0][0]);		// 1 1 1.57088
			Assert::AreEqual(1.25664e7, dense[1][1]);		// 2 2 1.25664e7
			Assert::AreEqual(.6088062015503876, dense[2][2]);		// 3 3 .6088062015503876
			Assert::AreEqual(-94.2528, dense[3][0]);		// 4 1 -94.2528
			Assert::AreEqual(15080.447999999997, dense[3][3]);		// 4 4 15080.447999999997
			Assert::AreEqual(.78544, dense[4][0]);		// 5 1 .78544
			Assert::AreEqual(3.14176, dense[4][4]);		// 5 5 3.14176
			Assert::AreEqual(-6.2832e6, dense[5][1]);		// 6 2 -6.2832e6
			Assert::AreEqual(1.25664e7, dense[5][5]);		// 6 6 1.25664e7
			Assert::AreEqual(-.3044031007751938, dense[6][2]);		// 7 3 -.3044031007751938
			Assert::AreEqual(.6088062015503876, dense[6][6]);		// 7 7 .6088062015503876
			Assert::AreEqual(-7540.223999999998, dense[7][3]);		// 8 4 -7540.223999999998
			Assert::AreEqual(-94.2528, dense[7][4]);		// 8 5 -94.2528
			Assert::AreEqual(15080.447999999997, dense[7][7]);		// 8 8 15080.447999999997
			Assert::AreEqual(94.2528, dense[8][3]);		// 9 4 94.2528
			Assert::AreEqual(.78544, dense[8][4]);		// 9 5 .78544
			Assert::AreEqual(3.14176, dense[8][8]);		// 9 9 3.14176
			Assert::AreEqual(-6.2832e6, dense[9][5]);		// 10 6 -6.2832e6
			Assert::AreEqual(1.25664e7, dense[9][9]);		// 10 10 1.25664e7
			Assert::AreEqual(-.3044031007751938, dense[10][6]);		// 11 7 -.3044031007751938
			Assert::AreEqual(.6088062015503876, dense[10][10]);		// 11 11 .6088062015503876
			Assert::AreEqual(-7540.223999999998, dense[11][7]);		// 12 8 -7540.223999999998
			Assert::AreEqual(-94.2528, dense[11][8]);		// 12 9 -94.2528
			Assert::AreEqual(15080.447999999997, dense[11][11]);		// 12 12 15080.447999999997
			Assert::AreEqual(94.2528, dense[12][7]);		// 13 8 94.2528
			Assert::AreEqual(.78544, dense[12][8]);		// 13 9 .78544
			Assert::AreEqual(3.14176, dense[12][12]);		// 13 13 3.14176
			Assert::AreEqual(94.2528, dense[13][11]);		// 14 12 94.2528
			Assert::AreEqual(.78544, dense[13][12]);		// 14 13 .78544
			Assert::AreEqual(1.57088, dense[13][13]);		// 14 14 1.57088
		}

		TEST_METHOD(test_toDense_method_with_makeSymmetric_set_to_true)
		{
			JNZ* jnz = new JNZ();

			jnz->fromMTXFile("resources/LFAT5.mtx");
			double** dense = jnz->toDense(true);

			// Check non-diagonal elements only!
			Assert::AreEqual(-94.2528, dense[3][0]); // 4 1 -94.2528
			Assert::AreEqual(-94.2528, dense[0][3]);
			
			Assert::AreEqual(.78544, dense[4][0]); // 5 1 .78544
			Assert::AreEqual(.78544, dense[0][4]);
			
			Assert::AreEqual(-6.2832e6, dense[5][1]); // 6 2 -6.2832e6
			Assert::AreEqual(-6.2832e6, dense[1][5]);
			
			Assert::AreEqual(-.3044031007751938, dense[6][2]); // 7 3 -.3044031007751938
			Assert::AreEqual(-.3044031007751938, dense[2][6]);

			Assert::AreEqual(-7540.223999999998, dense[7][3]); // 8 4 -7540.223999999998
			Assert::AreEqual(-7540.223999999998, dense[3][7]);

			Assert::AreEqual(-94.2528, dense[7][4]); // 8 5 -94.2528
			Assert::AreEqual(-94.2528, dense[4][7]);

			Assert::AreEqual(94.2528, dense[8][3]);	// 9 4 94.2528
			Assert::AreEqual(94.2528, dense[3][8]);
			
			Assert::AreEqual(.78544, dense[8][4]); // 9 5 .78544
			Assert::AreEqual(.78544, dense[4][8]);
			
			Assert::AreEqual(-6.2832e6, dense[9][5]); // 10 6 -6.2832e6
			Assert::AreEqual(-6.2832e6, dense[5][9]);

			Assert::AreEqual(-.3044031007751938, dense[10][6]); // 11 7 -.3044031007751938
			Assert::AreEqual(-.3044031007751938, dense[6][10]);

			Assert::AreEqual(-7540.223999999998, dense[11][7]); // 12 8 -7540.223999999998
			Assert::AreEqual(-7540.223999999998, dense[7][11]);

			Assert::AreEqual(-94.2528, dense[11][8]); // 12 9 -94.2528
			Assert::AreEqual(-94.2528, dense[8][11]);

			Assert::AreEqual(94.2528, dense[12][7]); // 13 8 94.2528
			Assert::AreEqual(94.2528, dense[7][12]);		

			Assert::AreEqual(.78544, dense[12][8]);	// 13 9 .78544
			Assert::AreEqual(.78544, dense[8][12]);		

			Assert::AreEqual(94.2528, dense[13][11]); // 14 12 94.2528
			Assert::AreEqual(94.2528, dense[11][13]);		

			Assert::AreEqual(.78544, dense[13][12]); // 14 13 .78544
			Assert::AreEqual(.78544, dense[12][13]);		
		}

		TEST_METHOD(test_fromDense_method)
		{
			JNZ* jnz = new JNZ();

			jnz->fromMTXFile("resources/LFAT5.mtx");
			double** dense = jnz->toDense();
			jnz->fromDense(dense, 14);
			Assert::AreEqual(14, jnz->Ind[0][0]);
		}

	};
}