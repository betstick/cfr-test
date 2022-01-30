#project(cfr-test)
#add_executable(cfr-test main.cpp)

#set(LIB_SRC lib/cfromreader/src/cfromreader.cpp)
#add_library(cfr SHARED ${LIB_SRC})

#set(COMPILE_OUTPUT_FOLDER ${CMAKE_SOURCE_DIR}/bin/)
#target_link_libraries(cfr-test PRIVATE cfr)
#target_include_directories(cfr-test PUBLIC "${PROJECT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/lib/cfromreader")

project(cfr-test)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(cfr-test main.h main.cpp)

set_target_properties ( cfr-test PROPERTIES
	OUTPUT_NAME "cfr-test"
	RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}" )

include_directories( "../lib/cfromreader" )

target_link_libraries(cfr-test cfr)