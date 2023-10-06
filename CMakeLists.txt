cmake_minimum_required(VERSION 3.27.6)
project(so VERSION 0.1.1 LANGUAGES C)

include(CTest)
enable_testing()

set(SO_SOURCES src/error.c src/resource.c)
add_library(so STATIC ${SO_SOURCES})

target_compile_options(so PRIVATE -Wall -Wextra -O3)

if (CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(so PRIVATE -flto)
    set_target_properties(so PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
elseif (MSVC)
    target_compile_options(so PRIVATE /GL)
    set_target_properties(so PROPERTIES LINK_FLAGS "/LTCG")
endif()

target_include_directories(so PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
