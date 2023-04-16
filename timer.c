
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include "dlfcn.h"
#include "sys/time.h"
#include "time.h"

#define FILE_NAME "log.csv"

#ifndef ENVS
#define ENVS { "EUID", "TEST", NULL } 
#endif

#ifndef DEFAULT_HEADERS
#define DEFAULT_HEADERS "ProgramName; CPU Time; Wall Time; Return Value"
#endif //DEFAULT_HEADERS
	   
#define DELIMITER "; "
#define SIZE_DELIMITER (sizeof(DELIMITER) - 1)

#define SIZE_DEFAULT_HEADERS (sizeof(DEFAULT_HEADERS) - 1)

char * get_header(int * len) {
	char * envs [] = ENVS; 
	int num_envs = 0;
	while (envs[num_envs] != NULL) num_envs ++;

	int size = SIZE_DEFAULT_HEADERS;
	int * sizes = malloc(sizeof(int) * num_envs);
	for (int i = 0; i < num_envs; i ++) {
		int t = strlen(envs[i]);
		size += t + SIZE_DELIMITER;
		sizes[i] = t;
	}

	int buffer_len = size + 1;
	char * ret = malloc(sizeof(char) * buffer_len);
	int position = 0;

	memcpy(ret + position, DEFAULT_HEADERS, SIZE_DEFAULT_HEADERS);
	position += SIZE_DEFAULT_HEADERS;

	for (int i = 0; i < num_envs; i ++) {
		memcpy(ret + position, DELIMITER, SIZE_DELIMITER);
		position += SIZE_DELIMITER;
		memcpy(ret + position, envs[i], sizes[i]);
		position += sizes[i];
	}

	ret[size] = '\0';
	*len = buffer_len;

	free(sizes);

	return ret;
}

char * get_envs() {
	char * envs [] = ENVS; 
	int num_envs = 0;
	while (envs[num_envs] != NULL) num_envs ++;

	int size = 0;
	int * sizes = malloc(sizeof(int) * num_envs);
	char ** env_pointers = malloc(sizeof(char *) * num_envs);

	for (int i = 0; i < num_envs; i ++) {
		env_pointers[i] = getenv(envs[i]);
		int t = 0;
		if(env_pointers[i] == NULL){
		} else {
			t = strlen(env_pointers[i]);
		}
		size += t + SIZE_DELIMITER;
		sizes[i] = t;
	}

	char * ret = malloc(sizeof(char) * size + 1);

	int position = 0;
	for (int i = 0; i < num_envs; i ++) {
		memcpy(ret + position, DELIMITER, SIZE_DELIMITER);
		position += SIZE_DELIMITER;
		if (env_pointers[i] != NULL) {
			memcpy(ret + position, env_pointers[i], sizes[i]);
			position += sizes[i];
		}
	}

	ret[size] = '\0';

	free(sizes);
	free(env_pointers);

	return ret;
}

int time_program(char * program) {
    struct timeval t1, t2;
	clock_t start_t, end_t;
    double elapsed_time, cpu_time;

    // Try to open the Library
    void * handle = dlopen(program, RTLD_LAZY);
    if (handle == NULL) {
        printf(
            "Error while loading shared libraries: %s: cannot open shared object file: No such file or directory",
            program
        );
        return -1;
    }

    /* Source for the Error Handling: Man Page dlopen */
    dlerror();    /* Clear any existing error */

    void * (* setup)(void) = dlsym(handle, "setup");

    /* Check if an actual Error occured */
    char * error = dlerror();
    if (error != NULL) {
        printf(
            "Symbol lookup error: Could not find setup Function\n"
        );
        return -1;
    }
	void * setup_pointer = setup();

    long (* run)(void *) = dlsym(handle, "setup");

    /* Check if an actual Error occured */
    error = dlerror();
    if (error != NULL) {
        printf(
            "Symbol lookup error: Could not find setup Function\n"
        );
        return -1;
    }

    gettimeofday(&t1, NULL);
	start_t = clock();

    long ret = run(setup_pointer);

	end_t = clock();
    gettimeofday(&t2, NULL);

	cpu_time = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    // compute and print the elapsed time in millisec
    elapsed_time = (t2.tv_sec - t1.tv_sec);  
    elapsed_time += ((double) (t2.tv_usec - t1.tv_usec)) / 10000000;

	bool writeHeaders = false;
	if (access(FILE_NAME, F_OK) == -1) {
		writeHeaders = true;
	}

	int pfd;
	if ((pfd = open(FILE_NAME, O_APPEND | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
		perror("Cannot open output file\n"); 
		return EXIT_FAILURE;
	}

	if (writeHeaders) {
		int size;
		char * headers = get_header(&size);
		headers[size - 1] = '\n';
		if(write(pfd, headers, size) == -1) {
			perror("Could not write to Output File\n"); 
			free(headers);
			return EXIT_FAILURE;
		}
		free(headers);
	}

	char * str, * env_string = get_envs();
	int len = asprintf(&str, "%s; %f s; %f s; %ld%s\n", program, cpu_time, elapsed_time, ret, env_string);
	if (write(pfd, str, len) == -1) {
		perror("Could not write to Output File\n"); 
		free(str);
		free(env_string);
		return EXIT_FAILURE;
	}

	free(str);
	free(env_string);
	close(pfd);

    return 0;
}


int main(int argc, char ** argv) {
	time_program(argv[1]);
}
