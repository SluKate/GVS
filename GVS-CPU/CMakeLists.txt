cmake_minimum_required(VERSION 3.24...3.30)
project(GVS-CPU VERSION 1.0 LANGUAGES CUDA)

add_library(GVS-CPU STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/GVS-CPU/include/dotprod.cu
    ${CMAKE_CURRENT_SOURCE_DIR}/GVS-CPU/include/dynblock.cu
)

target_include_directories(GVS-CPU PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(GVS-CPU PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(GVS-CPU PROPERTIES CUDA_ARCHITECTURES "native")

if(CMAKE_BUILD_TYPE MATCHES "DebugDevice")
    message(STATUS "Device debugging mode is enabled")
    target_compile_definitions(GVS-CPU PRIVATE DEBUG=1)
    target_compile_options(GVS-CPU
        PUBLIC $<$<COMPILE_LANGUAGE:CUDA>:-g -G>
    )
endif()

if(CMAKE_BUILD_TYPE MATCHES "DebugHost")
    message(STATUS "Host debugging mode is enabled")
    target_compile_definitions(GVS-CPU PRIVATE DEBUG=1)
    target_compile_options(GVS-CPU/GVS-CPU PUBLIC
        $<$<COMPILE_LANGUAGE:CUDA>:-g -Xcompiler -fsanitize=address -fsanitize=undefined>
    )
    target_link_libraries(GVS-CPU/GVS-CPU PUBLIC
        asan
    )
    target_link_options(GVS-CPU/GVS-CPU
        BEFORE PUBLIC -fsanitize=undefined
        PUBLIC -fsanitize=address
    )
endif()

if(CMAKE_BUILD_TYPE MATCHES "Release")
    message(STATUS "Release mode is enabled")
endif()

