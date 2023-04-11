
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#include "../../includes/TimeMeasure.cpp"
#include "../../includes/CSVHandler.cpp"

#ifndef NUM_SAMPLES
#define NUM_SAMPLES (500 * 1000 * 1000)
#endif // NUM_SAMPLES

sem_t sample_counter;

unsigned long monte_carlo_hits_critical(unsigned long numSamples)
{
	double x, y;
	unsigned long hitCounter = 0;
	unsigned int seed;

#pragma omp parallel shared(hitCounter) private(x, y, seed)

	seed = (unsigned long)clock() + omp_get_thread_num();
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

	return hitCounter;
}

unsigned long monte_carlo_hits_atomic(unsigned long numSamples)
{
	double x, y;
	unsigned long hitCounter = 0;
	unsigned int seed;

#pragma omp parallel shared(hitCounter) private(x, y, seed)
	seed = (unsigned long)clock() + omp_get_thread_num();

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

	return hitCounter;
}

unsigned long monte_carlo_hits_reduction(unsigned long numSamples)
{
	double x, y;
	unsigned long hitCounter = 0;
	unsigned int seed;

#pragma omp shared(hitCounter) for reduction(+ \
						  : hitCounter) // private(x, y, seed)
	for (unsigned long i = 0; i < numSamples; i++)
	{
		seed = (unsigned long)clock() + omp_get_thread_num();
		x = rand_r(&seed) / (RAND_MAX + 1.0);
		y = rand_r(&seed) / (RAND_MAX + 1.0);
		if (x * x + y * y < 1)
			hitCounter++;
	}
	return hitCounter;
}

struct args
{
	const int VARIANT;
};

struct args checkArgs(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Wrong number of arguments. Expected \"%s n \"\n", argv[0]);
		printf("With n being a number between 0 and 2 to decide the variant.\n");
		printf("0 -> Critical\n");
		printf("1 -> atomic\n");
		printf("2 -> reduction\n");
		exit(-1);
	}
	char *endptr;
	const int VARIANT = strtol(argv[1], &endptr, 10);
	if (*endptr != '\0')
	{
		printf("Number \"%s\" was not parseable. \"%s\" remained.\n", argv[1], endptr);
		exit(-2);
	}
	if (VARIANT < 0 || VARIANT > 2)
	{
		printf("Number must be between 0 and 2. Number was \"%d\".\n", VARIANT);
		exit(-3);
	}
	return (struct args){.VARIANT = VARIANT};
}

int main(int argc, char **argv)
{
	struct args args = checkArgs(argc, argv);
	int VARIANT = args.VARIANT;

	ompTimeMeasure.setTimer();
	userTimeMeasure.setTimer();
	cpuTimeMeasure.setTimer();

	unsigned long (*functions[])(unsigned long) = {monte_carlo_hits_critical, monte_carlo_hits_atomic, monte_carlo_hits_reduction};

	unsigned long hits = functions[VARIANT](NUM_SAMPLES);
	double pi = 4 * ((double)hits) / NUM_SAMPLES;

	int args_i[] = {VARIANT};
	double args_f[] = {pi, ompTimeMeasure.getTime_fs(), userTimeMeasure.getTime_fs(), cpuTimeMeasure.getTime_fs()};
	const char *headerNames[] = {"VARIANT", "Value_Pi", "OMP Time", "User Time", "CPU"};
	MyCSVHandler csvHandler("CSV.csv", headerNames, 5);
	csvHandler.writeValues(args_i, 1, args_f, 4);
}
