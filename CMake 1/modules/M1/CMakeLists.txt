cmake_minimum_required(VERSION 3.10)
project(M1)
#aux_source_directory(./include SRC)
#add_library(M1 ${SRC})
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src Path_M1)

add_library(M1 STATIC ${Path_M1})

target_include_directories(M1 PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(M1 PUBLIC A1)
#target_link_libraries(M1 PUBLIC ${PROJECT_SOURCE_DIR}/include)
