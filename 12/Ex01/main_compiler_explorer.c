#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define N 1000

void a(unsigned c1)
{
    unsigned c2 = 32 * c1;
    printf("a): %u\n", c2);
}

// Sould be applied if performing a multiplication is more expensive than a shift
void a_solution(unsigned c1)
{
    unsigned c2 = c1 << 5;
    printf("a_s): %u\n", c2);
}

void b(unsigned c1)
{
    unsigned c2 = 15 * c1;
    printf("b): %u\n", c2);
}

// Should be applied if performing a multiplication is more expensive than a shift and a subtraction
void b_solution(unsigned c1)
{
    unsigned c2 = (c1 << 4) - c1;
    printf("b_s): %u\n", c2);
}

void c(unsigned c1)
{
    unsigned c2 = 96 * c1;
    printf("c): %u\n", c2);
}

// Should be applied if performing a multiplication is more expensive than two shifts and an addition
void c_solution(unsigned c1)
{
    unsigned c2 = (c1 << 5) + (c1 << 6);
    printf("c_2): %u\n", c2);
}

void d(unsigned c1)
{
    unsigned c2 = 0.125 * c1;
    printf("d): %u\n", c2);
}

// Should be applied if performing a multiplication is more expensive than a shift
void d_solution(unsigned c1)
{
    unsigned c2 = c1 >> 3;
    printf("d_s): %u\n", c2);
}

void e(unsigned *a)
{
    unsigned sum_fifth = 0;
    for (int i = 0; i < N / 5; ++i)
    {
        sum_fifth += a[5 * i];
    }
    printf("e): %u\n", sum_fifth);
}

// Should be applied unless there is something that increases the performance of a ++ and multiplication operation to work faster than a single +=5
void e_solution(unsigned *a)
{
    unsigned sum_fifth = 0;
    for (int i = 0; i < N; i += 5)
    {
        sum_fifth += a[i];
    }
    printf("e_s): %u\n", sum_fifth);
}

void f(double *a)
{
    for (int i = 0; i < N; ++i)
    {
        a[i] += i / 5.3;
    }
    printf("f):");
    for (int i = 0; i < N; ++i)
    {
        printf("%lf, ", a[i]);
    }
}

// Should be applied if pointer increments are faster than accessing the array with an index and if the division is more expensive than a multiplication
// Unless there is a big overhead for definig a an additional pointer and 2 constants.
void f_solution(double *a)
{
    double *b = a;
    const double c = 1 / 5.3;
    const double limit = N * c;
    for (double i = 0; i < limit; i += c)
    {
        *(b++) += i;
    }
    printf("f_s):");
    for (int i = 0; i < N; ++i)
    {
        printf("%_slf, ", a[i]);
    }
}

void g(float c1)
{
    float c2 = -1 * c1;
    printf("g): %u\n", c2);
}

union float_bits
{
    float f;
    struct float_components
    {
        unsigned mantissa : 23;
        unsigned exponent : 8;
        unsigned sign : 1;
    } components;
};

// Creating a union is a overhead that is far bigger than a multiplication
void strength_reduction_g_union(float c1)
{
    union float_bits c1_bits = {.f = c1};
    union float_bits c2_bits = {.components = {.mantissa = c1_bits.components.mantissa, .exponent = c1_bits.components.exponent, .sign = !c1_bits.components.sign}};
    float c2 = c2_bits.f;
    printf("g_su): %u\n", c2);
}

// Creating a union is a overhead that is far bigger than a multiplication
void strength_reduction_g_union2(float c1)
{
    union float_bits c2 = {.f = c1};
    c2.components.sign = !c2.components.sign;
    float c3 = c2.f;
    printf("g_su2): %u\n", c2);
}

// Due to the additional overhead of creating a uint32_t variable this version is also slower.
void strength_reduction_g_bits(float c1)
{
    uint32_t buffer = (*((uint32_t *)&c1)) ^ (1 << 31);
    float c2 = *((float *)&buffer);
    printf("g_sb): %u\n", c2);
}

int main(void)
{
    return EXIT_SUCCESS;
}
