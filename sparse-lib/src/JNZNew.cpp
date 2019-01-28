#include "JNZNew.h"

JNZNew::JNZNew()
{
}

JNZNew::JNZNew(double** A, void** Ind, int n, int nnz) : A(A), Ind(Ind), n(n), nnz(nnz)
{
}

JNZNew::~JNZNew()
{
}

void JNZNew::fromMTXAsSymetricAndPositive(std::string fileName)
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
	while (data[pos] != '\n')
		++pos;
	data[pos] = '\0';
	nnz = (int)atoi(data);
	++pos;
	data += pos;
	pos = 0;

	//დავიწყოთ მეჩხერი არანულოვანი ქვემატრიცის შევსება
	//მეხსიერების გამოყოფა
	A = (double **)malloc(sizeof(double *)*n);
	Ind = (void**)malloc((n + 2) * sizeof(void*));								////////////
	Ind[0] = (int*)malloc((n + 1) * sizeof(int));								////////////
	Ind[1] = (char*)malloc(n * sizeof(char));								////////////
	((int*)Ind[0])[0] = n;

	//მაქს. სიგრძის ორი ვექტორი, სტრიქონში ინდექსის და მნიშვნელობებისთვის
	int* ind = (int*)malloc(sizeof(int)*n);
	double*	val = (double*)malloc(sizeof(double)*n);

	int row = 1;
	int c = 0;		//მთვლელი
					//ფაილიდან სტრიქონის წამოსაღები 
	double v; 	int i, j;
	//ბოლო სტრიქონის გარდა
	for (int ii = 0; ii < nnz - 1; ++ii)
	{
		//წავიკითხოთ ასეთი რიგით: j,i,v
		//j:
		while (data[pos] != ' ') ++pos;
		data[pos] = '\0';
		j = (int)atoi(data);
		++pos;
		data += pos;
		pos = 0;
		//i:
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
			arrayCopy(val, A[row], c);

			if (ind[c - 1] <= UCHAR_MAX)
				((char*)Ind[1])[row] = 'c';
			else
				if (ind[c - 1] <= USHRT_MAX)
					((char*)Ind[1])[row] = 's';
				else
					((char*)Ind[1])[row] = 'i';

			switch (((char*)Ind[1])[row])
			{
			case('c'):
				Ind[row + 2] = (unsigned char*)malloc(c * sizeof(unsigned char));
				intArrayCopy(ind, (unsigned char*)Ind[row + 2], c);
				break;
			case('s'):
				Ind[row + 2] = (unsigned short*)malloc(c * sizeof(unsigned short));
				intArrayCopy(ind, (unsigned short*)Ind[row + 2], c);
				break;
			case('i'):
				Ind[row + 2] = (unsigned int*)malloc(c * sizeof(unsigned int));
				intArrayCopy(ind, (unsigned int*)Ind[row + 2], c);
			}

			((int*)Ind[0])[row + 1] = c;
			row = i;
			c = 0;
		}
		val[c] = v;
		ind[c] = j - 1;
		++c;
	}

	{//ბოლო სტრიქონი
	 //წავიკითხოთ ასეთი რიგით: j,i,v
	 //j:
		while (data[pos] != ' ') ++pos;
		data[pos] = '\0';
		j = (int)atoi(data);
		++pos;
		data += pos;
		pos = 0;
		//i:
		while (data[pos] != ' ') ++pos;
		data[pos] = '\0';
		i = (int)atoi(data);
		++pos;
		data += pos;
		pos = 0;
		v = (double)atof(data);
		//მოვრჩით კითხვას..

		if (row != i)
		{
			--row;
			A[row] = (double*)malloc(c * sizeof(double));
			arrayCopy(val, A[row], c);

			if (ind[c - 1] <= UCHAR_MAX)
				((char*)Ind[1])[row] = 'c';
			else
				if (ind[c - 1] <= USHRT_MAX)
					((char*)Ind[1])[row] = 's';
				else
					((char*)Ind[1])[row] = 'i';

			switch (((char*)Ind[1])[row])
			{
			case('c'):
				Ind[row + 2] = (unsigned char*)malloc(c * sizeof(unsigned char));
				intArrayCopy(ind, (unsigned char*)Ind[row + 2], c);
				break;
			case('s'):
				Ind[row + 2] = (unsigned short*)malloc(c * sizeof(unsigned short));
				intArrayCopy(ind, (unsigned short*)Ind[row + 2], c);
				break;
			case('i'):
				Ind[row + 2] = (unsigned int*)malloc(c * sizeof(unsigned int));
				intArrayCopy(ind, (unsigned int*)Ind[row + 2], c);
			}

			((int*)Ind[0])[row + 1] = c;
			row = i;
			c = 0;
		}
		val[c] = v;
		ind[c] = j - 1;
		++c;
	}

	A[n - 1] = (double*)malloc(c * sizeof(double));
	arrayCopy(val, A[n - 1], c);

	if (ind[c - 1] <= UCHAR_MAX)
		((char*)Ind[1])[n - 1] = 'c';
	else
		if (ind[c - 1] <= USHRT_MAX)
			((char*)Ind[1])[n - 1] = 's';
		else
			((char*)Ind[1])[n - 1] = 'i';

	switch (((char*)Ind[1])[n - 1])
	{
	case('c'):
		Ind[n + 1] = (unsigned char*)malloc(c * sizeof(unsigned char));
		intArrayCopy(ind, (unsigned char*)Ind[n + 1], c);
		break;
	case('s'):
		Ind[n + 1] = (unsigned short*)malloc(c * sizeof(unsigned short));
		intArrayCopy(ind, (unsigned short*)Ind[n + 1], c);
		break;
	case('i'):
		Ind[n + 1] = (unsigned int*)malloc(c * sizeof(unsigned int));
		intArrayCopy(ind, (unsigned int*)Ind[n + 1], c);
	}

	((int*)Ind[0])[n] = c;

	free(ind);
	free(val);
	free(buffer);
}

