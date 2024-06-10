cmake_minimum_required(VERSION 3.22)
project(FodoDBG)

set(Executable fdbg)

include_directories(
  includes
  external/libelfin
  external/linenoise
)

file(GLOB FDBG_CPP_FILES "src/*.cpp")
file(GLOB FDBG_HPP_FILES "includes/*.hpp")

set(FDBG_SOURCES
  ${FDBG_CPP_FILES}
  ${FDBG_HPP_FILES}
)
add_library(fdbgl STATIC ${FDBG_SOURCES})

add_executable(${Executable} app/main.cpp external/linenoise/linenoise.c)
target_include_directories(${Executable} PUBLIC includes)
target_link_libraries(${Executable} fdbgl)

add_custom_target(
  libelfin
  COMMAND make
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/libelfin
)
target_link_libraries(${Executable}
  ${PROJECT_SOURCE_DIR}/external/libelfin/dwarf/libdwarf++.so
  ${PROJECT_SOURCE_DIR}/external/libelfin/elf/libelf++.so
)
add_dependencies(${Executable} libelfin)

add_executable(hello tests/hello.cc)
set_target_properties(hello PROPERTIES COMPILE_FLAGS "-g -O0")