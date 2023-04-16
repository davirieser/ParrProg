
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#include "../../includes/TimeMeasure.cpp"
#include "../../includes/CSVHandler.cpp"

#define ATOMIC_SUM 0
#define ARRAY_SUBSEQUENT 1
#define ARRAY_PADDING 2

// #ifndef VARIANT
// #define VARIANT SERIAL
// #endif // VARIANT

// #ifndef NUM_THREADS
// #define NUM_THREADS (1)
// #endif // NUM_THREADS

#define ENVIROMENT_VAR "EXECUTION_VARIANT"

#ifndef NUM_SAMPLES
#define NUM_SAMPLES (100 * 1000 * 1000)
#endif // NUM_SAMPLES

sem_t sample_counter;

unsigned long inline checkHit(unsigned int *seed)
{
	double x = (double)rand_r(seed) / (RAND_MAX + 1.0);
	double y = (double)rand_r(seed) / (RAND_MAX + 1.0);
	return x * x + y * y < 1;
}

unsigned long monte_carlo_atomic(unsigned long samples)
{
	unsigned long totalHits = 0;

#pragma omp parallel shared(totalHits)
	{
		unsigned long localSum = 0;
		unsigned int seed = omp_get_thread_num();
#pragma omp for
		for (unsigned long i = 0; i < samples; i++)
		{
			localSum += checkHit(&seed);
		}
#pragma omp atomic
		totalHits += localSum;
	}
	return totalHits;
}

unsigned long monte_carlo_array(unsigned long samples)
{
	unsigned long totalHits = 0;
	unsigned long localArray[omp_get_max_threads()] = {0};

#pragma omp parallel shared(totalHits, localArray)
	{
		unsigned int seed = omp_get_thread_num();
#pragma omp for
		for (unsigned long i = 0; i < samples; i++)
		{
			localArray[omp_get_thread_num()] += checkHit(&seed);
		}
	}
	for (int i = 0; i < omp_get_max_threads(); i++)
	{
		totalHits += localArray[i];
	}
	return totalHits;
}

#define CACHE_LINE_SIZE (64 * 8)

unsigned long monte_carlo_array_padded(unsigned long samples)
{
	unsigned long localArray[CACHE_LINE_SIZE / sizeof(unsigned long) * omp_get_max_threads()] = {0};
	unsigned long cache_line_distance = CACHE_LINE_SIZE / sizeof(*localArray);
	unsigned long totalHits = 0;

	// unsigned int size = sizeof(unsigned long) unsigned long totalHits = 0;
	// unsigned long localArray[omp_get_max_threads()] = {0};

#pragma omp parallel shared(totalHits, localArray)
	{
		unsigned int seed = omp_get_thread_num();
#pragma omp for
		for (unsigned long i = 0; i < samples; i++)
		{
			localArray[omp_get_thread_num() * cache_line_distance] += checkHit(&seed);
		}
	}
	for (int i = 0; i < omp_get_max_threads(); i++)
	{
		totalHits += localArray[i * cache_line_distance];
	}
	return totalHits;
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
		printf("Variant was not set.");
		printf("0 -> ATOMIC_SUM\n");
		printf("1 -> ARRAY_SUBSEQUENT\n");
		printf("2 -> ARRAY_PADDING\n");
		return -1;
	}
	// if (argc < 2)
	// {
	// 	printf("Wrong number of arguments. Expected \"%s n (m)\"\n", argv[0]);
	// 	printf("With n being a number between 0 and 2 to decide the variant and m the number of treds (for case 1 and 2).\n");
	// 	printf("0 -> ATOMIC_SUM\n");
	// 	printf("1 -> ARRAY_SUBSEQUENT\n");
	// 	printf("2 -> ARRAY_PADDING\n");
	// 	return -1;
	// }
	// char *endptr;
	// const int VARIANT = strtol(argv[1], &endptr, 10);
	// if (*endptr != '\0')
	// {
	// 	printf("Number \"%s\" was not parseable. \"%s\" remained.\n", argv[1], endptr);
	// 	return -2;
	// }
	// if (VARIANT < 0 || VARIANT > 2)
	// {
	// 	printf("Number must be between 0 and 2. Number was \"%d\".\n", VARIANT);
	// 	return -3;
	// }
	unsigned long hits;
	double pi;

	userTimeMeasure.setTimer();

	if (VARIANT == ATOMIC_SUM)
	{
		hits = monte_carlo_atomic(NUM_SAMPLES);
	}
	else if (VARIANT == ARRAY_SUBSEQUENT)
	{
		hits = monte_carlo_array(NUM_SAMPLES);
	}
	else if (VARIANT == ARRAY_PADDING)
	{
		hits = monte_carlo_array_padded(NUM_SAMPLES);
	}
	else
		exit(1);

	pi = 4 * ((double)hits / NUM_SAMPLES);

	const char *headerNames[] = {"VARIANT", "NUM_THREADS", "Value_Pi", "User_Time"};
	int args_i[] = {VARIANT, omp_get_max_threads()};
	double args_f[] = {pi, userTimeMeasure.getTime_fs()};
	MyCSVHandler csvHandler("Ex03_CSV.csv", headerNames, 4);
	csvHandler.writeValues(args_i, 2, args_f, 2);
}
