cmake_minimum_required (VERSION 2.6)
project (Arya)

set(Arya_VERSION_MAJOR 0)
set(Arya_VERSION_MINOR 0)
configure_file (
  "${PROJECT_SOURCE_DIR}/include/AryaConfig.h.in"
  "${PROJECT_SOURCE_DIR}/include/AryaConfig.h"
)


include_directories("include")

add_executable(main src/main.cpp)


