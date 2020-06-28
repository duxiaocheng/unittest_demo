add_executable(unittest_demo
    testPerf.cpp
    testStub.cpp
    gmock_global_sample.cpp
)

target_link_libraries(unittest_demo
    gtest_main
    gmock_main
    util
)

add_test(
    NAME
        unit
    COMMAND
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unittest_demo
)
