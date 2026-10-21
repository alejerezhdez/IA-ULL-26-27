cmake_minimum_required(VERSION 3.10)
project(pfsp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Si no se indica el tipo de compilación, usar Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Warnings útiles; en Debug, con símbolos para depurar y los assert activos
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
# Release: optimizado y con -DNDEBUG (desactiva los assert)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

add_executable(pfsp
  main.cpp
  instance.cpp
  genetic_algorithm.cpp
)