cmake_minimum_required(VERSION 3.10)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(test_lab3 test_lab3.cpp ../src/lab5.cpp)

target_include_directories(test_lab3 PRIVATE ../src)

target_link_libraries(test_lab3
    GTest::GTest
    GTest::Main
    Threads::Threads
)

add_test(NAME Figure COMMAND test_lab3)
