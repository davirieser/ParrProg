#include <stdio.h>
#include <omp.h>

void writeToFile(int version, long unsigned res, double time){
    	const char* fileName = "Ex02_CSV.csv";
	FILE* file = fopen(fileName, "r");
	int writeHeader = file == NULL;
	if (file != NULL)
	{
		fclose(file);
	}
	file = fopen(fileName, "a");
	if(writeHeader){
		fprintf(file, "Version;NumThreads;Result;executionTime\n");
	}
	// printf("res: %lu, time: %2.4f seconds\n", res, end_time - start_time);
	fprintf(file, "%d;%d;%lu;%6.4f;\n", version, omp_get_max_threads(), res, time);
	fclose(file);
}