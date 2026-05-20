cmake_minimum_required(VERSION 3.10)
project(math)
include_directories(${PROJECT_SOURCE_DIR}/include)
#aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)
add_library(math STATIC ${PROJECT_SOURCE_DIR}/src/Math.cpp)
find_package(OpenCV REQUIRED)

target_include_directories(math PUBLIC 
${PROJECT_SOURCE_DIR}/include)
target_link_libraries(math PUBLIC ${OpenCV_LIBS})
