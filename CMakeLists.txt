cmake_minimum_required(VERSION 3.22)
project(FodoDBG)

set(Executable fdbg)

add_executable(${Executable} app/main.cpp)