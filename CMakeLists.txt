cmake_minimum_required(VERSION 3.2)
project(penetrator)

set(SOURCE_FILES
  src/main.cpp
  src/keyboard.cpp
)

set(HEADER_FILES
  src/keyboard.hpp
)

add_executable(penetrator ${SOURCE_FILES} ${HEADER_FILES})
