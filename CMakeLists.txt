cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(fun)

option(FUN_BUILD_WITH_COVERAGE "Build with coverage" OFF)
if(FUN_BUILD_WITH_COVERAGE AND (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
    add_definitions(-DFUN_BUILD_WITH_COVERAGE)
    set(FUN_COVERAGE_FLAGS "--coverage")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${FUN_COVERAGE_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${FUN_COVERAGE_FLAGS}")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} ${FUN_COVERAGE_FLAGS}")
endif()

file(GLOB_RECURSE untest_sources
    "3rdparty/*.*"
    "include/*.*"
    "untests/*.*")
add_executable(${PROJECT_NAME} ${untest_sources})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)
target_include_directories(${PROJECT_NAME} PRIVATE include)

enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME})