double ** JNZNew::toDense(bool makeSymmetric)
{
	double** res = (double**)calloc(n, sizeof(double*));

	for (size_t i = 0; i < n; i++)
		res[i] = (double*)calloc(n, sizeof(double));

	int* indRowZero = ((int*)Ind[0]);

	for (size_t i = 0; i < n; i++)
	{
		char c = ((char*)Ind[1])[i];
		switch (c)
		{
		case('c'):
		{
			unsigned char* rowInd = (unsigned char*)Ind[i + 2];
			for (size_t j = 0; j < indRowZero[i + 1]; j++)
			{
				res[i][rowInd[j]] = A[i][j];
				if (makeSymmetric) {
					res[rowInd[j]][i] = A[i][j];
				}
			}
			break;
		}
		case('s'):
		{
			unsigned short* rowInd = (unsigned short*)Ind[i + 2];
			for (size_t j = 0; j < indRowZero[i + 1]; j++)
			{
				res[i][rowInd[j]] = A[i][j];
				if (makeSymmetric) {
					res[rowInd[j]][i] = A[i][j];
				}
			}
			break;
		}
		break;
		case('i'):
			unsigned int* rowInd = (unsigned int*)Ind[i + 2];
			for (size_t j = 0; j < indRowZero[i + 1]; j++)
			{
				res[i][rowInd[j]] = A[i][j];
				if (makeSymmetric) {
					res[rowInd[j]][i] = A[i][j];
				}
			}
			break;
		}
	}
	return res;
}

inline double JNZNew::fastMatrixRowByVectorProd(double* v, int* ind, double* y)
{
	double sum = .0;
	int size = ind[0];
	for (int k = 1; k < size; k++) {
		sum += v[k] * y[ind[k]];
	}
	return sum;
}

void JNZNew::fastMatrixByVector(double** m, void** index, double* x, double* res)
{
	const int k(((int*)index[0])[0]);

	int i, j;
	double *p;
	int size;

	for (i = 0; i < k; ++i)
		res[i] = 0.;

	for (i = 0; i < k; ++i)
	{
		size = ((int*)index[0])[i + 1];
		p = m[i];
		char c = ((char*)index[1])[i];
		switch (c)
		{
		case('c'):
		{
			unsigned char * qc = (unsigned char *)index[i + 2];
			res[i] += p[0] * x[qc[0]];

			for (j = 1; j < size; ++j)
			{
				res[i] += p[j] * x[qc[j]];
				res[qc[j]] += p[j] * x[i];
			}
		}
		break;
		case('s'):
		{
			unsigned short * qs = (unsigned short *)index[i + 2];
			res[i] += p[0] * x[qs[0]];

			for (j = 1; j < size; ++j)
			{
				res[i] += p[j] * x[qs[j]];
				res[qs[j]] += p[j] * x[i];
			}
		}
		break;
		case('i'):
			unsigned int * q = (unsigned int *)index[i + 2];
			res[i] += p[0] * x[q[0]];

			for (j = 1; j < size; ++j)
			{
				res[i] += p[j] * x[q[j]];
				res[q[j]] += p[j] * x[i];
			}
		}
	}
}

void JNZNew::arrayCopy(double* x, double* y, const int n)
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

int JNZNew::getSize()
{
	int size(0);

	// Add sizes of n, nnz variables
	size += 2 * sizeof(int);

	// Ind array's first row (same)
	size += (n + 1) * sizeof(int);

	// Ind second row (chars)
	size += n * sizeof(char);

	// Ind's next rows are determined by character in the second row's i'th position
	for (size_t i = 0; i < n; i++) 
	{
		switch (((char*)Ind[1])[i])
		{
		case('c'):
			size += ((int*)Ind[0])[i + 1] * sizeof(unsigned char);
			break;
		case('s'):
			size += ((int*)Ind[0])[i + 1] * sizeof(unsigned short);
			break;
		case('i'):
			size += ((int*)Ind[0])[i + 1] * sizeof(unsigned int);
		}
	}

	// Values array
	size += nnz * sizeof(double);

	return size;
}
