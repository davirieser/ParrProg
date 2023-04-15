
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#include "../../includes/TimeMeasure.cpp"
#include "../../includes/CSVHandler.cpp"

#ifndef VARIANT
#define VARIANT -1
#endif

#ifndef NUM_SAMPLES
#define NUM_SAMPLES (500 * 1000 * 1000)
#endif // NUM_SAMPLES

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
	const char *headerNames[] = {"NUM_THREDS", "VARIANT", "Value_Pi", "OMP Time", "User Time", "CPU Time"};
	MyCSVHandler csvHandler("Ex01_CSV.csv", headerNames, 6);
	if (VARIANT < 0 || VARIANT > 2)
	{
		printf("Variant was not set.\n");
		printf("0 -> ATOMIC_SUM\n");
		printf("1 -> ARRAY_SUBSEQUENT\n");
		printf("2 -> ARRAY_PADDING\n");
		char buffer[64];
		snprintf(buffer, 64, "Variant was not allowed. Value was %d.", VARIANT);
		csvHandler.writeErrorLine(buffer);
		return -1;
	}

	ompTimeMeasure.setTimer();
	userTimeMeasure.setTimer();
	cpuTimeMeasure.setTimer();

	unsigned long (*functions[])(unsigned long) = {monte_carlo_hits_critical, monte_carlo_hits_atomic, monte_carlo_hits_reduction};

	unsigned long hits = functions[VARIANT](NUM_SAMPLES);
	double pi = 4 * ((double)hits) / NUM_SAMPLES;

	int args_i[] = { omp_get_max_threads(), VARIANT};
	double args_f[] = {pi, ompTimeMeasure.getTime_fs(), userTimeMeasure.getTime_fs(), cpuTimeMeasure.getTime_fs()};
	csvHandler.writeValues(args_i, 2, args_f, 4);
}
