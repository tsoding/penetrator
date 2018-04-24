cmake_minimum_required(VERSION 3.2)
project(penetrator)

set(SOURCE_FILES
  src/main.cpp
  src/keyboard.cpp
  src/uinput.cpp
  src/uud.cpp
)

set(HEADER_FILES
  src/keyboard.hpp
  src/uinput.hpp
  src/uud.hpp
)

add_executable(penetrator ${SOURCE_FILES} ${HEADER_FILES})

# TODO: using target_compile_features instead because it's more cross-platform
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
