cmake_minimum_required(VERSION 3.22)
project(FodoDBG)

set(Executable fdbg)

include_directories(
  includes
  external/libelfin
  external/linenoise
)

add_executable(${Executable} app/main.cpp external/linenoise/linenoise.c)

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