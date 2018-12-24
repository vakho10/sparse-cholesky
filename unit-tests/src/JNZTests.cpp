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
			Assert::AreEqual(14, jnz->Ind[0][0]);
		}

		TEST_METHOD(test_toDense_method)
		{
			JNZ* jnz = new JNZ();

			jnz->fromMTXFile("resources/LFAT5.mtx");
			Assert::AreEqual(14, jnz->Ind[0][0]);
			
			double** dense = jnz->toDense();
			Assert::AreEqual(1.57088, dense[0][0]);
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