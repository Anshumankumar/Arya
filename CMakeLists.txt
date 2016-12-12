cmake_minimum_required (VERSION 2.6)
project (Arya)

find_package(BISON)
find_package(FLEX)
find_package(OpenMP)
find_package(GSL)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


set(Arya_VERSION_MAJOR 0)
set(Arya_VERSION_MINOR 0)
configure_file (
  "${PROJECT_SOURCE_DIR}/include/AryaConfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/AryaConfig.h"
)
add_compile_options(-O3)


BISON_TARGET(MyParser include/params/param.ypp ${CMAKE_CURRENT_BINARY_DIR}/param.tab.cpp)
FLEX_TARGET(MyScanner include/params/param.l  ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)


include_directories("include")
include_directories(${CMAKE_CURRENT_BINARY_DIR})


add_library(params SHARED  
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
    src/params/params.cpp src/params/paramConstructor.cpp )

add_executable(main src/main.cpp)
target_link_libraries(main params)
add_executable(mlMain src/mlMain.cpp)
target_link_libraries(mlMain params)
add_executable(parsertest src/params/main.cpp)
target_link_libraries(parsertest params)

#add_executable(lr src/ml/csvParser.cpp  src/ml/indexer.cpp src/ml/logisticRegression.cpp)
#target_link_libraries(lr params)

add_executable(neuralNetwork src/ml/neuralNetwork.cpp src/ml/indexer.cpp src/ml/csvParser.cpp)
target_link_libraries(neuralNetwork params)

add_executable(modelTest src/ml/main.cpp)
target_link_libraries(modelTest params ${GSL_LIBRARIES})
