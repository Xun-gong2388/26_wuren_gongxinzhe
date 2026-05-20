cmake_minimum_required(VERSION 3.10)
project(M2)
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src Path_M2)

add_library(M2 STATIC ${Path_M2})

target_include_directories(M2 PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/../../common/kalman 
${PROJECT_SOURCE_DIR}/include)
target_link_libraries(M2 PUBLIC common A1 A2 M1)
