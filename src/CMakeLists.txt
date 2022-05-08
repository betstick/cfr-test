message(STATUS "Given CMEM_LOCATION: " ${CMEM_DIR})
include_directories(${CMEM_DIR})

project(cfr-test)

#file(GLOB_RECURSE SOURCES "*.cpp")
set(SOURCES
	"main.cpp"
)

add_executable(cfr-test main.cpp)

set_target_properties ( cfr-test PROPERTIES
	OUTPUT_NAME "cfr-test"
	RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}" )


include_directories( "../lib/cfromreader" )

target_link_libraries(cfr-test cfr ${CMEM_DIR}/cmem.c)