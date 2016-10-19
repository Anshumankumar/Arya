cmake_minimum_required (VERSION 2.6)
project (Arya)

find_package(BISON)
find_package(FLEX)

set(Arya_VERSION_MAJOR 0)
set(Arya_VERSION_MINOR 0)
configure_file (
  "${PROJECT_SOURCE_DIR}/include/AryaConfig.h.in"
  "${PROJECT_SOURCE_DIR}/include/AryaConfig.h"
)
add_compile_options(-O3)


BISON_TARGET(MyParser include/params/param.ypp ${CMAKE_CURRENT_BINARY_DIR}/param.tab.cpp)
FLEX_TARGET(MyScanner include/params/param.l  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)


include_directories("include")
include_directories(${CMAKE_CURRENT_BINARY_DIR})


add_executable(main src/main.cpp src/params/params.cpp src/params/paramConstructor.cpp)
add_executable(mlMain src/mlMain.cpp)
add_executable(parsertest
  ${BISON_MyParser_OUTPUTS}
  ${FLEX_MyScanner_OUTPUTS}  src/params/params.cpp src/params/main.cpp src/params/paramConstructor.cpp)
