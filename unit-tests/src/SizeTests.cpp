#include "CppUnitTest.h"

#include "JNZ.h"
#include "JNZNew.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Solution to include resource files in the deployment:
// https://stackoverflow.com/questions/5779703/automatic-copy-files-to-output-during-application-building
namespace UnitTests
{
	TEST_CLASS(SizeTests)
	{
	public:

		TEST_METHOD(test_sizeof_method_for_JNZ)
		{
			// Test JNZ
			JNZ* jnz = new JNZ();
			jnz->fromMTXFile("resources/LFAT5.mtx");

			Assert::AreEqual(14, jnz->Ind[0][0]); // Check number of rows

			// Call getSize() method and check if size was calculated correctly
			Assert::AreEqual(428, jnz->getSize());
		}

		TEST_METHOD(test_sizeof_method_for_JNZNew)
		{
			// Test JNZNew
			JNZNew* jnzNew = new JNZNew();
			jnzNew->fromMTXAsSymetricAndPositive("resources/LFAT5.mtx");

			Assert::AreEqual(14, ((int*)jnzNew->Ind[0])[0]); // Check number of rows

			// Call getSize() method and check if size was calculated correctly
			Assert::AreEqual(352, jnzNew->getSize());
		}

	};
}