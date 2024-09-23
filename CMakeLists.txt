cmake_minimum_required(VERSION 3.10)

project(AlgorithmTrading)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Add the TWS API source directory
add_subdirectory(TwsApiCpp)

# Include subdirectories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/Helper)
include_directories(${CMAKE_SOURCE_DIR}/src/Data)
include_directories(${CMAKE_SOURCE_DIR}/src/Trades)
include_directories(${CMAKE_SOURCE_DIR}/src/Algorithms)

# Include TWS API headers
include_directories(${CMAKE_SOURCE_DIR}/TwsApiCpp)
include_directories(${CMAKE_SOURCE_DIR}/TwsApiCpp/src)
include_directories(${CMAKE_SOURCE_DIR}/TwsApiCpp/Shared)

# Collect all source files in the src directory
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Add your main executable
add_executable(AlgorithmTrading ${SOURCE_FILES})

# Link against the TWS API library
target_link_libraries(AlgorithmTrading PRIVATE TwsApiCpp)