cmake_minimum_required(VERSION 3.10)
project(A2)
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src Path_A2)

add_library(A2 STATIC ${Path_A2})

target_include_directories(A2 PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(A2 PUBLIC common A1)
#[[if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(A2 PRIVATE -include cstddef)
elseif(MSVC)
    target_compile_options(A2 PRIVATE /FI"cstddef")
endif()
]]#
target_compile_options(A2 PRIVATE -include cstddef)