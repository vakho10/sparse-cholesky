#include "JNZ.h"

JNZ::JNZ()
{
}

JNZ::JNZ(double** A, int** Ind, int n, int nnz) : A(A), Ind(Ind), n(n), nnz(nnz)
{
}

JNZ::~JNZ()
{
}

void JNZ::fromMTXAsSymetricAndPositive(std::string fileName)
{
	/* open an existing file for reading */
	FILE *infile = fopen(fileName.c_str(), "r");

	/* declare a file pointer */
	char * buffer;
	long numbytes;

	/* if the file does not exist */
	if (infile == NULL)
		throw std::exception("File does not exist!");

	/* Get the number of bytes */
	fseek(infile, 0L, SEEK_END);
	numbytes = ftell(infile);

	/* reset the file position indicator to
	the beginning of the file */
	fseek(infile, 0L, SEEK_SET);

	/* grab sufficient memory for the
	buffer to hold the text */
	buffer = (char*)calloc(numbytes, sizeof(char));

	/* memory error */
	if (buffer == NULL)
		throw std::exception("Memory error!");

	/* copy all the text into the buffer */
	fread(buffer, sizeof(char), numbytes, infile);
	fclose(infile);

	// Ignore comment section
	size_t pos = 0;
	char *data = buffer;
	while (data[pos] == '%')
	{
		++pos;
		while (data[pos] != '\n')
			++pos;
		data += (pos + 1);
		pos = 0;
	}

	// რაოდენობები წავიკითხეთ	
	while (data[pos] != ' ')
		++pos;
	data[pos] = '\0';
	n = (int)atoi(data);
	++pos;
	data += pos;

	// There is second n in the matrix file
	pos = 0;
	while (data[pos] != ' ')
		++pos;
	data[pos] = '\0';
	n = (int)atoi(data);
	++pos;
	data += pos;

	pos = 0;
	while (data[pos] != '\n') ++pos;
	data[pos] = '\0';
	nnz = (int)atoi(data);
	++pos;
	data += pos;
	pos = 0;
	// დავიწყოთ მეჩხერი აანულოვანი ქვემატრიცის შევსება
	// მეხსიერების გამოყოფა
	A = (double **)malloc(sizeof(double *)*n);
	Ind = (int**)malloc((n + 1) * sizeof(int*));
	Ind[0] = (int*)malloc((n + 1) * sizeof(int));
	Ind[0][0] = n;


	// მაქს. სიგრძის ორი ვექტორი, სტრიქონში ინდექსის და მნიშვნელობებისთვის
	int* ind = (int *)malloc(sizeof(int)*n);
	double*	val = (double *)malloc(sizeof(double)*n);

	int row = 1;
	int c = 0;		//მთვლელი
	// ფაილიდან სტრიქონის წამოსაღები 
	double v; 	int i, j;
	// ბოლო სტრიქონის გარდა
	for (int ii = 0; ii < nnz - 1; ++ii)
	{
		// წავიკითხოთ ასეთი რიგით: j,i,v
		// j:
		while (data[pos] != ' ') ++pos;
		data[pos] = '\0';
		j = (int)atoi(data);
		++pos;
		data += pos;
		pos = 0;
		// i:
		while (data[pos] != ' ') ++pos;
		data[pos] = '\0';
		i = (int)atoi(data);
		++pos;
		data += pos;
		pos = 0;
		while (data[pos] != '\n') ++pos;
		data[pos] = '\0';
		v = (double)atof(data);
		++pos;
		data += pos;
		pos = 0;
		if (row != i)
		{
			--row;
			A[row] = (double*)malloc(c * sizeof(double));
			Ind[row + 1] = (int*)malloc(c * sizeof(int));

			Functions::arrayCopy(val, A[row], c);
			Functions::intArrayCopy(ind, Ind[row + 1], c);
			Ind[0][row + 1] = c;
			row = i;
			////////cout << c << endl;////////
			c = 0;
		}
		val[c] = v;
		ind[c] = j - 1;
		++c;
		/**/
	}

	{// ბოლო სტრიქონი
		// წავიკითხოთ ასეთი რიგით: j,i,v
		// j:
		while (data[pos] != ' ') ++pos;
		data[pos] = '\0';
		j = (int)atoi(data);
		++pos;
		data += pos;
		pos = 0;
		// i:
		while (data[pos] != ' ') ++pos;
		data[pos] = '\0';
		i = (int)atoi(data);
		++pos;
		data += pos;
		pos = 0;
		v = (double)atof(data);
		// მოვრჩით კითხვას..

		if (row != i)
		{
			--row;
			A[row] = (double*)malloc(c * sizeof(double));
			Ind[row + 1] = (int*)malloc(c * sizeof(int));
			Functions::arrayCopy(val, A[row], c);
			Functions::intArrayCopy(ind, Ind[row + 1], c);
			Ind[0][row + 1] = c;
			row = i;
			c = 0;
		}
		val[c] = v;
		ind[c] = j - 1;
		++c;
	}
	A[n - 1] = (double*)malloc(c * sizeof(double));
	Ind[n] = (int*)malloc(c * sizeof(int));
	Functions::arrayCopy(val, A[n - 1], c);
	Functions::intArrayCopy(ind, Ind[n], c);
	Ind[0][n] = c;

	delete[] ind;
	delete[] val;
	free(buffer);
}

