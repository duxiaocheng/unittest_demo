#include "gtest/gtest.h"
#include "unittest_perf.h"

TEST(testPerf, sys_call_perf)
{
        Perf perf;
        while (perf.state())
        {
                getppid();
        }
}

TEST(testPerf, demo_case1)
{
    Perf perf;
    int i = 0;
    std::string x = "hello";
    perf.begin("A method");
    while (perf.state() && ++i < 1000000)
    {
       std::string copy(x);
    }
    perf.end();
    EXPECT_EQ(1000000, perf.get_benchmark().count);

    i = 0;
    perf.begin("B method");
    while (perf.state() && ++i < 1000000)
    {
       std::string copy(x);
    }
    perf.end();
    EXPECT_EQ(1000000, perf.get_benchmark().count);

    perf.begin("C method");
    while (perf.state())
    {
        std::string copy(x);
    }
    perf.end();
    EXPECT_GE(perf.get_benchmark().tv_used.tv_sec, PERF_TIME_DEFAULT / 1000);
}

TEST(testPerf, demo_case2)
{
        #define BUFFER_SIZE (1024*1024)
        char dst[BUFFER_SIZE];
        Perf perf(false);

        perf.begin("memset");
        while (perf.state())
        {
            memset(dst, 0, sizeof(dst));
        }
        perf.end();

        perf.begin("assignment =");
        while (perf.state())
        {
            for (unsigned int i = 0; i < sizeof(dst); i++)
            {
                dst[i] = '\0';
            }
        }
        perf.end();

        perf.begin("construct initial");
        while (perf.state())
        {
            char temp[BUFFER_SIZE] = {0};
            (void)temp;
        }
        perf.end();
}

