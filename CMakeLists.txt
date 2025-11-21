cmake_minimum_required(VERSION 3.10)
project(DecimalTests)



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


enable_testing()


find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(test_lab3 tests/test_lab3.cpp src/lab5.cpp)

target_link_libraries(test_lab3 GTest::GTest GTest::Main Threads::Threads)

add_test(NAME FigureTest COMMAND test_lab3)
