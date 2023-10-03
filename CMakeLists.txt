cmake_minimum_required(VERSION 3.27.6)
project(so VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

set(SO_SOURCES src/error.c src/resource.c)
add_library(so SHARED ${SO_SOURCES})

target_compile_options(so PRIVATE -Wall -Wextra -fPIC)
target_include_directories(so PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)