cmake_minimum_required(VERSION 3.10)

project(AlgorithmTrading)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the TWS API source directory
add_subdirectory(TwsApiCpp)
add_subdirectory(tests)

# Collect all source files in the src directory
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Create a library containing all your source code
add_library(AlgorithmTradingLib ${SOURCE_FILES})

# Set up all includes for the library with PUBLIC visibility
target_include_directories(AlgorithmTradingLib PUBLIC
    ${CMAKE_SOURCE_DIR}/src          # Covers all src subdirectories
    ${CMAKE_SOURCE_DIR}              # Add this to allow TwsApiCpp/src/... includes
)

# Link TwsApiCpp to the library. PUBLIC means both the library
# and anything linking to it gets TwsApiCpp's includes
target_link_libraries(AlgorithmTradingLib PUBLIC TwsApiCpp)

# Create the main executable from src/main.cpp
add_executable(AlgorithmTrading src/main.cpp)

# Link the executable to our library. PRIVATE because nothing
# will link against the executable
target_link_libraries(AlgorithmTrading PRIVATE AlgorithmTradingLib)
