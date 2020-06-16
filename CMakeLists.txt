cmake_minimum_required(VERSION 3.7)                  

project(car-lane-detection)

find_package(OpenCV 4.0 REQUIRED)

FILE(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(laneDetector ${SOURCES})
target_include_directories(laneDetector PUBLIC ${PROJECT_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(laneDetector PRIVATE ${OpenCV_LIBS})
target_compile_features(laneDetector PRIVATE cxx_std_17)

if(MSVC)
  target_compile_options(laneDetector PRIVATE /W3)
else()
  target_compile_options(laneDetector PRIVATE -Wall -Wextra -pedantic)
endif()