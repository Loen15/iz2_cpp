cmake_minimum_required(VERSION 3.0.0)
project(iz2 C)

set(CMAKE_C_STANDARD 99)
add_subdirectory(static_lib)
message("STATIC_LIB_INCLUDE_DIRS = ${STATIC_LIB_INCLUDE_DIRS}")
message("STATIC_LIB_LIBRARIES = ${STATIC_LIB_LIBRARIES}")

add_executable(${PROJECT_NAME} main.c)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )

target_include_directories(${PROJECT_NAME} PUBLIC ${STATIC_LIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${STATIC_LIB_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE m)

add_subdirectory(stat_tests)