#define USE_EXISTING_YS true
#define RANDOM_YS_MIN_VALUE -100
#define RANDOM_YS_MAX_VALUE 100

#include <iostream>
#include <string>
#include <chrono>
#include <random>

#include "boost\filesystem.hpp"
#include "boost\regex.hpp"
#include "boost\foreach.hpp"
#include "boost\algorithm\string\predicate.hpp" // For string comparison

#include "mmio.h"
#include "json.hpp"

#include "JNZ.h"
#include "CGSolver.h"
#include "CholeskySolver.h"

int main()
{
	using namespace std;
	using namespace boost;
	using namespace boost::filesystem;
	using namespace boost::algorithm;
	using namespace std::chrono;
	using json = nlohmann::json;

	time_point<high_resolution_clock> start, finish;

	// reusable variables
	long long time;
	JNZ* jnz = new JNZ();
	CGSolver* cgSolver = new CGSolver();
	CholeskySolver* choleskySolver = new CholeskySolver();

	// Random number generator for Ys
	random_device rd;
	default_random_engine dre(rd());

	regex matrixPattern("(.*\\.mtx)");
	string matrixTypes[] = { "small", "medium", "large" };

	for (size_t i = 0; i < 3; i++)
	{
		path matrixRootDir("E:\\Projects\\github\\sparse-cholesky\\profiler\\resources\\matrices\\");
		path matrixPath = matrixRootDir.append(matrixTypes[i] + "\\");

		// Iterate and read matrices
		for (recursive_directory_iterator iter(matrixPath), end; iter != end; ++iter)
		{
			path _path = iter->path();
			string name = _path.filename().string();

			if (regex_match(name, matrixPattern) && !is_directory(_path))
			{
				// Or if it ends with 'b.mtx'!
				if (ends_with(name, "b.mtx") || ends_with(name, "coord.mtx")) {
					cout << "Skipping file: " << name << ". It ends with *b or *coord!" << endl;
					continue;
				}

				// Ignore this matrix
				if (starts_with(name, "bibd")) {
					cout << "Skipping file: " << name << ". It starts with bibd!" << endl;
					continue;
				}

				// ფაილის წაკითხვა
				FILE *f;
				string filePath = _path.string();
				if ((f = fopen(filePath.c_str(), "r")) == NULL) {
					cout << "Couldn't read file: " << name << ". Skipping file!" << endl;
					continue;
				}

				// მატრიცის თავის ამოკითხვა
				MM_typecode matcode;
				if (mm_read_banner(f, &matcode) != 0) {
					cout << "Could not process Matrix Market banner for matrix: " << name << endl;
					continue;
				}

				// ზომის ამოკითხვა
				int ret_code, M, N, nnz;
				if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nnz)) != 0) {
					cout << "Couldn't read matrix sizes from: " << name << endl;
					continue;
				}

				// კვადრატულობის შემოწმება
				if (M != N) {
					cout << "Not a square matrix! Skipping: " << name << endl;
					continue;
				}

				// Y-ების გენერაცია (ან არსებულების გამოყენება)
				double *y = (double *)malloc(sizeof(double)*N);

				// მარჯვენა მხრის ფაილებიდან ამოკითხვა
				if (USE_EXISTING_YS)
				{
					string ysPath = filePath.substr(0, filePath.find(".mtx", 0)) + "_Ys.txt";

					// Open Ys file
					FILE *ysFile;
					if ((ysFile = fopen(ysPath.c_str(), "r")) == NULL) {
						cout << "Couldn't read Ys file for matrix: " << name << endl;
						continue;
					}

					for (size_t i = 0; i < N; i++)
						fscanf(ysFile, "%lg\n", &y[i]);

					// ფაილის დახურვა
					if (ysFile != stdin) {
						fclose(ysFile);
					}
				}
				else // მარჯვენა მხრის შემთხვევითი გენერაცია
				{
					uniform_real_distribution<double> di(RANDOM_YS_MIN_VALUE, RANDOM_YS_MAX_VALUE);
					for (size_t i = 0; i < N; i++) {
						y[i] = di(dre);
					}
				}

				/*
				 * იტერაციის დასაწყისი
				 */
				fprintf(stdout, "Matrix: %s. (n: %d, nnz: %d) \n", name, N, nnz);

				// წაკითხვის დრო
				start = high_resolution_clock::now();
				jnz->fromMTXAsSymetricAndPositive(filePath);
				finish = high_resolution_clock::now();
				time = duration_cast<nanoseconds>(finish - start).count();
				fprintf(stdout, "JNZ (read speed): %f milliseconds. \n", time / (double)1000000);

				// JNZ CG solve time
				start = high_resolution_clock::now();
				cgSolver->solve(jnz, y);
				finish = high_resolution_clock::now();
				time = duration_cast<nanoseconds>(finish - start).count();
				fprintf(stdout, "JNZ CG solve time: %f milliseconds. \n", time / (double)1000000);

				// JNZ Cholesky solve time
				start = high_resolution_clock::now();
				JNZ* L = choleskySolver->decompose(jnz);
				double* b = choleskySolver->forwardSubstitution(L, y);
				choleskySolver->backwardSubstitution(L, b, false);
				finish = high_resolution_clock::now();
				time = duration_cast<nanoseconds>(finish - start).count();
				fprintf(stdout, "JNZ Cholesky solve time: %f milliseconds. \n", time / (double)1000000);

				/*
				 * იტერაციის დასასრული
				 */
				cout << endl;

				if (L != NULL) delete L;
				if (b != NULL) delete b;
			}
			else
			{
				cout << "Not a matrix file: " << name << endl;
			}
		}
	}
	// გასუფთავება
	if (jnz != NULL) delete jnz;
	if (cgSolver != NULL) delete cgSolver;
	if (choleskySolver != NULL) delete choleskySolver;

	cout << "Program finished gracefully." << endl;
}