
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#include "../../includes/TimeMeasure.cpp"
#include "../../includes/CSVHandler.cpp"


#define SERIAL 0
#define SEMAPHORE 1
#define FIXED_ITER 2

// #ifndef VARIANT
// #define VARIANT SERIAL
// #endif // VARIANT

// #ifndef NUM_THREADS
// #define NUM_THREADS (1)
// #endif // NUM_THREADS

#ifndef NUM_SAMPLES
#define NUM_SAMPLES (700 * 1000 * 1000)
#endif // NUM_SAMPLES

sem_t sample_counter;

void* monte_carlo_hits(void* ptr)
{
	unsigned long samples = (unsigned long)ptr;
	double x, y;
	unsigned long hit = 0;
	unsigned int seed;

	for (unsigned long i = 0; i < samples; i++)
	{
		x = rand_r(&seed) / (RAND_MAX + 1.0);
		y = rand_r(&seed) / (RAND_MAX + 1.0);
		if (x * x + y * y < 1)
			hit++;
	}

	return (void*)hit;
}

double monte_carlo_serial(unsigned long samples)
{
	unsigned long hit = (unsigned long)monte_carlo_hits((void*)samples);
	return 4 * (((double)hit) / samples);
}

void* monte_carlo_hits_semaphore(void*)
{
	double x, y;
	unsigned long hit = 0;
	unsigned int seed;

	while (!sem_trywait(&sample_counter))
	{
		x = rand_r(&seed) / (RAND_MAX + 1.0);
		y = rand_r(&seed) / (RAND_MAX + 1.0);
		if (x * x + y * y < 1)
			hit++;
	}

	return (void*)hit;
}

double monte_carlo_semaphore(unsigned long samples, const size_t numThreads)
{
	unsigned long hit = 0;
	sem_init(&sample_counter, 0, NUM_SAMPLES);

	pthread_attr_t t_attr;
	if (pthread_attr_init(&t_attr) != 0)
	{
		perror("pthread_attr_init");
		return -1.0;
	}

	pthread_t *ids = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);
	if (ids == NULL)
	{
		perror("malloc");
		return -1.0;
	}

	for (size_t i = 0; i < numThreads; i++)
	{
		// Create Threads
		if (pthread_create(&ids[i], &t_attr, (void *(*)(void *))monte_carlo_hits_semaphore, NULL))
		{
			perror("pthread_create");
		}
	}

	// Join together all Threads
	for (size_t i = 0; i < numThreads;)
	{
		unsigned long ret;
		if (!pthread_join(ids[i], (void **)&ret))
		{
			hit += ret;
			i++;
		};
	}
	pthread_attr_destroy(&t_attr);
	free(ids);

	return 4 * (((double)hit) / samples);
}

double monte_carlo_fixed_iter(unsigned long samples, const size_t numThreads)
{
	// Integer Division always rounds down.
	// This leaves up to (numThreads - 1) for someone to do.
	// This implementation leaves the main-Thread to do them.
	unsigned long samples_per_thread = samples / numThreads;
	unsigned long remaining_calcs = samples % numThreads;
	unsigned long hit = 0;

	pthread_attr_t t_attr;
	if (pthread_attr_init(&t_attr) != 0)
	{
		perror("pthread_attr_init");
		return -1.0;
	}

	pthread_t *ids = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);
	if (ids == NULL)
	{
		perror("malloc");
		return -1.0;
	}

	for (size_t i = 0; i < numThreads; i++)
	{
		// Create Threads
		if (pthread_create(&ids[i], &t_attr, (void *(*)(void *))monte_carlo_hits, (void *)samples_per_thread))
		{
			perror("pthread_create");
		}
	}

	hit += (unsigned long)monte_carlo_hits((void*)remaining_calcs);

	// Join together all Threads
	for (size_t i = 0; i < numThreads;)
	{
		unsigned long ret;
		if (!pthread_join(ids[i], (void **)&ret))
		{
			hit += ret;
			i++;
		};
	}
	free(ids);
	pthread_attr_destroy(&t_attr);

	return 4 * (((double)hit) / samples);
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Wrong number of arguments. Expected \"%s n (m)\"\n", argv[0]);
		printf("With n being a number between 0 and 2 to decide the variant and m the number of treds (for case 1 and 2).\n");
		printf("0 -> SERIAL\n");
		printf("1 -> SEMAPHORE\n");
		printf("2 -> FIXED_ITER\n");
		return -1;
	}
	char *endptr;
	const int VARIANT = strtol(argv[1], &endptr, 10);
	if (*endptr != '\0')
	{
		printf("Number \"%s\" was not parseable. \"%s\" remained.\n", argv[1], endptr);
		return -2;
	}
	if (VARIANT < 0 || VARIANT > 2)
	{
		printf("Number must be between 0 and 2. Number was \"%d\".\n", VARIANT);
		return -3;
	}
	int numThreads = 1;
	if (VARIANT > 0)
	{
		if (argc >= 3)
		{
			numThreads = strtol(argv[2], &endptr, 10);
			if (*endptr != '\0')
			{
				printf("Number \"%s\" was not parseable. \"%s\" remained.\n", argv[2], endptr);
				return -4;
			}
			if (numThreads < 1 || numThreads > 100)
			{

				printf("Number \"%d\" was not in allowed range. Must be within 1 and 100.\n", numThreads);
				return -5;
			}
		}
	}
	// FILE *file = fopen("Data.csv", "a");
	// fprintf(file, "VARIANT: %d\n", VARIANT);
	if (VARIANT > 0)
	{
		// fprintf(file, "Number of Threads: %lu\n", numThreads);
	}
	double pi;
	cpuTimeMeasure.setTimer();
	userTimeMeasure.setTimer();
	const char* headerNames[] = {"VARIANT", "NUM_THREADS", "Value_Pi", "CPU_Time", "User_Time"};
	MyCSVHandler csvHandler("CSV.csv", headerNames, 5);

	if (VARIANT == SERIAL)
	{
		pi = monte_carlo_serial(NUM_SAMPLES);
	}
	else if (VARIANT == SEMAPHORE)
	{
		pi = monte_carlo_semaphore(NUM_SAMPLES, numThreads);
	}
	else if (VARIANT == FIXED_ITER)
	{
		pi = monte_carlo_fixed_iter(NUM_SAMPLES, numThreads);
	}
	else
		exit(1);
	int args_i[] = {VARIANT, numThreads};
	double args_f[] = {pi, cpuTimeMeasure.getTime_fs(), userTimeMeasure.getTime_fs()};
	int order[] = {1, 2, -1, -2, -3};
	csvHandler.writeValues(args_i, 2, args_f, 3, order);
}
