
#define _GNU_SOURCE 

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>

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
	vector_t force;
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

vector_t add_vectors(vector_t a, vector_t b) {
	return (vector_t) {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z,
	};
}

vector_t scale_vector(vector_t a, double scalar) {
	return (vector_t) {
		.x = a.x * scalar,
		.y = a.y * scalar,
		.z = a.z * scalar,
	};
}

vector_t acceleration_from_force(vector_t force, double body_mass, double time_step) {
	return (vector_t) {
		.x = (force.x / body_mass) * time_step,
		.y = (force.y / body_mass) * time_step,
		.z = (force.z / body_mass) * time_step,
	};
}

double compute_distance(vector_t a, vector_t b) {
	return sqrt(pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0) + pow(a.z - b.z, 2.0));
}

/* ----- Octree ----- */

typedef enum {
	INTERNAL,
	BODY,
	LEAF,
} octree_cell_state_e;

/*
 * A Cell has three possible "states" that it can have:
 * - It is an internal Cell that has 8 Subcells.
 * - It is a Body Cell that has a Body and no Subcells.
 * - It is a Leaf Cell with neither a Body nor Subcells.
 *   This "state" is signified by a mass smaller 0.
 */
typedef struct octree_cell_t {
	double mass;			  // Combined Mass of the Bodies in this Cell
							  // If the Mass is smaller 0 this is a Leaf.
	vector_t position;		  // Absolute Position of the Cell.
	vector_t center_position; // Center of Mass of the Bodies in this Cell
	vector_t cell_size;		  // Size of the Cell
	body_t * body;			  // Pointer to the Body in the Cell
							  // If this is null, the cell is internal
	struct octree_cell_t * subcells[8];
} octree_cell_t;

octree_cell_state_e get_octree_cell_state(octree_cell_t * cell) {
	if (cell->body != NULL) return BODY;
	return cell->mass >= 0 ? INTERNAL : LEAF;
}

octree_cell_t * create_octree_cell(vector_t position, vector_t size) {
	octree_cell_t * cell = malloc(sizeof(octree_cell_t));

	cell->position = position;
	cell->mass = -1;
	cell->center_position = (vector_t) {
		.x = 0.0,
		.y = 0.0,
		.z = 0.0,
	};
	cell->cell_size = size;
	cell->body = NULL;

	return cell;
}

void octree_assign_body(octree_cell_t * cell, body_t * body) {
	cell->body = body;
	cell->center_position = body->position;
	cell->mass = body->mass;
}

int octree_locate_subcell(octree_cell_t * cell, body_t * body) {
	vector_t bpos = body->position;
	vector_t cell_center = add_vectors(cell->position, scale_vector(cell->cell_size, 0.5));

	int x = bpos.x > cell_center.x ? 1 : 0;
	int y = bpos.y > cell_center.y ? 1 : 0;
	int z = bpos.z > cell_center.z ? 1 : 0;

	int subcell = (x << 2) + (y << 1) + z;

	printf("Subcell (%lf, %lf, %lf) (%lf, %lf, %lf): %d\n", bpos.x, bpos.y, bpos.z, cell_center.x, cell_center.y, cell_center.z, subcell);

	return subcell;
}

/*
 * Convert the given Cell from a Body to an internal Cell.
 */
void octree_generate_subcells(octree_cell_t * cell) {
	vector_t subcell_size = scale_vector(cell->cell_size, 0.5);

	for (int i = 0; i < 8; i ++) {
		vector_t offset = (vector_t) {
			.x = (i & 4) > 0 ? subcell_size.x : 0.0,
			.y = (i & 2) > 0 ? subcell_size.y : 0.0,
			.z = (i & 1) > 0 ? subcell_size.z : 0.0,
		};
		cell->subcells[i] = create_octree_cell(add_vectors(cell->position, offset), subcell_size);
		printf("Created Subcell at (%lf, %lf, %lf)\n", cell->subcells[i]->position.x, cell->subcells[i]->position.y, cell->subcells[i]->position.z);
	}

	// Move the current Cell's Body into the appropriate Subcell.
	// Mass and Center of Mass do not need to be adjusted because the Bodies didn't change.
	octree_cell_t * new_cell = cell->subcells[octree_locate_subcell(cell, cell->body)];
	octree_assign_body(new_cell, cell->body);
	cell->body = NULL;
}

