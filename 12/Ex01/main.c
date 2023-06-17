#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

struct time
{
    uint16_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint16_t milliseconds;
    uint16_t microseconds;
};

void standard_a();
void strength_reduction_a();
void standard_b();
void strength_reduction_b();
void standard_c();
void strength_reduction_c();
void standard_d();
void strength_reduction_d();
void standard_e();
void strength_reduction_e();
void standard_f();
void strength_reduction_f();
void standard_g();
void strength_reduction_g_union();
void strength_reduction_g_union2();
void strength_reduction_g_bits();
uint64_t benchmark(void *(*f)(void *), uint64_t N);
struct time get_time(uint64_t time);

#define BASE_N ((uint64_t)1e9)

void main()
{
    uint64_t standard_a_time = benchmark((void *(*)(void *))standard_a, BASE_N);
    uint64_t strength_reduction_a_time = benchmark((void *(*)(void *))strength_reduction_a, BASE_N);
    uint64_t standard_b_time = benchmark((void *(*)(void *))standard_b, BASE_N);
    uint64_t strength_reduction_b_time = benchmark((void *(*)(void *))strength_reduction_b, BASE_N);
    uint64_t standard_c_time = benchmark((void *(*)(void *))standard_c, BASE_N);
    uint64_t strength_reduction_c_time = benchmark((void *(*)(void *))strength_reduction_c, BASE_N);
    uint64_t standard_d_time = benchmark((void *(*)(void *))standard_d, BASE_N);
    uint64_t strength_reduction_d_time = benchmark((void *(*)(void *))strength_reduction_d, BASE_N);
    uint64_t standard_e_time = benchmark((void *(*)(void *))standard_e, BASE_N / 20);
    uint64_t strength_reduction_e_time = benchmark((void *(*)(void *))strength_reduction_e, BASE_N / 20);
    uint64_t standard_f_time = benchmark((void *(*)(void *))standard_f, BASE_N / 100);
    uint64_t strength_reduction_f_time = benchmark((void *(*)(void *))strength_reduction_f, BASE_N / 100);
    uint64_t standard_g_time = benchmark((void *(*)(void *))standard_g, BASE_N);
    uint64_t strength_reduction_g_time_union = benchmark((void *(*)(void *))strength_reduction_g_union, BASE_N);
    uint64_t strength_reduction_g_time_union2 = benchmark((void *(*)(void *))strength_reduction_g_union2, BASE_N);
    uint64_t strength_reduction_g_time_bits = benchmark((void *(*)(void *))strength_reduction_g_bits, BASE_N);

    const uint32_t maxBufferSize = 1024;
    uint32_t bufferSize = 0;
    char buffer[maxBufferSize];
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "standard_a, %lu, %lu\n", BASE_N, standard_a_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "strength_reduction_a, %lu, %lu\n", BASE_N, strength_reduction_a_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "standard_b, %lu, %lu\n", BASE_N, standard_b_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "strength_reduction_b, %lu, %lu\n", BASE_N, strength_reduction_b_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "standard_c, %lu, %lu\n", BASE_N, standard_c_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "strength_reduction_c, %lu, %lu\n", BASE_N, strength_reduction_c_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "standard_d, %lu, %lu\n", BASE_N, standard_d_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "strength_reduction_d, %lu, %lu\n", BASE_N, strength_reduction_d_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "standard_e, %lu, %lu\n", BASE_N, standard_e_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "strength_reduction_e, %lu, %lu\n", BASE_N, strength_reduction_e_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "standard_f, %lu, %lu\n", BASE_N, standard_f_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "strength_reduction_f, %lu, %lu\n", BASE_N, strength_reduction_f_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "standard_g, %lu, %lu\n", BASE_N, standard_g_time);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "strength_reduction_g_union, %lu, %lu\n", BASE_N, strength_reduction_g_time_union);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "strength_reduction_g_union2, %lu, %lu\n", BASE_N, strength_reduction_g_time_union2);
    bufferSize += snprintf(buffer + bufferSize, maxBufferSize - bufferSize, "strength_reduction_g_bits, %lu, %lu\n", BASE_N, strength_reduction_g_time_bits);
    int32_t remainingBufferSize = maxBufferSize - bufferSize;

    FILE *file = fopen("Ex01.csv", "a+");
    if (remainingBufferSize < 0)
    {
        fprintf(file, "buffer overflow by %d bytes\n", -remainingBufferSize);
    }
    else
    {
        fwrite(buffer, sizeof(char), bufferSize, file);
    }
    fclose(file);
    return;
}

struct time get_time(uint64_t time)
{
    struct time t;
    t.hours = time / 3600000000;
    t.minutes = (time % 3600000000) / 60000000;
    t.seconds = (time % 60000000) / 1000000;
    t.milliseconds = (time % 1000000) / 1000;
    t.microseconds = time % 1000;
    return t;
}

/**
 * @return time in microseconds
 */
uint64_t benchmark(void *(*f)(void *), uint64_t N)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (uint64_t i = 0; i < N; ++i)
    {
        f(NULL);
    }
    gettimeofday(&end, NULL);
    return (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
}

void standard_a()
{
    unsigned c1 = 7;
    unsigned c2 = 32 * c1;
}

void strength_reduction_a()
{
    unsigned c1 = 7;
    unsigned c2 = c1 << 5;
}

void standard_b()
{
    unsigned c1 = 7;
    unsigned c2 = 15 * c1;
}

void strength_reduction_b()
{
    unsigned c1 = 7;
    unsigned c2 = (c1 << 4) - c1;
}

void standard_c()
{
    unsigned c1 = 7;
    unsigned c2 = 96 * c1;
}

void strength_reduction_c()
{
    unsigned c1 = 7;
    unsigned c2 = (c1 << 5) + (c1 << 6);
}

void standard_d()
{
    unsigned c1 = 7;
    unsigned c2 = 0.125 * c1;
}

void strength_reduction_d()
{
    unsigned c1 = 7;
    unsigned c2 = c1 >> 3;
}

void standard_e()
{
    unsigned N = 100;
    unsigned a[N];
    unsigned sum_fifth = 0;
    for (int i = 0; i < N / 5; ++i)
    {
        sum_fifth += a[5 * i];
    }
}

void strength_reduction_e()
{
    unsigned N = 100;
    unsigned a[N];
    unsigned sum_fifth = 0;
    for (int i = 0; i < N; i += 5)
    {
        sum_fifth += a[i];
    }
}

void standard_f()
{
    unsigned N = 100;
    double a[N];
    for (int i = 0; i < N; ++i)
    {
        a[i] += i / 5.3;
    }
}

void strength_reduction_f()
{
    unsigned N = 100;
    double a[N];
    double *b = a;
    const double c = 1 / 5.3;
    const double limit = N * c;
    for (double i = 0; i < limit; i += c)
    {
        *(b++) += i;
    }
}

void standard_g()
{
    float c1 = 7;
    float c2 = -1 * c1;
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

void strength_reduction_g_union()
{
    float c1 = 7;
    union float_bits c1_bits = {.f = c1};
    union float_bits c2_bits = {.components = {.mantissa = c1_bits.components.mantissa, .exponent = c1_bits.components.exponent, .sign = !c1_bits.components.sign}};
    float c2 = c2_bits.f;
}

void strength_reduction_g_union2()
{

    union float_bits c1 = {.f = 7};
    c1.components.sign = !c1.components.sign;
    float c2 = c1.f;
}

void strength_reduction_g_bits()
{
    float c1 = 7;
    uint32_t buffer = (*((uint32_t *)&c1)) ^ (1 << 31);
    float c2 = *((float *)&buffer);
}