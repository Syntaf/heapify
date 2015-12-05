cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(heap CXX)

if(HPX_ROOT)
    set(HPX_DIR "${HPX_ROOT}/lib/cmake/HPX")
endif()

if(HPX_DIR)
    message(STATUS "searching for HPX in \"${HPX_ROOT}\" ...")
endif()

find_package(HPX REQUIRED)
include_directories(${HPX_INCLUDE_DIR})
link_directories(${HPX_LIBRARY_DIR})

message(STATUS "Found HPX: ${HPX_PREFIX}")

set(source
    heaps
    )

add_hpx_executable(${source} SOURCES ${source}.cpp 
    COMPONENT_DEPENDENCIES iostreams)
