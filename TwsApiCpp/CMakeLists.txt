cmake_minimum_required(VERSION 3.10)
project(TwsApiCpp)

set(CMAKE_CXX_STANDARD 17)

# Collect all source files
file(GLOB_RECURSE TWS_API_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Shared/*.cpp"
)

# Create the TWS API library
add_library(TwsApiCpp STATIC ${TWS_API_SOURCES})

# Include directories
target_include_directories(TwsApiCpp PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/Shared
)