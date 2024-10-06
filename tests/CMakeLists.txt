cmake_minimum_required(VERSION 3.24...3.30)
project(GVS-CPU-test VERSION 1.0 LANGUAGES CXX CUDA)

include(GoogleTest)
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)

FetchContent_MakeAvailable(googletest)

add_executable(GVS-CPU
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests.cu
)

target_include_directories(GVS-CPU PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../GVS-CPU/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(CUDAToolkit)

target_link_libraries(GVS-CPU PRIVATE
    GVS-CPU
    GTest::gtest_main
    CUDA::curand
)

enable_testing()
gtest_discover_tests(GVS-CPU-test)

