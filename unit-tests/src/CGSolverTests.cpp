#include "CppUnitTest.h"
#include "JNZ.h"
#include "CGSolver.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Solution to include resource files in the deployment:
// https://stackoverflow.com/questions/5779703/automatic-copy-files-to-output-during-application-building
namespace UnitTests
{
	TEST_CLASS(CGSolverTests)
	{
	public:

		TEST_METHOD(test_solve_method)
		{
			Assert::Fail(L"Not implemented yet!");
			/*JNZ* jnz = new JNZ();
			jnz->fromMTXAsSymetricAndPositive("resources/LFAT5.mtx");

			double* y = (double*)malloc(sizeof(double)*jnz->n);
			for (size_t i = 0; i < jnz->n; i++)
				y[i] = i + 1;

			double* x = (double*)calloc(jnz->n, sizeof(double));

			CGSolver* solver = new CGSolver(jnz, y, x);
			solver->solve();
			for (size_t i = 0; i < jnz->n; i++) 
			{
				Assert::AreNotEqual(0.0, x[i]);
			}*/
		}

		TEST_METHOD(test_that_jnz_and_jnzNew_solvers_give_same_results)
		{
			JNZ* jnz = new JNZ();
			jnz->fromMTXAsSymetricAndPositive("resources/LFAT5.mtx");
			
			JNZNew* jnzNew = new JNZNew();
			jnzNew->fromMTXAsSymetricAndPositive("resources/LFAT5.mtx");

			// Generate Ys
			double* y = (double*)malloc(sizeof(double)*jnz->n);
			for (size_t i = 0; i < jnz->n; i++)
				y[i] = i + 1;

			// JNZ CG solver
			CGSolver* solver = new CGSolver();
			double* x1 = solver->solve(jnz, y);

			// JNZNew CG solver
			double* x2 = solver->solve(jnzNew, y);

			// Compare results
			for (size_t i = 0; i < jnz->n; i++) {
				Assert::AreEqual(x1[i], x2[i]);
			}

		}

	};
}