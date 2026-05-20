cmake_minimum_required(VERSION 3.10)
project(A1)
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src Path_A1)

add_library(A1 STATIC ${Path_A1})

target_include_directories(A1 PUBLIC ${PROJECT_SOURCE_DIR}/include)
