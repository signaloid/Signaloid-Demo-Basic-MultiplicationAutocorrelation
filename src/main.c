/*
 *	Authored 2020, James Timothy Meech.
 *	Authored 2021, Phillip Stanley-Marbell.
 *	Authored 2022, Damien Zufferey.
 *
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions
 *	are met:
 *	*	Redistributions of source code must retain the above
 *		copyright notice, this list of conditions and the following
 *		disclaimer.
 *	*	Redistributions in binary form must reproduce the above
 *		copyright notice, this list of conditions and the following
 *		disclaimer in the documentation and/or other materials
 *		provided with the distribution.
 *	*	Neither the name of the author nor the names of its
 *		contributors may be used to endorse or promote products
 *		derived from this software without specific prior written
 *		permission.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *	POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uxhw.h>
#include <unistd.h>

typedef enum {
	kBenchmarkModesDirectComputation = 1 << 0,
} BenchmarkModes;

void	usage(FILE * out);


int
main(int argc, char *  argv[])
{
	BenchmarkModes mode = 0;
	FILE *         fpA = NULL;
	double *       samplesA;
	int            numberOfSamplesA;
	double         A, C;
	int            c;

	while ((c = getopt(argc, argv, "a:m:h")) != -1)
	{
		switch (c)
		{
		case 'a':
			/*
			 *	Open file for samples of A.
			 */
			fpA = fopen(optarg, "r");
			if (fpA == NULL)
			{
				fprintf(stderr, "Opening input samples for A failed.\n");
				exit(EXIT_FAILURE);
			}
			break;
		case 'm':
			/*
			 *	Get mode from arguments.
			 */
			if (strcmp("1", optarg) == 0)
			{
				mode |= kBenchmarkModesDirectComputation;
			}
			break;
		case 'h':
			usage(stdout);
			exit(EXIT_SUCCESS);
		default:
			fprintf(stderr, "arguments malformed\n");
			usage(stderr);
			exit(EXIT_FAILURE);
		}
	}

	/*
	 *	Use default is A not given
	 */
	if (fpA == NULL) {
		fpA = fopen("input-A.txt", "r");
		if (fpA == NULL)
		{
			fprintf(stderr, "Opening input samples for A failed.\n");
			exit(EXIT_FAILURE);
		}
	}

	/*
	 *	Load samples for variable A.
	 */
	fscanf(fpA, "%d\n", &numberOfSamplesA);
	if (numberOfSamplesA <= 0)
	{
		fprintf(stderr, "Illegal number of samples (%d) specified in samples file.\n", numberOfSamplesA);
		exit(EXIT_FAILURE);
	}

	samplesA = calloc(numberOfSamplesA, sizeof(double));
	if (samplesA == NULL)
	{
		fprintf(stderr, "malloc() failed for samplesA.\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < numberOfSamplesA; i++)
	{
		fscanf(fpA, "%lf\n", &samplesA[i]);
	}

	fclose(fpA);

	if (!(mode && kBenchmarkModesDirectComputation))
	{
		/*
		 *	Uncertainty Tracking computation
		 */
		printf("Implicit computation of A*A.\n");
		/*
		 *	Load samples into distributional double variables.
		 */
		A = UxHwDoubleDistFromSamples(samplesA, numberOfSamplesA);

		printf("A = %lf\n", A);

		/*
		 *	Perform a conventional-syntax multiplication.
		 */
		C = A * A;

		printf("C = A * A = %lf\n", C);
	}
	else
	{
		double	convolutionMethodResults[numberOfSamplesA];
		double	convolutionMethodMean;
		double	convolutionMethodTotal = 0;

		/*
		 *	Exact computation of A*A.
		 */
		printf("Exact Computation of A*A\n");

		A = UxHwDoubleDistFromSamples(samplesA, numberOfSamplesA);
		printf("A = %lf\n", A);

		for (size_t i = 0; i < numberOfSamplesA; i++)
		{
			convolutionMethodResults[i] = samplesA[i] * samplesA[i];
			convolutionMethodTotal += convolutionMethodResults[i];
		}

		convolutionMethodMean = convolutionMethodTotal / numberOfSamplesA;
		printf("C (explicitly-calculated convolution mean) = %lf\n", convolutionMethodMean);

		C = UxHwDoubleDistFromSamples(&convolutionMethodResults[0], numberOfSamplesA);
		printf("C = A * A = %lf\n", C);
	}

	free(samplesA);

	return 0;
}

void
usage(FILE * out)
{
	fprintf(out,
	        "Usage: -a <samples file> -m <mode: 1 for explicit "
	        "computation, 0 for implicit uncertainty tracking>\n");
}
