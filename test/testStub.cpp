#include <stdio.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "unittest_stub.h"
#include "unittest_gmock_global.h"

namespace {
// Stub example: global function(no static and no inline)
// __attribute__ ((noinline)) is used to force compiler not inline that function
// also can use compile option -fno-inline
int __attribute__ ((noinline)) foo(int a)
{
    static int var_for_no_inline = 0;
    var_for_no_inline++;
    printf("I am in %s!\n", __func__);
    return a;
}
int foo_stub(int a)
{
    static int var_for_no_inline = 0;
    var_for_no_inline++;
    printf("I am in %s!\n", __func__);
    return a+1;
}
TEST(StubTestSuite, globalFunction)
{
    int ret = foo(1);
    EXPECT_EQ(1, ret);

    Stub stub;
    stub.set(foo, foo_stub);
    ret = foo(1);
    EXPECT_EQ(2, ret);
}

MOCK_GLOBAL_FUNC1(foo_mock, int(int));
TEST(StubTestSuite, workWithGlobalGmock)
{
    // work with global gmock
    using ::testing::Return;
    Stub stub;
    stub.set(foo, foo_mock);
    EXPECT_GLOBAL_CALL(foo_mock, foo_mock(1))
        .Times(1)
        .WillOnce(Return(3));

    int ret = foo(1);
    EXPECT_EQ(3, ret);
}

class A {
public:
    int foo(int a) __attribute__ ((noinline)) {
        static int var_for_no_inline = 0;
        var_for_no_inline++;
        return a;
    }
};

int foo_instance_stub(void* obj, int a)
{
    static int var_for_no_inline = 0;
    var_for_no_inline++;
    A* o = (A*)obj;
    printf("o address:%p\n", o);
    printf("I am in %s\n", __func__);
    return a+1;
}

TEST(StubTestSuite, instanceFunction)
{
    Stub stub;
    stub.set(ADDR(A, foo), foo_instance_stub);
    A a;
    printf("a address:%p\n", &a);
    int ret = a.foo(1);
    EXPECT_EQ(2, ret);
}

} // end namespace
