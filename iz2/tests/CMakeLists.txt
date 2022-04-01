cmake_minimum_required(VERSION 3.0.0)
project(tests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fprofile-arcs -ftest-coverage -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )

if(STREAMS)
    file(GLOB SOURCES din_tests.cpp)
else()
    file(GLOB SOURCES stat_tests.cpp)
endif()

find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
if(STREAMS)
    target_include_directories(${PROJECT_NAME} PUBLIC ${DINAMIC_LIB_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${DINAMIC_LIB_LIBRARIES} GTest::gtest_main)
else()
    target_include_directories(${PROJECT_NAME} PUBLIC ${STATIC_LIB_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${STATIC_LIB_LIBRARIES} GTest::gtest_main)
endif()