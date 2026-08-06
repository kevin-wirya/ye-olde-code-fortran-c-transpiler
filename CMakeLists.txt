cmake_minimum_required(VERSION 3.10)
project(F77Transpiler VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
include_directories(
    include
    include/lexer
    include/parser
    include/parser/ast
    include/semantic
)
if(NOT FLEX_EXECUTABLE AND EXISTS "C:/msys64/usr/bin/flex.exe")
    set(FLEX_EXECUTABLE "C:/msys64/usr/bin/flex.exe")
endif()
find_package(FLEX REQUIRED)
FLEX_TARGET(Scanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(f77_transpiler ${SOURCES} ${FLEX_Scanner_OUTPUTS})
