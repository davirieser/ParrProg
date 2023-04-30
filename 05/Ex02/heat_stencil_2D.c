
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <omp.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#ifndef DEBUG
#define DEBUG 0
#endif // DEBUG

#if DEBUG
#ifdef __linux__
#include <SDL2/SDL.h>
#elif defined(_WIN32)
#include <SDL.h>
#endif
#endif

#define RESOLUTION_WIDTH 50
#define RESOLUTION_HEIGHT 50

#define FACTOR (0.01)

#define SERIAL 0
#define PARALLEL 1

#define ENVVAR_N "ENVVAR_N"
#define ENVVAR_VARIANT "ENVVAR_VARIANT"

#define PERROR fprintf(stderr, "%s:%d: error: %s\n", __FILE__, __LINE__, strerror(errno))
#define PERROR_GOTO(label) \
    do                     \
    {                      \
        PERROR;            \
        goto label;        \
    } while (0)

// -- vector utilities --

#define IND(y, x) ((y) * (N) + (x))

#if DEBUG 
void printTemperature(double *m, int N, int M);
void render(double *arr, SDL_Renderer *renderer, SDL_Texture *texture, int N);
#endif

// -- simulation code ---

#define SAVE_STRTOL(charPtr, endPtr, resultVar)                                \
    if (charPtr == NULL)                                                       \
    {                                                                          \
        printf("%d - %s was NULL\n", __LINE__, #charPtr);                      \
        return EXIT_FAILURE;                                                   \
    }                                                                          \
    resultVar = strtol(charPtr, &endPtr, 10);                                  \
    if (*endPtr != '\0')                                                       \
    {                                                                          \
        printf("Could not parse \"%s\", \"%s\" remained.\n", charPtr, endPtr); \
        return EXIT_FAILURE;                                                   \
    }

void executeParallelStep(double **B_param, double **A_param, int N, int source_x, int source_y)
{
    double *B = *B_param;
    double *A = *A_param;
#pragma omp parallel for collapse(2) schedule(guided)
    for (int y = 0; y < N; y++)
    {
        for (int x = 0; x < N; x++)
        {
            B[IND(x, y)] = A[IND(x, y)];

            // Ensure Heat Source doesn't change
            if (x == source_x && y == source_y)
            {
                continue;
            }

            if (x < (N - 1))
                B[IND(x, y)] += FACTOR * (A[IND(x + 1, y)] - 273);
            else
                B[IND(x, y)] += FACTOR * (A[IND(x, y)] - 273);
            if (x > 0)
                B[IND(x, y)] += FACTOR * (A[IND(x - 1, y)] - 273);
            else
                B[IND(x, y)] += FACTOR * (A[IND(x, y)] - 273);
            if (y < (N - 1))
                B[IND(x, y)] += FACTOR * (A[IND(x, y + 1)] - 273);
            else
                B[IND(x, y)] += FACTOR * (A[IND(x, y)] - 273);
            if (y > 0)
                B[IND(x, y)] += FACTOR * (A[IND(x, y - 1)] - 273);
            else
                B[IND(x, y)] += FACTOR * (A[IND(x, y)] - 273);
        }
    }

    double *temp = B;
    B = A;
    A = temp;
    *B_param = B;
    *A_param = A;
}

void executeSerialStep(double **B_param, double **A_param, int N, int source_x, int source_y)
{
    double *B = *B_param;
    double *A = *A_param;
    for (int y = 0; y < N; y++)
    {
        for (int x = 0; x < N; x++)
        {
            B[IND(x, y)] = A[IND(x, y)];

            // Ensure Heat Source doesn't change
            if (x == source_x && y == source_y)
            {
                continue;
            }

            if (x < (N - 1))
                B[IND(x, y)] += FACTOR * (A[IND(x + 1, y)] - 273);
            else
                B[IND(x, y)] += FACTOR * (A[IND(x, y)] - 273);
            if (x > 0)
                B[IND(x, y)] += FACTOR * (A[IND(x - 1, y)] - 273);
            else
                B[IND(x, y)] += FACTOR * (A[IND(x, y)] - 273);
            if (y < (N - 1))
                B[IND(x, y)] += FACTOR * (A[IND(x, y + 1)] - 273);
            else
                B[IND(x, y)] += FACTOR * (A[IND(x, y)] - 273);
            if (y > 0)
                B[IND(x, y)] += FACTOR * (A[IND(x, y - 1)] - 273);
            else
                B[IND(x, y)] += FACTOR * (A[IND(x, y)] - 273);
        }
    }

    double *temp = B;
    B = A;
    A = temp;
    *B_param = B;
    *A_param = A;
}

int main(int argc, char **argv)
{
    char *envvar_variant = getenv(ENVVAR_VARIANT);
    char *envvar_n = getenv(ENVVAR_N);
    char *endPtr;
    int variant;
    int N = 200;
    SAVE_STRTOL(envvar_variant, endPtr, variant);
    // 'parsing' optional eviromentVar = problem size
    if (envvar_n != NULL)
    {
        SAVE_STRTOL(envvar_n, endPtr, N);
    }
    int T = N * 10;

#if DEBUG && SDL
    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return 1;
    }
    window = SDL_CreateWindow(
        "Heat Stencil",
        0,
        0,
        N, N,
        SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    if (window == NULL)
    {
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        return 1;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        N, N);
    if (texture == NULL)
    {
        return 1;
    }
#endif

    // ---------- setup ----------

    // create a buffer for storing temperature fields
    double *A = malloc(sizeof(double) * N * N);

    if (!A)
        PERROR_GOTO(error_a);

    // set up initial conditions in A
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[IND(i, j)] = 273; // temperature is 0° C everywhere (273 K)
        }
    }

    // and there is a heat source
    int source_x = N / 4;
    int source_y = N / 4;
    A[IND(source_x, source_y)] = 273 + 60;

