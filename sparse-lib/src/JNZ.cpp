#include "JNZ.h"

#define MY_ATOF 

#ifdef MY_ATOF

//--------------------- string to numbers functions -------------

/*
* atof.c --
*
*	Source code for the "atof" library procedure.
*
* Copyright 1988 Regents of the University of California
* Permission to use, copy, modify, and distribute this
* software and its documentation for any purpose and without
* fee is hereby granted, provided that the above copyright
* notice appear in all copies.  The University of California
* makes no representations about the suitability of this
* software for any purpose.  It is provided "as is" without
* express or implied warranty.
*/

#ifndef lint
static char rcsid[] = "$Header: /sprite/src/lib/c/stdlib/RCS/atof.c,v 1.2 89/03/22 00:46:56 rab Exp $ SPRITE (Berkeley)";
#endif /* not lint */

#include <stdlib.h>
#include <ctype.h>

static int maxExponent = 511;	/* Largest possible base 10 exponent.  Any
								* exponent larger than this will already
								* produce underflow or overflow, so there's
								* no need to worry about additional digits.
								*/
static double powersOf10[] = {	/* Table giving binary powers of 10.  Entry */
	10.,			/* is 10^2^i.  Used to convert decimal */
	100.,			/* exponents into floating-point numbers. */
	1.0e4,
	1.0e8,
	1.0e16,
	1.0e32,
	1.0e64,
	1.0e128,
	1.0e256
};

/*
*----------------------------------------------------------------------
*
* atof --
*
*	This procedure converts a floating-point number from an ASCII
*	decimal representation to internal double-precision format.
*
* Results:
*	The return value is the floating-point equivalent of string.
*	If a terminating character is found before any floating-point
*	digits, then zero is returned.
*
* Side effects:
*	None.
*
*----------------------------------------------------------------------
*/

double my_atof(char *& string)
/* A decimal ASCII floating-point number,
* optionally preceded by white space.
* Must have form "-I.FE-X", where I is the
* integer part of the mantissa, F is the
* fractional part of the mantissa, and X
* is the exponent.  Either of the signs
* may be "+", "-", or omitted.  Either I
* or F may be omitted, or both.  The decimal
* point isn't necessary unless F is present.
* The "E" may actually be an "e".  E and X
* may both be omitted (but not just one).
*/
{
	bool sign, expSign = false;
	double fraction, dblExp, *d;
	char c;
	int exp = 0;		/* Exponent read from "EX" field. */
	int fracExp = 0;		/* Exponent that derives from the fractional
							* part.  Under normal circumstatnces, it is
							* the negative of the number of digits in F.
							* However, if I is very long, the last digits
							* of I get dropped (otherwise a long I with a
							* large negative exponent could cause an
							* unnecessary overflow on I alone).  In this
							* case, fracExp is incremented one for each
							* dropped digit.
							*/
	int mantSize;		/* Number of digits in mantissa. */
	int decPt;			/* Number of mantissa digits BEFORE decimal point. */
	char *pExp;			/* Temporarily holds location of exponent in string. */

						/*
						* Strip off leading blanks and check for a sign.
						*/

	char *& p = string;
	while (isspace(*p)) {
		p += 1;
	}
	if (*p == '-') {
		sign = true;
		p += 1;
	}
	else {
		if (*p == '+') {
			p += 1;
		}
		sign = false;
	}

	/*
	* Count the number of digits in the mantissa (including the decimal
	* point), and also locate the decimal point.
	*/

	decPt = -1;
	for (mantSize = 0; ; mantSize += 1)
	{
		c = *p;
		if (!isdigit(c))
		{
			if ((c != '.') || (decPt >= 0))
				break;
			decPt = mantSize;
		}
		p += 1;
	}

	/*
	* Now suck up the digits in the mantissa.  Use two integers to
	* collect 9 digits each (this is faster than using floating-point).
	* If the mantissa has more than 18 digits, ignore the extras, since
	* they can't affect the value anyway.
	*/

	pExp = p;
	p -= mantSize;
	if (decPt < 0) {
		decPt = mantSize;
	}
	else {
		mantSize -= 1;			/* One of the digits was the point. */
	}
	if (mantSize > 18) {
		fracExp = decPt - 18;
		mantSize = 18;
	}
	else {
		fracExp = decPt - mantSize;
	}
	if (mantSize == 0) {
		return 0.0;
	}
	else {
		int frac1, frac2;
		frac1 = 0;
		for (; mantSize > 9; mantSize -= 1)
		{
			c = *p;
			p += 1;
			if (c == '.') {
				c = *p;
				p += 1;
			}
			frac1 = 10 * frac1 + (c - '0');
		}
		frac2 = 0;
		for (; mantSize > 0; mantSize -= 1)
		{
			c = *p;
			p += 1;
			if (c == '.') {
				c = *p;
				p += 1;
			}
			frac2 = 10 * frac2 + (c - '0');
		}
		fraction = (1.0e9 * frac1) + frac2;
	}

	/*
	* Skim off the exponent.
	*/

	p = pExp;
	if ((*p == 'E') || (*p == 'e')) {
		p += 1;
		if (*p == '-') {
			expSign = true;
			p += 1;
		}
		else {
			if (*p == '+') {
				p += 1;
			}
			expSign = false;
		}
		while (isdigit(*p)) {
			exp = exp * 10 + (*p - '0');
			p += 1;
		}
	}
	if (expSign) {
		exp = fracExp - exp;
	}
	else {
		exp = fracExp + exp;
	}

	/*
	* Generate a floating-point number that represents the exponent.
	* Do this by processing the exponent one bit at a time to combine
	* many powers of 2 of 10. Then combine the exponent with the
	* fraction.
	*/

	if (exp < 0) {
		expSign = true;
		exp = -exp;
	}
	else {
		expSign = false;
	}
	if (exp > maxExponent) {
		exp = maxExponent;
	}
	dblExp = 1.0;
	for (d = powersOf10; exp != 0; exp >>= 1, d += 1) {
		if (exp & 01) {
			dblExp *= *d;
		}
	}
	if (expSign) {
		fraction /= dblExp;
	}
	else {
		fraction *= dblExp;
	}

	if (sign) {
		return -fraction;
	}
	return fraction;
}

