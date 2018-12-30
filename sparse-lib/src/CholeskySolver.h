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
};