cmake_minimum_required(VERSION 3.0.0)
project(iz2 C)

set(CMAKE_C_STANDARD 99)

option(STREAMS "Do u wanna use lib with strems" OFF)
option(TESTS "Do u wanna use lib with strems" oFF)

if (STREAMS)
    add_subdirectory(dinamic_lib)
    message("DINAMIC_LIB_INCLUDE_DIRS = ${DINAMIC_LIB_INCLUDE_DIRS}")
    message("DINAMIC_LIB_LIBRARIES = ${DINAMIC_LIB_LIBRARIES}")
else()
    add_subdirectory(static_lib)
    message("STATIC_LIB_INCLUDE_DIRS = ${STATIC_LIB_INCLUDE_DIRS}")
    message("STATIC_LIB_LIBRARIES = ${STATIC_LIB_LIBRARIES}")
endif()

add_executable(${PROJECT_NAME} main.c)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )

if (STREAMS)
    find_package(Threads REQUIRED)

    target_include_directories(${PROJECT_NAME} PUBLIC ${DINAMIC_LIB_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${DINAMIC_LIB_LIBRARIES} Threads::Threads)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
else()
    target_include_directories(${PROJECT_NAME} PUBLIC ${STATIC_LIB_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${STATIC_LIB_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

if (TESTS)
    add_subdirectory(tests)
endif()