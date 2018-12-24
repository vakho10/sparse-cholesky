#include <iostream>
#include "JNZ.h"

int main()
{
	JNZ* jnz = new JNZ();
	jnz->fromMTXFile("resources/LFAT5.mtx");
	
	double** A = jnz->toDense();
	jnz->fromDense(A, 14);
	A = jnz->toDense();
	for (size_t i = 0; i < jnz->n; i++)
	{
		for (size_t j = 0; j < jnz->n; j++)
		{
			std::cout << A[i][j] << " ";
		}
		std::cout << std::endl;
	}
}