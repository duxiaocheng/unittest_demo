#ifndef __UNITTEST_PERF_H__
#define __UNITTEST_PERF_H__

#include <stddef.h>
#include <time.h>

/* default testing time long (ms) */
#define PERF_TIME_DEFAULT     1500

class PerfInternal;
class Perf {
public:
typedef struct benchmark {
    const char*               name;
    struct timeval            tv_begin;
    struct timeval            tv_end;
    struct timeval            tv_used;
    unsigned long long        count;
    time_t                    time;
} Benchmark;

public:
    Perf(bool begin_perf = true);
    ~Perf();

    bool state();
    void begin(const char *name = NULL, int time = PERF_TIME_DEFAULT);
    void end();
    const Benchmark& get_benchmark() const;
    void report();

private:
    PerfInternal* internal;
};

#endif /* __UNITTEST_PERF_H__ */

