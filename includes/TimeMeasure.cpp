#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

class OMPTimer
{
private:
    double start;

public:
    OMPTimer()
    {
        setTimer();
    }

    void setTimer()
    {
        start = omp_get_wtime();
    }

    /**
     * @return The time in in seconds as a double since last setTimer().
     */
    double getTime_fs()
    {
        return (omp_get_wtime() - start);
    }

    /**
     * Executes getTime_us() and resets the timer.
     * @return The time in in seconds as a double since last setTimer().
     */
    double getTime_fs_AndSet()
    {
        double time = getTime_fs();
        setTimer();
        return time;
    }
} ompTimeMeasure;


class UserTimeMeasure
{
private:
    struct timeval start;

public:
    UserTimeMeasure()
    {
        gettimeofday(&start, NULL);
    }

    /**
     * Set the timer to the current time to calculate the difference with getTime_..()
     */
    void setTimer()
    {
        gettimeofday(&start, NULL);
    }

    /**
     * Get time in microsecends since las setTimer().
     */
    u_int64_t getTime_us()
    {
        struct timeval end;
        gettimeofday(&end, NULL);
        return ((u_int64_t)(end.tv_sec - start.tv_sec)) * 1000 * 1000 + end.tv_usec - start.tv_usec;
    }

    /**
     * Executes getTime_us() and resets the timer.
     * @return The time in microsecends since last setTimer().
     */
    u_int64_t getTime_us_AndSet()
    {
        u_int64_t time = getTime_us();
        setTimer();
        return time;
    }

    /**
     * @return The time in in seconds as a double since last setTimer().
     */
    double getTime_fs()
    {
        struct timeval end;
        gettimeofday(&end, NULL);
        return (end.tv_sec - start.tv_sec) + ((double)end.tv_usec - start.tv_usec) / (1000 * 1000);
    }

    /**
     * Executes getTime_us() and resets the timer.
     * @return The time in in seconds as a double since last setTimer().
     */
    double getTime_fs_AndSet()
    {
        double time = getTime_fs();
        setTimer();
        return time;
    }

} userTimeMeasure;

class CPUTimeMeasure
{
private:
    clock_t start;

public:
    CPUTimeMeasure()
    {
        start = clock();
    }

    /**
     * Set the timer to the current time to calculate the difference with getTime_fs()
     */
    void setTimer()
    {
        start = clock();
    }

    /**
     * @return The time in in seconds as a double since last setTimer().
     */
    double getTime_fs()
    {
        return ((double)(clock() - start)) / CLOCKS_PER_SEC;
    }

    /**
     * Executes getTime_us() and resets the timer.
     * @return The time in in seconds as a double since last setTimer().
     */
    double getTime_fs_AndSet()
    {
        double time = getTime_fs();
        setTimer();
        return time;
    }

} cpuTimeMeasure;