#if DEBUG
#if SDL
    render(A, renderer, texture, N);
    usleep(1000);
#else 
    printf("Computing heat-distribution for room size %dX%d for T=%d timesteps\n", N, N, T);

    printf("Initial:");
    printTemperature(A, N, N);
    printf("\n");
#endif
#endif

    // ---------- compute ----------

    // create a second buffer for the computation
    double *B = malloc(sizeof(double) * N * N);
    if (!B)
        PERROR_GOTO(error_b);
    // for each time step ..
    for (int t = 0; t < T; t++)
    {
        switch (variant)
        {
        case SERIAL:
            executeSerialStep(&B, &A, N, source_x, source_y);
            break;
        case PARALLEL:
            executeParallelStep(&B, &A, N, source_x, source_y);
            break;
        default:
            printf("Variant was not allowed value. Value was %d\n", variant);
            break;
        }

#if DEBUG
#if SDL
        render(A, renderer, texture, N);
        usleep(1000);
#else 
		// every 1000 steps show intermediate step
        if (!(t % 1000)) {
            printf("Step t=%d\n", t);
            printTemperature(A, N, N);
            printf("\n");
        }
#endif
#endif
    }

    // ---------- check ----------

    int success = 1;
#if DEBUG
    // simple verification if nowhere the heat is more then the heat source
    for (long long i = 0; i < N; i++)
    {
        for (long long j = 0; j < N; j++)
        {
            double temp = A[IND(i, j)];
            if (273 <= temp && temp <= 273 + 60)
                continue;
            success = 0;
            break;
        }
    }

    printf("Verification: %s\n", (success) ? "OK" : "FAILED");
#endif

    // ---------- cleanup ----------
#if DEBUG && SDL
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
#if SDL
void render(double *arr, SDL_Renderer *renderer, SDL_Texture *texture, int N)
{
    int *bytes = NULL;
    int width;

    SDL_RenderClear(renderer);

    SDL_LockTexture(texture, NULL, (void **)&bytes, &width);

    for (int y = 0; y < N; y++)
    {
        for (int x = 0; x < N; x++)
        {
            bytes[y * N + x] = lround(arr[IND(y, x)] - 273);
        }
    }

    SDL_UnlockTexture(texture);

    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_RenderPresent(renderer);
}

#else

void printTemperature(double *m, int N, int M) {
    const char *colors = " .-:=+*^X#%@";
    const int numColors = 12;

    // boundaries for temperature (for simplicity hard-coded)
    const double max = 273 + 30;
    const double min = 273 + 0;

    // set the 'render' resolution
    int W = RESOLUTION_WIDTH;
    int H = RESOLUTION_HEIGHT;

    // step size in each dimension
    int sW = N / W;
    int sH = M / H;

    // upper wall
    printf("\t");
    for (int u = 0; u < W + 2; u++) {
        printf("X");
    }
    printf("\n");
    // room
    for (int i = 0; i < H; i++) {
        // left wall
        printf("\tX");
        // actual room
        for (int j = 0; j < W; j++) {
            // get max temperature in this tile
            double max_t = 0;
            for (int x = sH * i; x < sH * i + sH; x++) {
                for (int y = sW * j; y < sW * j + sW; y++) {
                    max_t = (max_t < m[IND(x,y)]) ? m[IND(x,y)] : max_t;
                }
            }
            double temp = max_t;

            // pick the 'color'
            int c = ((temp - min) / (max - min)) * numColors;
            c = (c >= numColors) ? numColors - 1 : ((c < 0) ? 0 : c);

            // print the average temperature
            printf("%c", colors[c]);
        }
        // right wall
        printf("X\n");
    }
    // lower wall
    printf("\t");
    for (int l = 0; l < W + 2; l++) {
        printf("X");
    }
    printf("\n");
}
#endif
#endif
