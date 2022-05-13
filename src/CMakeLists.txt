message(STATUS "Given CMEM_LOCATION: " ${CMEM_DIR})

project(cfr-test)

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIR})
include_directories(${ZLIB_INCLUDE_DIRS})

include_directories(cmem)
include_directories(Umem)
include_directories(frld)

add_executable(cfr-test main.cpp)

set_target_properties ( cfr-test PROPERTIES
	OUTPUT_NAME "cfr-test"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}" )

target_link_libraries(cfr-test frld umem cmem ZLIB::ZLIB)