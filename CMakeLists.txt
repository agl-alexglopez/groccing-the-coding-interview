# This version gives us file sets which are great for library install.
cmake_minimum_required(VERSION 3.23)
project("groccc" 
  VERSION 0.40.0
  LANGUAGES C
  DESCRIPTION "Ensure your time spent preparing for a coding interview is wasted. Do it in C."
)
# Specify that C23 should be used
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

######################### Fetch C Container Collection ########################

include(FetchContent)
FetchContent_Declare(
  ccc
  URL https://github.com/skeletoss/ccc/releases/download/v0.62.1/ccc-v0.62.1.zip
  DOWNLOAD_EXTRACT_TIMESTAMP FALSE
)
FetchContent_MakeAvailable(ccc)
target_compile_options(ccc PRIVATE "-w")

#########################   Fetch Str_view Library     ########################

include(FetchContent)
FetchContent_Declare(
  str_view
  URL https://github.com/skeletoss/str_view/releases/download/v0.7.0/str_view-v0.7.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP FALSE
)
FetchContent_MakeAvailable(str_view)
target_compile_options(str_view PRIVATE "-w")

# For releases we only produce this and the source. No test or sample bloat.
add_subdirectory("${PROJECT_SOURCE_DIR}/utility")
add_subdirectory("${PROJECT_SOURCE_DIR}/${PROJECT_NAME}")
include(tools/scanners.cmake)

if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top-level one")
endif()


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/debug/bin)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

