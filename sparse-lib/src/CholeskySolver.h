#pragma once

#include "JNZ.h"
#include "Functions.h"

class CholeskySolver
{
public:
	CholeskySolver();
	~CholeskySolver();

	// აბრუნებს ჩოლესკის L მატრიცს JNZ ფორმატში
	JNZ* decompose(JNZ* jnz);
	double* forwardSubstitution(JNZ* L, double* y);
	double* backwardSubstitution(JNZ* U, double* b, bool isTransposed = true);

	// აბრუნებს ჩოლესკის უბრალო ერთგანზომილებიან მატრიცად
	double* decompose(double *inlineMatrix, int n);
	double* forwardSubstitution(double* inline_L, double* b, int n);
	double* backwardSubstitution(double* inline_U, double* b, int n, bool isTransposed = true);
};