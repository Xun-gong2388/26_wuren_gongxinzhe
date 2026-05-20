cmake_minimum_required(VERSION 3.10)
project("CMake 1" LANGUAGES CXX)
add_executable(test main.cpp)
target_link_directories(test
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/modules/M1/include 
   ${CMAKE_CURRENT_SOURCE_DIR}/common
   ${CMAKE_CURRENT_SOURCE_DIR}/modules
)
find_package(OpenCV REQUIRED)
include_directories(
    #${CMAKE_CURRENT_SOURCE_DIR}/common/kalman
    #${CMAKE_CURRENT_SOURCE_DIR}/common/math
    #${CMAKE_CURRENT_SOURCE_DIR}/modules/A1
    #${CMAKE_CURRENT_SOURCE_DIR}/modules/A2
    #${CMAKE_CURRENT_SOURCE_DIR}/modules/M1
    #${CMAKE_CURRENT_SOURCE_DIR}/modules/M2
    #${CMAKE_CURRENT_SOURCE_DIR}/modules
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/modules
    )
    add_subdirectory(common)
    add_subdirectory(modules)
    #add_subdirectory(kalman)
    #add_subdirectory(math)
    #add_subdirectory(A1)
    #add_subdirectory(A2)
    #add_subdirectory(M1)
    #add_subdirectory(M2)

    link_libraries(common modules)
  
    target_link_libraries(test
       PUBLIC  
          #common 
          #modules
          M1
          M2
          A1
          A2
          math
          kalman
          ${OpenCV_LIBS}
)
