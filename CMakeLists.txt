cmake_minimum_required(VERSION 3.12)
project(Task3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_executable(Task3 
    src/main.cpp
    src/point.cpp
    src/figure.cpp
    src/square.cpp
    src/rectangle.cpp
    src/trapezoid.cpp
    src/spisok.cpp
)

add_executable(tests
    tests/tester.cpp
    src/point.cpp
    src/figure.cpp
    src/square.cpp
    src/rectangle.cpp
    src/trapezoid.cpp
    src/spisok.cpp
)

target_link_libraries(tests GTest::GTest GTest::Main)

target_compile_options(Task3 PRIVATE -Wall -Wextra)
target_compile_options(tests PRIVATE -Wall -Wextra)

target_include_directories(Task3 PRIVATE include)
target_include_directories(tests PRIVATE include)