void octree_add(octree_cell_t * root, body_t * body) {
	bool run = true;
	octree_cell_t * cell = root;

	while (run) {
		switch (get_octree_cell_state(cell)) {
			case BODY:
				printf("Generating Subcells %p (%lf, %lf, %lf)\n", (void*) cell, cell->position.x, cell->position.y, cell->position.z);
				octree_generate_subcells(cell);
				cell = cell->subcells[octree_locate_subcell(cell, body)];
				break;
			case INTERNAL: {
				double bmass = body->mass, cmass = cell->mass, total_mass = cmass + bmass;
				vector_t bpos = body->position, cpos = cell->center_position;
				cell->center_position = (vector_t) {
					.x = (bpos.x * bmass + cpos.x * cmass) / total_mass,
					.y = (bpos.y * bmass + cpos.y * cmass) / total_mass,
					.z = (bpos.z * bmass + cpos.z * cmass) / total_mass,
				};
				cell->mass = total_mass;
				cell = cell->subcells[octree_locate_subcell(cell, body)];
				break;
			}
			case LEAF:
				octree_assign_body(cell, body);
				run = false;
				break;
		}
	}
}

void print_octree(octree_cell_t * root, int indent_level) {
	printf("%*s", 2 * indent_level, "");

	switch (get_octree_cell_state(root)) {
		case INTERNAL:
			printf("Internal with Mass %lf at (%lf, %lf, %lf)\n", 
				root->mass, 
				root->center_position.x,
				root->center_position.y,
				root->center_position.z
			);
			for (int i = 0; i < 8; i++) {
				print_octree(root->subcells[i], indent_level+1);
			}
			break;
		case BODY:
			printf("Body at (%lf, %lf, %lf) with Velocity (%lf, %lf, %lf) with Mass %lf\n",
				root->body->position.x,
				root->body->position.y,
				root->body->position.z,
				root->body->velocity.x,
				root->body->velocity.y,
				root->body->velocity.z,
				root->body->mass
			);
			break;
		case LEAF:
			printf("Leaf\n");
			break;
	}
}

void free_octree(octree_cell_t * root) {
	if (get_octree_cell_state(root) == INTERNAL) {
		for (int i = 0; i < 8; i++) {
			free_octree(root->subcells[i]);
		}
	}

	free(root);
}

/* ----- Body Update Functions ----- */

void calculate_velocity(body_t * body, double time_step) {
	vector_t acc = acceleration_from_force(body->force, body->mass, time_step);

	body->velocity = add_vectors(body->velocity, scale_vector(acc, time_step));
}

void calculate_position(body_t * body, double time_step) {
	body->position = add_vectors(body->position, scale_vector(body->velocity, time_step));
}

/* ----- System Definitions ----- */

universe_t init_system(int num_points, double grav_constant) {
	body_t * bodies = malloc(sizeof(body_t) * num_points);

	for (int i = 0; i < num_points; i ++) {
		bodies[i] = (body_t) {
			.position = generate_random_position(),
			.velocity = generate_random_velocity(),
			.force = (vector_t) { .x = 0.0, .y = 0.0, .z = 0.0 },
			.mass = rand_positive_double(100),
		};
	}
	
	return (universe_t) {
		.bodies = bodies,
		.num_bodies = num_points,
		.grav_constant = grav_constant,
	};
}

void simulate_universe(universe_t universe, double delta_time) {
	body_t * bodies = universe.bodies;
	for (int i = 0; i < universe.num_bodies; i ++) {
		calculate_position(bodies + i, delta_time);
	}
}

void cleanup_system(universe_t universe) {
	free(universe.bodies);
}

void plot_system(char * file_name, universe_t universe, bool truncate) {
	mode_t mode = O_CREAT | O_WRONLY;
	if (truncate) mode |= O_TRUNC;
	else mode |= O_APPEND;

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
	write(file, "\n\n", 2);
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
	srand(time(NULL));

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
		plot_system("data.dat", universe, 0);
		t += time_step;
	}

	generate_gnuplot_file(time_step, "data.dat");

	cleanup_system(universe);

	return EXIT_SUCCESS;
}

