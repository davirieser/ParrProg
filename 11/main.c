
#define _GNU_SOURCE 

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

/* ----- Constant Definitions ----- */

#ifndef DEFAULT_GRAV_CONSTANT
#define DEFAULT_GRAV_CONSTANT 6.674e-11
#endif

#ifndef MAX_X
#define MAX_X 1000.0
#endif
#ifndef MAX_Y
#define MAX_Y 1000.0
#endif
#ifndef MAX_Z
#define MAX_Z 1000.0
#endif

#ifndef MAX_INIT_VELOCITY 
#define MAX_INIT_VELOCITY 10
#endif

/* ----- Type Definitions ----- */

typedef struct {
	double x, y, z;
} vector_t;

typedef struct {
	vector_t position;
	vector_t velocity;
	vector_t acceleration;
	double mass;
} body_t;

typedef struct {
	double grav_constant;
	long num_bodies;
	body_t * bodies;
} universe_t;

/* ----- Utility Function Definitions ----- */

/*
 * Generate random number between 0 and max.
 */
double rand_positive_double(double max){
   return max * (rand() / ((double)RAND_MAX + 1));
}

/*
 * Generate random number between -(max/2) and max/2.
 */
double rand_double(double max){
	return max * ((rand() / ((double)RAND_MAX + 1)) - 0.5);
}

vector_t generate_random_position() {
	return (vector_t) { 
		.x = rand_positive_double(MAX_X), 
		.y = rand_positive_double(MAX_Y), 
		.z = rand_positive_double(MAX_Z) 
	}; 
}

vector_t generate_random_velocity() {
	return (vector_t) { 
		.x = rand_double(MAX_INIT_VELOCITY), 
		.y = rand_double(MAX_INIT_VELOCITY), 
		.z = rand_double(MAX_INIT_VELOCITY) 
	}; 
}

/* ----- System Definitions ----- */

universe_t init_system(int num_points, double grav_constant) {
	body_t * bodies = malloc(sizeof(body_t) * num_points);

	for (int i = 0; i < num_points; i ++) {
		bodies[i] = (body_t) {
			.position = generate_random_position(),
			.velocity = generate_random_velocity(),
			.acceleration = (vector_t) { .x = 0.0, .y = 0.0, .z = 0.0 },
			.mass = rand(),
		};
	}
	
	return (universe_t) {
		.bodies = bodies,
		.num_bodies = num_points,
		.grav_constant = grav_constant,
	};
}

void simulate_universe(universe_t universe, double delta_time) {
	// TODO
}

void cleanup_system(universe_t universe) {
	free(universe.bodies);
}

void plot_system(char * file_name, universe_t universe, bool truncate) {
	mode_t mode = O_CREAT | O_WRONLY;
	if (truncate) mode |= O_TRUNC;

	int file = open(file_name, mode, 0644);
	if (file == -1) return;

	for (int i = 0; i < universe.num_bodies; i ++) {
		int bytes;
		char * buf;
		vector_t position = universe.bodies[i].position;
		if ((bytes = asprintf(&buf, "%lf %lf %lf\n", position.x, position.y, position.z)) != -1) {
			write(file, buf, bytes);
			free(buf);
		}
	}
	write(file, "\n", 1);
	close(file);
}

/* ----- GnuPlot Generation Function ----- */

#define GNU_PLOT_FILE "particle.plt"

#define GNU_PLOT_HEADER "set terminal gif animate delay 100 # set gif to animate in a frame delay of 100 ms\n\
set output 'output.gif' # write to the file output.gif\n\
\n\
set style line 2 lc rgb 'black' pt 7 # set line to be a filled circle of color black\n\
stats '%s' nooutput # read data from data.dat file \n\
set xrange [0:%lf]\n\
set yrange [0:%lf]\n\
set zrange [0:%lf]\n\
\n\
do for [i=1:int(STATS_blocks)] {\n\
   splot '%s' index (i-1) with points ls 2 ps 0.4 # for each datapoint plot the point\n\
}\n"

void generate_gnuplot_file(double time_step, char * file_name) {
	int file = open(GNU_PLOT_FILE, O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0644);
	if (file == -1) {
		return;
	}

	int bytes;
	char * contents;
	if ((bytes = asprintf(&contents, GNU_PLOT_HEADER, file_name, MAX_X, MAX_Y, MAX_Z, file_name)) != -1) {
		write(file, contents, bytes);
	}

	free(contents);
	close(file);
}

/* ----- Main Function ----- */

int main(int argc, char ** argv) {
	// Parse Command Line Arguments
	if (argc != 4 && argc != 5) {
		printf("Usage: <%s> <Number of Bodies> <Time Step> <Maximum Time> [Gravitational Constant]\n", argv[0]);
		return EXIT_FAILURE;
	}

	char * p;
	int num_bodies = strtol(argv[1], &p, 10);
	if (*p != '\0') {
		printf("Could not convert Number of Bodies: \"%s\"\n", argv[1]);
		return EXIT_FAILURE;
	}

	double time_step = strtod(argv[2], &p);
	if (*p != '\0') {
		printf("Could not convert Time Step: \"%s\"\n", argv[2]);
		return EXIT_FAILURE;
	}

	double max_time = strtod(argv[3], &p);
	if (*p != '\0') {
		printf("Could not convert Maximum Time: \"%s\"\n", argv[3]);
		return EXIT_FAILURE;
	}

	double grav_constant; 
	if (argc >= 5) {
		grav_constant = strtod(argv[4], &p);
		if (*p != '\0') {
			printf("Could not convert Maximum Time: \"%s\"\n", argv[4]);
			return EXIT_FAILURE;
		}
	} else {
		grav_constant = DEFAULT_GRAV_CONSTANT;
	}

	// Check Values are valid
	if (max_time <= time_step) {
		printf("Max Time is smaller than Time Step\n");
		return EXIT_FAILURE;
	}
	if (isinf(max_time)) {
		printf("Max Time cannot be infinite\n");
		return EXIT_FAILURE;
	}
	if (isnan(max_time)) {
		printf("Max Time is not a number\n");
		return EXIT_FAILURE;
	}

	// Generate Universe 
	universe_t universe = init_system(num_bodies, grav_constant);
	printf(
		"Simulating Universe with \n\t- %d Bodies\n\t- Gravitational Constant = %lf\n\t- From 0 to %lf using Time Step %lf\n", 
		num_bodies, 
		grav_constant, 
		max_time, 
		time_step
	);

	plot_system("data.dat", universe, 1);
	// Simulate Universe
	double t = 0;
	while (t < max_time) {
		simulate_universe(universe, time_step);
		t += time_step;
	}

	cleanup_system(universe);

	generate_gnuplot_file(time_step, "data.dat");
	
	return EXIT_SUCCESS;
}

