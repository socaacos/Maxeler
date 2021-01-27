/**
 * Document: MaxCompiler Tutorial (maxcompiler-tutorial.pdf)
 * Chapter: 6      Example: 3      Name: Vectors
 * MaxFile name: Vectors
 * Summary:
 *    Streams a vector of integers to the dataflow engine and confirms that the
 *    returned stream contains the integers values doubled.
 */

#include <stdio.h>
#include <stdlib.h>
#include "Maxfiles.h"
#include <MaxSLiCInterface.h>

int check(int vectorSize, uint32_t *outVector, uint32_t *expectedVector)
{
	int status = 0;
	for (int i = 0; i < vectorSize * Vectors_vectorSize; i++) {
		if (outVector[i] != expectedVector[i]) {
			fprintf(stderr, "Error detected outVector[%d] != %d\n",
				outVector[i], expectedVector[i]);
			status = 1;
		}
	}
	return status;
}

void VectorsCPU(int32_t vectorSize, uint32_t *inVector, uint32_t *outVector)
{
	for (int i = 0; i < vectorSize * Vectors_vectorSize; i++) {
		outVector[i] = inVector[i] * 8;
	}
}

int main()
{
	const int vectorSize = Vectors_vectorSize*16;
	const int streamSize = 8;
	size_t sizeBytes = vectorSize * streamSize * sizeof(uint32_t);
	uint32_t *matrix = malloc(sizeBytes);
	uint32_t *matrixOut = malloc(sizeBytes);
	uint32_t *expectedVector = malloc(sizeBytes);

	for (int i = 0; i < streamSize*vectorSize; i++) {
		matrix[i] = i;
	}

	for (int i = 0; i < streamSize*vectorSize ; i++) {
		matrixOut[i] = 0;
	}

	VectorsCPU(streamSize*vectorSize, matrix, matrixOut);
	
	
	printf("Running DFE.\n");
	Vectors(streamSize, matrixOut, expectedVector);

	int status = check(streamSize*vectorSize, matrixOut, expectedVector);
	if (status)
		printf("Test failed.\n");
	else
		printf("Test passed OK!\n");

	return status;
}
