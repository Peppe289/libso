# Libso.so

Library taken from the operating systems course. Updated to fix, compiled in 64 bit and optimized. With the new cmake implementation it is easy to compile.


# Install

```sh
sudo curl https://raw.githubusercontent.com/Peppe289/libso/master/src/ourhdr.h -o /usr/local/include/ourhdr.h
sudo wget https://github.com/Peppe289/libso/releases/download/libso-0.1.1/libso.a -o /usr/local/lib/libso.a
```

Using with CMAKE
---

```cmake
cmake_minimum_required(VERSION 3.6.0)
project(es VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(es main.c)
target_compile_options(es PUBLIC -Wall -pg -Wextra)

# link libso library
link_directories(/usr/local/lib/)
target_link_libraries(es so m)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
```

# Developer

How to compile
---
```sh
mkdir build && cd build && cmake .. && make
```

Change compiler
---
For change compiler `export CC=<compiler>`