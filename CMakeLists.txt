cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(fun)

file(GLOB_RECURSE untest_sources "*.cpp" "*.hpp")
add_executable(${PROJECT_NAME} ${untest_sources})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
target_include_directories(${PROJECT_NAME} PRIVATE include)

enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME})
