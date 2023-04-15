
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#include "../../includes/TimeMeasure.cpp"
#include "../../includes/CSVHandler.cpp"

#ifndef NUM_SAMPLES
#define NUM_SAMPLES (5 * 1000 * 1000)
#endif // NUM_SAMPLES

#define ENVIROMENT_VAR "EXECUTION_VARIANT"

unsigned long monte_carlo_hits_critical(unsigned long numSamples)
{
	double x, y;
	unsigned long hitCounter = 0;
	unsigned int seed;

#pragma omp parallel default(none) shared(hitCounter, numSamples) private(x, y, seed)
	{
		seed = (unsigned long)clock() + omp_get_thread_num();
#pragma omp for
		for (unsigned long i = 0; i < numSamples; i++)
		{
			x = rand_r(&seed) / (RAND_MAX + 1.0);
			y = rand_r(&seed) / (RAND_MAX + 1.0);
			if (x * x + y * y < 1)
			{
#pragma omp critical
				hitCounter++;
			}
		}
	}
	return hitCounter;
}

unsigned long monte_carlo_hits_atomic(unsigned long numSamples)
{
	double x, y;
	unsigned long hitCounter = 0;
	unsigned int seed;

#pragma omp parallel default(none) shared(hitCounter, numSamples) private(x, y, seed)
	{
		seed = (unsigned long)clock() + omp_get_thread_num();
#pragma omp for
		for (unsigned long i = 0; i < numSamples; i++)
		{
			seed = (unsigned long)clock() + omp_get_thread_num();
			x = rand_r(&seed) / (RAND_MAX + 1.0);
			y = rand_r(&seed) / (RAND_MAX + 1.0);
			if (x * x + y * y < 1)
			{
#pragma omp atomic
				hitCounter++;
			}
		}
	}
	return hitCounter;
}

unsigned long monte_carlo_hits_reduction(unsigned long numSamples)
{
	double x, y;
	unsigned long hitCounter = 0;

#pragma omp default(none) private(seed, x, y) shared(hitCounter) for reduction(+ \
																			   : hitCounter)
	for (unsigned long i = 0, seed = omp_get_thread_num(); i < numSamples; i++)
	{
		x = rand_r((unsigned int *)&seed) / (RAND_MAX + 1.0);
		y = rand_r((unsigned int *)&seed) / (RAND_MAX + 1.0);
		if (x * x + y * y < 1)
			hitCounter++;
	}
	return hitCounter;
}

int main(int argc, char **argv)
{
	int numThreads = omp_get_max_threads();
	char* ENV_CHAR = getenv(ENVIROMENT_VAR);
	if(ENV_CHAR == NULL){
		printf("Enviroment variable %s was not found.\n", ENVIROMENT_VAR);
		return -1;
	}
	char* endPtr;
	const int variant = strtol(ENV_CHAR, &endPtr, 10);
	if(*endPtr != '\0'){
		printf("%s could not be converted.\n", ENV_CHAR);
	}
	if (variant < 0 || variant > 2)
	{
		printf("Variant was not set.\n");
		printf("0 -> ATOMIC_SUM\n");
		printf("1 -> ARRAY_SUBSEQUENT\n");
		printf("2 -> ARRAY_PADDING\n");
		return -1;
	}

	ompTimeMeasure.setTimer();
	userTimeMeasure.setTimer();
	cpuTimeMeasure.setTimer();

	unsigned long (*functions[])(unsigned long) = {monte_carlo_hits_critical, monte_carlo_hits_atomic, monte_carlo_hits_reduction};

	unsigned long hits = functions[variant](NUM_SAMPLES);
	double pi = 4 * ((double)hits) / NUM_SAMPLES;

	const char *headerNames[] = {"NUM_THREDS", "VARIANT", "Value_Pi", "OMP Time", "User Time", "CPU Time"};
	int args_i[] = {numThreads, variant};
	double args_f[] = {pi, ompTimeMeasure.getTime_fs(), userTimeMeasure.getTime_fs(), cpuTimeMeasure.getTime_fs()};
	MyCSVHandler csvHandler("Ex01_CSV.csv", headerNames, 6);
	csvHandler.writeValues(args_i, 2, args_f, 4);
}
