cmake_minimum_required(VERSION 3.15)
project(OOP-LAB-5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем директории с заголовочными файлами
include_directories(include)

# Основная программа
add_executable(main src/main.cpp)

# Тесты
add_executable(tests tests/tester.cpp)

# Ищем Google Test
find_package(GTest REQUIRED)

# Линкуем тесты с Google Test
target_link_libraries(tests GTest::gtest GTest::gtest_main)

# Если не работает, попробуй:
# target_link_libraries(tests gtest gtest_main pthread)