void JNZ::fromMTXFile(std::string fileName)
{
	// ფაილის შემოწმება
	FILE *f;
	if ((f = fopen(fileName.c_str(), "r")) == NULL)
		throw std::exception("File does not exist!");

	// ბანერის შემოწმება
	MM_typecode matcode;
	if (mm_read_banner(f, &matcode) != 0)
		throw std::exception("Could not process Matrix Market banner.");

	// TODO მომავალში აუცილებლად დავამატოთ ფილტრები!

	// მატრიცის ზომების წაკითხვა (M, N, NNZ)
	int ret_code;
	if ((ret_code = mm_read_mtx_crd_size(f, &n, &n, &nnz)) != 0)
		throw std::exception("Could not read the matrix size.");

	// მეხსიერების გამოყოფა
	A = (double **)malloc(sizeof(double *)*n);
	Ind = (int**)malloc((n + 1) * sizeof(int*));
	Ind[0] = (int*)malloc((n + 1) * sizeof(int));
	Ind[0][0] = n;

	// შევქმნათ მაქსიმალური სიგრძის ორი ვექტორი, სტრიქონში ინდექსის და მნიშვნელობებისთვის
	int* _indices = (int *)malloc(sizeof(int)*n);
	double*	_values = (double *)malloc(sizeof(double)*n);

	// დროებითი ცვლადები
	int I, J;
	double val;
	int row(1), counter(0);

	// ციკლი ბოლო სტრიქონამდე
	for (int i = 0; i < nnz - 1; i++)
	{
		// ერთი სტრიქონის ამოკითხვა
		fscanf(f, "%d %d %lg\n", &I, &J, &val);

		// ახალ სტრიქონზე გადასვლამდე შევინახოთ ძველი სტრიქონი
		if (row != I)
		{
			A[row - 1] = (double*)malloc(counter * sizeof(double));
			Ind[row] = (int*)malloc(counter * sizeof(int));

			Functions::arrayCopy(_values, A[row - 1], counter);
			Functions::intArrayCopy(_indices, Ind[row], counter);
			Ind[0][row] = counter; // რაოდენობის შენახვა
			row = I;
			counter = 0;
		}

		// დროებითი სტრიქონების შევსება
		_indices[counter] = J - 1; // სვეტის გასწორება
		_values[counter] = val;

		++counter;
	}

	// ბოლო სტრიქონის წაკითხვა
	{
		fscanf(f, "%d %d %lg\n", &I, &J, &val);

		if (row != I)
		{
			A[row - 1] = (double*)malloc(counter * sizeof(double));
			Ind[row] = (int*)malloc(counter * sizeof(int));

			Functions::arrayCopy(_values, A[row - 1], counter);
			Functions::intArrayCopy(_indices, Ind[row], counter);
			Ind[0][row] = counter; // რაოდენობის შენახვა
			row = I;
			counter = 0;
		}

		// დროებითი სტრიქონების შევსება
		_indices[counter] = J - 1; // სვეტის გასწორება
		_values[counter] = val;

		++counter;
	}
	A[n - 1] = (double*)malloc(counter * sizeof(double));
	Ind[n] = (int*)malloc(counter * sizeof(int));
	Functions::arrayCopy(_values, A[n - 1], counter);
	Functions::intArrayCopy(_indices, Ind[n], counter);
	Ind[0][n] = counter;

	// გასუფთავება
	delete[] _indices;
	delete[] _values;

	if (f != stdin) {
		fclose(f);
	}
}

