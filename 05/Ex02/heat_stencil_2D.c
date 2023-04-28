
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <omp.h>
#include <string.h>
#include <unistd.h>

#include <math.h>
#ifdef __linux__
#include <SDL2/SDL.h>
#elif defined(_WIN32)
#include <SDL.h>
#endif

#define RESOLUTION_WIDTH 50
#define RESOLUTION_HEIGHT 50

#define FACTOR (0.01)

#define SERIAL 0
#define PARALLEL 1

#ifndef VARIANT
#define VARIANT PARALLEL
#endif//VARIANT

#ifndef DEBUG
#define DEBUG 0
#endif//DEBUG

#define PERROR fprintf(stderr, "%s:%d: error: %s\n", __FILE__, __LINE__, strerror(errno))
#define PERROR_GOTO(label) \
	do { \
		PERROR; \
		goto label; \
	} while (0)


// -- vector utilities --

#define IND(y, x) ((y) * (N) + (x))

#if DEBUG
void render(double * arr, SDL_Renderer * renderer, SDL_Texture * texture, int N);
#endif

// -- simulation code ---

int main(int argc, char **argv) {
    // 'parsing' optional input parameter = problem size
    int N = 200;
    if (argc > 1) {
        N = atoi(argv[1]);
    }
    int T = N * 10;

#if DEBUG
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Renderer* renderer = NULL;
	SDL_Texture* texture = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
    }
    window = SDL_CreateWindow(
		"Heat Stencil", 
		0, 
		0, 
		N, N, 
		SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS
	);
    if (window == NULL) {
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        return 1;
    }

	texture = SDL_CreateTexture(
		renderer, 
		SDL_PIXELFORMAT_RGBA8888, 
		SDL_TEXTUREACCESS_STREAMING, 
		N, N
	);
    if (texture == NULL) {
        return 1;
    }
#endif

    // ---------- setup ----------

    // create a buffer for storing temperature fields
    double *A =  malloc(sizeof(double) * N * N);

    if(!A) PERROR_GOTO(error_a);

    // set up initial conditions in A
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[IND(i,j)] = 273; // temperature is 0° C everywhere (273 K)
        }
    }

    // and there is a heat source
    int source_x = N / 4;
    int source_y = N / 4;
    A[IND(source_x,source_y)] = 273 + 60;

#if DEBUG
	render(A, renderer, texture, N);
	usleep(1000);
#endif

    // ---------- compute ----------

    // create a second buffer for the computation
    double *B = malloc(sizeof(double) * N * N);
    if(!B) PERROR_GOTO(error_b);
    // for each time step ..
    for (int t = 0; t < T; t++) {
#if VARIANT == PARALLEL
#pragma omp parallel for collapse(2) schedule(guided)
#endif
		for (int y = 0; y < N; y ++) {
			for (int x = 0; x < N; x ++) {
				B[IND(x,y)] = A[IND(x,y)];

				// Ensure Heat Source doesn't change
				if (x == source_x && y == source_y) {
					continue;
				}

				if (x < (N-1))	B[IND(x,y)] += FACTOR * (A[IND(x+1,y	)] - 273); 
				else			B[IND(x,y)] += FACTOR * (A[IND(x	,y	)] - 273); 
				if (x > 0)		B[IND(x,y)] += FACTOR * (A[IND(x-1,y	)] - 273); 
				else			B[IND(x,y)] += FACTOR * (A[IND(x	,y	)] - 273); 
				if (y < (N-1))	B[IND(x,y)] += FACTOR * (A[IND(x	,y+1)] - 273); 
				else			B[IND(x,y)] += FACTOR * (A[IND(x	,y	)] - 273); 
				if (y > 0)		B[IND(x,y)] += FACTOR * (A[IND(x	,y-1)] - 273); 
				else			B[IND(x,y)] += FACTOR * (A[IND(x	,y	)] - 273); 
			}
		}

		double * temp = B;
		B = A;
		A = temp;

#if DEBUG
		render(A, renderer, texture, N);
		usleep(1000);
#endif
    }

    // ---------- check ----------

    int success = 1;
#if DEBUG
    // simple verification if nowhere the heat is more then the heat source
    for (long long i = 0; i < N; i++) {
        for (long long j = 0; j < N; j++) {
            double temp = A[IND(i,j)];
            if (273 <= temp && temp <= 273 + 60)
                continue;
            success = 0;
            break;
        }
    }

    printf("Verification: %s\n", (success) ? "OK" : "FAILED");
#endif


    // ---------- cleanup ----------
#if DEBUG
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
#endif

    error_b:
    free(B);
    error_a:
    free(A);

    return (success) ? EXIT_SUCCESS : EXIT_FAILURE;
}

#if DEBUG
void render(double * arr, SDL_Renderer * renderer, SDL_Texture * texture, int N) {
	int * bytes = NULL;
	int width;

	SDL_RenderClear(renderer);

	SDL_LockTexture(texture, NULL, (void **) &bytes, &width);

	for (int y = 0; y < N; y ++) {
		for (int x = 0; x < N; x ++) {
			bytes[y * N + x] = lround(arr[IND(y, x)] - 273) ;
		}
	}

	SDL_UnlockTexture(texture);

	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_RenderPresent(renderer);
}
#endif

