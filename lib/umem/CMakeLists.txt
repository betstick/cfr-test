cmake_minimum_required(VERSION 3.4)
project(cmem VERSION 0.0.1 DESCRIPTION "umem library v0.0.1")

include(GNUInstallDirs)

if(WIN32)
	add_compile_definitions(_WIN32)
endif()

include_directories(cmem)

add_library(umem STATIC umem.h umem.c)

target_link_libraries(umem cmem)

target_include_directories(umem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})