#include "unittest_perf.h"

#include <unistd.h>
#include <sys/time.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <vector>
#include <string>

class PerfInternal {
public:
    PerfInternal(bool begin_perf = true);
    ~PerfInternal();

    bool state();
    void begin(const char *name, time_t time);
    void end();
    const Perf::Benchmark& get_benchmark() const;
    void report();

private:
    PerfInternal(PerfInternal& other);
    PerfInternal& operator=(const PerfInternal& other);
#if __cplusplus >= 201103L
    PerfInternal(PerfInternal&& other);
    PerfInternal& operator=(const PerfInternal&& other);
#endif

    static void* thread_body(void * arg);
    void create_timer(time_t time);
    void destroy_timer();

private:
    volatile bool                    _running;
    bool                             _reported;
    Perf::Benchmark                  _b;
    std::vector<Perf::Benchmark>     _benchmarks;
    pthread_t                        _th;
    pthread_mutex_t                  _mtx;
};

PerfInternal::PerfInternal(bool begin_perf/* = true*/)
{
    memset(&_b, 0, sizeof(_b));
    pthread_mutex_init(&_mtx, NULL);
    _running = false;
    _reported = false;
    _th = 0;
    if (begin_perf)
    {
        begin(NULL, PERF_TIME_DEFAULT);
    }
}

PerfInternal::~PerfInternal()
{
    if (_b.name != NULL)
    {
        end();
    }
    report();
    pthread_mutex_destroy(&_mtx);
}

bool PerfInternal::state()
{
    _b.count++;
    return _running;
}

void PerfInternal::begin(const char *name, time_t time)
{
    destroy_timer();
    memset(&_b, 0, sizeof(_b));

    if (NULL == name)
    {
        name = "<anonymity>";
    }
    _b.name = name;
    create_timer(time);
    gettimeofday(&_b.tv_begin, NULL);
}

void PerfInternal::end()
{
    gettimeofday(&_b.tv_end, NULL);
    destroy_timer();
    timersub(&_b.tv_end, &_b.tv_begin, &_b.tv_used);
    _benchmarks.push_back(_b);
    memset(&_b, 0, sizeof(_b)); // mark for stoped
}

const Perf::Benchmark& PerfInternal::get_benchmark() const
{
    return _benchmarks.back();
}

void PerfInternal::report()
{
    _reported = true;
    char title[256] = {0};
    snprintf(title, sizeof(title) - 1, "%-20s %12s %12s %10s %10s",
        "Benchmark", "HZ", "1/HZ", "Time(us)", "Count");
    fprintf(stdout, "%s\n", title);
    fprintf(stdout, "%s\n", std::string(strlen(title), '-').c_str());
    for (unsigned i = 0; i < _benchmarks.size(); i++)
    {
        const Perf::Benchmark &b = _benchmarks[i];
        fprintf(stdout, "%-20s %12.0f %12.9f %10llu %10llu\n", b.name,
            b.count / (b.tv_used.tv_sec + 0.000001 * b.tv_used.tv_usec),
            (b.tv_used.tv_sec + 0.000001 * b.tv_used.tv_usec) / b.count,
            b.tv_used.tv_sec * 1000000ULL + b.tv_used.tv_usec,
            b.count);
    }
}

void* PerfInternal::thread_body(void *arg)
{
    PerfInternal *that = static_cast<PerfInternal*>(arg);
    int unit = 10;
    //fprintf(stdout, "run thread(%x):%d start....\n", pthread_self(), that->b_.time);
    for (int ms = that->_b.time; that->_running && ms > 0; ms -= unit)
    {
        usleep((ms > unit ? unit : ms) * 1000);
    }
    pthread_mutex_lock(&that->_mtx);
    that->_running = false;
    pthread_mutex_unlock(&that->_mtx);
    return NULL;
}

void PerfInternal::create_timer(time_t time)
{
    _b.time = time; // set value before starting thread
    pthread_mutex_lock(&_mtx);
    _running = true;
    pthread_mutex_unlock(&_mtx);
    int ret = pthread_create(&_th, NULL, PerfInternal::thread_body, this);
    if (ret != 0)
    {
        _th = 0;
        fprintf(stderr, "Create thread error: %s\n", strerror(errno));
        return;
    }
}

void PerfInternal::destroy_timer()
{
    pthread_mutex_lock(&_mtx);
    _running = false;
    pthread_mutex_unlock(&_mtx);
    if (_th != 0)
    {
        pthread_cancel(_th);
        pthread_join(_th, NULL); // wait thread exit!
        //fprintf(stdout, "exit thread(%x) end...\n", _th);
        _th = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////

Perf::Perf(bool begin_perf/* = true*/)
{
    internal = new PerfInternal(begin_perf);
}

Perf::~Perf()
{
    delete internal;
    internal = NULL;
}

bool Perf::state()
{
    return internal->state();
}

void Perf::begin(const char *name, int time)
{
    internal->begin(name, time);
}

void Perf::end()
{
    internal->end();
}

const Perf::Benchmark& Perf::get_benchmark() const
{
    return internal->get_benchmark();
}

void Perf::report()
{
    internal->report();
}

