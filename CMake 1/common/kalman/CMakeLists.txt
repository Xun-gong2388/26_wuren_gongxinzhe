cmake_minimum_required(VERSION 3.10)
project(kalman)
#aux_source_directory(${PROJECT_SOURCE_DIR}/. SRC)
add_library(kalman INTERFACE)
include_directories(${PROJECT_SOURCE_DIR}/include)
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries(kalman 
    INTERFACE 
    ${PROJECT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)
target_include_directories(kalman 
    INTERFACE
    ${PROJECT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)   
set(kalman_INCLUDE_DIR)
list(APPEND kalman_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
list(APPEND kalman_INCLUDE_DIR "${OpenCV_INCLUDE_DIRS}")

set(kalman_INCLUDE_DIRS ${kalman_INCLUDE_DIR}
                        CACHE PATH "kalman filter include directories")