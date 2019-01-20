#include "CppUnitTest.h"
#include "JNZNew.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Solution to include resource files in the deployment:
// https://stackoverflow.com/questions/5779703/automatic-copy-files-to-output-during-application-building
namespace UnitTests
{
	TEST_CLASS(JNZNewTests)
	{
	public:

		TEST_METHOD(test_fromMTXAsSymetricAndPositive_method)
		{
			JNZNew* jnzNew = new JNZNew();

			jnzNew->fromMTXAsSymetricAndPositive("resources/LFAT5.mtx");
			double** dense = jnzNew->toDense(true);

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
	};
}