void JNZ::fromDense(double** matrix, const int n)
{
	this->fromDense(matrix, n, n);
}

void JNZ::fromDense(double** matrix, const int m, const int n)
{
	A = (double **)malloc(sizeof(double *)*m);
	Ind = (int**)malloc((m + 1) * sizeof(int*));
	Ind[0] = (int*)malloc((n + 1) * sizeof(int));
	Ind[0][0] = m;

	int* _indices = (int *)malloc(sizeof(int)*n);
	double*	_values = (double *)malloc(sizeof(double)*n);

	for (size_t i = 0, counter = 0; i < m; i++, counter = 0)
	{		
		for (size_t j = 0; j < n; j++)
		{
			// თუ არანულოვანია, მაშინ ვინახავთ
			if (matrix[i][j] != 0) {
				_indices[counter] = j;
				_values[counter] = matrix[i][j];
				++counter;
			}

			// თუ ბოლო სვეტია, მაშინ გადაგვაქვს რეალურ სტრიქონში
			if (j == n - 1) {
				A[i] = (double*)malloc(counter * sizeof(double));
				Ind[i + 1] = (int*)malloc(counter * sizeof(int));

				Functions::arrayCopy(_values, A[i], counter);
				Functions::intArrayCopy(_indices, Ind[i + 1], counter);
				Ind[0][i + 1] = counter; // რაოდენობის შენახვა
			}
		}
	}

	// გასუფთავება
	delete[] _indices;
	delete[] _values;
}

double** JNZ::toDense()
{
	double** res = (double**)calloc(n, sizeof(double*));
	for (size_t i = 0; i < n; i++) {
		res[i] = (double*)calloc(n, sizeof(double));
	}
	for (size_t i = 1; i <= n; i++)
	{
		for (size_t j = 0; j < Ind[0][i]; j++)
		{
			res[i - 1][Ind[i][j]] = A[i - 1][j];
		}
	}
	return res;
}

inline double JNZ::fastMatrixRowByVectorProd(double* v, int* ind, double* y)
{
	double sum = .0;
	int size = ind[0];
	for (int k = 1; k < size; k++) {
		sum += v[k] * y[ind[k]];
	}
	return sum;
}

void JNZ::fastMatrixByVector(double** m, int** index, double* x, double* res)
{
	const int k(Ind[0][0]);

	int i, j;
	double *p;
	int *q;
	int size;

	for (i = 0; i < k; ++i)
		res[i] = 0.;

	for (i = 0; i < k; ++i)
	{
		p = m[i];
		q = index[i + 1];
		size = index[0][i + 1];
		res[i] += p[0] * x[q[0]];

		for (j = 1; j < size; ++j)
		{
			res[i] += p[j] * x[q[j]];
			res[q[j]] += p[j] * x[i];
		}
	}
}