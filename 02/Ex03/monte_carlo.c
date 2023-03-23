
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#define SERIAL 0
#define SEMAPHORE 1
#define FIXED_ITER 2

#ifndef VARIANT
#define VARIANT SERIAL
#endif//VARIANT

#ifndef NUM_THREADS
#define NUM_THREADS (1)
#endif//NUM_THREADS

#ifndef NUM_SAMPLES
#define NUM_SAMPLES (700 * 1000 * 1000)
#endif//NUM_SAMPLES
 
sem_t sample_counter;

unsigned long monte_carlo_hits(unsigned long samples) {
	double x, y;
	unsigned long hit = 0;
	unsigned int seed;

	for (unsigned long i = 0; i < samples; i++) {
		x = rand_r(&seed) / (RAND_MAX + 1.0);
		y = rand_r(&seed) / (RAND_MAX + 1.0);
		if (x * x + y * y < 1) 
			hit ++;
	}
	
	return hit;
}

double monte_carlo_serial(unsigned long samples) {
	unsigned long hit = monte_carlo_hits(samples);
	return 4 * (((double) hit)/samples);
}

unsigned long monte_carlo_hits_semaphore() {
	double x, y;
	unsigned long hit = 0;
	unsigned int seed;

	while(!sem_trywait(&sample_counter)) {
		x = rand_r(&seed) / (RAND_MAX + 1.0);
		y = rand_r(&seed) / (RAND_MAX + 1.0);
		if (x * x + y * y < 1) 
			hit ++;
	}

	return hit;
}

double monte_carlo_semaphore(unsigned long samples) {
	unsigned long hit = 0;
	sem_init(&sample_counter, 0, NUM_SAMPLES);

    pthread_attr_t t_attr;
    if (pthread_attr_init(&t_attr) != 0) {
        perror("pthread_attr_init");
        return -1.0;
    }

    pthread_t * ids = malloc(sizeof(pthread_t) * NUM_THREADS);
	if (ids == NULL) {
        perror("malloc");
        return -1.0;
	}

    for (size_t i = 0; i < NUM_THREADS; i++) {
        // Create Threads
        if (pthread_create(&ids[i], &t_attr, (void *(*)(void *)) monte_carlo_hits_semaphore, NULL)){
            perror("pthread_create");
        }
    }

    // Join together all Threads
    for (size_t i = 0; i < NUM_THREADS;) {
		unsigned long ret;
        if (!pthread_join(ids[i], (void *) &ret)) {
			hit += ret;
            i++;
        };
    }
	
	return 4 * (((double) hit) / samples);
}

double monte_carlo_fixed_iter(unsigned long samples) {
	// Integer Division always rounds down.
	// This leaves up to (NUM_THREADS - 1) for someone to do.
	// This implementation leaves the main-Thread to do them.
	unsigned long samples_per_thread = samples / NUM_THREADS;
	unsigned long remaining_calcs = samples % NUM_THREADS;
	unsigned long hit = 0;

    pthread_attr_t t_attr;
    if (pthread_attr_init(&t_attr) != 0) {
        perror("pthread_attr_init");
        return -1.0;
    }

    pthread_t * ids = malloc(sizeof(pthread_t) * NUM_THREADS);
	if (ids == NULL) {
        perror("malloc");
        return -1.0;
	}

    for (size_t i = 0; i < NUM_THREADS; i++) {
        // Create Threads
        if (pthread_create(&ids[i], &t_attr, (void *(*)(void *)) monte_carlo_hits, (void *) samples_per_thread)){
            perror("pthread_create");
        }
    }

	hit += monte_carlo_hits(remaining_calcs);

    // Join together all Threads
    for (size_t i = 0; i < NUM_THREADS;) {
		unsigned long ret;
        if (!pthread_join(ids[i], (void *) &ret)) {
			hit += ret;
            i++;
        };
    }
	
	return 4 * (((double) hit) / samples);
}

int main() {
	double pi;
/*
	printf("VARIANT: %d\n", VARIANT);

	clock_t start, end;
	double cpu_time_used;

	start = clock();
*/
#if VARIANT == SERIAL
	pi = monte_carlo_serial(NUM_SAMPLES);
#elif VARIANT == SEMAPHORE
	pi = monte_carlo_semaphore(NUM_SAMPLES);
#elif VARIANT == FIXED_ITER 
	pi = monte_carlo_fixed_iter(NUM_SAMPLES);
#else
	exit(1);
#endif
/*
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("Pi: %lf\n", pi);
	printf("time: %2.4f seconds\n", cpu_time_used);
*/
}

