
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define DEFAULT_GRAV_CONSTANT 6.674e-11

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

vector_t generate_random_point() {
	return (vector_t) { .x = rand(), .y = rand(), .z = rand() }; 
}

universe_t init_system(int num_points, double grav_constant) {
	body_t * bodies = malloc(sizeof(body_t) * num_points);

	for (int i = 0; i < num_points; i ++) {
		bodies[i] = (body_t) {
			.position = generate_random_point(),
			.velocity = generate_random_point(),
			.acceleration = generate_random_point(),
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

int main(int argc, char ** argv) {
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
			printf("Could not convert Maximum Time: \"%s\"\n", argv[3]);
			return EXIT_FAILURE;
		}
	} else {
		grav_constant = DEFAULT_GRAV_CONSTANT;
	}

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

	universe_t universe = init_system(num_bodies, grav_constant);
	printf(
		"Simulating Universe with \n\t- %d Bodies\n\t- Gravitational Constant = %lf\n\t- From 0 to %lf using Time Step %lf\n", 
		num_bodies, 
		grav_constant, 
		max_time, 
		time_step
	);

	double t = 0;
	while (t < max_time) {
		simulate_universe(universe, time_step);
		t += time_step;
	}

	cleanup_system(universe);
	
	return EXIT_SUCCESS;
}

