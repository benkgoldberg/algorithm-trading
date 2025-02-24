add_subdirectory(${CMAKE_SOURCE_DIR}/googletest googletest_build)

add_executable(SimulatedExecutionTest
    Trades/SimulatedExecutionTest.cpp
)

target_link_libraries(SimulatedExecutionTest
    PRIVATE
    gtest
    gtest_main
    AlgorithmTradingLib
)

add_test(NAME SimulatedExecutionTest COMMAND SimulatedExecutionTest)
