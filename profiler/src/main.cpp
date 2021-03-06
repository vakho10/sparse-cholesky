#define USE_EXISTING_YS false
#define RANDOM_YS_MIN_VALUE -100
#define RANDOM_YS_MAX_VALUE 100

#include <iostream>
#include <iomanip>
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
#include "JNZNew.h"
#include "CGSolver.h"
//#include "CholeskySolver.h"

int main()
{
	using namespace std;
	using namespace boost;
	using namespace boost::filesystem;
	using namespace boost::algorithm;
	using namespace std::chrono;
	using json = nlohmann::json;

	json jsonResults = json::array();

	time_point<high_resolution_clock> start, finish;

	// reusable variables
	long long time;
	JNZ* jnz = new JNZ();
	JNZNew* jnzNew = new JNZNew();
	CGSolver* cgSolver = new CGSolver();
	//CholeskySolver* choleskySolver = new CholeskySolver();

	// Random number generator for Ys
	random_device rd;
	default_random_engine dre(rd());

	regex matrixPattern("(.*\\.mtx)");

	const int matrixTypesSize = 1;
	string matrixTypes[matrixTypesSize] = { "medium" }; // , "medium", "large"

	const int matricesToSkipSize = 3;
	string matricesToSkip[matricesToSkipSize] = { "Andrews.mtx", "bcsstk38.mtx", "bloweybq.mtx" };

	for (size_t i = 0; i < matrixTypesSize; i++)
	{
		json tmpResults = json::array();

		path matrixRootDir("C:\\Users\\v.laluashvili\\Desktop\\koba\\medium_matrices\\");
		path matrixPath = matrixRootDir; // matrixRootDir.append(matrixTypes[i] + "\\");

		// Iterate and read matrices
		for (recursive_directory_iterator iter(matrixPath), end; iter != end; ++iter)
		{
			path _path = iter->path();
			string name = iter->path().filename().string();

			if (regex_match(name, matrixPattern) && !is_directory(_path))
			{
				// Skip bad matrices
				if (std::find(matricesToSkip, matricesToSkip + matricesToSkipSize, name)
					!= (matricesToSkip + matricesToSkipSize)) {
					std::cout << "Skipping bad matrix: " << name << endl;
					continue;
				}

				// Or if it ends with 'b.mtx'!
				if (ends_with(name, "b.mtx") || ends_with(name, "coord.mtx")) {
					//cout << "Skipping file: " << name << ". It ends with *b or *coord!" << endl;
					continue;
				}

				// Ignore this matrix
				if (starts_with(name, "bibd")) {
					//cout << "Skipping file: " << name << ". It starts with bibd!" << endl;
					continue;
				}

				// ფაილის წაკითხვა
				FILE *f;
				string filePath = _path.string();
				if ((f = fopen(filePath.c_str(), "r")) == NULL) {
					//cout << "Couldn't read file: " << name << ". Skipping file!" << endl;
					continue;
				}

				// მატრიცის თავის ამოკითხვა
				MM_typecode matcode;
				if (mm_read_banner(f, &matcode) != 0) {
					//cout << "Could not process Matrix Market banner for matrix: " << name << endl;
					continue;
				}

				// ზომის ამოკითხვა
				int ret_code, M, N, nnz;
				if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nnz)) != 0) {
					//cout << "Couldn't read matrix sizes from: " << name << endl;
					continue;
				}

				// კვადრატულობის შემოწმება
				if (M != N) {
					//cout << "Not a square matrix! Skipping: " << name << endl;
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
						std::cout << "Couldn't read Ys file for matrix: " << name << std::endl;
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
				json tmpResult = {
					{"name", name},
					{"n", N },
					{"nnz", nnz}
				};

				// წაკითხვის დრო
				start = high_resolution_clock::now();
				auto times = jnz->fromMTXAsSymetricAndPositive(filePath);
				finish = high_resolution_clock::now();
				time = duration_cast<nanoseconds>(finish - start).count();
				fprintf(stdout, "JNZ (read speed): %f milliseconds. \n", (times.first + times.second) / (double)1000000);
				fprintf(stdout, "JNZ (read speed full): %f milliseconds. \n", time / (double)1000000);

				// წაკითხვის დრო (JNZNew)
				/*start = high_resolution_clock::now();
				jnzNew->fromMTXAsSymetricAndPositive(filePath);
				finish = high_resolution_clock::now();
				time = duration_cast<nanoseconds>(finish - start).count();
				fprintf(stdout, "JNZNew (read speed): %f milliseconds. \n", time / (double)1000000);*/

				// JNZ CG solve time
				start = high_resolution_clock::now();
				cgSolver->solve(jnz, y);
				finish = high_resolution_clock::now();
				time = duration_cast<nanoseconds>(finish - start).count();
				fprintf(stdout, "JNZ CG solve time: %f milliseconds. \n", time / (double)1000000);
				tmpResult["jnz"] = {
					{"size", jnz->getSize() },
					{"solve", time / (double)1000000 }
				}; 

				// (2) JNZNew CG solve time
				/*start = high_resolution_clock::now();
				cgSolver->solve(jnzNew, y);
				finish = high_resolution_clock::now();
				time = duration_cast<nanoseconds>(finish - start).count();
				fprintf(stdout, "JNZNew CG solve time: %f milliseconds. \n", time / (double)1000000);
				tmpResult["jnz_new"] = {
					{"size", jnzNew->getSize() },
					{"solve", time / (double)1000000 }
				};*/

				// JNZ Cholesky solve time
				/*start = high_resolution_clock::now();
				JNZ* L = choleskySolver->decompose(jnz);
				double* b = choleskySolver->forwardSubstitution(L, y);
				choleskySolver->backwardSubstitution(L, b, false);
				finish = high_resolution_clock::now();
				time = duration_cast<nanoseconds>(finish - start).count();
				fprintf(stdout, "JNZ Cholesky solve time: %f milliseconds. \n", time / (double)1000000);*/

				/*
				 * იტერაციის დასასრული
				 */
				std::cout << endl;

				/*if (L != NULL) delete L;
				if (b != NULL) delete b;*/

				tmpResults.push_back(tmpResult);
			}
			else
			{
				std::cout << "Not a matrix file: " << name << std::endl;
			}
		}
		jsonResults.push_back({
			{"type", matrixTypes[i]},
			{"results", tmpResults }
			});
	}
	// გასუფთავება
	if (jnz != NULL) delete jnz;
	if (cgSolver != NULL) delete cgSolver;
	//if (choleskySolver != NULL) delete choleskySolver;

	// Save results into file
	std::ofstream jsonResultOfs("profiler-web/src/assets/results.json");
	jsonResultOfs << std::setw(4) << jsonResults << std::endl;

	std::cout << "Program finished gracefully." << endl;
}