//restores index from the ASCII c-styoe string, pointer moves to the end of the string
int make_index(char*& str)
{
	int res = 0;
	while (isdigit(*str))
	{
		res = res * 10 + *str - '0';
		++str;
	}
	return res;
}
//--------------------end string to numbers functions -----------
#endif

JNZ::JNZ()
{
}

JNZ::JNZ(double** A, int** Ind, int n, int nnz) : A(A), Ind(Ind), n(n), nnz(nnz)
{
}

JNZ::~JNZ()
{
}

std::pair<long long, long long> JNZ::fromMTXAsSymetricAndPositive(std::string fileName)
{
	std::pair<long long, long long> times;
	std::chrono::time_point<std::chrono::high_resolution_clock> start, finish;

	/* open an existing file for reading */
	FILE *infile = fopen(fileName.c_str(), "r");

	/* if the file does not exist */
	if (infile == NULL)
		throw std::exception("File does not exist!");

	// Start clock...
	start = std::chrono::high_resolution_clock::now();

	/* declare a file pointer */
	char * buffer;
	long numbytes;

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

	// Finish clock...
	finish = std::chrono::high_resolution_clock::now();

	fclose(infile);

	times.first = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
	fprintf(stdout, "Buffer speed: %f milliseconds. \n", times.first / (double)1000000);

	// Start clock...
	start = std::chrono::high_resolution_clock::now();

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
#ifdef MY_ATOF
		v = my_atof(data);
#else
		v = (double)atof(data);
#endif
		++pos;
		data += pos;
		pos = 0;
		if (row != i)
		{
			--row;
			A[row] = (double*)malloc(c * sizeof(double));
			Ind[row + 1] = (int*)malloc(c * sizeof(int));

			arrayCopy(val, A[row], c);
			intArrayCopy(ind, Ind[row + 1], c);
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
#ifdef MY_ATOF
		v = my_atof(data);
#else
		v = (double)atof(data);
#endif
		// მოვრჩით კითხვას..

		if (row != i)
		{
			--row;
			A[row] = (double*)malloc(c * sizeof(double));
			Ind[row + 1] = (int*)malloc(c * sizeof(int));
			arrayCopy(val, A[row], c);
			intArrayCopy(ind, Ind[row + 1], c);
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
	arrayCopy(val, A[n - 1], c);
	intArrayCopy(ind, Ind[n], c);
	Ind[0][n] = c;

	delete[] ind;
	delete[] val;
	free(buffer);

	// Finish clock...
	finish = std::chrono::high_resolution_clock::now();
	times.second = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
	fprintf(stdout, "Read speed: %f milliseconds. \n", times.second / (double)1000000);
	return times;
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
	Ind[0] = (int*)calloc((n + 1), sizeof(int)); // ნულებით თავიდან
	Ind[0][0] = n; // შევინახოთ სტრიქონების რაოდენობა

	/*	შენიშვნა!
		მატრიცის ფაილში სტრიქონები შეიძლება მიყოლებით არ იყოს! */

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
			// თუ რაიმე ელემენტები ამოკითხული გვქონდა, მაშინ
			// ძველები გადააკოპირე დროებითებში: 
			// [0-დან : რაოდენობამდე] ... [ახლები ისედაც წერია ძველების მერე]
			if (Ind[0][row] != 0)
			{
				arrayCopy(A[row - 1], _values, Ind[0][row]);
				intArrayCopy(Ind[row], _indices, Ind[0][row]);
			}

			// შექმენი (ძველი ზომისა და) მთვლელის გათვალისწინებით
			A[row - 1] = (double*)malloc(counter * sizeof(double));
			Ind[row] = (int*)malloc(counter * sizeof(int));
			Ind[0][row] = counter;

			arrayCopy(_values, A[row - 1], counter);
			intArrayCopy(_indices, Ind[row], counter);
			row = I;
			counter = Ind[0][row]; // მთვლელი გახადე რაოდენობის ტოლი
		}

		// დროებითი სტრიქონების შევსება
		_indices[counter] = J - 1;
		_values[counter] = val;

		++counter;
	}

	// ბოლო სტრიქონის წაკითხვამდე უნდა მოხდეს ბოლო სტრიქონის ძველი ელემენტების 
	// უკვე არსებობის შემოწმება და არსებული დროებითი ცვლადების შესაბამისი განახლება.
	if (Ind[0][row] != 0)
	{
		// ძველები გადააკოპირე დროებითებში!
		arrayCopy(A[row - 1], _values, Ind[0][row]);
		intArrayCopy(Ind[row], _indices, Ind[0][row]);
	}

	// ბოლო სტრიქონის წაკითხვა
	{
		// ერთი სტრიქონის ამოკითხვა
		fscanf(f, "%d %d %lg\n", &I, &J, &val);

		// ახალ სტრიქონზე გადასვლამდე შევინახოთ ძველი სტრიქონი
		if (row != I)
		{
			// შექმენი (ძველი ზომისა და) მთვლელის გათვალისწინებით
			A[row - 1] = (double*)malloc(counter * sizeof(double));
			Ind[row] = (int*)malloc(counter * sizeof(int));
			Ind[0][row] = counter;

			arrayCopy(_values, A[row - 1], counter);
			intArrayCopy(_indices, Ind[row], counter);
			row = I;
			counter = Ind[0][row]; // მთვლელი გახადე რაოდენობის ტოლი
		}

		// დროებითი სტრიქონების შევსება
		_indices[counter] = J - 1;
		_values[counter] = val;

		++counter;
	}
	A[n - 1] = (double*)malloc(counter * sizeof(double));
	Ind[n] = (int*)malloc(counter * sizeof(int));
	Ind[0][n] = counter;

	arrayCopy(_values, A[n - 1], counter);
	intArrayCopy(_indices, Ind[n], counter);

	// გასუფთავება
	delete[] _indices;
	delete[] _values;

	// ფაილის დახურვა
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

				arrayCopy(_values, A[i], counter);
				intArrayCopy(_indices, Ind[i + 1], counter);
				Ind[0][i + 1] = counter; // რაოდენობის შენახვა
			}
		}
	}

	// გასუფთავება
	delete[] _indices;
	delete[] _values;
}

double** JNZ::toDense(bool makeSymmetric)
{
	double** res = (double**)calloc(n, sizeof(double*));

	for (size_t i = 0; i < n; i++)
		res[i] = (double*)calloc(n, sizeof(double));

	for (size_t i = 1; i <= n; i++)
	{
		for (size_t j = 0; j < Ind[0][i]; j++)
		{
			res[i - 1][Ind[i][j]] = A[i - 1][j];
			if (makeSymmetric) {
				res[Ind[i][j]][i - 1] = A[i - 1][j];
			}
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

void JNZ::arrayCopy(double* x, double* y, const int n)
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

void JNZ::intArrayCopy(int* x, int* y, const int n)
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

int JNZ::getSize()
{
	int size(0);

	// Add sizes of n, nnz variables
	size += 2 * sizeof(int);

	// Ind array's first row
	size += (n + 1) * sizeof(int);

	// Ind additional rows
	size += nnz * sizeof(int);

	// Values array
	size += nnz * sizeof(double);
	return size;
}
