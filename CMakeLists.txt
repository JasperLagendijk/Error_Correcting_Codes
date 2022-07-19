cmake_minimum_required(VERSION 3.10)

SET(GCC_COVERAGE_COMPILE_FLAGS "-O3 -march=native -mtune=native")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

project(Error_Correcting_Codes VERSION 1.0)

option(USE_LDPC "Use LDPC library" ON)

configure_file(ECC_Config.h.in ECC_Config.h)
if(USE_LDPC)
	add_subdirectory(LDPC)
	list(APPEND EXTRA_LIBS LDPC)
	list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/LDPC")
endif()	

list (APPEND EXTRA_LIBS m)	
add_executable(Simulation sim.c)

target_link_libraries(Simulation PUBLIC ${EXTRA_LIBS})


target_include_directories(Simulation PUBLIC
							"${PROJECT_BINARY_DIR}"
							${EXTRA_INCLUDES}
							)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )


