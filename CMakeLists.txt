cmake_minimum_required(VERSION 3.10)
project(ByteTrackCPP)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)

# Find OpenCV
find_package(OpenCV REQUIRED)
# Find Eigen3
find_package(Eigen3 REQUIRED)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
)

# Source files from src/
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Test source files from test/
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

# Create the test executable
add_executable(test_app ${SRC_FILES} ${TEST_SRC_FILES})

# Link OpenCV and Eigen3 (header-only, so Eigen3 doesn't need linking)
target_link_libraries(test_app 
    ${OpenCV_LIBS}
    Eigen3::Eigen
)

target_include_directories(test_app PRIVATE ${OpenCV_INCLUDE_DIRS})
